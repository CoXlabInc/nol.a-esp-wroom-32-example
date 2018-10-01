#include <cox.h>
#include <BLEDevice.hpp>

BLEScan* pBLEScan = nullptr;
bool isScanning = false;

static void eventButtonPressed() {
  if (isScanning) {
    Serial.println("* Turn off scanner!");
    pBLEScan->stop();
    isScanning = false;
  } else {
    isScanning = pBLEScan->start(0, nullptr);
    if (isScanning) {
      printf("* Turn on scanner!\n");
    } else {
      printf("* Scan failed.\n");
    }
  }
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.printf("Advertised Device: %s (RSSI:%d dB)\n", advertisedDevice.toString().c_str(), advertisedDevice.getRSSI());
  }
};

void setup() {
  Serial.begin(115200);
  printf("\n*** [ESP-WROOM-32] BLE Scanner ***\n");

  pinMode(0, INPUT);
  attachInterrupt(0, eventButtonPressed, FALLING);

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(), true);
  pBLEScan->activeScan = true;
  isScanning = pBLEScan->start(0, nullptr);
  if (isScanning) {
    printf("* Now scanning...\n");
  } else {
    printf("* Scan failed.\n");
  }
}
