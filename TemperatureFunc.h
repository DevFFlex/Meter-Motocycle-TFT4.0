
class TemperatureClass {
private:
  TFT_eSPI *tft;
  
  unsigned long previousTimeForTempScan = 0;
  int delayForTempScan = 20000 ; //millisecond

  float temp_celsius = 0;
  float temp_fahrenheit = 0;
  float temp_kelvin = 0;

  void temperatureUpdate() {

    if (millis() - previousTimeForTempScan > delayForTempScan) {
      temp_celsius = random(0, 30);
      
      previousTimeForTempScan = millis();
    }
  }

public:
  TemperatureClass(TFT_eSPI *tft) {
    this->tft = tft;
    previousTimeForTempScan = millis();
  }


  void loop() {
    temperatureUpdate();
  }


  // ----------------- getter ------------------

  float getTempC() {
    return this->temp_celsius;
  }
  float getTempF() {
    return this->temp_fahrenheit;
  }
  float getTempK() {
    return this->temp_kelvin;
  }
};