 /*
 * Fan Controller for Server Case
 */

#include "DHT.h"
#include "math.h"

/* LED CLASS */

class LED {
  public:
    int pin;
    int refLevel;
    LED(int pin, int refLevel);
    void manage(int level);
};

LED::LED(int pin, int refLevel) : pin(pin), refLevel(refLevel) {
  digitalWrite(pin, LOW);
}

void LED::manage(int level) {
  digitalWrite(pin, level >= refLevel ? HIGH : LOW);
}

/* FAN CLASS */

class FAN {
  public:
    int pin;
    FAN(int pin);
    void setSpeed(int value);
};

FAN::FAN(int pin) : pin(pin) {}

void FAN::setSpeed(int pwmPercent) {
  analogWrite(pin, pwmPercent * 255 / 100);
}

/* VARIABLES DECLARATION */

const int delayMilliSec = 5000; // Delay between each loop (in ms)

LED ledGreen(9, 20);    // pin :  9, level : 20%
LED ledBlue(10, 40);    // pin : 10, level : 40%
LED ledYellow(11, 60);  // pin : 11, level : 60%
LED ledRed(12, 80);     // pin : 12, level : 80%

FAN fanIn(5);           // pin :  5
FAN fanOut(6);          // pin :  6

DHT dht(2, 'DHT22');    // pin :  2, type : DHT22

/* FUNCTIONS DECLARATION */

void manageFreshness(float temperature, float humidity) {
  int percent = (floor(temperature + 0.5) - 20) / 10;
  if (percent < 0) {
    percent = 0;
  } else if (percent > 100) {
    percent = 100;
  }

  fanIn.setSpeed(percent);
  fanOut.setSpeed(percent);

  ledGreen.manage(percent);
  ledBlue.manage(percent);
  ledYellow.manage(percent);
  ledRed.manage(percent);
}

/* MAIN PROGRAM */

void setup() {
  dht.begin();
}

void loop(){
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  if (!isnan(humidity) && !isnan(temperature)) {
    manageFreshness(temperature, humidity);
  }
  delay(delayMilliSec);
}
