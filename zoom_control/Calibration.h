uint addr = 0;


// data structure

struct calibration {
    uint center_calibration = 512; // This is what your natural center is
    uint top_calibration = 1023;   // Top minimum value
    uint bottom_calibration = 1; // Bottom minimum value
    bool calibrated = false;
  } data;


calibration get_data() {

  // commit 512 bytes of ESP8266 flash (for "EEPROM" emulation)
  // this step actually loads the content (512 bytes) of flash into 
  // a 512-byte-array cache in RAM
  EEPROM.begin(512);

  // read bytes (i.e. sizeof(data) from "EEPROM"),
  // in reality, reads from byte-array cache
  // cast bytes into structure called data
  EEPROM.get(addr,data);
  if (data.calibrated) {
    
    return data;

  } 
  else {
    
    data.center_calibration = 512;
    data.top_calibration = 1023;
    data.bottom_calibration = 1;
    return data;

    }
  
}

bool set_calibration(calibration *ptr) {

  Serial.println("NEW values are: "+String(ptr->center_calibration)+", "+String(ptr->top_calibration)+", "+String(ptr->bottom_calibration));

  // replace values in byte-array cache with modified data
  // no changes made to flash, all in local byte-array cache
  EEPROM.put(addr,data);

  // actually write the content of byte-array cache to
  // hardware flash.  flash write occurs if and only if one or more byte
  // in byte-array cache has been changed, but if so, ALL 512 bytes are 
  // written to flash
  EEPROM.commit(); 

  if (data.calibrated) {
    return true;  
  }
  return false;
    
}
