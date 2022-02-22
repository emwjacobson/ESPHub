#ifndef CONFIG_H
#define CONFIG_H

#define PRINT(msg) Serial.print(msg)
#define PRINTLN(msg) Serial.println(msg)

///////////////////////////////////////
//////// General Configuration ////////
///////////////////////////////////////

// NODE_NAME is who the node will report it's data as, should be
// unique between devices if you want seperate entities.
#define NODE_NAME "NodeName"

// HUB_SSID is the SSID that the Hub node broadcasts and the Follower nodes connect to.
#define WIFI_SSID "YourWifi"
#define WIFI_PASSWD "YourWifiPassword"

#define MQTT_HOST "your.mqtt.broker"
#define MQTT_PORT 1883
#define MQTT_USERNAME "broker_username"
#define MQTT_PASSWD "broker_password"

// Sleep time is in microseconds. 60 seconds = 60e6 microseconds
#define SLEEP_TIME 30e6


//////////////////////////////////////
//////// Sensor Configuration ////////
//////////////////////////////////////



//////////////////////////////////////////
//    Temperature & Humidity Sensors    //
//////////////////////////////////////////

// THE DHT11 HAS NOT BEEN IMPLEMENTED IN MQTT REWRITE
// #define DHT11_Sensor // Temperature and Humidity sensor
//   #define DHT11_PIN D7
//   #define DHT11_READ_FAHRENHEIT true

#define AHT10_Sensor // Temperature and Humidity sensor
  #define AHT10_READ_FAHRENHEIT true
//   #define AHT10_TEMP_OFFSET 0 // An offset is nice when the device might heat itself up (ESPHub v0.1-v0.4)



///////////////////////
//    CO2 Sensors    //
///////////////////////

// THE CCS811 HAS NOT BEEN IMPLEMENTED IN MQTT REWRITE
// #define CCS811_Sensor // CO2 sensor. *NEEDS WARMUP PERIOD*



/////////////////////////
//    Light Sensors    //
/////////////////////////

#define BH1750_Sensor



/////////////////////////////
//    Resistive Sensors    //
/////////////////////////////

// THE RESISTIVESOIL HAS NOT BEEN IMPLEMENTED IN MQTT REWRITE
// #define ResistiveSoil_Sensor // Uses the ESP8266's only A0 analog pin!

// THE PHOTORESISTIVE HAS NOT BEEN IMPLEMENTED IN MQTT REWRITE
// #define Photoresistor_Sensor // Uses the ESP8266's only A0 analog pin!

// THE VOLTAGEDIVIDER HAS NOT BEEN IMPLEMENTED IN MQTT REWRITE
// #define Voltage_Divider // Uses the ESP8266's only A0 analog pin!
//   #define VDIV_R1 9920
//   #define VDIV_R2 810
//   #define VDIV_ANALOG_OFFSET -8

#endif