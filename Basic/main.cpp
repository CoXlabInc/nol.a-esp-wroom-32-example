#include <cox.h>

// Timer timerHello;

static void taskHello(void *) {
  Serial.println("Hello! This is Serial.");
  Serial2.println("Hello! This is Serial2.");
  Serial.printf("Delay 5 sec...\n");
  delay(5000);
  postTask(taskHello, NULL);
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

  // timerHello.onFired(taskHello, NULL);
  // timerHello.startPeriodic(1000);
  postTask(taskHello, NULL);
}
