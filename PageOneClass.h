#include "esp32-hal.h"

struct ObjScope {

  float x1;
  float y1;
  float width;
  float height;
  float x2;
  float y2;

};



class PageOneClass {




private:  //------------------------------------------------------------------------------------ Private ------------------------------------
  TFT_eSPI *tft;
  BatteryClass *batteryClass;
  TemperatureClass *temperatureClass;
  STimeClass *stimeClass;

  bool MeterDraw_Animation_isRun = true;
  void meterDraw_animation() {
    static int16_t center_point[] = { tft->width() / 2, tft->height() / 2 };
    static int16_t point_pos[] = { center_point[0], center_point[1] + 250 };
    static float degree = 180, degree_count = 30, l = 300, red = 255, green = 255, blue = 255;

    float line_pos[] = { point_pos[0], point_pos[1], cos(degree_to_radian(degree + degree_count)) * l + point_pos[0], sin(degree_to_radian(degree + degree_count)) * l + point_pos[1] };

    for (int i = 0; i < 50; i++) tft->drawPixel(line_pos[2] + i, line_pos[3] + i, tft->color565(red, green, blue));





    if (degree_count > 180 - 30) MeterDraw_Animation_isRun = false;
    else {
      degree_count += 0.1;
      if (red > 0 ) red -= 0.4;
      if (GREEN > 0) {
        green -= 0.05;
      }
    }
  }


  //------------------------------------- Battery ------------------------------------

  int batteryValTrue = 50;
  float batteryVal = 0;
  float batteryValDirect = 1;
  unsigned long previousTimeForBatteryGuiUpdate;
  float delayForBatteryGuiUpdate = 10;
  float pitchSize = 5;
  bool oncRun_Battery = true;
  float battery_pos[4];
  float battery_pos2[4];
  float batteryCursor = 0;
  String sout = "";
  void battery_animation() {

    //run one time
    if (oncRun_Battery) {
      oncRun_Battery = false;

      battery_pos[0] = tft->width() - 100;                    //x
      battery_pos[1] = 10;                                    //y
      battery_pos[2] = (tft->width() - battery_pos[0]) - 10;  // width
      battery_pos[3] = 30;                                    // height

      battery_pos2[0] = battery_pos[0] + pitchSize;
      battery_pos2[1] = battery_pos[1] + pitchSize;
      battery_pos2[2] = battery_pos[2] - (pitchSize * 2);
      battery_pos2[3] = battery_pos[3] - (pitchSize * 2);

      float batteryCursor = 0;

      sout = "Battery";
      tft->setTextSize(2);
      tft->setTextColor(YELLOW);
      tft->setCursor(battery_pos[0] + ((battery_pos[2] / 2) - (tft->textWidth(sout) / 2)), battery_pos[3] + 20);
      tft->print(sout);

      sout = "12 V";
      tft->setTextSize(2);
      tft->setTextColor(GREEN);
      tft->setCursor(battery_pos[0] + ((battery_pos[2] / 2) - (tft->textWidth(sout) / 2)), battery_pos[3] + 40);
      tft->print(sout);

      sout = "1 A";
      tft->setTextSize(2);
      tft->setTextColor(SKY);
      tft->setCursor(battery_pos[0] + ((battery_pos[2] / 2) - (tft->textWidth(sout) / 2)), battery_pos[3] + 60);
      tft->print(sout);
    }


    if (millis() - previousTimeForBatteryGuiUpdate > delayForBatteryGuiUpdate) {
      previousTimeForBatteryGuiUpdate = millis();

      if (batteryVal < 100) batteryVal += batteryValDirect;
      Serial.println(batteryVal);

      tft->drawRect(battery_pos[0], battery_pos[1], battery_pos[2], battery_pos[3], WHITE);

      // (batteryVal < 30) ? tft->fillRect(battery_pos2[0], battery_pos2[1], battery_pos2[2], battery_pos2[3], RED) : tft->fillRect(battery_pos2[0], battery_pos2[1], battery_pos2[2], battery_pos2[3], GREEN);

      batteryCursor = map(batteryVal, 0, 100, battery_pos2[0], battery_pos2[0] + battery_pos2[2]);

      for (int i = 0; i < battery_pos2[3]; i++) {
        if (batteryValDirect > 0) {
          tft->drawPixel(batteryCursor, battery_pos2[1] + i, GREEN);
        } else if (batteryValDirect < 0) {
          tft->drawPixel(batteryCursor, battery_pos2[1] + i, BLACK);
        }
      }

      sout = String(batteryVal);
      tft->setTextSize(1.5);
      tft->setTextColor(WHITE, BLACK);
      tft->setCursor(battery_pos[0] + (battery_pos[2] / 2) - (tft->textWidth(sout) / 2), battery_pos[1] + 10);
      tft->print(sout);
    }
  }

