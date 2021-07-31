#!/bin/bash

# start mqtt broker
mkdir logs
chown mosquitto:mosquitto logs
mosquitto -c mosquitto/mosquitto_min.conf &

# start server
python server.py --broker localhost --broker-port 1883