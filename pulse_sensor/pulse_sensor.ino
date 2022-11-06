#include <DFRobot_Heartrate.h>
 
const int sensorPin = 25;
 
DFRobot_Heartrate heartrate(DIGITAL_MODE); 
 
void setup() {
  Serial.begin(115200);
}
 
void loop() {
  int rateValue;
   
  heartrate.getValue(sensorPin);   
  rateValue = heartrate.getRate();
     
  if(rateValue>0){
    Serial.println(rateValue);
  }
   
  delay(20);
}