  void battery_run() {


    if (Serial.available()) {
      String data = Serial.readString();
      int dataInt = data.toInt();
      if (dataInt > 100) batteryValTrue = 100;
      else if (dataInt < 0) batteryValTrue = 0;
      else batteryValTrue = dataInt;
    }

    if (millis() - previousTimeForBatteryGuiUpdate > delayForBatteryGuiUpdate) {
      previousTimeForBatteryGuiUpdate = millis();
      batteryValTrue = batteryClass->getBatteryPersen();

      for (int i = 0; i < battery_pos2[3]; i++) {
        if (batteryValDirect > 0) {
          tft->drawPixel(batteryCursor - 1, battery_pos2[1] + i, GREEN);
          tft->drawPixel(batteryCursor, battery_pos2[1] + i, GREEN);
        } else if (batteryValDirect < 0) {
          tft->drawPixel(batteryCursor + 1, battery_pos2[1] + i, BLACK);
          tft->drawPixel(batteryCursor, battery_pos2[1] + i, BLACK);
        }
      }

      if (batteryVal < batteryValTrue) batteryValDirect = 1;
      else if (batteryVal > batteryValTrue) batteryValDirect = -1;


      if (batteryVal != batteryValTrue) batteryVal += batteryValDirect;
      Serial.println(batteryVal);

      tft->drawRect(battery_pos[0], battery_pos[1], battery_pos[2], battery_pos[3], WHITE);

      // (batteryVal < 30) ? tft->fillRect(battery_pos2[0], battery_pos2[1], battery_pos2[2], battery_pos2[3], RED) : tft->fillRect(battery_pos2[0], battery_pos2[1], battery_pos2[2], battery_pos2[3], GREEN);

      batteryCursor = map(batteryVal, 0, 100, battery_pos2[0], battery_pos2[0] + battery_pos2[2]);


      sout = String(batteryVal);
      tft->setTextSize(1.5);
      tft->setTextColor(WHITE, BLACK);
      tft->setCursor(battery_pos[0] + (battery_pos[2] / 2) - (tft->textWidth(sout) / 2), battery_pos[1] + 10);
      tft->print(sout);
    }
  }


  //------------------------------------- STime ------------------------------------

  String stimeOut = "";
  char c[8] = { '0', '0', ':', '0', '0', ':', '0', '0' };
  byte stime_i = 0;
  int slide = 0;
  int slide_direct = 1;
  int slide_speed = 1;
  unsigned long previousTimeForSTimeGuiUpdate;
  void stime_animation() {

    if (millis() - previousTimeForSTimeGuiUpdate > 100) {
      tft->setTextColor(WHITE, BLACK);
      tft->setTextSize(3);
      tft->setCursor(tft->width() / 2 - (tft->textWidth(stimeOut) / 2), 50);
      // String textOut = String((char)random(48,58)) + String((char)random(48,58)) + ":" + String((char)random(48,58)) + String((char)random(48,58)) + ":" + String((char)random(48,58)) + String((char)random(48,58));
      tft->print(stimeOut);
      if (stime_i < 8) stimeOut += String(c[stime_i++]);


      tft->fillRect(
        tft->width() / 2 - (tft->textWidth(stimeOut) / 2),
        50 + 30,
        (tft->textWidth(stimeOut) / 2),
        15,
        tft->color565(255, 0, 200));

      tft->fillRect(
        tft->width() / 2,
        50 - 30,
        (tft->textWidth(stimeOut) / 2),
        15,
        tft->color565(0, 251, 255));

      previousTimeForSTimeGuiUpdate = millis();
    }
  }

