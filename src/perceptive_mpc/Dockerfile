FROM osrf/ros:noetic-desktop-full

# select bash as default shell
SHELL ["/bin/bash", "-c"]

# install catkin
RUN apt update && apt install python3-catkin-tools python3-wstool -y
RUN source /opt/ros/melodic/setup.bash

#install system deps
RUN apt update && apt install libglpk-dev -y

# Set the working directory
WORKDIR /usr/src

RUN mkdir -p perceptive_mpc_ws/src/perceptive_mpc
WORKDIR /usr/src/perceptive_mpc_ws

RUN catkin init
RUN catkin config --extend /opt/ros/melodic --cmake-args -DCMAKE_BUILD_TYPE=Release
WORKDIR /usr/src/perceptive_mpc_ws/src
COPY . ./perceptive_mpc/
RUN wstool init . ./perceptive_mpc/perceptive_mpc_https.rosinstall
RUN catkin build perceptive_mpc

#source workspace
WORKDIR /usr/src/perceptive_mpc_ws
