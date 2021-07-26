#!/bin/bash

if [ ! -d "venv" ]; then
    python3 -m venv venv
fi

if [ -d "venv/bin/activate" ]; then
    source venv/bin/activate
else
    source venv/Scripts/activate
fi

pip install -r requirements.txt