  void stime_run() {
    tft->setTextColor(WHITE, BLACK);
    tft->setTextSize(3);
    tft->setCursor(tft->width() / 2 - (tft->textWidth(stimeClass->getSTime()) / 2), 50);
    tft->print(stimeClass->getSTime());


    if (millis() - previousTimeForSTimeGuiUpdate > 20) {

      // //clear pink
      // tft->fillRect(
      //   tft->width() / 2 - (tft->textWidth(stimeOut) / 2) - 50,
      //   50 + 30,
      //   tft->width() / 2,
      //   15,
      //   BLACK
      // );

      // //clear sky
      // tft->fillRect(
      //   tft->width() / 2,
      //   50 - 30,
      //   (tft->textWidth(stimeOut) / 2) + 50,
      //   15,
      //   BLACK
      // );



      if (slide < 0 || slide > 50) slide_direct *= -1;
      slide += slide_direct * slide_speed;

      tft->fillRect(
        tft->width() / 2 - (tft->textWidth(stimeOut) / 2) - slide,
        50 + 30,
        (tft->textWidth(stimeOut) / 2),
        15,
        (slide_direct < 0) ? tft->color565(0, 0, 0) : tft->color565(255, 0, 200));

      tft->fillRect(
        tft->width() / 2,
        50 - 30,
        (tft->textWidth(stimeOut) / 2) + slide,
        15,
        (slide_direct < 0) ? tft->color565(0, 0, 0) : tft->color565(0, 251, 255));

      previousTimeForSTimeGuiUpdate = millis();
    }
  }


  //------------------------------------- Temperature ------------------------------------

  String tempPattern = "Temperature";
  String tempOut = "";
  byte temp_i = 0;
  int tempRefreshTime = 50;
  bool isLoadFinish = false;
  float temp_valC = 30;
  unsigned long previousTimeForTempGuiUpdate;
  void temp_animation() {
    if (millis() - previousTimeForTempGuiUpdate > tempRefreshTime) {
      tft->setTextColor(WHITE, BLACK);
      tft->setTextSize(2);
      tft->setCursor(0, 0);
      tft->print(tempOut);
      if (temp_i < 13) {
        tempOut += String(tempPattern[temp_i++]);
      } else {
        isLoadFinish = true;
      }

      if (isLoadFinish) {
        float textPTCenter = tft->textWidth(tempPattern) / 2;

        String s1 = String(temp_valC) + "C";
        tft->setTextColor(ORANGE, BLACK);
        tft->setTextSize(2);
        tft->setCursor(textPTCenter - tft->textWidth(s1) / 2, 20);
        tft->print(s1);

        String s2 = String((temp_valC * 1.8) + 32) + "F";
        tft->setTextColor(SKY, BLACK);
        tft->setTextSize(2);
        tft->setCursor(textPTCenter - tft->textWidth(s2) / 2, 40);
        tft->print(s2);

        String s3 = String(temp_valC + 273.15) + "K";
        tft->setTextColor(YELLOW, BLACK);
        tft->setTextSize(2);
        tft->setCursor(textPTCenter - tft->textWidth(s3) / 2, 60);
        tft->print(s3);

        isLoadFinish = false;
      }

      previousTimeForTempGuiUpdate = millis();
    }
  }
  void temp_run() {
    temp_valC = temperatureClass->getTempC();
    tempRefreshTime = 1000;
    temp_animation();
  }


