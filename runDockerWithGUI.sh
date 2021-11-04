#!/bin/bash

echo "ROS2 PROJECTS PATH              = ${PWD}/ros_projects"

HOSTPATH=${PWD}/ros_projects
CONTAINERPATH=/home/ros_projects

echo "HOST BINDMOUNT PATH         = ${HOSTPATH}"
echo "CONTAINER BINDMOUNT PATH    = ${CONTAINERPATH}"

XSOCK=/tmp/.X11-unix
XAUTH=/tmp/.docker.xauth
touch $XAUTH
xauth nlist $DISPLAY | sed -e 's/^..../ffff/' | xauth -f $XAUTH nmerge -

docker rm ros2foxydockercontainer
docker run --detach \
	--name ros2foxydockercontainer \
	-it \
	-p 1984:1984 \
	-p 7000-8000:7000-8000 \
	-v $HOSTPATH:$CONTAINERPATH:rw \
	-v $XSOCK:$XSOCK:rw \
	-v $XAUTH:$XAUTH:rw \
	-e DISPLAY=$DISPLAY \
	-e XAUTHORITY=$XAUTH \
	--ipc="host" \
	ros2foxy:latest

docker container attach ros2foxydockercontainer
