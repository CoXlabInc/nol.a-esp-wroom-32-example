#include <cox.h>
#include <HTTPClient.hpp>

const char *rootCA = \
"-----BEGIN CERTIFICATE-----\n"\
"MIIFdDCCBFygAwIBAgIQJ2buVutJ846r13Ci/ITeIjANBgkqhkiG9w0BAQwFADBv\n"\
"MQswCQYDVQQGEwJTRTEUMBIGA1UEChMLQWRkVHJ1c3QgQUIxJjAkBgNVBAsTHUFk\n"\
"ZFRydXN0IEV4dGVybmFsIFRUUCBOZXR3b3JrMSIwIAYDVQQDExlBZGRUcnVzdCBF\n"\
"eHRlcm5hbCBDQSBSb290MB4XDTAwMDUzMDEwNDgzOFoXDTIwMDUzMDEwNDgzOFow\n"\
"gYUxCzAJBgNVBAYTAkdCMRswGQYDVQQIExJHcmVhdGVyIE1hbmNoZXN0ZXIxEDAO\n"\
"BgNVBAcTB1NhbGZvcmQxGjAYBgNVBAoTEUNPTU9ETyBDQSBMaW1pdGVkMSswKQYD\n"\
"VQQDEyJDT01PRE8gUlNBIENlcnRpZmljYXRpb24gQXV0aG9yaXR5MIICIjANBgkq\n"\
"hkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAkehUktIKVrGsDSTdxc9EZ3SZKzejfSNw\n"\
"AHG8U9/E+ioSj0t/EFa9n3Byt2F/yUsPF6c947AEYe7/EZfH9IY+Cvo+XPmT5jR6\n"\
"2RRr55yzhaCCenavcZDX7P0N+pxs+t+wgvQUfvm+xKYvT3+Zf7X8Z0NyvQwA1onr\n"\
"ayzT7Y+YHBSrfuXjbvzYqOSSJNpDa2K4Vf3qwbxstovzDo2a5JtsaZn4eEgwRdWt\n"\
"4Q08RWD8MpZRJ7xnw8outmvqRsfHIKCxH2XeSAi6pE6p8oNGN4Tr6MyBSENnTnIq\n"\
"m1y9TBsoilwie7SrmNnu4FGDwwlGTm0+mfqVF9p8M1dBPI1R7Qu2XK8sYxrfV8g/\n"\
"vOldxJuvRZnio1oktLqpVj3Pb6r/SVi+8Kj/9Lit6Tf7urj0Czr56ENCHonYhMsT\n"\
"8dm74YlguIwoVqwUHZwK53Hrzw7dPamWoUi9PPevtQ0iTMARgexWO/bTouJbt7IE\n"\
"IlKVgJNp6I5MZfGRAy1wdALqi2cVKWlSArvX31BqVUa/oKMoYX9w0MOiqiwhqkfO\n"\
"KJwGRXa/ghgntNWutMtQ5mv0TIZxMOmm3xaG4Nj/QN370EKIf6MzOi5cHkERgWPO\n"\
"GHFrK+ymircxXDpqR+DDeVnWIBqv8mqYqnK8V0rSS527EPywTEHl7R09XiidnMy/\n"\
"s1Hap0flhFMCAwEAAaOB9DCB8TAfBgNVHSMEGDAWgBStvZh6NLQm9/rEJlTvA73g\n"\
"JMtUGjAdBgNVHQ4EFgQUu69+Aj36pvE8hI6t7jiY7NkyMtQwDgYDVR0PAQH/BAQD\n"\
"AgGGMA8GA1UdEwEB/wQFMAMBAf8wEQYDVR0gBAowCDAGBgRVHSAAMEQGA1UdHwQ9\n"\
"MDswOaA3oDWGM2h0dHA6Ly9jcmwudXNlcnRydXN0LmNvbS9BZGRUcnVzdEV4dGVy\n"\
"bmFsQ0FSb290LmNybDA1BggrBgEFBQcBAQQpMCcwJQYIKwYBBQUHMAGGGWh0dHA6\n"\
"Ly9vY3NwLnVzZXJ0cnVzdC5jb20wDQYJKoZIhvcNAQEMBQADggEBAGS/g/FfmoXQ\n"\
"zbihKVcN6Fr30ek+8nYEbvFScLsePP9NDXRqzIGCJdPDoCpdTPW6i6FtxFQJdcfj\n"\
"Jw5dhHk3QBN39bSsHNA7qxcS1u80GH4r6XnTq1dFDK8o+tDb5VCViLvfhVdpfZLY\n"\
"Uspzgb8c8+a4bmYRBbMelC1/kZWSWfFMzqORcUx8Rww7Cxn2obFshj5cqsQugsv5\n"\
"B5a6SE2Q8pTIqXOi6wZ7I53eovNNVZ96YUWYGGjHXkBrI/V5eu+MtWuLt29G9Hvx\n"\
"PUsE2JOAWVrgQSQdso8VYFhH2+9uRv0V9dlfmrPb2LjkQLPNlzmuhbsdjrzch5vR\n"\
"pu/xO28QOG8=\n"\
"-----END CERTIFICATE-----\n";

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
  printf("- http get test\n");
  printf("- https get test\n");
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
      goto success;
    } else {
      printf("- Invalid index.\n");
    }
  } else if (memcmp(keyBuf, "http", 4) == 0) {
    HTTPClient http;

    if (strcmp(keyBuf, "http get test") == 0) {
      http.begin("http://www.coxlab.kr/");
    } else if (strcmp(keyBuf, "https get test") == 0) {
      http.begin("https://town.coxlab.kr/", rootCA);
    } else {
      goto error;
    }

    int responseCode = http.GET();
    if (responseCode > 0) {
      printf("- responseCode:%d\n", responseCode);
      if (responseCode == HTTP_CODE_OK) {
        http.writeToStream(&Serial);
      }
    } else {
      printf("- GET failed (error:%s)\n", http.errorToString(responseCode).c_str());
    }
    http.end();
  } else {
    printf("* Unknown command\n");
  }

  error:
  printMenu();

  success:
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
    printf("- IP:%s\n", WiFi.localIP().toString().c_str());
    printf("- Gateway:%s\n", WiFi.gatewayIP().toString().c_str());
    printf("- Subnet Mask:%s\n", WiFi.subnetMask().toString().c_str());
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
