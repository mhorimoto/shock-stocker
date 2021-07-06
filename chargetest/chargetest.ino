/*
  コンデンサの充放電をテストする。
  ArduinoのデジタルポートのINPUT/OUTPUTを切り替えて
  充放電させる。
*/

int mb,dcb,chb;
int pmb,pdcb,pchb;
int va1;

void setup(void) {
  pinMode(2,INPUT_PULLUP); // 計測ボタン
  pinMode(6,INPUT_PULLUP); // 充電ボタン
  pinMode(7,INPUT_PULLUP); // 放電ボタン
  pinMode(A1,INPUT);       // アナログIN
  pmb = 1;                 // 計測ボタンOFF
  pchb = 1;                // 充電ボタンOFF
  pdcb = 1;                // 放電ボタンOFF
  Serial.begin(115200);
}

void loop(void) {
  mb  = digitalRead(2);
  chb = digitalRead(6);
  dcb = digitalRead(7);
  if ((pmb==1)&&(mb==0)) { // 計測開始
    Serial.print("SAMPLING=");
    va1 = analogRead(A1);
    Serial.println(va1,DEC);
    delay(100);
  }
  if ((pchb==1)&&(chb==0)) { // 充電開始
    Serial.print("CHARGING ");
    pinMode(A1,OUTPUT);
    digitalWrite(A1,HIGH);
    delay(200);
    Serial.println("DONE");
    pinMode(A1,INPUT);
  }
  if ((pdcb==1)&&(dcb==0)) { // 放電開始
    Serial.print("DISCHARGING ");
    pinMode(A1,OUTPUT);
    digitalWrite(A1,LOW);
    delay(600);
    Serial.println("DONE");
    pinMode(A1,INPUT);
  }
  pchb = chb;
  pdcb = dcb;
  pmb = mb;
}
