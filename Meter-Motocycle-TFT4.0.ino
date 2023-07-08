#include "SPI.h"
#include "TFT_eSPI.h"
#include "timer.h"
#include <Ticker.h>

TFT_eSPI tft = TFT_eSPI();

#include "Var.h"
#include "functionExtension.h"
#include "pic1.h"
#include "TimerFunc.h"
#include "TemperatureFunc.h"
#include "BatteryFunc.h"

#include "StartPageClass.h"
#include "PageOneClass.h"


BatteryClass batteryClass(&tft);
TemperatureClass temperatureClass(&tft);
STimeClass stimeClass(&tft);

StartPageClass SPC(&tft);
PageOneClass POC(&tft,&batteryClass,&temperatureClass,&stimeClass);

void setup() {


  Serial.begin(115200);
  Serial.setTimeout(50);
  tft.init();
  // tft.setFreeFont(FF21);
  tft.setRotation(3);
  tft.fillScreen(BLACK);
  SPC.start();
  tft.fillScreen(BLACK);

  
  POC.setup();
}

void loop() {
  batteryClass.loop();
  temperatureClass.loop();
  stimeClass.loop();


  POC.loop();
}
















