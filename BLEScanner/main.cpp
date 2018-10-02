#include <cox.h>
#include <BLEDevice.hpp>

BLEScan* pBLEScan = nullptr;
bool isScanning = false;

class MyAdvertisedDeviceListener: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    BLEAddress addr = advertisedDevice.getAddress();
    Serial.printf("[SCANNED] Source Address: %s", addr.toString().c_str());

    if (advertisedDevice.haveName()) {
      Serial.printf(", Name: %s", advertisedDevice.getName().c_str());
    }

    if (advertisedDevice.haveAppearance()) {
      Serial.printf(", Appearance: 0x%04X", advertisedDevice.getAppearance());
    }

    if (advertisedDevice.haveManufacturerData()) {
      Serial.print(", Manufacturer data:");
      std::string manufacturerData = advertisedDevice.getManufacturerData();
      for (uint8_t i = 0; i < manufacturerData.length(); i++) {
        Serial.printf(" %02X", (uint8_t) manufacturerData.data()[i]);
      }
    }

    if (advertisedDevice.haveServiceUUID()) {
      BLEUUID serviceUUID = advertisedDevice.getServiceUUID();
      Serial.printf(", service UUID: %s", serviceUUID.toString().c_str());
    }

    if (advertisedDevice.haveTXPower()) {
      int8_t txPower = advertisedDevice.getTXPower();
      Serial.printf(", TX power:%d dBm", txPower);
    }

    if (advertisedDevice.haveRSSI()) {
      int rssi = advertisedDevice.getRSSI();
      Serial.printf(", RSSI: %d dB", rssi);
    }

    Serial.println();
  }
};


MyAdvertisedDeviceListener listener;

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

void setup() {
  Serial.begin(115200);
  printf("\n*** [ESP-WROOM-32] BLE Scanner ***\n");

  pinMode(0, INPUT);
  attachInterrupt(0, eventButtonPressed, FALLING);

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(&listener, true);
  pBLEScan->activeScan = true;
  isScanning = pBLEScan->start(0, nullptr);
  if (isScanning) {
    printf("* Now scanning...\n");
  } else {
    printf("* Scan failed.\n");
  }
}
