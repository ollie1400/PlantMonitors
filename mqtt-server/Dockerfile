# Use the official lightweight Python image.
# https://hub.docker.com/_/python
FROM python:3.9

RUN mkdir -p ./app
WORKDIR /app
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y \
       mosquitto \
       sudo
COPY . ./
RUN pip install -r requirements.txt

#RUN useradd -m docker && echo "docker:docker" | chpasswd && adduser docker sudo
#USER docker
ENTRYPOINT ./startserver.sh