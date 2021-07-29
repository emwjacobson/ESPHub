#ifndef CONFIG_H
#define CONFIG_H

//////// General Configuration ////////

// HUB_SSID is the SSID that the Master node broadcasts and the Follower nodes connect to.
#define HUB_SSID "HUB WIFI"
#define HUB_PASSWD "SomeFunkyPassword"

// Set HUB_SSID_HIDDEN to 0 to hide SSID from showing up.
#define HUB_SSID_HIDDEN 0



//////// Master Node Configuration ////////

// EXTERNAL_SSID is the SSID that the Master node will connect to to relay data from the follower nodes.
#define EXTERNAL_SSID ""
#define EXTERNAL_PASSWD ""

#define MAX_SENSORS 8
#define MAX_NODES 8



//////// Follower Node Configuration ////////

#define NODE_NAME_PREFIX "NODE_"

#endif