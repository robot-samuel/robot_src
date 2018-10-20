#! /bin/bash

rm -fr ./cpp/*
rm -fr ./c/*

PROJECT_NAME=robot2015_common

protoc -I=. --cpp_out=./cpp/ *.proto

if [ ! -d "../include/$PROJECT_NAME/cpp" ]; then
  mkdir ../include/$PROJECT_NAME/cpp
fi


rm -fr ../include/$PROJECT_NAME/cpp/*.h

cp -fr ./cpp/*.h ../include/$PROJECT_NAME/cpp/




if [  -d "../../joyde/include/$PROJECT_NAME" ]; then
  rm -fr ../../joyde/include/$PROJECT_NAME/cpp/*.h
  cp -fr ./cpp/*.h ../../joyde/include/$PROJECT_NAME/cpp/
fi

if [  -d "../../zmq_ros/include/$PROJECT_NAME" ]; then
  rm -fr ../../zmq_ros/include/$PROJECT_NAME/cpp/*.h
  cp -fr ./cpp/*.h ../../zmq_ros/include/$PROJECT_NAME/cpp/
fi

if [  -d "../../MCtrlNode/include/$PROJECT_NAME" ]; then
  rm -fr ../../MCtrlNode/include/$PROJECT_NAME/cpp/*.h
  cp -fr ./cpp/*.h ../../MCtrlNode/include/$PROJECT_NAME/cpp/
fi

