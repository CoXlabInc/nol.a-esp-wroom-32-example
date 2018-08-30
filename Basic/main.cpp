#include <cox.h>
#include <esp32/ESP32Serial.hpp>

Timer timerHello;
Timer timerHello2;
ESP32Serial Serial2(ESP32Serial::PORT2, 17, 16);

static void taskHello(void *) {
  struct timeval t;
  gettimeofday(&t, NULL);

  Serial.printf(
    "Hello every 1sec. This is Serial. gettimeofday:%u.%06u,", t.tv_sec, t.tv_usec
  );
  Serial2.printf(
    "Hello every 1sec. This is Serial2. gettimeofday:%u.%06u,", t.tv_sec, t.tv_usec
  );

  digitalToggle(2);
  Serial.printf(" GPIO0:%d, GPIO2:%d\n", digitalRead(0), digitalRead(2));
}

static void taskHello2(void *) {
  struct timeval t;
  gettimeofday(&t, NULL);

  Serial.printf(
    "Hello every 5sec. This is Serial. gettimeofday:%u.%06u\n", t.tv_sec, t.tv_usec
  );
  Serial2.printf(
    "Hello every 5sec. This is Serial2. gettimeofday:%u.%06u\n", t.tv_sec, t.tv_usec
  );
}

static void eventKeyPressed(GPIOInterruptInfo_t &info) {
  struct timeval now;
  gettimeofday(&now, NULL);
  Serial.printf(
    "* Key connected to GPIO(%d) is pressed! (interrupt:%u.%06u, now:%u.%06u)\n",
    info.pin, info.tEnteredISR.tv_sec, info.tEnteredISR.tv_usec, now.tv_sec, now.tv_usec
  );
}

static void eventSerialRx(SerialPort &p) {
  while (p.available() > 0) {
    char c = p.read();
    Serial.write(c); //echo or forward
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("*** [ESP-WROOM-32] Basic ***");
  Serial.onReceive(eventSerialRx);
  Serial.listen();

  Serial2.begin(9600);
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
  attachInterrupt(0, eventKeyPressed, NULL, FALLING);
}
