#include "stdint.h"
// ------------------------- struct ----------------------

typedef struct Position_struct {
  float x;
  float y;
}Position;


// ------------------------- function ----------------------

float degree_to_radian(float degree) {
  return (degree * M_PI) / 180.0;
}

float radians_to_degree(float radians) {
  return radians * (180.0 / M_PI);
}


float debugPoint(float x,float y){
  tft.fillCircle(x,y,3,RED);
}
float debugPointClear(float x,float y){
  tft.fillCircle(x,y,3,BLACK);
}






