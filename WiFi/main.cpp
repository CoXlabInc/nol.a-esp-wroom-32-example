#include <cox.h>

char keyBuf[256];

static void scanNetworks() {
  printf("* Scanning networks...\n");
  int n = WiFi.scanNetworks();
  printf("* Scan done\n");

  if (n == 0) {
    printf("- No networks found.\n");
  } else {
    printf("- %d networks found.\n", n);
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      printf("[%d] '%s'", i, WiFi.SSID(i));

      const uint8_t *mac = WiFi.BSSID(i);
      printf(
        " MAC:%02X:%02X:%02X:%02X:%02X:%02X, %d dB %s\n",
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
        WiFi.RSSI(i), (WiFi.encryptionType(i) == WiFi.AUTH_OPEN)? " " : "*"
      );
    }
  }
}

static void printMenu() {
  printf("*** Commands ***\n");
  printf("- scan\n");
  printf("- connect {last scanned network index} {password}\n");
  printf("- disconnect\n");
}

static void eventKeyboard(SerialPort &) {
  unsigned i;
  char password[20];
  if (strcmp(keyBuf, "scan") == 0) {
    scanNetworks();
  } else if (strcmp(keyBuf, "disconnect") == 0) {
    if (WiFi.isConnected()) {
      WiFi.disconnect();
      delay(100);
    } else {
      printf("* WiFi is not connected.\n");
    }
  } else if (sscanf(keyBuf, "connect %u %s", &i, password) == 2) {
    if (WiFi.isConnected()) {
      WiFi.disconnect();
      while (WiFi.isConnected());
    }

    const char *ssid = WiFi.SSID(i);
    if (ssid) {
      printf("- Connect to '%s'...\n", ssid);
      WiFi.begin(ssid, password);
      Serial.inputKeyboard(keyBuf, sizeof(keyBuf) - 1);
      return;
    } else {
      printf("- Invalid index.\n");
    }
  } else {
    printf("* Unknown command\n");
  }

  printMenu();
  Serial.inputKeyboard(keyBuf, sizeof(keyBuf) - 1);
}

static void eventWiFi(WiFiClass::event_id_t event, void *info) {
  printf("* WiFi event(%d): ", event);

  switch (event) {
    case (WiFi.EVENT_STA_CONNECTED):
    printf("connected.\n");
    break;

    case (WiFi.EVENT_STA_DISCONNECTED):
    printf("disconnected.\n");
    break;

    case (WiFi.EVENT_STA_GOT_IP):
    printf("got IP:\n");
    printf(
      "- IP:%u.%u.%u.%u\n",
      WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]
    );
    printf(
      "- Gateway:%u.%u.%u.%u\n",
      WiFi.gatewayIP()[0], WiFi.gatewayIP()[1], WiFi.gatewayIP()[2], WiFi.gatewayIP()[3]
    );
    printf(
      "- Subnet Mask:%u.%u.%u.%u\n",
      WiFi.subnetMask()[0], WiFi.subnetMask()[1], WiFi.subnetMask()[2], WiFi.subnetMask()[3]
    );
    break;

    case (WiFi.EVENT_STA_LOST_IP):
    printf("lost IP\n");
    break;

    default:
    printf("unhandled\n");
    break;
  }
}

void setup() {
  Serial.begin(115200);
  printf("\n*** [ESP-WROOM-32] WiFi Test ***\n");
  Serial.onReceive(eventKeyboard);
  Serial.inputKeyboard(keyBuf, sizeof(keyBuf) - 1);

  WiFi.mode(WiFi.MODE_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.onEvent(eventWiFi);

  printMenu();
}
