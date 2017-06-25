void setup() {
  Bean.enableConfigSave(false);
}
 
void loop() {
  // Get the temperature
  int temperature = Bean.getTemperature();

  // Build a new name for Bean using the temperature
  String name = "temp ";
  name += temperature;

  // Set Bean's new name
  Serial.println(name);
  Bean.setBeanName(name);

  Bean.sleep(10000);
}