  //------------------------------------- Gyro ------------------------------------
  ObjScope gyro_GraphicBox;
  ObjScope gyro_TextBox;
  float baseLine_position[4];
  float gyroGate_position[4];
  float degreeGate = 30;
  float degreeGateDirect = 1;
  String gyro_str = "";
  String gyro_str_rDegree = "";
  String gyro_str_lDegree = "";
  unsigned long previousTimeForGyroGuiUpdate;
  float delayForGyroGuiUpdate = 30;
  bool oncRun_Gyro = true;
  void gyro_animation() {

    if (oncRun_Gyro) {
      oncRun_Gyro = false;
      tft->drawLine(tft->width() / 2, 0, tft->width() / 2, tft->height(), RED);
      

      // ----------------- Graphic Box ----------------
      gyro_GraphicBox.x1 = (tft->width() / 2) + 10;
      gyro_GraphicBox.y1 = (tft->height() / 2) + 30 + 20;
      gyro_GraphicBox.width = 150;
      gyro_GraphicBox.height = 125 - 20;
      gyro_GraphicBox.x2 = gyro_GraphicBox.x1 + gyro_GraphicBox.width;
      gyro_GraphicBox.y2 = gyro_GraphicBox.y1 + gyro_GraphicBox.height;

      baseLine_position[0] = gyro_GraphicBox.x1;
      baseLine_position[1] = gyro_GraphicBox.y2 - 20;
      baseLine_position[2] = gyro_GraphicBox.width;
      baseLine_position[3] = 10;

      gyroGate_position[0] = baseLine_position[0] + (gyro_GraphicBox.width /2);
      gyroGate_position[1] = baseLine_position[1];
      gyroGate_position[2] = gyroGate_position[0] + ((gyro_GraphicBox.width /2) * cos(degree_to_radian(-degreeGate)));
      gyroGate_position[3] = gyroGate_position[1] + ((gyro_GraphicBox.width /2) * sin(degree_to_radian(-degreeGate)));

      // tft->drawRect(gyro_GraphicBox.x1,gyro_GraphicBox.y1,gyro_GraphicBox.width,gyro_GraphicBox.height,YELLOW);

      tft->fillRect(baseLine_position[0],baseLine_position[1],baseLine_position[2],baseLine_position[3],WHITE);

      tft->drawLine(gyroGate_position[0],gyroGate_position[1],gyroGate_position[2],gyroGate_position[3],GREEN);

      debugPoint(gyroGate_position[0],gyroGate_position[1]);
      debugPoint(gyroGate_position[2],gyroGate_position[3]);

       // ----------------- Text Box ----------------
      gyro_TextBox.x1 = gyro_GraphicBox.x2 + 5;
      gyro_TextBox.y1 = gyro_GraphicBox.y1 + 25;
      gyro_TextBox.width = 70;
      gyro_TextBox.height = 80;
      gyro_TextBox.x2 = gyro_TextBox.x1 + gyro_TextBox.width;
      gyro_TextBox.y2 = gyro_TextBox.y1 + gyro_TextBox.height;

      tft->drawRect(gyro_TextBox.x1,gyro_TextBox.y1,gyro_TextBox.width,gyro_TextBox.height,YELLOW);

      gyro_str = "R:";
      gyro_str_rDegree = (degreeGate <= 90) ? (90 - (int)degreeGate < 10) ? "0"+String(90 - (int)degreeGate) : String(90 - (int)degreeGate) : "00";
      tft->setTextColor(WHITE,BLACK);
      tft->setTextSize(2);
      tft->setCursor(gyro_TextBox.x1 + 10 ,gyro_TextBox.y1 + 20);
      tft->print(gyro_str);
      tft->setTextColor(SKY,BLACK);
      tft->print("00");

      gyro_str = "L:";
      gyro_str_lDegree = (degreeGate > 90) ? ((int)degreeGate - 90 < 10) ? "0"+String((int)degreeGate - 90) : String((int)degreeGate - 90) : "00";
      tft->setTextColor(WHITE,BLACK);
      tft->setTextSize(2);
      tft->setCursor(gyro_TextBox.x1 + 10 ,gyro_TextBox.y1 + 40);
      tft->print(gyro_str);
      tft->setTextColor(SKY,BLACK);
      tft->print("00");
      
    }


    if (millis() - previousTimeForGyroGuiUpdate > delayForGyroGuiUpdate){
      previousTimeForGyroGuiUpdate = millis();

      debugPointClear(gyroGate_position[0],gyroGate_position[1]);
      debugPointClear(gyroGate_position[2],gyroGate_position[3]);
      tft->drawLine(gyroGate_position[0],gyroGate_position[1],gyroGate_position[2],gyroGate_position[3],BLACK);

      degreeGate+=degreeGateDirect;


      gyroGate_position[2] = gyroGate_position[0] + ((gyro_GraphicBox.width /2) * cos(degree_to_radian(-degreeGate)));
      gyroGate_position[3] = gyroGate_position[1] + ((gyro_GraphicBox.width /2) * sin(degree_to_radian(-degreeGate)));

      tft->drawLine(gyroGate_position[0],gyroGate_position[1],gyroGate_position[2],gyroGate_position[3],GREEN);
      debugPoint(gyroGate_position[0],gyroGate_position[1]);
      debugPoint(gyroGate_position[2],gyroGate_position[3]);





      
    }
  }

