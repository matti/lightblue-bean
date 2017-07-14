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


uint8_t expectedPacket[] = {
    // general discoverable mode advertises indefinitely
    0x02,  // length of this data
    GAP_ADTYPE_FLAGS,
    GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,

    // two-byte broadcast
    0x03,  // length of this data including the data type byte
    GAP_ADTYPE_MANUFACTURER_SPECIFIC,
    0xAC  // arbitrary data so we can make sure this is
          // the Bean we're looking for
};


void setup() {
  Bean.enableConfigSave(false);
  
  Bean.setCustomAdvertisement(customAdvertData, sizeof(customAdvertData));
  Bean.enableCustom();
  Bean.setAdvertisingInterval(100);
}

void loop() {
  ObserverAdvertisementInfo info;
  int res = Bean.getObserverMessage(&info, 2000);

  if (res != 1) {
    Serial.println("No message received");
    return;
  }

  // check to see if this packet is form the custom advertiser
  bool isEqual = true;
  for (int i = 0; i < sizeof(expectedPacket) - 1; i++) {
    if (expectedPacket[i] != info.advData[i]) {
      isEqual = false;
    }
  }

  if (!isEqual) {
    return;
  }


  Serial.print("BT lämpötila: ");
  Serial.println(info.advData[6]);

  customAdvertData[6] = info.advData[6];

  Bean.setCustomAdvertisement(customAdvertData, sizeof(customAdvertData));
  Bean.sleep(1000);
}
