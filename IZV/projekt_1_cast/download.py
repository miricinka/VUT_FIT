#!/usr/bin/env python

"""download.py skript downloads and parses data from specified url, IZV school project"""
__author__      = "Mirka Kolarikova"

import requests, os, re, sys, csv, gzip, pickle
from bs4 import BeautifulSoup
import numpy as np
from zipfile import ZipFile
from io import TextIOWrapper

class DataDownloader:
	"""A class that downloads data from url to folder.
	Keyword arguments:
	url -- url to download data from (default "https://ehw.fit.vutbr.cz/izv/") 
	folder -- folder to store data
	cache_filename -- name of cache
	""" 
	def __init__ (self, url="https://ehw.fit.vutbr.cz/izv/", folder="data", cache_filename="data_{}.pkl.gz"):
		"""
		Keyword arguments:
		url -- url to download data from (default - "https://ehw.fit.vutbr.cz/izv/") 
		folder -- folder to store data (default - "data")
		cache_filename -- name of cache (default - "data_{}.pkl.gz")
		"""
		self.url = url
		self.folder = folder
		self.cache_filename = cache_filename
		self.region_dic = {
        	'PHA': '00',
        	'STC': '01',
        	'JHC': '02',
        	'PLK': '03',
        	'ULK': '04',
        	'HKK': '05',
        	'JHM': '06',
        	'MSK': '07',
        	'OLK': '14',
        	'ZLK': '15',
        	'VYS': '16',
        	'PAK': '17',
        	'LBK': '18',
        	'KVK': '19'}
		self.region_data_dic = {
        	'PHA': None,
        	'STC': None,
        	'JHC': None,
        	'PLK': None,
        	'ULK': None,
        	'HKK': None,
        	'JHM': None,
        	'MSK': None,
        	'OLK': None,
        	'ZLK': None,
        	'VYS': None,
        	'PAK': None,
        	'LBK': None,
        	'KVK': None}

	def download_data (self):
		""" Dowloads data from url into folder. Folder is created if doesnt exist."""

		#create folder if doesnt exist
		if not os.path.exists(self.folder):
			os.makedirs(self.folder)
		with requests.Session() as s:
			#download webpage html
			headers = {'user-agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.102 Safari/537.36'}
			r = s.get(self.url, headers=headers)
			#parse html
			soup = BeautifulSoup(r.text, "html.parser")
			table = soup.find('table')
			link_list = []
			last_link_list = []
			#get last month of every year
			for td in table.find_all('td'):
				if td.get('rowspan'):
					if link_list:
						last_link_list.append(link_list[-1])
						link_list = []
				a = td.find_all('a')
				if(a):
					link = a[-1].get('href')
					link_list.append(link)
			#append last year
			last_link_list.append(link_list[-1])
			#download last link of every year
			for item in last_link_list:
				download_link = self.url + item
				split = download_link.split('/')
				if split:
					file_name = split[-1]
				with s.get(download_link, stream=True) as r:
					with open(f'{self.folder}/{file_name}','wb') as f:
						for chunk in r.iter_content(chunk_size=16384):
							f.write(chunk)


	def parse_region_data(self, region):
		"""Downloads data if needed and parses them into tuple(list[str], list[np.ndarray]),
		where list[str] containes string with meaning of each numpy array in list[np.ndarray].
		Arguments:
		region -- code of region to parse
		"""

		#download data
		if not os.path.exists(self.folder) or not os.listdir(self.folder):
			self.download_data()

		#prepare list for numpy arrays
		list_of_lists = [[] for _ in range(64) ]
		#list of collumns (https://www.policie.cz/soubor/polozky-formulare-hlavicky-souboru-xlsx.aspx)
		list_str = ["p1", "p36", "p37", "p2a", "weekday(p2a)", "p2b", "p6", "p7", "p8", "p9", "p10", 
		"p11", "p12", "p13a", "p13b", "p13c", "p14", "p15", "p16", "p17", "p18","p19", "p20", "p21", 
		"p22", "p23", "p24", "p27", "p28", "p34", "p35", "p39", "p44", "p45a", "p47", "p48a", "p49", 
		"p50a", "p50b", "p51", "p52", "p53", "p55a", "p57", "p58", "a", "b", "d", "e", "f", "g", "h", 
		"i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "p5a"]
		#define type of each numpy array
		types = ["U12","i8","i8","M8","U4","i8","i8","i8","i8","i8","i8","i8","i8","i8","i8","i8",
		"i8","i8","i8","i8","i8","i8","i8","i8","i8","i8","i8","i8","i8","i8","i8","i8","i8","i8",
		"i8","i8","i8","i8","i8","i8","i8","i8","i8","i8","i8","f8","f8","f8","f8","f8","f8","U16",
		"U16","U16","U16","U16","U16","U16","U16","U16","U16","U16","U16","i8"]

		#convert region code to file name
		region_file_name = self.region_dic.get(region)
		if(region_file_name == None):
			sys.exit("Incorrect region code")
		region_file_name = region_file_name + '.csv'

		#extract region data
		for file in os.scandir(self.folder):
			if file.path.endswith(".zip"):
				with ZipFile(file, 'r') as zipfile:
					with zipfile.open(region_file_name, 'r') as csv_file:
						reader = csv.reader(TextIOWrapper(csv_file, 'windows-1250'), delimiter=';')
						for row in reader:
							for index, item in enumerate(row):
								if item == "" or item == "XX" or item == "A:" or item == "B:" or item == "D:" or item == "E:" or item == "F:" or item == "G:":
									list_of_lists[index].append("-1")
								else:
									item = item.replace(",", ".")
									list_of_lists[index].append(item)

		#convert list of lists to list of numpy arrays					
		for list_index, one_list in enumerate(list_of_lists):
			list_of_lists[list_index] = np.asarray(one_list, dtype=types[list_index])

		items_count = len(list_of_lists[0])
		np_region = np.full(items_count, region)

		#return with region column
		return (["region"] + list_str, [np_region] + list_of_lists)




	def get_list(self, regions = None):
		"""Returns parsed data of specified regions in tuple(list[str], list[np.ndarray]),
		where list[str] containes string with meaning of each numpy array in list[np.ndarray].
		Keyword arguments:
		regions -- list of region codes to parse (default - parse all regions)
		"""

		if regions == None:
			regions = ['PHA', 'STC', 'JHC', 'PLK', 'ULK', 'HKK', 'JHM', 'MSK', 'OLK', 'ZLK', 'VYS', 'PAK', 'LBK', 'KVK']

		regions_list = []	
		for region in regions:
			if(self.region_data_dic.get(region) != None):
				#get from internal variable
				data = self.region_data_dic.get(region)
				regions_list.append(data[1])
			elif os.path.exists(self.folder + '/' + self.cache_filename.format(region)):
				#load from cache
				with gzip.open(self.folder + '/' + self.cache_filename.format(region),'rb') as f:
					data = pickle.load(f)
				#prepare for concatenating
				regions_list.append(data[1])
			else:
				#parse data
				data = self.parse_region_data(region)
				#create cache
				with gzip.open(self.folder + '/' + self.cache_filename.format(region),'wb') as f:
					pickle.dump(data, f)
				#add to internal variable
				self.region_data_dic[region] = data
				#prepare for concatenating
				regions_list.append(data[1])


		#concatenate all regions 
		result_list = []
		concatenated_list = []
		for i in range(65):
			for sloupec, region in enumerate(regions_list):
				result_list.append(regions_list[sloupec][i])
			result_tuple = tuple(result_list)
			x = np.concatenate(result_tuple)
			concatenated_list.append(x)
			result_list = []

		#return concatenated regions
		return (data[0], concatenated_list)



if __name__ == "__main__":

	mydownloader = DataDownloader()
	print("Dowload data for:")
	print("['PHA', 'OLK', 'JHM']")
	a = mydownloader.get_list(["PHA", "OLK", "JHM"])
	print("Columns are: ")
	print(a[0])
	print("Number of records:")
	print(len(a[1][0]))


