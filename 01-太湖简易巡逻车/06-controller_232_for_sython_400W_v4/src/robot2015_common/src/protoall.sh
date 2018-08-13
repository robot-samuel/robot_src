#! /bin/bash

rm -fr ./cpp/*
rm -fr ./c/*

PROJECT_NAME=robot2015_common

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

if [  -d "../../bms/include/$PROJECT_NAME" ]; then
  rm -fr ../../bms/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../bms/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../bms/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../bms/include/$PROJECT_NAME/c/
fi

if [  -d "../../extern_sensor/include/$PROJECT_NAME" ]; then
  rm -fr ../../extern_sensor/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../extern_sensor/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../extern_sensor/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../extern_sensor/include/$PROJECT_NAME/c/
fi

if [  -d "../../imu/include/$PROJECT_NAME" ]; then
  rm -fr ../../imu/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../imu/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../imu/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../imu/include/$PROJECT_NAME/c/
fi

if [  -d "../../joyde/include/$PROJECT_NAME" ]; then
  rm -fr ../../joyde/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../joyde/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../joyde/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../joyde/include/$PROJECT_NAME/c/
fi

if [  -d "../../laser_ctrol/include/$PROJECT_NAME" ]; then
  rm -fr ../../laser_ctrol/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../laser_ctrol/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../laser_ctrol/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../laser_ctrol/include/$PROJECT_NAME/c/
fi

if [  -d "../../management/include/$PROJECT_NAME" ]; then
  rm -fr ../../management/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../management/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../management/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../management/include/$PROJECT_NAME/c/
fi

if [  -d "../../remoter/include/$PROJECT_NAME" ]; then
  rm -fr ../../remoter/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../remoter/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../remoter/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../remoter/include/$PROJECT_NAME/c/
fi

if [  -d "../../sythondriver/include/$PROJECT_NAME" ]; then
  rm -fr ../../sythondriver/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../sythondriver/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../sythondriver/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../sythondriver/include/$PROJECT_NAME/c/
fi

if [  -d "../../zmq_ros/include/$PROJECT_NAME" ]; then
  rm -fr ../../zmq_ros/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../zmq_ros/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../zmq_ros/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../zmq_ros/include/$PROJECT_NAME/c/
fi

if [  -d "../../motecdriver/include/$PROJECT_NAME" ]; then
  rm -fr ../../motecdriver/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../motecdriver/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../motecdriver/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../motecdriver/include/$PROJECT_NAME/c/
fi

if [  -d "../../dgvmsg/include/$PROJECT_NAME" ]; then
  rm -fr ../../dgvmsg/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../dgvmsg/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../dgvmsg/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../dgvmsg/include/$PROJECT_NAME/c/
fi

if [  -d "../../ph3read/include/$PROJECT_NAME" ]; then
  rm -fr ../../ph3read/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../ph3read/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../ph3read/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../ph3read/include/$PROJECT_NAME/c/
fi

if [  -d "../../pantiltctrl/include/$PROJECT_NAME" ]; then
  rm -fr ../../pantiltctrl/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../pantiltctrl/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../pantiltctrl/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../pantiltctrl/include/$PROJECT_NAME/c/
fi
