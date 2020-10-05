//
//   Shock Stocker with ADS1115
//

#include <Wire.h>
#include <Adafruit_ADS1015.h>    //https://github.com/adafruit/Adafruit_ADS1X15

//#define CFACTOR 0.18750 // GAIN_TWOTHIRDS: +/-6.144V range: Calibration factor (mV/bit) for ADS1115 
#define CFACTOR 0.12500 // GAIN_ONE: +/-4.096V range: Calibration factor (mV/bit) for ADS1115
//#define CFACTOR 0.06250 // GAIN_TWO: +/-2.048V range: Calibration factor (mV/bit) for ADS1115
//#define CFACTOR 0.03125 // GAIN_FOUR: +/-1.024V range: Calibration factor (mV/bit) for ADS1115
//#define CFACTOR 0.01563 // GAIN_EIGHT: +/-/0.512V range: Calibration factor (mV/bit) for ADS1115
//#define CFACTOR 0.00781 // GAIN_SIXTEEN: +/-0.256V range: Calibration factor (mV/bit) for ADS1115

#define DCHGX  2  // Discharge Relay for X axis
#define DCHGY  3  // Discharge Relay for Y axis
#define DCHGZ  4  // Discharge Relay for Z axis

Adafruit_ADS1115 ads; // ADS1115 make instance  I2C Address 0x48

void setup(void) {
  Serial.begin(115200);
  Serial.println("SHOCK STOCKER VERSION 1.00");
  Serial.println("3 SECONDS WAITING");
  pinMode(DCHGX,OUTPUT);
  pinMode(DCHGY,OUTPUT);
  pinMode(DCHGZ,OUTPUT);
  digitalWrite(DCHGX,HIGH);
  digitalWrite(DCHGY,HIGH);
  digitalWrite(DCHGZ,HIGH);
  delay(3000);
  digitalWrite(DCHGX,LOW);
  digitalWrite(DCHGY,LOW);
  digitalWrite(DCHGZ,LOW);
  Serial.println("FLUSH CAPACITORS");
  /* ----- Setting of ADS1115 AD converter ----- */
  ads.setGain(GAIN_ONE);
  ads.begin();
  Serial.println("ADS1115 INITIALIZED");
  Serial.end();
}

#define LIMITV  30000  // 0x7530 91% for Fullscale

void loop(void) {
  static int i=0;
  int16_t wx,wy,wz; // The direct value read from ADS 
  boolean wxf,wyf,wzf;
  char txt[80];
  
  delay(1000);
  wx = ads.readADC_SingleEnded(0);
  wy = ads.readADC_SingleEnded(1);
  wz = ads.readADC_SingleEnded(2);
  if (wx>LIMITV) {
    wxf = true;
  } else {
    wxf = false;
  }
  if (wy>LIMITV) {
    wyf = true;
  } else {
    wyf = false;
  }
  if (wz>LIMITV) {
    wzf = true;
  } else {
    wzf = false;
  }
  Serial.begin(115200);
  if (wxf||wyf||wzf) {
    sprintf(txt,"OV,%d,%d,%d,%d",i,wx,wy,wz);
    Serial.print(txt);
    if (wxf) {
      digitalWrite(DCHGX,HIGH);
      delay(2000);
      digitalWrite(DCHGX,LOW);
      Serial.println("FL-X");
    }
    if (wyf) {
      digitalWrite(DCHGY,HIGH);
      delay(2000);
      digitalWrite(DCHGY,LOW);
      Serial.println("FL-Y");
    }
    if (wzf) {
      digitalWrite(DCHGZ,HIGH);
      delay(2000);
      digitalWrite(DCHGZ,LOW);
      Serial.println("FL-Z");
    }
  }
  sprintf(txt,"DT,%d,%d,%d,%d",i,wx,wy,wz);
  Serial.println(txt);
  Serial.end();
  i++;
}
