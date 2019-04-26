#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <ArduinoNunchuk.h>

RF24 radio(7, 8); // CE, CSN
ArduinoNunchuk nunchuk = ArduinoNunchuk();

const byte address[6] = "00001";
void setup() {
  Serial.begin(9600);
  nunchuk.init();
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
}
void loop() {

  nunchuk.update();
  int analogX = nunchuk.analogX ;
  int analogY = nunchuk.analogY ;

  int mapTestX = map(analogX , 129 , 225 , 0 , 100);
  int mapTestY = map(analogY , 129 , 225 , 0 , 100);

  Serial.print(mapTestX);
  Serial.print(' ');
  Serial.println(mapTestY);
  
  radio.write(&mapTestX, sizeof(mapTestX));
  //delay(100);
  radio.write(&mapTestY, sizeof(mapTestY));
  //delay(1000);
}
