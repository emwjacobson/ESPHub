# ESPHub

The goal of this project is to create a single "Hub" device that "Follower" devices can connect and relay data from sensors. This data would then be able to be presented in a JSON object that could be imported into Home Assistant for easy monitoring.

![Flowchat](/images/flow.png)

# Configuration

Inside of the `src/` folder there is a file called `config.h`. This is the configuration file for the Hub and Follower nodes.

## General Configuration

```C++
#define NODE_NAME "Bedroom"
```

`NODE_NAME` is the name of the actual node, whether it be the Hub or a Follower. All Follower nodes should have a unique node name (unless you want them reporting as the same entity, in which case you would set them to the same name.)

Names are case sensitive, so `Bedroom` and `bedroom` will show up as 2 seperate devices on the Hub.

```C++
#define HUB_SSID "ESPHub"
#define HUB_PASSWD "ESPHubPassword"
#define HUB_SSID_HIDDEN 0
```
`HUB_SSID` and `HUB_PASSWORD` is the SSID and Password that you want the Hub to setup for the followers. Optionally this SSID can be hidden if you set `HUB_SSID_HIDDEN` to `1`. This should be a unique network, different than any home network you have.

```C++
#define NAME_BUFFER 16
#define TYPE_BUFFER 16
#define VALUE_BUFFER 8
```
These `_BUFFER` variables represent the size of the buffer allocated for each node's `NAME`, and each sensors `TYPE` and `VALUE`. Realistically these numbers shouldnt need to be changed unless you know what you're doing.

One thing to note, if you name a Follower node something with length > `NAME_BUFFER`, it will be truncataed to `NAME_BUFFER` length.

```C++
#define MAX_SENSORS 8
```
This is the maximum number of sensors that any one node will report. This item applies to both the Hub and the Follower.

On a Hub device, this number dictates how many sensors that each device that reports to it can have. This number is independent per node meaning that if `MAX_SENSORS = 8` and you have `3` Follower Nodes, you can have a maximum of `24` devices if you have all `8` sensors per device.

## Setting up the Hub node

The Hub node is the main "coordinator" between all of the Follower nodes. It is the one that all of the Followers will report data back to.

```C++
#define EXTERNAL_SSID ""
#define EXTERNAL_PASSWD ""
```

These options set the SSID and Password for the Access Point you want this device to be a part of, typically your home network.

```C++
#define MAX_NODES 8
```

This is the maximum number of Follower nodes that you want to be able to report back to you. If `MAX_NODES = 8`, the up to 8 Followers can report their data to you.

**Note:** Multiple devices registered with the same name *can* technically report as 1 device, meaning you can more than `MAX_NODES` physical devices. The shared-name devices must still have <= `MAX_SENSORS` many sensors combined between them.


## Setting up Follower nodes.

Follower setup should start with giving the node a unique `NODE_NAME` to identify itself.

To enable a sensor for a Follower node, uncomment the line and any other options.

eg. To enable a DHT11 (Temperature and Humidity Sensor) connected to Pin D4, you would uncomment the following lines

```C++
#define DHT11_Sensor
  #define DHT11_PIN D4
```

Be sure to note how many sensor readings your device actually outputs, as a single device can report multiple values (such as the DHT11 device reporting temperature and humidity.)
