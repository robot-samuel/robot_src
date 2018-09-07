#! /bin/bash

rm -fr ./cpp/*
rm -fr ./c/*

PROJECT_NAME=robot2015_common
NODE1_NAME=management
NODE2_NAME=sensor_weather_FRT_FWS


protoc -I=. --cpp_out=./cpp/ *.proto
protoc-c -I=. --c_out=./c/ *.proto

if [ ! -d "../include/$PROJECT_NAME/cpp" ]; then
  mkdir ../include/$PROJECT_NAME/cpp
fi

if [ ! -d "../include/$PROJECT_NAME/c" ]; then
  mkdir ../include/$PROJECT_NAME/c
fi

rm -fr ../include/$PROJECT_NAME/cpp/*.h
rm -fr ../include/$PROJECT_NAME/c/*.h

cp -fr ./cpp/*.h ../include/$PROJECT_NAME/cpp/
cp -fr ./c/*.h ../include/$PROJECT_NAME/c/


if [  -d "../../$NODE1_NAME/include/$PROJECT_NAME" ]; then
  rm -fr ../../$NODE1_NAME/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../$NODE1_NAME/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../$NODE1_NAME/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../$NODE1_NAME/include/$PROJECT_NAME/c/
fi

if [  -d "../../$NODE2_NAME/include/$PROJECT_NAME" ]; then
  rm -fr ../../$NODE2_NAME/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../$NODE2_NAME/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../$NODE2_NAME/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../$NODE2_NAME/include/$PROJECT_NAME/c/
fi

if [  -d "../../zmq_ros/include/$PROJECT_NAME" ]; then
  rm -fr ../../zmq_ros/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../zmq_ros/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../zmq_ros/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../zmq_ros/include/$PROJECT_NAME/c/
fi


if [  -d "../../dgvmsg/include/$PROJECT_NAME" ]; then
  rm -fr ../../dgvmsg/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../dgvmsg/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../dgvmsg/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../dgvmsg/include/$PROJECT_NAME/c/
fi


if [  -d "../../e9gpio/include/$PROJECT_NAME" ]; then
  rm -fr ../../e9gpio/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../e9gpio/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../e9gpio/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../e9gpio/include/$PROJECT_NAME/c/
fi
