#include <cox.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <esp32/ESP32SPI.hpp>
#include <dev/Adafruit_VS1053.hpp>

ESP32SPI spi(ESP32SPI::VSPI, 18, 19, 5);
Adafruit_VS1053_FilePlayer player(spi, 26, 12, 27, 14);

static void eventMusicPlayDone() {
  Serial.println("Play done");
  digitalWrite(25, LOW);
  System.unmountSdMmcFatFs();
}

static void eventKeyPressed() {
  error_t err = System.mountSdMmcFatFs("/sdcard", 1, 4, 22, 15, 21);
  if (err != ERROR_SUCCESS) {
    Serial.printf("* Mount SD card failed: %d\n", err);
    return;
  }

  Serial.println("Let's play music!");
  player.setVolume(0, 0);
  if (player.startPlayingFile("/sdcard/music.mp3", eventMusicPlayDone)) {
    digitalWrite(25, HIGH);
  } else {
    Serial.println("Cannot play...:-/");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("*** [ESP-WROOM-32] Adafruit Music Maker Example ***");

  uint8_t x = player.begin();
  Serial.printf("Player began:%02X\n", x);

  pinMode(0, INPUT);
  attachInterrupt(0, eventKeyPressed, FALLING);

  Serial.println("Press button to play a music!");

  pinMode(25, OUTPUT);
  digitalWrite(25, LOW);
}
