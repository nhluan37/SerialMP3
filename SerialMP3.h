/*--------------------------------------------------------------------
  SerialMP3.h - Library for Serial MP3 Player board GD3300D chip.
  Created by nhLuan, December 24, 2023.
--------------------------------------------------------------------*/

#ifndef SerialMP3_h
#define SerialMP3_h

#include "stdint.h"
#include "Arduino.h"
#include "SoftwareSerial.h"

/*--------------Control Command--------------*/
#define CMD_NEXT              0x01 // Next song
#define CMD_PERVIOUS          0x02 //Previous song
#define CMD_PLAY_INDEX        0x03 // Play with i song (i: 1 -> 255)
#define CMD_VOLUME_UP         0x04 // Volume up
#define CMD_VOLUME_DOWN       0x05 // Volume down
#define CMD_SET_VOLUME        0x06 // Set volume (0 -> 30)
#define CMD_SINGLE_CYCLE_PLAY 0x08 // Single cycle play
#define CMD_SELECT_DEVICE     0x09 // Select storage device (microSD: 2)
#define CMD_SLEEP_MODE        0x0A // Chip enters sleep mode
#define CMD_WAKE_UP           0x0B // Chip wakes up
#define CMD_RESET             0x0C // Chip reset
#define CMD_PLAY              0x0D // Play
#define CMD_PAUSE             0x0E // Pause
#define CMD_PLAY_FOLDER_FILE  0x0F // Play the song with folder and song name
#define CMD_STOP              0x16 // Stop
#define CMD_CYCLE_FOLDER      0x17 // Cycle play with folder name
#define CMD_SHUFFLE_PLAY      0X18 // Shuffle Play
#define CMD_SET_SINGLE_CYCLE  0x19 // Set single cycle play
#define CMD_PLAY_VOLUME       0x22 // Set the volume and play the second

/*---------------Query Command---------------*/
#define CMD_QUERY_STATUS            0x42 // Query current state
#define CMD_QUERY_VOLUME            0x43 // Query volume
#define CMD_QUERY_TOTAL_SONG        0x48 // Query the total number of songs
#define CMD_QUERY_SONG              0x4C // Query the current song
#define CMD_QUERY_TOTAL_SONG_FOLDER 0x4E // Query the total number of songs in the current folder
#define CMD_QUERY_TOTAL_FOLDER      0x4F // Query the total number of folder

/*-----------------------Options-----------------------*/

class SerialMP3 {
  public:
    SerialMP3(uint8_t rx, uint8_t tx);
    
    void showDebug(bool status); // true to show debug message

    void init();
    uint8_t available();
    char read();

    void sendCommand(uint8_t command);
    void sendCommand(uint8_t command, uint8_t data2);
    void sendCommand(uint8_t command, uint8_t data1, uint8_t data2);
    
    void play();  // Play
    void play(uint8_t i); // Play the i song
    void play(uint8_t i, uint8_t volume); // Play the i song with volume

    void playNext();  // Play next song
    void PlayPrevious();  // Play previous song
    void playFolder(uint8_t f); // Play all song in the f folder
    void playFolderFile(uint8_t f, uint8_t i); // Play with folder and song name

    void volumeUp();  // Volume increased one
    void volumeDown();  // Volume decrease one
    void setVolume(uint8_t volume); //Set the volume (0 -> 30)

    void playSignleCycle(uint8_t i);  // Single cycle play i file.
    void playSignleCycle(uint8_t f, uint8_t i);  // Single cycle play i file from f folder
    
    void pause(); // Pause

    void stop();  // Stop play

    void sleep();     // Chip enters sleep mode
    void wakeup();    // Chip wakes up
    void reset();     // Chip reset

    // Query commands
    void queryStatus(); // Query current state
    void queryVolume(); // Query volume
    void queryTotalSong(); // Query the total number of songs
    void querySongCurrent(); // Query the current song
    void queryTotalSongInFolder(); // Query the total number of songs in the current folder
    void queryTotalFolder(); // Query the total number of folder

    String decodeMP3Answer(); // Decode Serial MP3 Player answer

  private:
    SoftwareSerial *serial_mp3;
    String answerString();
    String byteToStringHex(uint8_t byte);
    bool show_debug;
    uint8_t answer_buff[10] = {0}; // Buffer for the answers.
};

#endif