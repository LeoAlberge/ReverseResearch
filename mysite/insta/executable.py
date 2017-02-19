import os
import subprocess
import csv
import math
from operator import itemgetter

accuracy="2"
scriptsDir = os.path.dirname(__file__)+"\\static\\images_algorithms\\"
images_path=os.path.dirname(os.path.dirname(__file__))+"\\media\\"

load_db_path=scriptsDir+"load_db.exe"
request_path=scriptsDir+"request.exe"

print(images_path)
def load_db(images_path,load_db_path):
	successful_process = subprocess.check_call([load_db_path,images_path,accuracy],shell=True)

def request_scores(request_path,image_request_path):
	successful_process = subprocess.check_call([request_path,image_request_path,accuracy],shell=True)

def ranking(ranks,number):
	ranks1=[]
	with open(r'C:\Users\leoal\Desktop\Tdlog\mysite\scores.txt', 'rt') as csvfile:
		spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
		for row in spamreader:
			temp= (row[0],row[1])
			ranks1.append(temp)
	ranks1=sorted(ranks1,key=itemgetter(1),reverse=True)
	ranks1=ranks1[:number]
	for item in ranks1 :
		ranks.append(item)

