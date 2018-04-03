#include <cox.h>
#include <sys/unistd.h>
#include <sys/stat.h>

void setup() {
  Serial.begin(115200);
  Serial.println("*** [ESP-WROOM-32] SDCard Example ***");

  error_t err = System.mountSdMmcFatFs("/sdcard", 1, 4, 2, 15, 21);
  if (err != ERROR_SUCCESS) {
    Serial.printf("* Mount SD card failed: %d\n", err);
    return;
  }

  // Use POSIX and C standard library functions to work with files.

  // First create a file.
  Serial.println("Opening file...");
  FILE* f = fopen("/sdcard/hello.txt", "w");
  if (f == NULL) {
    Serial.println("Failed to open file for writing");
    return;
  }
  Serial.flush();
  fprintf(f, "Hello Nol.A-SDK!\n");
  fclose(f);
  Serial.println("File written.");

  // Check if destination file exists before renaming
  f = fopen("/sdcard/foo.txt", "r");
  if (f) {
    fclose(f);
    unlink("/sdcard/foo.txt");
    Serial.println("Removed /sdcard/foo.txt.");
  }

  // Rename original file
  Serial.println("Renaming file /sdcard/hello.txt to /sdcard/foo.txt...");
  if (rename("/sdcard/hello.txt", "/sdcard/foo.txt") != 0) {
    Serial.println("Rename failed.");
    return;
  }
  Serial.println("Renamed.");

  // Open renamed file for reading
  Serial.println("Reading file /sdcard/foo.txt...");
  f = fopen("/sdcard/foo.txt", "r");
  if (f == NULL) {
    Serial.println("Failed to open file for reading.");
    return;
  }
  Serial.println("Read done.");

  char line[20];
  fgets(line, sizeof(line), f);
  fclose(f);

  // strip newline
  char* pos = strchr(line, '\n');
  if (pos) {
    *pos = '\0';
  }
  Serial.printf("Read from file: '%s'\n", line);
}
