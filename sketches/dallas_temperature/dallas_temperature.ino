#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 0 
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

void setup() {
  Bean.enableConfigSave(false);
  sensors.begin();

  Bean.sleep(10000);
  Bean.setAdvertisingInterval(100);  
}

void loop() {
  int batteryPercentage = Bean.getBatteryLevel();
    
  sensors.requestTemperatures();
 
  double tempBean = Bean.getTemperature(); 
  float tempFirst = sensors.getTempCByIndex(0);
  float tempSecond = sensors.getTempCByIndex(1);

  char tempBeanChar[4];
  char tempFirstChar[4];
  char tempSecondChar[4];
  
  String name = "t-";
  name += batteryPercentage;
  name += "-";
  name += dtostrf(tempBean,4,1,tempBeanChar);
  name += "-";
  name += dtostrf(tempFirst,4,1,tempFirstChar);
  name += "-";
  name += dtostrf(tempSecond,4,1,tempSecondChar);

  Serial.println(name);
  // 21 chars max?
  Bean.setBeanName(name);


  Bean.sleep(5000);
}
