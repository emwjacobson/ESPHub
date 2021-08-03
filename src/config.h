#ifndef CONFIG_H
#define CONFIG_H


///////////////////////////////////////
//////// General Configuration ////////
///////////////////////////////////////

// HUB_SSID is the SSID that the Master node broadcasts and the Follower nodes connect to.
#define HUB_SSID "ESPHub"
#define HUB_PASSWD "ESPHubPassword"

// Set HUB_SSID_HIDDEN to 0 to hide SSID from showing up.
#define HUB_SSID_HIDDEN 0

// NODE_NAME is who the node will report it's data as, should be
// unique between devices if you want seperate entities.
#define NODE_NAME "Bedroom"

// The buffer sizes are the maximum length of characters for the
// node name, type, and value of sensors. Anything longer will be truncated.
#define NAME_BUFFER 16
#define TYPE_BUFFER 16
#define VALUE_BUFFER 8

// MAX_SENSORS is the maximum amount of sensors that any one follower node can have.
#define MAX_SENSORS 8



///////////////////////////////////////////
//////// Master Node Configuration ////////
///////////////////////////////////////////

// EXTERNAL_SSID is the SSID that the Master node will connect to to relay data from the follower nodes.
#define EXTERNAL_SSID ""
#define EXTERNAL_PASSWD ""

// MAX_NODES is the maximum amount of Follower nodes that will be allowed to connect to the master node.
#define MAX_NODES 8



/////////////////////////////////////////////
//////// Follower Node Configuration ////////
/////////////////////////////////////////////

// Uncomment all of the sensors that you would like to enable on this follower node
// Make sure that the number of sensors enabled is <= MAX_SENSORS in the 'General Configuration' section.
#define DHT11

#endif