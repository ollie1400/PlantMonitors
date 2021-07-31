SHELL:=bash

CURRENT_DIR = $(shell pwd)
PYTHON = $(shell which python)
PIP = $(shell which pip)


# package
build:
	docker build -t mqtt-server .

run:
	docker run -p 1234:1234 -p 1883:1883 -t mqtt-server --network host

publish:
	@twine upload dist/* -r codi