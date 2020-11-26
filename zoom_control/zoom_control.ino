#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int joystick_pin = 0;  // analog pin used to connect the joystick
int val;    // variable to read the value from the analog pin
int center_calibration = 761;
int top_calibration = 1024;
int bottom_calibration = 103;
int dead_zone = 2;



void setup() {
//  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
Serial.begin(115200);
}

void loop() {
  int bottom_corrected = 0;
  int top_corrected = 0;
  
  val = analogRead(joystick_pin);            // reads the value of the potentiometer (value between 0 and 1023)
  
  if (val <= center_calibration - dead_zone)
  {
    Serial.print("\nBottom End Value: ");
    val = map(val, bottom_calibration, center_calibration, 80, 90);
//    Serial.print(bottom_corrected);
//    Serial.print(":");
    
  }
  else if (val >= center_calibration + dead_zone)
  {
    Serial.print("\nTop End Value: ");
    val = map(val, center_calibration, top_calibration, 90, 100);
//    Serial.print(top_corrected);
//    Serial.print(":");
  }
  else {
    Serial.print("\nCenter Value: ");
    val = 90;
  }
  
 
  Serial.println(val);
  delay(200);
//  myservo.write(val);                  // sets the servo position according to the scaled value
  delay(15);                           // waits for the servo to get there
}


bool calibrate () {
  Serial.println("Please place stick at center");
  
  
}
