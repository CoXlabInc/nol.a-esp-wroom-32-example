#include <cox.h>

Timer timerScan;

static void taskScanNetworks(void *) {
  printf("* Scanning networks...\n");
  int n = WiFi.scanNetworks();
  printf("* Scan done\n");

  if (n == 0) {
    printf("- No networks found.\n");
  } else {
    printf("- %d networks found.\n", n);
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      printf("[%d] %s", i + 1, WiFi.SSID(i));

      const uint8_t *mac = WiFi.BSSID(i);
      printf(
        " %02X:%02X:%02X:%02X:%02X:%02X (%d) %s\n",
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
        WiFi.RSSI(i), (WiFi.encryptionType(i) == WiFi.AUTH_OPEN)? " " : "*"
      );
    }
  }
}

void setup() {
  Serial.begin(115200);
  printf("\n*** [ESP-WROOM-32] WiFi Test ***\n");

  WiFi.mode(WiFi.MODE_STA);
  WiFi.disconnect();
  delay(100);

  postTask(taskScanNetworks, NULL);
  timerScan.onFired(taskScanNetworks, NULL);
  timerScan.startPeriodic(10000);
}
