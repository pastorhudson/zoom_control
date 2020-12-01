//  **********************************************************************//
//  author: Ron Hudson                                                 //
//  date:   11-25-2020                                                      //
//  version: 0.1                                                          //
//  **********************************************************************//
//
//  This code is free to use, edit and distrubate as long as you use it for non profit projects

#include <Servo.h>
#include <EEPROM.h>
#include "Calibration.h"


//*****************define your pins here***********************************************//
#define button 16        // button on gpio16 D0
#define joystick_pin 0 // analog pin used to connect the joystick
#define top_green_pin 14
#define top_red_pin 12
#define bottom_green_pin 13
#define bottom_red_pin 15

//****************define the min, max and center you want for your joystick here*******//
int val;                      // variable to read the value from the analog pin
int center_calibration = 512; // This is what your natural center is
int top_calibration = 1023;   // Top maximum value
int bottom_calibration = 1; // Bottom minimum value
int dead_zone = 4;            // How much play should the stick have before it reads above or below values



Servo myservo;  // create servo object to control a servo

struct Ledstate {
  bool top_red = false;
  bool top_green = false;
  bool bottom_red = false;
  bool bottom_green = false;
}lstate;


void setup() {
  pinMode(top_red_pin, OUTPUT);
  pinMode(top_green_pin, OUTPUT);
  pinMode(bottom_red_pin, OUTPUT);
  pinMode(bottom_green_pin, OUTPUT);
  set_led_state();
  myservo.attach(4);  // attaches the servo on pin D2 to the servo object
  Serial.begin(115200);

  data = get_data();
    Serial.println("Old values are: "+String(data.center_calibration)+", "+String(data.top_calibration)+", "+String(data.bottom_calibration));
  if (not data.calibrated) {
    calibrate();
  }

  
}

void loop() {
  
  val = analogRead(joystick_pin);            // reads the value of the potentiometer (value between 0 and 1023)
  Serial.println(val);
  Serial.println();

  if (val < data.center_calibration - dead_zone)
  {
    
    Serial.print("\nBottom End Value: ");
    val = map(val, data.bottom_calibration - dead_zone, center_calibration, 70, 90);
    
  }
  
  else if (val > data.center_calibration + dead_zone)
  {
    
    Serial.print("\nTop End Value: ");
    val = map(val, center_calibration + dead_zone, top_calibration, 90, 110);
    
  }
  
  else {
    
    Serial.print("\nCenter Value: ");
    val = 90;
    
  }
  
  Serial.println(val);
  Serial.println("Old values are: "+String(data.center_calibration)+", "+String(data.top_calibration)+", "+String(data.bottom_calibration));

  myservo.write(val);                  // sets the servo position according to the scaled value
  delay(15);                           // waits for the servo to get there
  
  if (digitalRead(button) == LOW) { // Listen for calibration
  calibrate();
  }

}


bool calibrate () {
  
  lstate.top_red = true;
  lstate.top_green = false;
  lstate.bottom_red = true;
  lstate.bottom_green = false;
  set_led_state();
  Serial.println("Please place stick at center");
  data.center_calibration = get_calibration();
  Serial.println("Center Calibration set: ");
  Serial.println(data.center_calibration);
  
  lstate.top_red = true;
  lstate.top_green = false;
  lstate.bottom_red = false;
  lstate.bottom_green = false;
  set_led_state();
  Serial.println("Please place stick at top");
  data.top_calibration = get_calibration();
  Serial.println("Top Calibration set: ");
  Serial.println(data.top_calibration);

  lstate.top_red = false;
  lstate.top_green = false;
  lstate.bottom_red = true;
  lstate.bottom_green = false;
  set_led_state();
  Serial.println("Please place stick at bottom");
  data.bottom_calibration = get_calibration();
  Serial.println("Bottom Calibration set: ");
  Serial.println(data.bottom_calibration);

  led_off();
  lstate.bottom_green = true;
  lstate.top_green = true;
  set_led_state();

  for (int i = 0; i < 7; i++) {    //take 100 readings
  lstate.bottom_green = !lstate.bottom_green;
  lstate.top_green = !lstate.top_green;
  
  set_led_state();
  delay(300);
  }

  data.calibrated = true;
  set_calibration(&data);
  return true;
}


int get_calibration() {
  int calibration = 0;    //reset the values

  delay(2500);
  Serial.print("calibrating position");
   for (int i = 0; i < 100; i++) {    //take 100 readings
    Serial.print(".");
    if ( (i % 10) == 0) {
      if (lstate.top_red) {
        digitalWrite(top_red_pin, HIGH);
      }
      if (lstate.bottom_red) {
        digitalWrite(bottom_red_pin, HIGH);
      }
    }
    else {
      digitalWrite(top_red_pin, LOW);
      digitalWrite(bottom_red_pin, LOW);
      digitalWrite(top_green_pin, LOW);
      digitalWrite(bottom_green_pin, LOW);
    }
    calibration += analogRead(joystick_pin);
    delay(5);
  }
  calibration /=  100;
  return calibration;
}

void set_led_state(){
  for (int i = 0; i < 4; i++) {
    if (lstate.top_red){      
      digitalWrite(top_red_pin, HIGH);
    }
    else {
      digitalWrite(top_red_pin, LOW);
    }
    
    if (lstate.bottom_red){      
      digitalWrite(bottom_red_pin, HIGH);
    }
    else {
      digitalWrite(bottom_red_pin, LOW);
    }
    
    if (lstate.top_green){      
      digitalWrite(top_green_pin, HIGH);
    }
    else {
      digitalWrite(top_green_pin, LOW);
    }
    
    if (lstate.bottom_green){      
      digitalWrite(bottom_green_pin, HIGH);
    }
    else {
      digitalWrite(bottom_green_pin, LOW);
    }

  }
  return;
}

void led_off() {
  lstate.top_red = false;
  lstate.bottom_red = false;
  lstate.top_green = false;
  lstate.bottom_green = false;
  set_led_state();
}
