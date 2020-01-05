/*
 FileName:Dht11AndSoilMoistureWithJson
 Author: Sarang Gandhi, Nikhil and Neeraj

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

Use Arduino Json library for formatting the data in json format


*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <string.h>
#include "DHT.h"

#define DHTPIN D2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
#define MAX 100
// Update these with values suitable for your network.

char bufForH[MAX]; 
char bufForT[MAX];
//char bufForHI[MAX]; 
char bufFormositureSensor[MAX];

const char* ssid = "HawaHawai";
const char* password = "Nokia5500!";
//const char* mqtt_server = "192.168.0.100";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

long lastMsg = 0;
char msg[100];
int value = 0;

const int moistureSensorPin = A0; 
const int moistureSensorD0Pin = D1;
int moistureSensorAnlogValue;  
int moistureSensorDigitalValue;  
const int limit = 300; 


void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("WiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("farmSensor1", "hello humidity");
     // client.publish("farmSensor2", "hello temperature");  
     // client.publish("farmSensor3", "hello heat index"); 
      // ... and resubscribe
      client.publish("farmSensor", "Connecting . . .");
      client.subscribe("inTopic");
    } else {
      Serial.print("Connection failed, rc=");
      Serial.print(client.state());
      Serial.println("Trying again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(D1, INPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  dht.begin();
}

void loop() {
  delay(2000);
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

     if(digitalRead(moistureSensorD0Pin) == 1) {
          moistureSensorDigitalValue = 0;
          digitalWrite(LED_BUILTIN, HIGH); 
     } else {
          moistureSensorDigitalValue = 1;
          digitalWrite(LED_BUILTIN, LOW); 
     }
     Serial.print("\tDigital Value : ");
     Serial.println(moistureSensorDigitalValue);

 
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Read temperature as Celsius (the default)
  float f = dht.readTemperature(true);// Read temperature as Fahrenheit (isFahrenheit = true)

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  
  // Compute heat index in Fahrenheit (the default)
/*  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
*/

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
 /* Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));*/

/* char bufForH[MAX]; 
 char bufForT[MAX]; */

    gcvt(h, 3, bufForH); 
    gcvt(t, 3, bufForT); 
  /*  gcvt(hic, 3, bufForHI); */
    
  /*long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;*/
    snprintf (bufForH , 150, " Humidity : %3.2f , ", h );
    //snprintf (bufForH , 150, "Humidity %f ,", h );
    //snprintf (bufForT , 150, "Temperature %f", t );
    snprintf (bufForT , 150, " Temperature : %3.2f ", t );
  /*  snprintf (bufForHI, 150, "Heat Index %f", hic);*/
    
    /*Serial.print("Publish message for humidity: ");
    Serial.println(bufForH);
    Serial.print("Publish message for temperature: ");
    Serial.println(bufForT);*/

    strcat(bufForH ,bufForT );

    snprintf (bufFormositureSensor , 150, " Soil Moisture: %d ,", moistureSensorDigitalValue );
    
    //itoa(moistureSensorDigitalValue,bufFormositureSensor,10); // here 10 means decimal

    strcat(bufFormositureSensor ,bufForH );

   // client.publish("farmSensor1",bufFormositureSensor);
    //client.publish("farmSensor1", bufForH);
 // client.publish("farmSensor2", bufForT);
 // client.publish("farmSensor3", bufForHI);

  // Json Formatting
          Serial.println();
          StaticJsonBuffer<300> JSONbuffer;
          JsonObject& JSONencoder = JSONbuffer.createObject();
          JSONencoder["Humidity"] = h;
          JSONencoder["Temperature"] = t;
          JSONencoder["Soil Mositure"] = moistureSensorDigitalValue;
          
          int lenghtSimple = JSONencoder.measureLength();
          Serial.print("Less overhead JSON message size: ");
          Serial.println(lenghtSimple);

          Serial.println("Less overhead JSON message: ");
          char JSONmessageBuffer[100];
          JSONencoder.printTo(JSONmessageBuffer);
          JSONencoder.printTo(Serial);
          //MQTT Topic is farmSensor 
          client.publish("farmSensor",JSONmessageBuffer);
          Serial.println();
          Serial.println();
          delay(50000);
          Serial.println("—————");
}
