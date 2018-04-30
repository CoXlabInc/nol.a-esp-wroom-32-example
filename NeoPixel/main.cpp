#include <cox.h>
#include <dev/Adafruit_NeoPixel.hpp>

Adafruit_NeoPixel strip(40, 4, NEO_GRB + NEO_KHZ800);
Timer timerLoop;
uint8_t state = 0;

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(uint8_t WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
    System.feedWatchdog();
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
    System.feedWatchdog();
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
    System.feedWatchdog();
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);
      System.feedWatchdog();

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);
      System.feedWatchdog();

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("*** [NeoPixel Test] ***");

  strip.begin();
  strip.show();

  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);

  timerLoop.onFired([](void *) {
    switch (state) {
      case 0:
      colorWipe(strip.Color(255, 0, 0), 50);
      printf("colorWipe:Red\n");
      timerLoop.startOneShot(1000);
      break;

      case 1:
      colorWipe(strip.Color(0, 255, 0), 50);
      printf("colorWipe:Green\n");
      timerLoop.startOneShot(1000);
      break;

      case 2: colorWipe(strip.Color(0, 0, 255), 50);
      printf("colorWipe:Blue\n");
      timerLoop.startOneShot(1000);
      break;

      case 3: theaterChase(strip.Color(127, 127, 127), 50);
      printf("theaterChase:White\n");
      timerLoop.startOneShot(50);
      break;

      case 4:
      theaterChase(strip.Color(127, 0, 0), 50);
      printf("theaterChase:Red\n");
      timerLoop.startOneShot(50);
      break;

      case 5:
      theaterChase(strip.Color(0, 0, 127), 50);
      printf("theaterChase:Blue\n");
      timerLoop.startOneShot(50);
      break;

      case 6:
      rainbow(20);
      printf("rainbow\n");
      timerLoop.startOneShot(20);
      break;

      case 7:
      printf("rainbowCycle\n");
      rainbowCycle(20);
      timerLoop.startOneShot(20);
      break;

      case 8:
      printf("theaterChaseRainbow\n");
      theaterChaseRainbow(50);
      timerLoop.startOneShot(50);
      break;

      default:
      strip.clear();
      strip.show();
      timerLoop.startOneShot(1000);
      break;
    }

    if (++state >= 10) {
      state = 0;
    }
  }, NULL);
  timerLoop.fire();
}
