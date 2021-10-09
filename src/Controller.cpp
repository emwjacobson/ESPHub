#ifndef CONTROLLER_CPP
#define CONTROLLER_CPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Controller.h"
#include "config.h"

#include "sensors/ManagedSensor.h"
#include "sensors/CCS811.h"
#include "sensors/DHT11.h"
#include "sensors/Photoresistor.h"
#include "sensors/BH1750.h"
#include "sensors/ResistiveSoil.h"
#include "sensors/AHT10.h"
#include "sensors/VoltageDivider.h"

void Controller::connectToAP(const char* SSID, const char* passwd) {
  WiFi.begin(SSID, passwd);
}

bool Controller::setupSoftAP(const char* SSID, const char* passwd) {
  WiFi.softAPConfig(IPAddress(10,1,0,1), IPAddress(10,1,0,1), IPAddress(255,255,255,0));
  return WiFi.softAP(SSID, passwd, 1, HUB_SSID_HIDDEN, 8);
}

void Controller::addSensor(Sensor* sensor) {
  this->sensors.push_back(sensor);
}

void Controller::registerSensors() {
  #ifdef DHT11_Sensor
  this->addSensor(new DHT11Sensor(DHT11_PIN));
  #endif

  #ifdef AHT10_Sensor
  this->addSensor(new AHT10Sensor());
  #endif

  #ifdef CCS811_Sensor
  this->addSensor(new CCS811());
  #endif

  #ifdef Photoresistor_Sensor
  this->addSensor(new Photoresistor());
  #endif

  #ifdef BH1750_Sensor
  this->addSensor(new BH1750Sensor());
  #endif

  #ifdef ResistiveSoil_Sensor
  this->addSensor(new ResistiveSoil());
  #endif

  #ifdef Voltage_Divider
  this->addSensor(new VoltageDivider());
  #endif
}

#endif