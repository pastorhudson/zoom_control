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

//****************define the min, max and center you want for your joystick here*******//
int val;                      // variable to read the value from the analog pin
int center_calibration = 512; // This is what your natural center is
int top_calibration = 1023;   // Top maximum value
int bottom_calibration = 1; // Bottom minimum value
int dead_zone = 4;            // How much play should the stick have before it reads above or below values

Servo myservo;  // create servo object to control a servo

void setup() {
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
  
  Serial.println("Please place stick at center");
  data.center_calibration = get_calibration();
  Serial.println("Center Calibration set: ");
  Serial.println(data.center_calibration);
  Serial.println("Please place stick at top");
  data.top_calibration = get_calibration();
  Serial.println("Top Calibration set: ");
  Serial.println(data.top_calibration);
  Serial.println("Please place stick at bottom");
  data.bottom_calibration = get_calibration();
  Serial.println("Bottom Calibration set: ");
  Serial.println(data.bottom_calibration);

//  if (center_calibration > bottom_calibration < top_calibration) {
//    return true
//  }
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
    calibration += analogRead(joystick_pin);
    delay(5);
  }
  calibration /=  100;
  return calibration;
}
