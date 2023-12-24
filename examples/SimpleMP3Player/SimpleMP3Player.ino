/*----------------------------------------------------
 Simple music play with Serial MP3 Player GD3300D chip.
 Copy the "exam.mp3" file to an empty SD card
 Connect board Serial MP3 Player with Arduino:
      TX  --> Pin 10
      RX  --> Pin 11
      GND --> GND
      VCC --> 5V 
----------------------------------------------------*/

#include "SerialMP3.h"

#define RX_PIN 10  // RX pin 10
#define TX_PIN 11  // TX pin 11

SerialMP3 mp3(RX_PIN, TX_PIN);

void setup() {
  // put your setup code here, to run once:
  mp3.showDebug(true); // print command send to Serial MP3 Player
  Serial.begin(9600); // Start serial interface
  mp3.init(); // Initialization Serial MP3 Player
}

void loop() {
  // put your main code here, to run repeatedly:
  mp3.play(); // play exam.mp3
  delay(2000); // wait 2 second
}
