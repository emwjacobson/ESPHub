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

// Set HUB_SSID_HIDDEN to 0 to hide SSID from showing up.
#define HUB_SSID_HIDDEN 0



////////////////////////////////////////
//////// Hub Node Configuration ////////
////////////////////////////////////////

// EXTERNAL_SSID is the SSID that the Hub node will connect to to relay data from the follower nodes.
#define EXTERNAL_SSID ""
#define EXTERNAL_PASSWD ""



/////////////////////////////////////////////
//////// Follower Node Configuration ////////
/////////////////////////////////////////////

// Only one mode should be uncommented. If multiple are uncommented then the first one will be
// active one. The Readme provides more details about each mode.
// #define MODE_DEEP_SLEEP
#define MODE_ACTIVE_DELAY
// #define MODE_ACTIVE

// Uncomment all of the sensors that you would like to enable on this follower node

#define DHT11_Sensor // Temperature and Humidity sensor
  #define DHT11_PIN D4
  #define DHT11_READ_FAHRENHEIT true

#define CCS811_Sensor

#endif