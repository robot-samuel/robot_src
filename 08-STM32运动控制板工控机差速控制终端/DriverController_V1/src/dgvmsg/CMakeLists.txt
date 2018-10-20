cmake_minimum_required(VERSION 2.8.3)
project(dgvmsg)


find_package(catkin REQUIRED COMPONENTS
  message_generation
  geometry_msgs
)



## Generate messages in the 'msg' folder
 add_message_files(
   	FILES
	ctrl_ModeMessage.msg
	Imu.msg
	Joy.msg
	String.msg
	Twist.msg
	Vector3.msg
	LaserScan.msg
	LaserEcho.msg
	JoyFeedbackArray.msg
	JoyFeedback.msg
	DriverNode.msg
	DriverVelocity.msg
	Encounter.msg
	EncounterV.msg
	remoter.msg
	remoterctrl.msg
	DriverStatues.msg
	Bms.msg
	ExternSensor.msg
 )

generate_messages(DEPENDENCIES  geometry_msgs)

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS message_runtime geometry_msgs)

  