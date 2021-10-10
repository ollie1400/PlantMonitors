from pytest import fixture
from mqtt_server.utils import start_container_if_not_running
import requests
import json
from pyprotos.measurements_pb2 import Measurements
from datetime import datetime
import paho.mqtt.publish as publish
import time


@fixture(autouse=True, scope='session')
def setup_tests():

    # start the server
    start_container_if_not_running(build=False)

    # yield to run tests
    yield


def test_query_sensor():
    """
    Sensors should be empty initially
    """
    response = requests.get("http://localhost:1234/sensors/")
    assert response.status_code == 200, f"Server returned status code {response.status_code}"
    assert len(response.headers) > 0
    response_json = json.loads(response.content)
    assert isinstance(response_json, list)
    assert len(response_json) == 0


def __get_next_sensor() -> str:
    response = requests.get("http://localhost:1234/sensors/next/")
    assert response.status_code == 200, f"Server returned status code {response.status_code}"
    assert len(response.headers) > 0
    response_json = json.loads(response.content)
    assert isinstance(response_json, str)
    return response_json


def test_next_sensor():
    """
    Check next sensor endpoint
    """
    next_sensor = __get_next_sensor()
    assert next_sensor == "sensor0"


def __send_measurement(topic, lux, humidity, temperature_C, soil, salt, battery_mV):
    measurement = Measurements()
    measurement.timestamp = int(datetime.timestamp(datetime.now()))
    measurement.lux = lux
    measurement.humidity = humidity
    measurement.temperature_C = temperature_C
    measurement.soil = soil
    measurement.salt = salt
    measurement.battery_mV = battery_mV
    measurement_bytes = measurement.SerializeToString()
    publish.single(topic,
                   payload=measurement_bytes, hostname="localhost", port=1883)


def test_send_message():

    next_sensor = __get_next_sensor()
    next_sensor_number = int(next_sensor.replace("sensor", ""))

    # send some measurements
    for lux in range(10):
        __send_measurement(f"sensors/{next_sensor}",
                           lux, 40.5, 31.2, 10, 12, 3.34)
        time.sleep(0.5)

    # next sensor should have incremented
    new_next_sensor = __get_next_sensor()
    new_next_sensor_number = int(new_next_sensor.replace("sensor", ""))

    assert new_next_sensor_number == (next_sensor_number + 1)
