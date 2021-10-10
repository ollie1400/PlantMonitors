import docker
import subprocess
import os
from typing import List


def is_server_container_running():
    """
    Check if the docker container with a name `mqtt_server` is running
    """
    running_containers = docker.from_env().containers.list()
    if len(running_containers) == 0:
        return False

    for i in range(len(running_containers)):
        running_container = running_containers[i]
        if running_container.name == "mqtt_server":
            return True
    return False


def __run_make_command(args: List[str] = []) -> None:
    this_file_path = os.path.dirname(os.path.abspath(__file__))
    root_dir = os.path.join(this_file_path, "..")
    cwd = os.getcwd()
    os.chdir(root_dir)
    full_args = ["make"] + args
    make_build_result = subprocess.check_call(full_args)
    os.chdir(cwd)
    if make_build_result != 0:
        full_args_string = " ".join(full_args)
        raise Exception(
            f"'{full_args_string}' command exited with code {make_build_result}")


def start_container():
    __run_make_command(["run"])


def start_container_if_not_running(build=True):
    if build:
        run_make_build()
    if not is_server_container_running():
        start_container()

    


def run_make_build():
    """
    Invoke  `make build` to build the docker container
    Implementation depends on the location of this file w.r.t the project root
    i.e. if this file is moved, this function will break
    """
    __run_make_command(["build"])


if __name__ == "__main__":
    start_container()
