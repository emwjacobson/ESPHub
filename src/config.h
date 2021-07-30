#ifndef CONFIG_H
#define CONFIG_H

//////// General Configuration ////////

// HUB_SSID is the SSID that the Master node broadcasts and the Follower nodes connect to.
#define HUB_SSID "ESPHub"
#define HUB_PASSWD "ESPHubPassword"

// Set HUB_SSID_HIDDEN to 0 to hide SSID from showing up.
#define HUB_SSID_HIDDEN 0



//////// Master Node Configuration ////////

// EXTERNAL_SSID is the SSID that the Master node will connect to to relay data from the follower nodes.
#define EXTERNAL_SSID ""
#define EXTERNAL_PASSWD ""

#define MAX_SENSORS 8
#define MAX_NODES 8



//////// Follower Node Configuration ////////

// NODE_NAME is who the node will report it's data as, should be
//unique between devices if you want seperate entities.
#define NODE_NAME "Bedroom"

#endif