#include <cox.h>

Timer timerHello;
Timer timerHello2;

static void taskHello(void *) {
  Serial.printf(
    "Hello every 1sec. msec:%u, usec:%u, sec:%u,",
    millis(), micros(), seconds()
  );
  Serial2.printf(
    "Hello every 1sec. This is Serial. msec:%u, usec:%u, sec:%u\n",
    millis(), micros(), seconds()
  );

  digitalToggle(2);
  Serial.printf(" GPIO0:%d, GPIO2:%d\n", digitalRead(0), digitalRead(2));
}

static void taskHello2(void *) {
  Serial.printf(
    "Hello every 5sec. msec:%u, usec:%u, sec:%u\n",
    millis(), micros(), seconds()
  );
  Serial2.printf(
    "Hello every 5sec. msec:%u, usec:%u, sec:%u\n",
    millis(), micros(), seconds()
  );
}

static void eventKeyPressed() {
  Serial.printf("* Key connected to GPIO0 is pressed!\n");
}

static void eventSerialRx(SerialPort &p) {
  while (p.available() > 0) {
    char c = p.read();
    p.write(c); //echo
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("*** [ESP-WROOM-32] Basic ***");
  Serial.onReceive(eventSerialRx);
  Serial.listen();

  Serial2.begin(115200);
  Serial2.println("*** [ESP-WROOM-32] Basic ***");
  Serial2.onReceive(eventSerialRx);
  Serial2.listen();

  timerHello.onFired(taskHello, NULL);
  timerHello.startPeriodic(1000);

  timerHello2.onFired(taskHello2, NULL);
  timerHello2.startPeriodic(5000);

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  pinMode(0, INPUT);
  attachInterrupt(0, eventKeyPressed, FALLING);
}
