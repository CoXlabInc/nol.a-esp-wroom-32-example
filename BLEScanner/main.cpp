#include <cox.h>

BLEMac *ble;

static void eventButtonPressed() {
  if (ble->isScanning()) {
    Serial.println("* Turn off scanner!");
    ble->endScan();
  } else {
    error_t err = ble->beginScan();
    if (err == ERROR_SUCCESS) {
      Serial.println("* Turn on scanner!");
    } else {
      Serial.print("* Error during turning on scanner: ");
      Serial.print(err);
    }
  }
}

static void eventBLEAdvScanned(
  BLEMac::AdvPDU_t pduType,
  const BLEAddress *addr,
  const BLEAddress *directedAddr,
  const uint8_t *data,
  uint8_t len,
  int8_t rssi) {
  Serial.printf("[SCANNED] PDU type:%u, Source Address:", (uint8_t) pduType);
  if (addr->isPeer) {
    Serial.print("(P)");
  }
  for (uint8_t i = 0; i < 6; i++) {
    Serial.printf(" %02X", addr->octet[i]);
  }
  if (directedAddr) {
    Serial.printf(", Directed Address:");
    if (directedAddr->isPeer) {
      Serial.printf("(P)");
    }
    for (uint8_t i = 0; i < 6; i++) {
      Serial.printf(" %02X", directedAddr->octet[i]);
    }
  }
  Serial.print(", Data:");
  for (uint8_t i = 0; i < len; i++) {
    Serial.printf(" %02X", data[i]);
  }
  Serial.printf(", RSSI:%d dB\n", rssi);
}

void setup() {
  Serial.begin(115200);
  printf("\n*** [ESP-WROOM-32] BLE Scanner ***\n");

  pinMode(0, INPUT);
  attachInterrupt(0, eventButtonPressed, FALLING);

  ble = System.initBLE();
  if (ble) {
    printf("* BLE stack initialized successfully.\n");
    ble->setScanInterval(10000);
    ble->setScanWindow(10000);

    ble->onScanned(eventBLEAdvScanned);
    error_t err = ble->beginScan();
    printf("* beginScan: %d\n", err);
  } else {
    printf("* BLE stack initializing failed\n");
  }
}
