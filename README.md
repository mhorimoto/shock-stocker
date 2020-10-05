# shock-stocker

ピエゾ素子による衝撃起電力をコンデンサに蓄電して
その電圧を測ることで間接的に衝撃ストレスの積算量を
計測する装置。

初代では、ArduinoのA/Dコンバータ(ADC)を使用していた
が、解像度が低い(10bit)ために変化量を細かく見ることが
出来なかった。

このバージョンでは、ADS1115を使用して16bits解像度で
蓄電量を調べることにした。

なお、Arduino内蔵ADCではないので、ポートの簡単な
切り替えでコンデンサ蓄電の放電が出来ないので、放電の
ためのリレーを追加している。
このリレーをON(make)するとGNDに直結されて放電する。

## HARDWARE INTERFACE

### ADS1115

    ・I2C Address 0x48
    ・X axis port A0
    ・Y axis port A1
    ・Z axis port A2
    ・Library https://github.com/adafruit/Adafruit_ADS1X15

### Relay for Discharge

    ・X axis port D2
    ・Y axis port D3
    ・Z axis port D4

