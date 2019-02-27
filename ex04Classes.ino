#include <SPI.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
// Set up the TFT object
 
const int LOOP_SPEED = 25;
 
unsigned long primary_timer; //main loop timer
 
#define BACKGROUND TFT_BLACK
#define BALL_COLOR TFT_WHITE
 
const uint8_t BUTTON_PIN = 16; //CHANGE YOUR WIRING TO PIN 16!!! (FROM 19)
 
 
class Ball{
  float x_pos = 64; //x position
  float y_pos = 80; //y position
  float x_vel; //x velocity
  float y_vel; //y velocity
  float x_accel; //x acceleration
  float y_accel; //y acceleration
  int BALL_CLR;
  int BKGND_CLR;
  float MASS; //for starters
  int RADIUS; //radius of ball
  float K_FRICTION;  //friction coefficient
  float K_SPRING;  //spring coefficient
  int LEFT_LIMIT; //left side of screen limit
  int RIGHT_LIMIT; //right side of screen limit
  int TOP_LIMIT; //top of screen limit
  int BOTTOM_LIMIT; //bottom of screen limit
  int DT; //timing for integration
  public: //equals in declaration below are defaults!
  Ball(int rad=4, float ms=1, int ball_color=TFT_WHITE, int background_color=BACKGROUND){
    x_pos = 64; //x position
    y_pos = 80; //y position
    x_vel = 0; //x velocity
    y_vel = 0; //y velocity
    x_accel = 0; //x acceleration
    y_accel = 0; //y acceleration
    BALL_CLR = ball_color; //ball color
    BKGND_CLR = background_color;
    MASS = ms; //for starters
    RADIUS = rad; //radius of ball
    K_FRICTION = 0.05;  //friction coefficient
    K_SPRING = 0.9;  //spring coefficient
    LEFT_LIMIT =RADIUS; //left side of screen limit
    RIGHT_LIMIT = 127-RADIUS; //right side of screen limit
    TOP_LIMIT = RADIUS; //top of screen limit
    BOTTOM_LIMIT = 159-RADIUS; //bottom of screen limit
    DT = LOOP_SPEED; //timing for integration
  }
  void step(float x_force=0, float y_force=0){ //defaults to 0
    //update to your working ex02 implementation
    tft.drawCircle(x_pos, y_pos, RADIUS,BKGND_CLR);
    
  x_accel = (x_force - K_FRICTION * x_vel) / MASS;
  y_accel = (y_force - K_FRICTION * y_vel) / MASS;
  
  x_vel = x_vel + 0.001 * DT * x_accel; 
  y_vel = y_vel + 0.001 * DT * y_accel; 
 
  moveBall(); 

    tft.drawCircle(x_pos, y_pos, RADIUS,BALL_CLR);
  }
  
  void reset(int x=64, int y=32){
    x_pos = x;
    y_pos = y;
    x_vel = 0;
    y_vel = 0;
    x_accel = 0;
    y_accel = 0;
  }
  private:
  void moveBall(){
  
    float old_x_vel = x_vel;
    float old_y_vel = y_vel;
    float old_x_pos = x_pos;
    float old_y_pos = y_pos;
    
    float temp_pos_x = old_x_pos + x_vel * .001 * DT;
    float temp_pos_y = old_y_pos + y_vel * .001 * DT;
    
    if(temp_pos_x > RIGHT_LIMIT){
      x_pos = RIGHT_LIMIT - K_SPRING * (temp_pos_x - RIGHT_LIMIT);
      x_vel = -K_SPRING * old_x_vel;
      
    }else if(temp_pos_x < LEFT_LIMIT){
      x_pos = LEFT_LIMIT - K_SPRING * (temp_pos_x - LEFT_LIMIT);
      x_vel = -K_SPRING * old_x_vel;
      
    }else{
      x_pos = temp_pos_x;
    }
    
    
    if(temp_pos_y < TOP_LIMIT){
      y_pos = TOP_LIMIT - K_SPRING * (temp_pos_y - TOP_LIMIT);
      y_vel = -K_SPRING * old_y_vel;
      
    }else if(temp_pos_y > BOTTOM_LIMIT){
      y_pos = BOTTOM_LIMIT - K_SPRING * (temp_pos_y - BOTTOM_LIMIT);
      y_vel = -K_SPRING * old_y_vel;
      
    }else{
      y_pos = temp_pos_y;
    }
    
}
};
 
 Ball lots[20];
void setup() {
  Serial.begin(115200); //for debugging if needed.
  tft.init();
  tft.setRotation(2);
  tft.setTextSize(1);
  tft.fillScreen(BACKGROUND);
  randomSeed(analogRead(0));  //initialize random numbers
  for (int i=0; i<20; i++) {
    lots[i].reset();
  }
  delay(20); //wait 20 milliseconds
  for (int i=0; i<20; i++) {
    lots[i].step(random(-30000,30000),random(-30000,30000));
  }
  primary_timer = millis();
}
 
void loop() {
  for (int i=0; i<20; i++){
    lots[i].step();
  }
  while (millis()-primary_timer<LOOP_SPEED); //wait for primary timer to increment
  primary_timer = millis();
}
