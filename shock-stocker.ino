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
int16_t pwx,pwy,pwz;  // The past value  from ADS 

void setup(void) {
  Serial.begin(115200);
  Serial.println("SHOCK STOCKER VERSION 1.10");
  Serial.println("1 SECONDS WAITING");
  pinMode(DCHGX,OUTPUT);
  pinMode(DCHGY,OUTPUT);
  pinMode(DCHGZ,OUTPUT);
  digitalWrite(DCHGX,HIGH);
  delay(300);
  digitalWrite(DCHGX,LOW);
  digitalWrite(DCHGY,HIGH);
  delay(300);
  digitalWrite(DCHGY,LOW);
  digitalWrite(DCHGZ,HIGH);
  delay(300);
  digitalWrite(DCHGZ,LOW);
  Serial.println("FLUSH CAPACITORS");
  /* ----- Setting of ADS1115 AD converter ----- */
  ads.setGain(GAIN_ONE);
  ads.begin();
  Serial.println("ADS1115 INITIALIZED");
  Serial.end();
  pwx = ads.readADC_SingleEnded(0);
  if (pwx<0) pwx=0;
  pwy = ads.readADC_SingleEnded(1);
  if (pwy<0) pwy=0;
  pwz = ads.readADC_SingleEnded(2);
  if (pwz<0) pwz=0;
}

#define LIMITV  30000  // 0x7530 91% for Fullscale

void loop(void) {
  static int i=0;
  int16_t wx,wy,wz; // The direct value read from ADS 
  int16_t dwx,dwy,dwz; // The different value 
  boolean wxf,wyf,wzf;
  char txt[80];
  
  delay(1000);
  wx = ads.readADC_SingleEnded(0);
  wy = ads.readADC_SingleEnded(1);
  wz = ads.readADC_SingleEnded(2);
  dwx = wx - pwx;  pwx = wx;
  if (dwx<0) dwx = 0;
  dwy = wy - pwy;  pwy = wy;
  if (dwy<0) dwy = 0;
  dwz = wz - pwz;  pwz = wz;
  if (dwz<0) dwz = 0;
  
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
      delay(200);
      digitalWrite(DCHGX,LOW);
      Serial.println("FL-X");
    }
    if (wyf) {
      digitalWrite(DCHGY,HIGH);
      delay(200);
      digitalWrite(DCHGY,LOW);
      Serial.println("FL-Y");
    }
    if (wzf) {
      digitalWrite(DCHGZ,HIGH);
      delay(200);
      digitalWrite(DCHGZ,LOW);
      Serial.println("FL-Z");
    }
  }
  sprintf(txt,"DT,%d,%d,%d,%d,%d,%d,%d",i,dwx,dwy,dwz,wx,wy,wz);
  Serial.println(txt);
  Serial.end();
  i++;
}
