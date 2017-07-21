// NOTE: If packet is malformed, the last ok packet is used

static uint8_t customAdvertData[] = {
    2,  // length of this data
    GAP_ADTYPE_FLAGS,
    GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,  // general discoverable mode advertises indefinitely

    27,
    GAP_ADTYPE_MANUFACTURER_SPECIFIC,
    0,
    1,   // length: 3 would be here
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    11,
    12,
    13,
    14,
    15,
    16,
    17,
    18,
    19,
    20,
    21,
    22,
    23,
    24,
    25   // length: 27
};

void setup() {
  Bean.enableConfigSave(false);
  Bean.sleep(10000);
  
  Bean.setAdvertisingInterval(1000);
}

void loop() {
//  String temperatureHex = String(temperature, HEX);
//  Serial.print("Temp in HEX is: "); Serial.println(temperatureHex);
  
  //Bean.setCustomAdvertisement(customAdvertData, sizeof(customAdvertData));
  customAdvertData[6] = millis();
  Bean.setCustomAdvertisement(customAdvertData, sizeof(customAdvertData));
  Bean.enableCustom();
  
  Bean.sleep(1000);
}
