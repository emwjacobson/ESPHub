# ESPHub

The goal of this project is to create a single "Hub" device that "Follower" devices can connect and relay data from sensors. This data would then be able to be presented in a JSON object that could be imported into Home Assistant for easy monitoring.

![Flowchat](/images/flow.png)

- [ESPHub](#esphub)
- [Configuration](#configuration)
  - [General Configuration](#general-configuration)
  - [Hub Configuration](#hub-configuration)
  - [Follower Configuration](#follower-configuration)
- [Home Assistant Setup](#home-assistant-setup)



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
#define HUB_PASSWD "ChangeMe!"
#define HUB_SSID_HIDDEN 0
```
`HUB_SSID` and `HUB_PASSWORD` is the SSID and Password that you want the Hub to setup for the followers. Optionally this SSID can be hidden if you set `HUB_SSID_HIDDEN` to `1`. This should be a unique network, different than any home network you have.

## Hub Configuration

The Hub node is the main "coordinator" between all of the Follower nodes. It is the one that all of the Followers will report data back to.

```C++
#define EXTERNAL_SSID ""
#define EXTERNAL_PASSWD ""
```

These options set the SSID and Password for the Access Point you want this device to be a part of, typically your home network.

## Follower Configuration

Follower setup should start with giving the node a unique `NODE_NAME` to identify itself.

There are 3 modes that a follower can be in, `MODE_DEEP_SLEEP`, `MODE_ACTIVE_SLEEP`, and `MODE_ACTIVE`. Only **one** should be uncommented in the config file.

```C++
#define
#define MODE_ACTIVE_DELAY
#define MODE_ACTIVE
```

The modes are described as follows:
- `MODE_DEEP_SLEEP`: The node will go into a deep sleep mode for between 1 and 2 minutes. This is good for super low power consumption and sensors that can take readings without warming up.
- `MODE_ACTIVE_DELAY`: The node will always remain on, this is good for sensors that might need a warmup period after starting. After reporting data the node will pause for between 1 and 2 minutes before reporting again.
- `MODE_ACTIVE`: This one continuously reports data, will only pause for 1 second before data readings.

**NOTE**: The Hub's SoftAP is limited to only 8 clients connected at a time. The Active and Active_Delay functions **DO NOT** disconnect from the AP after reporting data. This means that you will be limited to 8 Active Follower nodes connected at a time. When using the Deep Sleep mode, the node will fully disconnect from the AP. This should allow for many more nodes to be added as they should not all be connected at the same time.

To enable a sensor for a Follower node, uncomment the line and any other options.

eg. To enable a DHT11 (Temperature and Humidity Sensor) connected to Pin D4, you would uncomment the following lines

```C++
#define DHT11_Sensor
  #define DHT11_PIN D2
  #define DHT11_READ_FAHRENHEIT true
```

Be sure to note how many sensor readings your device actually outputs, as a single device can report multiple values (such as the DHT11 device reporting temperature and humidity.)

# Home Assistant Setup

For the sake of this example it will be assumed that the Hub node is located at `192.168.1.100`. The actual address of your Hub node should be statically assigned to prevent it from getting a new IP after a restart and to prevent any other changes.

The `/collect` endpoint has data from all of the connected followers and their sensors.

The `/info` endpoint has data about the Hub node itself.

The Hub can be connected to Home Assistant through a [RESTful Sensor](https://www.home-assistant.io/integrations/sensor.rest/).

The following example is how you can import data from the `/info` endpoint into Home Assistant. The following will need to placed under the `sensor:` header in the `configuration.yaml` file.

```yaml
- platform: rest
  name: esphub_stats
  json_attributes:
    - Free Heap
    - Heap Fragmentation
    - Num Nodes
  resource: http://192.168.1.100/info
  value_template: "OK"
- platform: template
  sensors:
    esphub_info_heap_free:
      value_template: "{{ state_attr('sensor.esphub_stats', 'Free Heap')|int }}"
      friendly_name: "Heap Free"
      unit_of_measurement: "Bytes"
    esphub_info_heap_fragmentation:
      value_template: "{{ state_attr('sensor.esphub_stats', 'Heap Fragmentation')|int }}"
      friendly_name: "Heap Fragmentation"
      unit_of_measurement: "%"
    esphub_info_num_nodes:
      value_template: "{{ state_attr('sensor.esphub_stats', 'Num Nodes')|int }}"
      friendly_name: "Number of Nodes"
```

This will introduce three new sensors, `sensor.esphub_info_heap_free`, `sensor.esphub_info_heap_fragmentation`, and `sensor.esphub_info_num_nodes`.

The following example shows how you can import data from the `/collect` endpoint into Home Assistant. Again this will go under the `sensor:` header. In this example, we are assuming that there is a single follower node with the name `Bedroom` and has 2 sensors: `Temperature` and `Humidity`.

```yaml
sensor:
- platform: rest
  name: esphub
  json_attributes_path: "$.nodes"
  json_attributes:
    - Bedroom
  resource: http://192.168.1.9/collect
  value_template: "OK"
- platform: template
  sensors:
    esphub_bedroom_temp:
      value_template: "{{ state_attr('sensor.esphub', 'Bedroom').Temperature }}"
      friendly_name: "Bedroom Temperature"
      unit_of_measurement: "°F"
    esphub_bedroom_humidity:
      value_template: "{{ state_attr('sensor.esphub', 'Bedroom').Humidity }}"
      friendly_name: "Bedroom Humidity"
      unit_of_measurement: "%"
```