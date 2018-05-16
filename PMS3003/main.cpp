#include <cox.h>
#include <PMS3003.hpp>
#include <Adafruit_SSD1306.hpp>
#include <Adafruit_GFX.hpp>
#include <esp32/ESP32TwoWire.hpp>

PMS3003 pms3003 = PMS3003(Serial2, 19, 18);
ESP32TwoWire Wire(0, 27, 14, 400000);
Adafruit_SSD1306 display(-1, Wire, 0x3C);

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
}

void setup() {
  Serial.begin(115200);
  printf("\n*** [ESP-WROOM-32] PMS3003 Particle sensor test ***\n");
  pms3003.begin();
  pms3003.onReadDone(eventSensorMeasuredDone);

  display.begin(SSD1306_SWITCHCAPVCC);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.clearDisplay();
  display.display();
}
