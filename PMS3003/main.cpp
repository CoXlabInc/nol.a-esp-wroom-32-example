#include <cox.h>
#include <PMS3003.hpp>
#include <Adafruit_SSD1306.hpp>
#include <Adafruit_GFX.hpp>
#include <esp32/ESP32TwoWire.hpp>
#include <esp32/ESP32Serial.hpp>
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

#define IOTOWN_TOKEN "00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff"; // replace with your token
#define IOTOWN_NID   "WF1" // replace with your node ID

ESP32Serial Serial2(ESP32Serial::PORT2, 17, 16);
PMS3003 pms3003 = PMS3003(Serial2, 19, 18);
ESP32TwoWire Wire(0, 27, 14, 400000);
Adafruit_SSD1306 display(-1, Wire, 0x3C);
Timer timerReport;
String jsonData;
uint16_t seq = 0;

static void taskPeriodicReport(void *) {
  if (jsonData.length() == 0) {
    printf("* Report data is not ready.\n");
    return;
  }

  digitalWrite(2, HIGH);

  if (WiFi.isConnected()) {
    printf("[#%u] POST data: %s\n", seq++, jsonData.c_str());

    HTTPClient http;
    http.begin("https://town.coxlab.kr/api/data", rootCA);
    http.addHeader("Content-Type", "application/json");
    int responseCode = http.POST(jsonData);

    if (responseCode == HTTP_CODE_OK) {
      http.writeToStream(&Serial);
      Serial.println();
      Serial.flush();
    } else {
      printf("- POST failed (error:%s)\n", http.errorToString(responseCode).c_str());
      http.end();
    }
  } else {
    printf("* WiFi is not connected.\n");
  }

  digitalWrite(2, LOW);
}

static void eventSensorMeasuredDone(
  int32_t pm1_0_CF1,
  int32_t pm2_5_CF1,
  int32_t pm10_0_CF1,
  int32_t pm1_0_Atmosphere,
  int32_t pm2_5_Atmosphere,
  int32_t pm10_0_Atmosphere
) {
  printf("* (CF=1) PM1.0:%d, PM2.5:%d, PM10:%d (unit: ug/m^3)\n", pm1_0_CF1, pm2_5_CF1, pm10_0_CF1);
  printf("* (Atmosphere) PM1.0:%d, PM2.5:%d, PM10:%d (unit: ug/m^3)\n", pm1_0_Atmosphere, pm2_5_Atmosphere, pm10_0_Atmosphere);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("PM1.0: "); display.println(pm1_0_Atmosphere);
  display.print("PM2.5: "); display.println(pm2_5_Atmosphere);
  display.print("PM10 : "); display.println(pm10_0_Atmosphere);
  display.display();

  jsonData = "{\"type\":\"2\",\"token\":\"";
  jsonData += IOTOWN_TOKEN;
  jsonData += "\",\"nid\":\"";
  jsonData += IOTOWN_NID;
  jsonData += "\",\"data\":{\"PM1_0\":";
  jsonData += pm1_0_Atmosphere;
  jsonData += ",\"PM2_5\":";
  jsonData += pm2_5_Atmosphere;
  jsonData += ",\"PM10\":";
  jsonData += pm10_0_Atmosphere;
  jsonData += "}}";
  printf("To be POST data: %s\n", jsonData.c_str());
}

static void eventWiFi(WiFiClass::event_id_t event, void *info) {
  printf("* WiFi event(%d): ", event);

  switch (event) {
    case (WiFi.EVENT_STA_CONNECTED):
    printf("connected.\n");
    break;

    case (WiFi.EVENT_STA_DISCONNECTED):
    printf("disconnected.\n");
    timerReport.stop();
    WiFi.reconnect();
    break;

    case (WiFi.EVENT_STA_GOT_IP):
    printf("got IP:\n");
    printf("- IP:%s\n", WiFi.localIP().toString().c_str());
    printf("- Gateway:%s\n", WiFi.gatewayIP().toString().c_str());
    printf("- Subnet Mask:%s\n", WiFi.subnetMask().toString().c_str());
    timerReport.startPeriodic(10000);
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
  printf("\n*** [ESP-WROOM-32] PMS3003 Particle sensor test ***\n");

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  pms3003.begin();
  pms3003.onReadDone(eventSensorMeasuredDone);

  WiFi.mode(WiFi.MODE_STA);
  WiFi.disconnect();

  display.begin(SSD1306_SWITCHCAPVCC);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.clearDisplay();
  display.print("* PMS3003 *");
  display.display();

  delay(100);

  WiFi.onEvent(eventWiFi);
  WiFi.begin("coxlab2", "johnsnow");

  timerReport.onFired(taskPeriodicReport, NULL);
}
