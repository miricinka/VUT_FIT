#!/usr/bin/env python

"""get_stat.py sript creates graph visualization of accidents for specified regions, IZV school project"""
__author__      = "Mirka Kolarikova"

from download import DataDownloader
import matplotlib.pyplot as plt
import numpy as np
import argparse, os

def plot_stat(data_source, fig_location = None, show_figure = False):
	"""Visualises data in bar graphs
	Aguments:
	data_source -- data to visualize
	Keyword argumens:
	fig_location -- visualization will be stored here (default - None)
	show_figure -- show visualization on screen (default - False)
	"""

	#get years and regions from data_source
	regions = data_source[1][0]
	years = data_source[1][4].astype('datetime64[Y]').astype("U12")
	unique_region_num = (len(np.unique(regions)))
	stack_reg_year = np.stack([regions, years])
	#count data
	uniqie_reg_year, count = np.unique(stack_reg_year, return_counts=True, axis=1)

	#divide data by years
	title = ["2016", "2017", "2018", "2019", "2020"]
	region_list = [[], [], [], [], []]
	count_list = [[], [], [], [], []]
	for index, item in enumerate(uniqie_reg_year[1]):
		if item == "2016":
			count_list[0].append(count[index])
			region_list[0].append(uniqie_reg_year[0][index])
		elif item == "2017":
			count_list[1].append(count[index])
			region_list[1].append(uniqie_reg_year[0][index])
		elif item == "2018":
			count_list[2].append(count[index])
			region_list[2].append(uniqie_reg_year[0][index])
		elif item == "2019":
			count_list[3].append(count[index])
			region_list[3].append(uniqie_reg_year[0][index])
		elif item == "2020":
			count_list[4].append(count[index])
			region_list[4].append(uniqie_reg_year[0][index])
		else:
			print("chyba")

	#start A4 visualization
	plt.figure(figsize=(8.27, 11.69))
	plt.suptitle("Počet nehod v učitých krajích České republiky v posledních letech")

	#create 5 bar graphs 
	for i in range(5):
		plt.subplot(5, 1, i + 1)
		#set year as title
		plt.title(title[i])
		plot = plt.bar(region_list[i], count_list[i], color='red')
		plt.ylim(0, 25000)
		plt.ylabel('Počet nehod')
		plt.xlabel('Kód kraje')
		plt.grid(color='#95a5a6', linestyle='--', linewidth=0.5, axis='y', alpha=0.7)

		#get regions with most accidents by order
		npcount = np.array(count_list[i])
		sortedcount = np.argsort(npcount)
		order = [None] * unique_region_num
		for i, index in enumerate(sortedcount):
			order[index] = unique_region_num - i

		#set texts
		for index, value in enumerate(plot):
			plt.text(value.get_x() + value.get_width()/2.,
            		5,value.get_height(), ha='center', va='bottom', fontsize=8)
			plt.text(value.get_x() + value.get_width()/2.,
					5 + value.get_height(), order[index],ha='center', va='bottom', fontsize=8)

	plt.tight_layout()
		
	#save file
	if fig_location:
		(dirname, filename) = os.path.split(fig_location)
		#create folder if doesnt exist
		if dirname:
			if not os.path.exists(dirname):
				os.makedirs(dirname)
		plt.savefig(fig_location)
	#show figure
	if show_figure == True:
		plt.show()




if __name__ == "__main__":
	#parse arguments
	argparser = argparse.ArgumentParser()
	argparser.add_argument('--fig_location', dest='fig_location',help="Folder to store image")
	argparser.add_argument('--show_figure', action='store_true',help="Show figure in window")
	args = argparser.parse_args()

	#create stats for ["PHA", "OLK", "JHM"]
	data_source = DataDownloader().get_list(["PHA", "OLK", "JHM"])
	plot_stat(data_source, args.fig_location, args.show_figure)

