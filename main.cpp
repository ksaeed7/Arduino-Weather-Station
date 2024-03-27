#include <Arduino.h>
#include <CoRTOS.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>
#include <SparkFunBME280.h>
#include <Wire.h>

// DEFINES
#define BUTTONPIN 2

#define HUMIDITY_HIGH 55
#define HUMIDITY_MED 30

#define TEMPERATURE_HOT 45
#define TEMPERATURE_LOW 5

#define PRESSURE_DELTA 0.1

// GLOBALS
SSD1306AsciiWire oled;
BME280 sensor;

byte displayMode;

bool displayEnabled;

float altitude;
float pressure;
float humidity;
float temperature;
float old_pressure;

// FUNCTION DECLARATIONS
void sensorTask();
void displayTask();
void buttonTask();
void historicalPressure();
void idleTimer();

// FUNCTION DEFINITIONS
void sensorTask() {
  humidity = sensor.readFloatHumidity();
  temperature = sensor.readTempC();
  altitude = sensor.readFloatAltitudeMeters();
  pressure = sensor.readFloatPressure() / 3386.389; // Convert to inHg
}

void displayTask() {
  // oled.clear();
  switch (displayMode) {
  case 0:
    oled.print("Temperature: ");
    oled.println(temperature);
    oled.print("Pressure: ");
    oled.println(pressure);
    oled.print("Humidity: ");
    oled.println(humidity);
    oled.print("Altitude: ");
    oled.println(altitude);
    break;
  case 1:
    oled.print("Pressure: ");
    if (pressure - PRESSURE_DELTA > old_pressure) {
      oled.println("RISING");
    } else if (pressure + PRESSURE_DELTA < old_pressure) {
      oled.println("FALLING");
    } else {
      oled.println("STEADY");
    }
    oled.print("Temperature: ");
    if (temperature > TEMPERATURE_HOT) {
      oled.println("HOT");
    } else if (temperature < TEMPERATURE_LOW) {
      oled.println("LOW");
    } else {
      oled.println("COLD");
    }
    oled.print("Humidity: ");
    if (humidity > HUMIDITY_MED) {
      if (humidity > HUMIDITY_HIGH) {
        oled.println("WET");
      } else {
        oled.println("MOIST");
      }
    } else {
      oled.println("DRY");
    }
    break;
  default:
    oled.println("Something went wrong.");
  }
}

void buttonTask() {
  if (digitalRead(BUTTONPIN) == LOW) {
    if (displayEnabled) {
      displayMode = (displayMode + 1) % 2;
    }
    displayEnabled = true;
    cortos.unlockTask(displayTask);
    cortos.unlockTask(idleTimer);
    cortos.sleep(idleTimer);
    cortos.setSleepTime(sensorTask, 100, true);
  }
}

void historicalPressure() { old_pressure = pressure; }

void idleTimer() {
  displayEnabled = false;
  cortos.lockTask();
  cortos.lockTask(displayTask);
  cortos.setSleepTime(sensorTask, 60000, true);
  oled.clear();
}

void setup() {
  pinMode(BUTTONPIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin();
  Wire.setClock(400000L);
  oled.begin(&Adafruit128x32, 0x3C);
  oled.setFont(Adafruit5x7);
  Serial.begin(115200);
  sensor.setI2CAddress(0x76);
  if (!sensor.beginI2C()) {
    Serial.println("No sensor detected. Please check it and reset.");
    oled.clear();
    oled.println("No sensor detected.");
    oled.println("Please check it and reset.");
    while (1)
      ; // Freeze
  }
  cortos.init();
  cortos.enableDebug(true); // Alternatively, omit this line to disable most of the debug print overhead.

  cortos.registerTask(historicalPressure, "Update Historical Pressure", 0, (SLEEPFOR | DEADLINE));
  cortos.setSleepTime(3600000, true);
  cortos.registerTask(buttonTask, "Button Task", 0, (SLEEPFOR | DEADLINE));
  cortos.setSleepTime(50, true);
  cortos.registerTask(idleTimer, "Idle Timer Task", 0, 0);
  cortos.setSleepTime(10000);
  cortos.registerTask(displayTask, "Display Task", 10, (SLEEPFOR | DEADLINE));
  cortos.setSleepTime(100, true);
  cortos.registerTask(sensorTask, "Sensor Update Task", 1, (SLEEPFOR | DEADLINE));
  cortos.setSleepTime(100, true);

  // By setting no sleep times/flags for these tasks and making them all the same priority, they will run forever
  // in round-robin order (that is, in the order they were added).
}

void loop() { cortos.scheduler(); }
