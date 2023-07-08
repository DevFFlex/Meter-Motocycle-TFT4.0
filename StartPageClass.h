
class StartPageClass {

private:
  TFT_eSPI *tft;

public:
  StartPageClass(TFT_eSPI *tft) {
    this->tft = tft;
  }

  void start() {

    Position line1, line2;
    line1.x = 0;
    line1.y = 40;
    line2.x = tft->width();
    line2.y = tft->height() - 60;
    float line_speed = 10;

    Position clsline1, clsline2;
    clsline1.x = 0;
    clsline1.y = 40;
    clsline2.x = tft->width();
    clsline2.y = tft->height() - 60;
    float clsline_speed = 1;


    Position center_point1, loop_circle1;
    center_point1.x = tft->width() / 2;
    center_point1.y = tft->height() / 2;
    float loop_circle_degree1 = 0;
    float loop_circle_l1 = 65;


    Position center_point2, loop_circle2;
    center_point2.x = tft->width() / 2;
    center_point2.y = tft->height() / 2;
    float loop_circle_degree2 = 90;
    float loop_circle_l2 = 30;




    for (int i = 0; i < 250; i++) {

      //circle 1
      loop_circle1.x = (cos(degree_to_radian(loop_circle_degree1)) * loop_circle_l1) + center_point1.x;
      loop_circle1.y = (sin(degree_to_radian(loop_circle_degree1)) * loop_circle_l1) + center_point1.y;
      loop_circle_degree1 += 1;

      tft->fillCircle(loop_circle1.x, loop_circle1.y, 10, tft->color565(0, 138, 255));

      //circle 2
      loop_circle2.x = (cos(degree_to_radian(loop_circle_degree2)) * loop_circle_l2) + center_point2.x;
      loop_circle2.y = (sin(degree_to_radian(loop_circle_degree2)) * loop_circle_l2) + center_point2.y;
      loop_circle_degree2 -= 1;

      tft->fillCircle(loop_circle2.x, loop_circle2.y, 5, tft->color565(255, 0, 0));


      //line 1 & 2
      tft->fillRect(line1.x, line1.y, 20, 20, tft->color565(255, 255, 255));
      tft->fillRect(line2.x, line2.y, 20, 20, tft->color565(0, 255, 0));

      line1.x += line_speed;
      line2.x -= line_speed;

      if (line1.x > tft->width() / 2) {
        tft->fillRect(clsline1.x, clsline1.y, 20, 20, tft->color565(0, 0, 0));
        clsline1.x += line_speed;
      }

      if (line2.x < tft->width() / 2) {
        tft->fillRect(clsline2.x, clsline2.y, 20, 20, tft->color565(0, 0, 0));
        clsline2.x -= line_speed;
      }

      if (line_speed > 1) line_speed -= 0.2;

      delay(10);
    }
  }
};



