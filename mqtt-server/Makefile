SHELL:=bash

CURRENT_DIR = $(shell pwd)
PYTHON = $(shell which python)
PIP = $(shell which pip)


# package
build:
	# --no-cache
	docker build --no-cache -t mqtt-server .

run:
	docker run -p 1234:1234 -t mqtt-server 

publish:
	@twine upload dist/* -r codi