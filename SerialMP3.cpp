/*--------------------------------------------------------------------
  SerialMP3.h - Library for Serial MP3 Player board GD3300D chip.
  Created by nhLuan, December 24, 2023.
--------------------------------------------------------------------*/

#include "stdint.h"
#include "Arduino.h"
#include "SerialMP3.h"

SerialMP3::SerialMP3(uint8_t rx, uint8_t tx) {
  serial_mp3 = new SoftwareSerial(rx, tx);
  show_debug = false;
}

void SerialMP3::showDebug(bool status) {
  show_debug = status;  // 0: not debug, 1: debug
}

void SerialMP3::init() {
  serial_mp3->begin(9600); // Start mp3 communication
  delay(500); // Wait for initialization
  reset();
  delay(500);
  sendCommand(CMD_SELECT_DEVICE, 2); // Defaut device microSD
  delay(500); // Wait for initialization
  
  if (show_debug) {
    Serial.println("MP3 ready!");
  }
}

uint8_t SerialMP3::available() {
  return serial_mp3->available();
}

char SerialMP3::read() {
  return serial_mp3->read();
}

void SerialMP3::play() {
  sendCommand(CMD_PLAY);
}

void SerialMP3::play(uint8_t i) {
  // play with i file number name
  // i: 1 --> 255
  sendCommand(CMD_PLAY_INDEX, i);
}

void SerialMP3::play(uint8_t i, uint8_t volume) {
  // play i file number name with volume
  // i: 1 --> 255
  // volume: 0 --> 30
  sendCommand(CMD_PLAY_VOLUME, volume, i);
}

void SerialMP3::playNext() {
  sendCommand(CMD_NEXT);
}

void SerialMP3::PlayPrevious() {
  sendCommand(CMD_PERVIOUS);
}

void SerialMP3::playFolder(uint8_t f) {
  sendCommand(CMD_PLAY_FOLDER_FILE, f);
}

void SerialMP3::playFolderFile(uint8_t f, uint8_t i) {
  sendCommand(CMD_PLAY_FOLDER_FILE, f, i);
}

void SerialMP3::volumeUp() {
  sendCommand(CMD_VOLUME_UP);
}

void SerialMP3::volumeDown() {
  sendCommand(CMD_VOLUME_DOWN);
}

void SerialMP3::setVolume(uint8_t volume) {
  // volume 0 --> 30
  sendCommand(CMD_SET_VOLUME, volume);
}

void SerialMP3::playSignleCycle(uint8_t i) {
  sendCommand(CMD_SINGLE_CYCLE_PLAY, i);
}

void SerialMP3::playSignleCycle(uint8_t f, uint8_t i) {
  sendCommand(CMD_SINGLE_CYCLE_PLAY, f, i);
}

void SerialMP3::pause() {
  sendCommand(CMD_PAUSE);
}

void SerialMP3::stop() {
  sendCommand(CMD_STOP);
}

void SerialMP3::sleep() {
  sendCommand(CMD_SLEEP_MODE);
}

void SerialMP3::wakeup() {
  sendCommand(CMD_WAKE_UP);
}

void SerialMP3::reset() {
  sendCommand(CMD_RESET);
}

// Query commands
void SerialMP3::queryStatus() {
  sendCommand(CMD_QUERY_STATUS);
}
void SerialMP3::queryVolume() {
  sendCommand(CMD_QUERY_VOLUME);
}
void SerialMP3::queryTotalSong() {
  sendCommand(CMD_QUERY_TOTAL_SONG);
}
void SerialMP3::querySongCurrent() {
  sendCommand(CMD_QUERY_SONG);
}
void SerialMP3::queryTotalSongInFolder() {
  sendCommand(CMD_QUERY_TOTAL_SONG_FOLDER);
}
void SerialMP3::queryTotalFolder() {
  sendCommand(CMD_QUERY_TOTAL_FOLDER);
}


void SerialMP3::sendCommand(uint8_t command) {
  sendCommand(command, 0, 0);
}

void SerialMP3::sendCommand(uint8_t command, uint8_t data2) {
  sendCommand(command, 0, data2);
}

