class STimeClass {
private:
  TFT_eSPI *tft;
  unsigned long previousTime;
  int hour = 0;
  int minute = 0;
  int second = 0;
  String stime_str = "";

  void STimeUpdate() {
    second++;
    if (second >= 60) {
      second = 0;
      minute++;
      if (minute >= 60) {
        minute = 0;
        hour++;
        if (hour >= 24) {
          hour = 0;
        }
      }
    }

    String hour_str = "", minute_str = "", second_str = "";
    (hour < 10) ? hour_str = "0" + String(hour) : hour_str = String(hour);
    (minute < 10) ? minute_str = "0" + String(minute) : minute_str = String(minute);
    (second < 10) ? second_str = "0" + String(second) : second_str = String(second);


    stime_str = hour_str + ":" + minute_str + ":" + second_str;
  }

public:
  STimeClass(TFT_eSPI *tft) {
    this->tft = tft;
    previousTime = millis();
  }


  void loop() {
    if (millis() - previousTime > 1000) {   
      STimeUpdate();
      previousTime = millis();
    }  
  }

  int getHour() {
    return this->hour;
  }
  int getMinute() {
    return this->minute;
  }
  int getSecond() {
    return this->second;
  }

  String getSTime(){
    return this->stime_str;
  }
};

