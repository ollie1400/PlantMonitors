from pytest import fixture
from mqtt_server.utils import start_container_if_not_running
import requests
import json


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
