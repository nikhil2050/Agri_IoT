import paho.mqtt.client as mqttClient
import time
from tinydb import TinyDB, Query

#Declare our database variable and the file to store our data in
#This code should be modified for the database operations.
db = TinyDB('todolist3.json')

dataFromPublisher =  '{"2":{"Status":"New","Category":"Home","DueDate":"5/11/18","Description":"Do the Laundry"}}'
 
def on_connect(client, userdata, flags, rc):
 
    if rc == 0:
 
        print("Connected to broker")
 
        global Connected                #Use global variable
        Connected = True                #Signal connection 
 
    else:
 
        print("Connection failed")
 
def on_message(client, userdata, message):
	#db.insert(message.payload)
	#db.insert(dataFromPublisher)
	db.insert({"2":{"Status":"New","Category":"Home","DueDate":"5/11/18","Description":"Do the Laundry"}})
	print ("Message received: "  + message.payload)
 
db.all()
for item in db:
	print("items are ",item)
Connected = False   #global variable for the state of the connection
 
#const char* mqtt_server = "broker.mqtt-dashboard.com";
broker_address= "broker.mqtt-dashboard.com" 
#broker_address= "192.168.0.100"  #Broker address
port = 1883                         #Broker port
#user = "yourUser"                    #Connection username
#password = "yourPassword"            #Connection password
 
client = mqttClient.Client("Python")               #create new instance
#client.username_pw_set(user, password=password)    #set username and password
client.on_connect= on_connect                      #attach function to callback
client.on_message= on_message                      #attach function to callback
 
client.connect(broker_address, port=port)          #connect to broker
 
client.loop_start()        #start the loop
 
while Connected != True:    #Wait for connection
    time.sleep(0.1)
 
client.subscribe("farmSensor1")
client.subscribe("farmSensor2")
client.subscribe("farmSensor3")
 
try:
    while True:
        time.sleep(1)
 
except KeyboardInterrupt:
    print "exiting"
    client.disconnect()
    client.loop_stop()