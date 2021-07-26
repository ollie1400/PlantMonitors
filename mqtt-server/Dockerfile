# Use the official lightweight Python image.
# https://hub.docker.com/_/python
FROM python:3.9

RUN mkdir -p ./app
WORKDIR /app
COPY . ./
RUN pip install -r requirements.txt

RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y avahi-daemon

ENTRYPOINT ./startserver.sh