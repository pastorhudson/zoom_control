//  **********************************************************************//
//  author: Ron Hudson                                                 //
//  date:   11-25-2020                                                      //
//  version: 0.1                                                          //
//  **********************************************************************//
//
//  This code is free to use, edit and distrubate as long as you use it for non profit projects

#include <Servo.h>


//*****************define your pins here***********************************************//
#define button 16        // button on gpio16 D0
#define joystick_pin 0 // analog pin used to connect the joystick

//****************define the min, max and center you want for your joystick here*******//
int val;                      // variable to read the value from the analog pin
int center_calibration = 512; // This is what your natural center is
int top_calibration = 0;   // Top minimum value
int bottom_calibration = 0; // Bottom minimum value
int dead_zone = 2;            // How much play should the stick have before it reads above or below values

Servo myservo;  // create servo object to control a servo

void setup() {
//  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
Serial.begin(115200);
}

void loop() {
  
  val = analogRead(joystick_pin);            // reads the value of the potentiometer (value between 0 and 1023)

  if (val <= center_calibration - dead_zone)
  {
    
    Serial.print("\nBottom End Value: ");
    val = map(val, bottom_calibration, center_calibration, 80, 90);
    
  }
  
  else if (val >= center_calibration + dead_zone)
  {
    
    Serial.print("\nTop End Value: ");
    val = map(val, center_calibration, top_calibration, 90, 100);
    
  }
  
  else {
    
    Serial.print("\nCenter Value: ");
    val = 90;
    
  }
  
 
  Serial.println(val);
//  myservo.write(val);                  // sets the servo position according to the scaled value
  delay(15);                           // waits for the servo to get there
  
  if (digitalRead(button) == LOW) { // Listen for calibration
  calibrate();
  }

}


bool calibrate () {
  Serial.println("Please place stick at center");
  center_calibration = get_calibration();
  Serial.println("Center Calibration set: ");
  Serial.println(center_calibration);
  Serial.println("Please place stick at top");
  top_calibration = get_calibration();
  Serial.println("Top Calibration set: ");
  Serial.println(top_calibration);
  Serial.println("Please place stick at bottom");
  bottom_calibration = get_calibration();
  Serial.println("Bottom Calibration set: ");
  Serial.println(bottom_calibration);

//  if (center_calibration > bottom_calibration < top_calibration) {
//    return true
//  }

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
