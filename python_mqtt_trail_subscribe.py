import paho.mqtt.client as mqttClient
import time
from tinydb import TinyDB, Query
from flask import Flask, jsonify, request #import objects from the Flask model
app = Flask(__name__) #define app using Flask

 
def on_connect(client, userdata, flags, rc):
 
    if rc == 0:
        print("Connected to broker")
 
        global Connected                #Use global variable
        Connected = True                #Signal connection 
 
    else:
        print("Connection failed")

print ("Here before on_message")

def on_message(client, userdata, message):
    
    print ("Here in on_message")
    #print ("Message received: "  , message.payload)
    print ("message received  ", str(message.payload.decode("utf-8")))
    print ("topic: " + message.topic)


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
 
client.subscribe("farmSensor")
 
try:
    while True:
        time.sleep(1)
 
except KeyboardInterrupt:
    print ("exiting")
    client.disconnect()
    client.loop_stop()


if __name__ == '__main__':
    #app.run(debug=True, port=8080) #run app on port 8080 in debug mode
    app.run(host='127.0.0.1', port=50000, debug=True)
