#!/bin/bash

# start mqtt broker
avahi-daemon --reload

# start server
python server.py --broker 127.0.0.1