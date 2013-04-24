import serial
import string
import MySQLdb
import pprint

db = MySQLdb.connect(host="YOURHOST", # your host, usually localhost
                     user="USER", # your username
                      passwd="PASSWORD", # your password
                      db="DB_NAME") # name of the data base
cur = db.cursor() 

arduinoPort = 'PATH TO PORT'
ser = serial.Serial()

ser.setPort(arduinoPort) #boundRate 9600 automatically set

#Loop in folder /dev/ for a ttyACM {0-n}, that's the Arduino code.


ser.setTimeout(2)

try:
	ser.open()
except: 
	print('Wow, dude: error!')

else:
	for i in range (50):
		ardString = ser.readline()
		#print(ardString)
		valueMatrix= ardString.split(' ')
		if len(valueMatrix)> 1 :
			pprint.pprint(valueMatrix)
		#pprint.pprint(valueMatrix)
			projectId = valueMatrix[0]
			value = valueMatrix[1]
			#print("value received:"+string+ " interpreted as: project Id = "+projectId+" and value = "+value)
			cur.execute('INSERT INTO ard_project_data (project_id, value) VALUES ("'+projectId+'", "'+value+'")')
			db.commit()

	ser.close()
	print('connection closed')
	db.close()
	print('database closed')
print('end')

