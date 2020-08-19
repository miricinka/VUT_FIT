
"""
 * File name: http_resolver.py
 * Description: Projekt 1 do předmětu IPK 2020, FIT VUT
 * Athor: Mirka Kolaříková (xkolar76)
 * Date: 6.3.2020
 """

import socket
import sys
import re

#curl "localhost:1234/resolve?name=www.fit.vutbr.cz\&type=A"
#curl -i localhost:1234/resolve?name=www.fit.vutbr.cz\&type=A
#curl "localhost:1231/resolve?name=147.229.9.23\&type=PTR"
#curl -i localhost:1231/resolve?name=147.229.9.23\&type=PTR
#curl -i --data-binary @queries.txt -X POST http://localhost:1234/dns-query
HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
PORT = int(sys.argv[1])       # Port to listen on (non-privileged ports are > 1023)



def get_host(ip):
    try:
        adresa = socket.gethostbyaddr(ip)
        host = str(adresa[0])
        return host;
    except Exception:
        #error = "404 not found.\r\n"
        #error = error.encode()
        #conn.sendall(error)
        return 0 #-chyba

def get_ip(host):
    try:
        stranky = socket.gethostbyname(host)
        ip = str(stranky)
        return ip;
    except Exception:
        #error = "404 not found.\r\n"
        #error = error.encode()
        #conn.sendall(error)
        return 0 #- chyba



S = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
S.bind((HOST, PORT))
S.listen()
while True:
	#connection
	CONN, ADDR = S.accept()
	#read, send
	while True:
		DATA = CONN.recv(1024)
		if not DATA:
			break
		"""
		RESPONSE = 'Response from server.\r\n\r\n'
		RESPONSE = RESPONSE.encode()
		CONN.sendall(RESPONSE)
		print('Mesage sent')
		"""
		data_decoded = DATA.decode()
		data_decoded = data_decoded.split("\n")
		post_instruction_array = data_decoded;
		data_decoded = data_decoded[0]
		data_decoded = data_decoded.split(" ")
		instruction = data_decoded[0]
		question = data_decoded[1]

		arg_num = len(data_decoded)
		if(arg_num != 3):
			error = "400 bad request\r\n\r\n".encode();
			CONN.sendall(error)
			break

		#instruction GET
		if instruction == "GET":
			question = question.split("=")
			#[0] = /resolve?name, [1] = www.fit.vutbr.cz\\&type, [2]=A
			if(question[0] != "/resolve?name"):
				error = "HTTP/1.1 400 Bad Request\r\n\r\n".encode();
				CONN.sendall(error)
				break #jak ukoncit?
			web_or_ip = question[1].split("\\")
			web_or_ip = web_or_ip[0]
			web_or_ip = web_or_ip.split("&")
			web_or_ip = web_or_ip[0]
			#print(web_or_ip)

			#IP:PTR=web
			if(re.fullmatch(r"^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+$",web_or_ip) != None and question[2] == "PTR"):
				ip = web_or_ip
				web = get_host(ip)
				if(web == 0):
					error = "HTTP/1.1 404 Not Found\r\n\r\n".encode();
					CONN.sendall(error)
					break 
				message = "HTTP/1.1 200 OK\r\n\r\n"+ip+":PTR="+web+"\r\n"
				message = message.encode()
				CONN.sendall(message)
				break
			#ADDR:A=IP
			elif (re.fullmatch(r"^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+$",web_or_ip) == None and question[2] == "A"):
				web = web_or_ip
				ip = get_ip(web)
				if(ip == 0):
					error = "HTTP/1.1 404 Not Found\r\n\r\n".encode();
					CONN.sendall(error)
					break #jak ukoncit?
				message = "HTTP/1.1 200 OK\r\n\r\n"+web+":A="+ip+"\r\n"
				message = message.encode()
				CONN.sendall(message)
				break
			else:
				error = "HTTP/1.1 400 Bad Request\r\n\r\n".encode();
				CONN.sendall(error)
				break #jak ukoncit?


		#instruction POST
		elif instruction =="POST":
			if(question != '/dns-query'):
				message = "HTTP/1.1 400 Bad Request\r\n\r\n".encode()
				CONN.sendall(message)
				break
			array_len = len(post_instruction_array)
			i = 7
			message = "HTTP/1.1 200 OK\r\n\r\n"
			ok = 0
			incorrect = 0
			while i < array_len:
				actual_instr = post_instruction_array[i]
				actual_instr = actual_instr.split(":")
				if(len(actual_instr) != 2):
					i += 1
					incorrect = 1
					continue
				#odstraneni bilych znaku
				actual_instr[0] = actual_instr[0].strip() 
				actual_instr[1] = actual_instr[1].strip()
				#prazdny radek
				if(actual_instr[0] == ''):
					pass
				#ip:PTR OK
				elif(re.fullmatch(r"^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+$",actual_instr[0]) != None and actual_instr[1]== "PTR"):
					ip = actual_instr[0]
					web = get_host(ip)
					if(web == 0):
						pass
					else:
						message += ip+":PTR="+web+"\r\n"
						ok = 1
				#ip:A not Ok
				elif(re.fullmatch(r"^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+$",actual_instr[0]) != None and actual_instr[1]== "A"):
					pass
				#web:ptr not ok
				elif(re.fullmatch(r"^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+$",actual_instr[0]) == None and actual_instr[1]== "PTR"):
					pass
				#web:A ok
				elif(actual_instr[1]== "A"):
					web = actual_instr[0]
					ip = get_ip(web)
					if(ip == 0):
						pass
					else:
						message += web+":A="+ip+"\r\n"
						ok = 1
				else:
					incorrect = 1;
					#ignorovani chybnych
				i += 1

			if(ok == 1):
				message = message.encode()
				CONN.sendall(message)
			elif(ok == 0 and incorrect == 1):
				message = "HTTP/1.1 400 Bad Request\r\n\r\n"
				message = message.encode()
				CONN.sendall(message)
			elif(ok == 0):
				message = "HTTP/1.1 404 Not Found\r\n\r\n"
				message = message.encode()
				CONN.sendall(message)
			break;


		else:
			error = "HTTP/1.1 405 Method Not Allowed\r\n\r\n".encode();
			CONN.sendall(error)
			break #jak ukoncit? asi ok

	CONN.close()

S.close()

