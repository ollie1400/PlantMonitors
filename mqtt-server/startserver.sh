#!/bin/bash

# start mqtt broker
#mosquitto -c mosquitto/mosquitto_min.conf &
mosquitto -p 1883 &

# start server
python server.py --broker localhost --broker-port 1883