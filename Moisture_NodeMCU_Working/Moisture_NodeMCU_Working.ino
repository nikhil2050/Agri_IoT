int WET= 16; // Wet Indicator at Digital PIN D0
int DRY= 2;  // Dry Indicator at Digital PIN D4

int sense_Pin= 0; // Soil Sensor input at Analog PIN A0
int value= 0;
int valueOfA0 = 0;
const int sensor_pin = A0;

void setup() {
   Serial.begin(9600);
   pinMode(WET, OUTPUT);
   pinMode(DRY, OUTPUT);
   delay(2000);
}

void loop() {
   
    float moisture_percentage;
    moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) );
    Serial.print("Soil Moisture(in Percentage) = ");
    Serial.print(moisture_percentage);
    Serial.println("%");
   
   Serial.print("MOISTURE LEVEL : ");
   value= analogRead(sense_Pin);
   value= value/10;

   valueOfA0 = analogRead(sensor_pin);
   Serial.println(valueOfA0);
   
   Serial.println(value);
   if(value<50) 
   {
      digitalWrite(WET, HIGH);
   }
   else
   {
      digitalWrite(DRY,HIGH);
   }
   delay(1000);
   digitalWrite(WET,LOW);
   digitalWrite(DRY, LOW);
}
