#ifndef CONFIG_H
#define CONFIG_H

#include "ESP8266WiFi.h"

///////////////////////////////////////
//////// General Configuration ////////
///////////////////////////////////////

// NODE_NAME is who the node will report it's data as, should be
// unique between devices if you want seperate entities.
#define NODE_NAME "Bedroom"

// HUB_SSID is the SSID that the Hub node broadcasts and the Follower nodes connect to.
#define HUB_SSID "ESPHub"
#define HUB_PASSWD "ChangeMe!"



////////////////////////////////////////
//////// Hub Node Configuration ////////
////////////////////////////////////////

// EXTERNAL_SSID is the SSID that the Hub node will connect to to relay data from the follower nodes.
#define EXTERNAL_SSID ""
#define EXTERNAL_PASSWD ""

// Set HUB_SSID_HIDDEN to 0 to hide SSID from showing up.
#define HUB_SSID_HIDDEN 0



/////////////////////////////////////////////
//////// Follower Node Configuration ////////
/////////////////////////////////////////////

// Only one mode should be uncommented. If multiple are uncommented then the first one will be
// active one. The Readme provides more details about each mode.
// #define MODE_DEEP_SLEEP
#define MODE_ACTIVE_DELAY
  #define ACTIVE_DELAY_INTERNET_DISCONNECT
// #define MODE_ACTIVE



//////////////////////////////////////
//////// Sensor Configuration ////////
//////////////////////////////////////


//
// Temperature & Humidity Sensors
//

// #define DHT11_Sensor // Temperature and Humidity sensor
//   #define DHT11_PIN D7
//   #define DHT11_READ_FAHRENHEIT true

#define AHT10_Sensor // Temperature and Humidity sensor
  #define AHT10_READ_FAHRENHEIT true
  #define AHT10_TEMP_OFFSET 0 // An offset is nice when the device might heat itself up (ESPHub v0.1-v0.4)

//
// CO2 Sensors
//

// #define CCS811_Sensor // CO2 sensor. *NEEDS WARMUP PERIOD*

//
// Light Sensors
//

#define BH1750_Sensor

//
// Resistive Sensors
//

// #define ResistiveSoil_Sensor // Uses the ESP8266's only A0 analog pin!

// #define Photoresistor_Sensor // Uses the ESP8266's only A0 analog pin!

// #define Voltage_Divider // Uses the ESP8266's only A0 analog pin!
//   #define VDIV_R1 9920
//   #define VDIV_R2 810
//   #define VDIV_ANALOG_OFFSET -8

#endif