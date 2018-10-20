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

if [  -d "../../kincors232Fd133/include/$PROJECT_NAME" ]; then
  rm -fr ../../kincors232Fd133/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../kincors232Fd133/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../kincors232Fd133/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../kincors232Fd133/include/$PROJECT_NAME/c/
fi





if [  -d "../../joyde/include/$PROJECT_NAME" ]; then
  rm -fr ../../joyde/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../joyde/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../joyde/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../joyde/include/$PROJECT_NAME/c/
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



if [  -d "../../zmq_ros/include/$PROJECT_NAME" ]; then
  rm -fr ../../zmq_ros/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../zmq_ros/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../zmq_ros/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../zmq_ros/include/$PROJECT_NAME/c/
fi

if [  -d "../../kincocanfd133/include/$PROJECT_NAME" ]; then
  rm -fr ../../kincocanfd133/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../kincocanfd133/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../kincocanfd133/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../kincocanfd133/include/$PROJECT_NAME/c/
fi

if [  -d "../../dkincocanfd133/include/$PROJECT_NAME" ]; then
  rm -fr ../../dkincocanfd133/include/$PROJECT_NAME/cpp/*.h
  rm -fr ../../dkincocanfd133/include/$PROJECT_NAME/c/*.h
  cp -fr ./cpp/*.h ../../dkincocanfd133/include/$PROJECT_NAME/cpp/
  cp -fr ./c/*.h ../../dkincocanfd133/include/$PROJECT_NAME/c/
fi

