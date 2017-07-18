#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 0 
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

// copypaste from bean sources (can't be higher or lower than MAX and MIN)
// Longest Apple Bluetooth Design Guide Advertising Interval is also 1285
#define BEAN_MIN_ADVERTISING_INT_MS 20
#define BEAN_MAX_ADVERTISING_INT_MS 1285

#define BUILD_NUMBER 7

float lastTempBean;
float lastTempFirst;
float lastTempSecond;

void setup() {
  // MAYBEFIX: didn't work unless saved in config?
  //Bean.enableConfigSave(true);
  //Bean.setAdvertisingInterval(BEAN_MAX_ADVERTISING_INT_MS);
  
  Bean.enableConfigSave(false);

  Bean.sleep(15000);

  Bean.setAdvertisingInterval(BEAN_MAX_ADVERTISING_INT_MS);

  sensors.begin();
}

void loop() {
  int batteryPercentage = Bean.getBatteryLevel();
  batteryPercentage = batteryPercentage / 10;
  
  sensors.requestTemperatures();
 
  float tempBean = Bean.getTemperature();
  tempBean = tempBean + 2;
  float tempFirst = sensors.getTempCByIndex(0);
  float tempSecond = sensors.getTempCByIndex(1);

  if (tempFirst == -127.0) {
    tempFirst = 0.0;
  }
  if (tempSecond == -127.0) {
    tempSecond = 0.0;
  }

  if (
    tempBean == lastTempBean &&
    tempFirst == lastTempFirst &&
    tempSecond == lastTempSecond
   ) {
    Bean.sleep(60000);
    return;
  }

  char tempBeanChar[4];
  char tempFirstChar[4];
  char tempSecondChar[4];
  
  String name = "t/";
  name += batteryPercentage;
  name += "/";
  name += dtostrf(tempBean,4,1,tempBeanChar);
  name += "/";
  name += dtostrf(tempFirst,4,1,tempFirstChar);
  name += "/";
  name += dtostrf(tempSecond,4,1,tempSecondChar);
  name += "/";
  name += BUILD_NUMBER;
  
  Serial.println(name);
  // 21 chars max?
  Bean.setBeanName(name);
  
  Bean.enableAdvertising(true);
  Bean.sleep(10000);
  Bean.enableAdvertising(false);
  Bean.sleep(50000);
}
