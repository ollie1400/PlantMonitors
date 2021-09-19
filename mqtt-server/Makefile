SHELL:=bash

CURRENT_DIR = $(shell pwd)
PYTHON = $(shell which python)
PIP = $(shell which pip)


# package
build:
	docker build -t mqtt-server .

# gcloud compute firewall-rules delete plantserver-in
# gcloud compute firewall-rules delete plantserver-out

provision:
	gcloud compute firewall-rules create plantserver-in \
		--direction=ingress \
		--rules=tcp:1883,udp:1883,tcp:1234,udp:1234 \
		--action=allow \
		--source-ranges=0.0.0.0/0 \
		--target-tags plantserver

	gcloud compute firewall-rules create plantserver-out \
		--direction=egress \
		--rules=tcp:1883,udp:1883,tcp:1234,udp:1234 \
		--action=allow \
		--destination-ranges=0.0.0.0/0 \
		--target-tags plantserver

	gcloud compute instances create-with-container instance-1 \
		--container-image=us.gcr.io/plantmonitors/mqtt-server:latest \
		--machine-type=f1-micro \
		--maintenance-policy=MIGRATE \
		--zone=us-central1-a \
		--description="MQTT Server" \
		--tags=plantserver,http-server,https-server

teardown:
	gcloud compute instances delete instance-1 --zone=us-central1-a --quiet
	gcloud compute firewall-rules delete plantserver-in --quiet
	gcloud compute firewall-rules delete plantserver-out --quiet


publish:
	docker tag mqtt-server us.gcr.io/plantmonitors/mqtt-server
	docker push us.gcr.io/plantmonitors/mqtt-server:latest

run:
	docker run -p 1234:1234 -p 1883:1883 -t mqtt-server --network host