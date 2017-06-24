static uint8_t customAdvertData[] = {
  // general discoverable mode advertises indefinitely
  0x02,  // length of this data
  GAP_ADTYPE_FLAGS,
  GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
  
  // two-byte broadcast
  0x03,  // length of this data including the data type byte
  GAP_ADTYPE_MANUFACTURER_SPECIFIC,
  0xAC,  // arbitrary data so we can make sure this is
         // the Bean we're looking for
  0x00
};

void setup() {
  Bean.setCustomAdvertisement(customAdvertData, sizeof(customAdvertData));
  Bean.enableCustom();
  Bean.setAdvertisingInterval(100);
}

void loop() {
  uint8_t temperature = Bean.getTemperature();
  Serial.print("Temp in decimal is: "); Serial.println(temperature);
  String temperatureHex = String(temperature, HEX);
  Serial.print("Temp in HEX is: "); Serial.println(temperatureHex);
  
  customAdvertData[6] = temperature;
  Bean.setCustomAdvertisement(customAdvertData, sizeof(customAdvertData));
  Bean.sleep(1000);
}