  void gyro_run() {

    if (millis() - previousTimeForGyroGuiUpdate > delayForGyroGuiUpdate){
      previousTimeForGyroGuiUpdate = millis();

      debugPointClear(gyroGate_position[0],gyroGate_position[1]);
      debugPointClear(gyroGate_position[2],gyroGate_position[3]);
      tft->drawLine(gyroGate_position[0],gyroGate_position[1],gyroGate_position[2],gyroGate_position[3],BLACK);

      if(degreeGate < 0 || degreeGate > 180)degreeGateDirect *= -1;
      
      degreeGate+=degreeGateDirect;

      gyroGate_position[2] = gyroGate_position[0] + ((gyro_GraphicBox.width /2) * cos(degree_to_radian(-degreeGate)));
      gyroGate_position[3] = gyroGate_position[1] + ((gyro_GraphicBox.width /2) * sin(degree_to_radian(-degreeGate)));

      tft->drawLine(gyroGate_position[0],gyroGate_position[1],gyroGate_position[2],gyroGate_position[3],GREEN);
      debugPoint(gyroGate_position[0],gyroGate_position[1]);
      debugPoint(gyroGate_position[2],gyroGate_position[3]);


      //----------------------------textUpdate----------------------------------

      gyro_str = "R:";
      gyro_str_rDegree = (degreeGate <= 90) ? (90 - (int)degreeGate < 10) ? "0"+String(90 - (int)degreeGate) : String(90 - (int)degreeGate) : "00";
      tft->setTextColor(WHITE,BLACK);
      tft->setTextSize(2);
      tft->setCursor(gyro_TextBox.x1 + 10 ,gyro_TextBox.y1 + 20);
      tft->print(gyro_str);
      tft->setTextColor(SKY,BLACK);
      tft->print(gyro_str_rDegree);

      gyro_str = "L:";
      gyro_str_lDegree = (degreeGate > 90) ? ((int)degreeGate - 90 < 10) ? "0"+String((int)degreeGate - 90) : String((int)degreeGate - 90) : "00";
      tft->setTextColor(WHITE,BLACK);
      tft->setTextSize(2);
      tft->setCursor(gyro_TextBox.x1 + 10 ,gyro_TextBox.y1 + 40);
      tft->print(gyro_str);
      tft->setTextColor(SKY,BLACK);
      tft->print(gyro_str_lDegree);
    }



  }











public:  //------------------------------------------------------------------------------------ Public ------------------------------------

  PageOneClass(TFT_eSPI *tft, BatteryClass *batteryClass, TemperatureClass *temperatureClass, STimeClass *stimeClass) {
    this->tft = tft;
    this->batteryClass = batteryClass;
    this->temperatureClass = temperatureClass;
    this->stimeClass = stimeClass;

    previousTimeForBatteryGuiUpdate = millis();
    previousTimeForSTimeGuiUpdate = millis();
    previousTimeForTempGuiUpdate = millis();
    previousTimeForGyroGuiUpdate = millis();

  }

  void setup() {
    while (MeterDraw_Animation_isRun) {
      meterDraw_animation();
      battery_animation();
      stime_animation();
      temp_animation();
      gyro_animation();
    }
  }

  void loop() {
    stime_run();
    temp_run();
    battery_run();
    gyro_run();
  }
};