void SerialMP3::sendCommand(uint8_t command, uint8_t data1, uint8_t data2) {

  // Command structure 0x7E 0xFF 0x06 CMD FBACK DATA1 DATA2 0xEF
  uint8_t send_buf[8] = { 0 };  // Buffer for send command.
  String send_mp3 = "";         // mesage debug

  delay(20);
  send_buf[0] = 0x7E;     // Start byte
  send_buf[1] = 0xFF;     // Version info
  send_buf[2] = 0x06;     // Command length not including Start and End byte.
  send_buf[3] = command;  // Command
  send_buf[4] = 0x01;     // Feedback: 0x00 = not feedback, 0x01 = feedback
  send_buf[5] = data1;    // Data high
  send_buf[6] = data2;    // Data low
  send_buf[7] = 0xEF;     // Ending byte

  for (int i = 0; i < 8; i++) {
    serial_mp3->write(send_buf[i]);
    send_mp3 += byteToStringHex(send_buf[i]);
  }

  if (show_debug) {
    Serial.print("Send: ");
    Serial.println(send_mp3); // Watch what are we sending
  }
  delay(500); // Wait between sending commands.
}

String SerialMP3::decodeMP3Answer() {
  // Response Structure  0x7E 0xFF 0x06 RSP 0x00 0x00 DAT 0xFE 0xBA 0xEF
  // RSP Response code
  // DAT Response additional data

  String decoded_answer = "";

  decoded_answer = answerString();

  switch (answer_buff[3]) {
    case 0x3A:
      decoded_answer += " -> MicroSD card inserted.";
      break;

    case 0x3B:
      decoded_answer += " -> MicroSD card removed.";
      break;

    case 0x3D:
      decoded_answer += " -> Completed play num " + String(answer_buff[6], DEC);
      break;

    case 0x3F:
      switch (answer_buff[6]) {
        case 2: decoded_answer += " -> MicroSD ready."; break;
      }
      break;

    case 0x40:
      decoded_answer += " -> Error!";
      break;

    case 0x41:
      decoded_answer += " -> Data recived correctly. ";
      break;

    case 0x42:
      switch (answer_buff[6]) {
        case 0: decoded_answer += " -> Status: stopped"; break;
        case 1: decoded_answer += " -> Status: playing"; break;
        case 2: decoded_answer += " -> Status: paused"; break;
      }
      break;

    case 0x43:
      decoded_answer += " -> Vol playing: " + String(answer_buff[6], DEC);
      break;

    case 0x48:
      decoded_answer += " -> File count: " + String(answer_buff[6], DEC);
      break;


    case 0x4C:
      decoded_answer += " -> Playing: " + String(answer_buff[6], DEC);
      break;

    case 0x4E:
      decoded_answer += " -> Folder file count: " + String(answer_buff[6], DEC);
      break;

    case 0x4F:
      decoded_answer += " -> Folder count: " + String(answer_buff[6], DEC);
      break;
  }


  answer_buff[3] = 0;  // Clear answer_bufff.
  return decoded_answer;
}


/*----------------------------Private Functio----------------------------*/

/*Function returns hex String data with byte(uint8_t) as parameter */
String SerialMP3::byteToStringHex(uint8_t b) {
  String str_hex = "";

  str_hex += "0X";
  if (b < 16) {
    str_hex += "0";  // add 0 if byte < 16
  }
  str_hex += String(b, HEX);
  str_hex += " ";

  return str_hex;
}


/*Function returns a String answer from module Serial MP3 Player. */
String SerialMP3::answerString() {
  // Response Structure  0x7E 0xFF 0x06 RSP 0x00 0x00 DAT 0xFE 0xBA 0xEF
  // RSP Response code
  // DAT Response additional data
  // if there are something available start to read from mp3 serial.
  // if there are "0x7E" it's a beginning.
  // read while something readed and it's not the end "0xEF"

  uint8_t b;
  String mp3_answer = "";  // Answer from the Serial MP3 Player
  uint8_t buff_index = 0;

  if (serial_mp3->available()) {
    do {
      b = serial_mp3->read();

      if (b == 0x7E) {
        buff_index = 0;
        mp3_answer = "";
      }

      answer_buff[buff_index] = b;
      mp3_answer += byteToStringHex(answer_buff[buff_index]);
      buff_index++;
    } while (b != 0xEF);
  }

  return mp3_answer;
}
