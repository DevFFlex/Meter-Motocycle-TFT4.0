class BatteryClass {
private:
  TFT_eSPI *tft;
  Ticker t1;

  unsigned long previousTimeForBatteryScan = 0;
  int delayForBatteryScan = 60000 ; //millisecond

  float battery = 88; // 0 - 100
  float voltage = 0;
  float current = 0;

  void voltageUpdate(){
    voltage = random(1, 57);
  }

  void batteryUpdate(){
    if (millis() - previousTimeForBatteryScan > delayForBatteryScan) {
      if(battery > 0)battery -= 1;
      
      previousTimeForBatteryScan = millis();
    }
  }

public:
  BatteryClass(TFT_eSPI *tft) {
    this->tft = tft;
    previousTimeForBatteryScan = millis();
  }


  void loop(){
    // voltageUpdate();
    batteryUpdate();
  }

  // ----------------- getter ------------------

  float getVoltage(){
    return this->voltage;
  }
  float getBatteryPersen(){
    return this->battery;
  }


  

};
