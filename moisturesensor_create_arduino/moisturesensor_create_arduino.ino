const int moistureSensorPin = A0; 
const int moistureSensorD0Pin = D1;
int moistureSensorAnlogValue;  
int moistureSensorDigitalValue;  
const int limit = 300; 

void setup() {
 Serial.begin(9600);
 pinMode(D1, INPUT);
}

void loop() {

 // moistureSensorAnlogValue = analogRead(moistureSensorPin); 
 // Serial.print("Analog Value : "); Serial.print(100-(moistureSensorAnlogValue*100/1023));
 if(digitalRead(moistureSensorD0Pin) == 1) {
  moistureSensorDigitalValue = 0;
  digitalWrite(LED_BUILTIN, HIGH); 
 } else {
  moistureSensorDigitalValue = 1;
  digitalWrite(LED_BUILTIN, LOW); 
 }
 Serial.print("\tDigital Value : ");
 Serial.println(moistureSensorDigitalValue);
 
 /*if (moistureSensorAnlogValue<limit) {
  digitalWrite(LED_BUILTIN, HIGH); 
 } else {
  digitalWrite(LED_BUILTIN, LOW); 
 }*/
 
 delay(1000); 
}
