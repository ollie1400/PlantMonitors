Please run `activate_venv.bat` (on windows), or `activate_venv.sh` (on linux) before doing anything.

# Broker setup

A raspberry pi is set up as the server.  It is
 - running the MQTT broker (mosquitto)
 - running `server.py` as a service
 - running an mDNS service


## Running the server locally

Set up the python environment by running `source activate_venv.sh`

On raspberry pi you might need to run to make numpy work correctly (see https://numpy.org/devdocs/user/troubleshooting-importerror.html)
`sudo apt-get install libatlas-base-dev`

Run the python server by doing `python server.py`

You can do both of the above steps by running `startserver.bat` on windows, or `startserver.sh` on linux.

### Server options

You can run `server.py` with the following arguments.  All of these are optional:
- `--no-relay` Don't start the MQTT relay (useful for debugging if no relay exists or you don't want to use it)
- `--broker <mqtt_broker>` Specify the MQTT broker to use.  Default is `ttgo-server.local`
- `--port <flask_port>` Specify which port the Flask server is hosted on.  Default is `1234`
- `--db <db_file>` Specify the relative path to the database file to use.  Default is `databases\databases.db`

## Setting up the mDNS service

```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install avahi-daemon
```
Open `/etc/hosts` and change the line `127.0.1.1` to the local domain name you want to use.  In this case we use `ttgo-server`.

Open `/etc/hostname` and change the line to `ttgo-server`

Then reboot
```
sudo reboot
```

The server will now be addressable via `ttgo-server.local`

## Setting up the server as a system service

- Ensure that you have a linux user `ttgo` (`sudo adduser ttgo`)
- Edit `systemd/ttgo-server.service` to point to where you have checked out the repo, and copy it to `/etc/systemd/system`
- Reload config with `sudo systemctl daemon-reload`
- Start the service with `sudo systemctl restart ttgo-server.service`

Check any output from the server using `sudo journalctl -u ttgo-server.service`

# Pushing to GCP

Build docker container with `make build`

To run the container locally, do `make run`

## Publishing to GCP

Run `gcloud auth configure-docker` to configure docker to use gcp for auth, if this hasn't been done already

Run `make publish` to push to GCP



## Helpful links and notes

- `https://davidhamann.de/2018/02/11/integrate-bokeh-plots-in-flask-ajax/` how to update Bokeh graphs in real time
- https://www.howtogeek.com/167190/how-and-why-to-assign-the-.local-domain-to-your-raspberry-pi/
- https://www.howtogeek.com/167195/how-to-change-your-raspberry-pi-or-other-linux-devices-hostname/
- https://www.shubhamdipt.com/blog/how-to-create-a-systemd-service-in-linux/
- on one occasion, the `server.py` wasn't responsive when running as a service until a few minutes after starting...
- on one occasion, another laptop couldn't find `ttgo-server.local` until the wifi on that machine was disconnected/reconnected