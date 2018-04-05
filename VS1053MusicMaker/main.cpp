#include <cox.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <esp32/ESP32SPI.hpp>
#include <dev/Adafruit_VS1053.hpp>

ESP32SPI spi(ESP32SPI::VSPI, 18, 19, 5);
Adafruit_VS1053_FilePlayer player(spi, 26, 12, 27, 14);

static void eventKeyPressed() {
  Serial.println("Let's play music!");
  player.setVolume(0, 0);
  if (!player.startPlayingFile("/sdcard/music.mp3")) {
    Serial.println("Cannot play...:-/");
  }
  Serial.println("Play done");

  // uint8_t buf[32];
  // FILE *f = fopen("/sdcard/music.mp3", "r");
  //
  // int read;
  // uint32_t x = 0;
  // while ((read = fread(buf, 1, 32, f)) > 0) {
  //   Serial.printf("[%d]", x);
  //   for (uint8_t i = 0; i < 32; i++) {
  //     Serial.printf(" %02X", buf[i]);
  //   }
  //   Serial.printf("\n");
  //   x++;
  //   System.feedWatchdog();
  // }
}

void setup() {
  Serial.begin(115200);
  Serial.println("*** [ESP-WROOM-32] Adafruit Music Maker Example ***");

  error_t err = System.mountSdMmcFatFs("/sdcard", 1, 4, 22, 15, 21);
  if (err != ERROR_SUCCESS) {
    Serial.printf("* Mount SD card failed: %d\n", err);
    return;
  }

  uint8_t x = player.begin();
  Serial.printf("Player began:%02X\n", x);

  FILE *f = fopen("/sdcard/music.mp3", "r");
  if (!f) {
    Serial.println("There is no music.mp3 file on the SD card. Please, check it.");
    return;
  }
  fclose(f);

  pinMode(0, INPUT);
  attachInterrupt(0, eventKeyPressed, FALLING);

  Serial.println("Press button to play a music!");
}
