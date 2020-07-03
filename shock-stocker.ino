void setup(void) {
  Serial.begin(115200);
  Serial.println("SHOCK STOCKER VERSION 0.12");
  Serial.println("5 SECONDS WAITING");
  pinMode(A0,OUTPUT);    // X
  pinMode(A1,OUTPUT);    // Y
  pinMode(A2,OUTPUT);    // Z
  digitalWrite(A0,LOW);
  digitalWrite(A1,LOW);
  digitalWrite(A2,LOW);
  delay(5000);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  Serial.println("FLUSH CAPACITORS");
  Serial.end();
}

#define LIMITV  150

void loop(void) {
  static int i=0,wx,wy,wz;
  boolean wxf,wyf,wzf;
  char txt[80];
  
  delay(1000);
  wx = analogRead(A0);
  wy = analogRead(A1);
  wz = analogRead(A2);
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
      pinMode(A0,OUTPUT);
      digitalWrite(A0,LOW);
      delay(5000);
      pinMode(A0,INPUT);
      Serial.println("FL-X");
    }
    if (wyf) {
      pinMode(A1,OUTPUT);
      digitalWrite(A1,LOW);
      delay(5000);
      pinMode(A1,INPUT);
      Serial.println("FL-Y");
    }
    if (wzf) {
      pinMode(A2,OUTPUT);
      digitalWrite(A2,LOW);
      delay(5000);
      pinMode(A2,INPUT);
      Serial.println("FL-Z");
    }
  }
  sprintf(txt,"DT,%d,%d,%d,%d",i,wx,wy,wz);
  Serial.println(txt);
  Serial.end();
  i++;
}
