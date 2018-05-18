#include <cox.h>

Timer timerHello;
Timer timerLEDOff;
BLEMac *ble;

static const uint8_t beaconInfo[0x17] = {
  0x02, //Beacon
  0x15, //Length
  0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89, 0x9a, 0xab, 0xbc, 0xcd, 0xde, 0xef, 0xf0, //UUID
  0x01, 0x02, //Major
  0x03, 0x04, //Minor
  0xC3, // The Beacon's measured TX power in this implementation.
};

static void taskHello(void *) {
  digitalWrite(2, HIGH);
  timerLEDOff.startOneShot(10);
}

static void taskLEDOff(void *) {
  digitalWrite(2, LOW);
}

static void eventButtonPressed() {
  if (ble->isAdvertising()) {
    Serial.println("* Turn off beacon!");
    ble->endAdvertise();
    timerHello.stop();
  } else {
    error_t err = ble->beginAdvertise();
    if (err == ERROR_SUCCESS) {
      Serial.println("* Turn on beacon!");
      timerHello.startPeriodic(1000);
    } else {
      Serial.print("* Error during turning on beacon: ");
      Serial.println(err);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("*** [ESP-WROOM-32] BLE Beacon ***");

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  timerHello.onFired(taskHello, NULL);
  timerLEDOff.onFired(taskLEDOff, NULL);

  pinMode(0, INPUT);
  attachInterrupt(0, eventButtonPressed, FALLING);

  ble = System.initBLE();
  if (ble) {
    Serial.println("* BLE stack initialized successfully.");

    ble->setManufacturerData(
      0x02E5, // Espressif Inc.
      beaconInfo,
      sizeof(beaconInfo)
    );
    ble->setAdvInterval(2000000);
    error_t err = ble->beginAdvertise();
    Serial.print("* beginAdvertise: "); Serial.println(err);
    if (err == ERROR_SUCCESS) {
      timerHello.startPeriodic(1000);
    }
  } else {
    Serial.println("* BLE stack initializing failed.");
  }
}
