#include <cox.h>

Timer timerHello;

static void taskHello(void *) {
  Serial.printf(
    "Hello! This is Serial. msec:%u, usec:%u, sec:%u\n",
    millis(), micros(), seconds()
  );
  Serial2.printf("Hello! This is Serial2.\n");
}

static void eventSerialRx(SerialPort &p) {
  while (p.available() > 0) {
    char c = p.read();
    p.write(c); //echo
  }
}

void setup() {
  Serial.begin(115200);
  Serial.printf("*** [ESP-WROOM-32] Basic ***\n");
  Serial.onReceive(eventSerialRx);
  Serial.listen();

  Serial2.begin(115200);
  Serial2.printf("*** [ESP-WROOM-32] Basic ***\n");
  Serial2.onReceive(eventSerialRx);
  Serial2.listen();

  timerHello.onFired(taskHello, NULL);
  timerHello.startPeriodicMicros(1000000);
}
