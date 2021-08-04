# ESPHub

The goal of this project is to create a single "Hub" device that "Follower" devices can connect and relay data from sensors. This data would then be able to be presented in a JSON object that could be imported into Home Assistant for easy monitoring.

![Flowchat](/images/flow.png)

## Configuration

Inside of the `src/` folder there is a file called `config.h`. This is the configuration file for the Hub and Follower nodes.