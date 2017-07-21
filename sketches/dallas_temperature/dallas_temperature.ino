#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 0 
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

// copypaste from bean sources (can't be higher or lower than MAX and MIN)
// Longest Apple Bluetooth Design Guide Advertising Interval is also 1285
#define BEAN_MIN_ADVERTISING_INT_MS 20
#define BEAN_MAX_ADVERTISING_INT_MS 1285

#define BUILD_NUMBER 1
#define POWERSAVE true
#define POWERSAVE_DUPLICATES false
#define POWERSAVE_SENDING_TIME 15000
#define POWERSAVE_SLEEPING_TIME 30000

float lastTempBean = 0.0;
float lastTempFirst = 0.0;
float lastTempSecond = 0.0;

static uint8_t customAdvertData[] = {
    2,  // length of this data
    GAP_ADTYPE_FLAGS,
    GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,  // general discoverable mode advertises indefinitely
    27,
    GAP_ADTYPE_MANUFACTURER_SPECIFIC,
    99,
    0,   // length: 3 would be here
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0   // length: 27
};

void setup() {
  //Bean.enableConfigSave(true);
  //Bean.setBeanName("toinen");
  Bean.enableConfigSave(false);
  
  // SLOW to propagate!
  //Bean.setAdvertisingInterval(BEAN_MAX_ADVERTISING_INT_MS);
  
  Bean.sleep(15000);

  sensors.begin();
}

void loop() {
 
  int batteryPercentage = Bean.getBatteryLevel();
  batteryPercentage = batteryPercentage / 10;

  int batteryVoltage = Bean.getBatteryVoltage(); 
  batteryVoltage = batteryVoltage / 10;

  if (POWERSAVE) {
    Bean.setAdvertisingInterval(50);
  } else {
    Bean.setAdvertisingInterval(BEAN_MIN_ADVERTISING_INT_MS);
  }
  
  sensors.requestTemperatures();
 
  float tempBean = Bean.getTemperature();
  tempBean = tempBean + 1;
  float tempFirst = round(sensors.getTempCByIndex(0) * 10)/10.0;
  float tempSecond = round(sensors.getTempCByIndex(1) * 10)/10.0;

  // Should return 85 on error (datasheet)
  if (tempFirst == 85.0 || tempSecond == 85.0) {
    Serial.println("error reading temperatures. sleep 5000.");
    Bean.sleep(5000);
    return;
  }
  
  // if not connected, then -127.0
  if (tempFirst == -127.0) {
    tempFirst = 0.0;
    Serial.println("tempFirst not connected");
  }
  if (tempSecond == -127.0) {
    tempSecond = 0.0;
    Serial.println("tempSecond not connected");
  }

  if (
    tempBean == lastTempBean &&
    tempFirst == lastTempFirst &&
    tempSecond == lastTempSecond
  ) {
    if (POWERSAVE_DUPLICATES) {
      Serial.println("temperatures did not change, sleeping 30000.");
      Bean.sleep(30000);
      return;
    } else {
      Serial.println("temperatures did not change, powersave not enabled, will continue.");
    }
  }
  
  lastTempBean = tempBean;
  lastTempFirst = tempFirst;
  lastTempSecond = tempSecond;
  
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

  int tempFirstSig = (int) tempFirst;
  int tempSecondSig = (int) tempSecond;
  
  float tempFirstMantissa = tempFirst - tempFirstSig;
  float tempSecondMantissa = tempSecond - tempSecondSig;

  customAdvertData[6] = BUILD_NUMBER;
  customAdvertData[7] = batteryVoltage;
  customAdvertData[8] = batteryPercentage;
  customAdvertData[9] = (int) tempBean;
  customAdvertData[10] = (int) tempFirstSig;
  customAdvertData[11] = (int) (tempFirstMantissa * 10);
  customAdvertData[12] = (int) tempSecondSig;
  customAdvertData[13] = (int) (tempSecondMantissa * 10);  

  Bean.setCustomAdvertisement(customAdvertData, sizeof(customAdvertData));
  Bean.enableCustom();
    
  if (POWERSAVE) {
    Bean.enableAdvertising(true);
 
    Serial.print("advertising enabled. sleep ");
    Serial.println(POWERSAVE_SENDING_TIME);
    
    Bean.sleep(POWERSAVE_SENDING_TIME);
    Bean.enableAdvertising(false);
    Bean.disableCustom();
    
    Serial.print("advertising disabled. sleep ");
    Serial.println(POWERSAVE_SLEEPING_TIME);
    
    Bean.sleep(POWERSAVE_SLEEPING_TIME);
  } else {
    Serial.println("powersave disabled. sleep 5000");
    Bean.sleep(5000);
  }
}
