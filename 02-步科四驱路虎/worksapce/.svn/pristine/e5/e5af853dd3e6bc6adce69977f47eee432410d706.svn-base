#pragma once

#include <diagnostic_updater/diagnostic_updater.h>
#include "ros/ros.h"
#include <sensor_msgs/Joy.h>
#include <linux/joystick.h>



class Joystick
{
private:
      ros::NodeHandle nh_;
      bool open_;               
      std::string joy_dev_;
      double deadzone_;
      double autorepeat_rate_;  // in Hz.  0 for no repeat.
      double coalesce_interval_; // Defaults to 100 Hz rate limit.
      int event_count_;
      int pub_count_;
      ros::Publisher pub_;
      double lastDiagTime_;
       int joy_fd;
       
    diagnostic_updater::Updater diagnostic_;
     sensor_msgs::Joy joy_msg; // Here because we want to reset it on device close.
public:
    Joystick();
    ~Joystick() ;
    void diagnostics(diagnostic_updater::DiagnosticStatusWrapper& stat);
    void init();
    int devopen();
    void devclose();
    void procReadPub();
    void print_joymsg();
    bool recordjoyinput( js_event event);
    int check_joymsg_init();

};


