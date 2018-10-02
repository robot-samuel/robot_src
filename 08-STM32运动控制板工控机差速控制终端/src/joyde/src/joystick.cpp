#include <unistd.h>
#include <math.h>

#include <fcntl.h>

#include "joystick.hpp"

#define DUBUG_ECHO 0


Joystick::Joystick(): nh_(), diagnostic_()
{
}

Joystick::~Joystick() {



}

///\brief Publishes diagnostics and status
void Joystick::diagnostics(diagnostic_updater::DiagnosticStatusWrapper& stat)
{
  double now = ros::Time::now().toSec();
  double interval = now - lastDiagTime_;
  if (open_)
    stat.summary(0, "OK");
  else
    stat.summary(2, "Joystick not open.");
  
  stat.add("topic", pub_.getTopic());
  stat.add("device", joy_dev_);
  stat.add("dead zone", deadzone_);
  stat.add("autorepeat rate (Hz)", autorepeat_rate_);
  stat.add("coalesce interval (s)", coalesce_interval_);
  stat.add("recent joystick event rate (Hz)", event_count_ / interval);
  stat.add("recent publication rate (Hz)", pub_count_ / interval);
  stat.add("subscribers", pub_.getNumSubscribers());
  event_count_ = 0;
  pub_count_ = 0;
  lastDiagTime_ = now;
}

void Joystick::init()
{
    //初始化注册诊断模块
    //diagnostic_.add("Joystick Driver Status", this, &Joystick::diagnostics);
   // diagnostic_.setHardwareID("none");

     // Parameters
    ros::NodeHandle nh_param("~");
    pub_ = nh_.advertise<sensor_msgs::Joy>("Joyctrl", 1);
    if(ros::param::has("/Joystick/dev")){
          
            nh_param.getParam("/Joystick/dev", joy_dev_);
        }
    else{
          ros::param::set("/Joystick/dev", "/dev/input/js0");
          nh_param.param<std::string>("/Joystick/dev", joy_dev_, "/dev/input/js0");
        }
    nh_param.param<double>("deadzone", deadzone_, 0.05);
    nh_param.param<double>("autorepeat_rate", autorepeat_rate_, 0);
    nh_param.param<double>("coalesce_interval", coalesce_interval_, 0.001);  


    // Checks on parameters
    if (autorepeat_rate_ > 1 / coalesce_interval_){
                  ROS_WARN("joy_node: autorepeat_rate (%f Hz) > "
                  "1/coalesce_interval (%f Hz"
            ") does not make sense. Timing behavior is not well defined.", 
            autorepeat_rate_, 
            1/coalesce_interval_);
        }
    
        if (deadzone_ >= 1)
        {
          ROS_WARN("joy_node: deadzone greater than 1 was requested. The "
    "semantics of deadzone have changed. It is now related to the range [-1:1] "
    "instead of [-32767:32767]. For now I am dividing your deadzone by 32767, but" 
    "this behavior is deprecated so you need to update your launch file.");
          deadzone_ /= 32767;
        }
      if (deadzone_ > 0.9)
        {
              ROS_WARN("joy_node: deadzone (%f) greater than 0.9, setting it to 0.9", 
                            deadzone_);
                        deadzone_ = 0.9;
            }
            
       if (deadzone_ < 0)
            {
              ROS_WARN("joy_node: deadzone_ (%f) less than 0, setting to 0.", 
                            deadzone_);
                            deadzone_ = 0;
            }
        if (autorepeat_rate_ < 0)
                {
                  ROS_WARN("joy_node: autorepeat_rate (%f) less than 0, setting to 0.", 
            autorepeat_rate_);
                  autorepeat_rate_ = 0;
                }
                
        if (coalesce_interval_ < 0)
                {
                  ROS_WARN("joy_node: coalesce_interval (%f) less than 0, setting to 0.", 
            coalesce_interval_);
                  coalesce_interval_ = 0;

                }



        



}

int Joystick::devopen()
{
    
    bool first_fault = true;
    //open device
     joy_fd = -1;
    
    while (true)
    {
        ros::spinOnce();
        if (!nh_.ok()){
                ROS_INFO("joy_node shut down.");
                return joy_fd;
            }
        joy_fd = open(joy_dev_.c_str(), O_RDONLY);
        
         if (joy_fd != -1)
         {
        // There seems to be a bug in the driver or something where the
        // initial events that are to define the initial state of the
        // joystick are not the values of the joystick when it was opened
        // but rather the values of the joystick when it was last closed.
        // Opening then closing and opening again is a hack to get more
        // accurate initial state data.
                devclose();
                joy_fd = open(joy_dev_.c_str(), O_RDONLY|O_NONBLOCK);
             }
      if (joy_fd != -1){
                return joy_fd;
        }
      
      if (first_fault)
      {
        ROS_ERROR("Couldn't open joystick %s. Will retry every second.", joy_dev_.c_str());
        first_fault = false;
      }
      sleep(1.0);
    //  diagnostic_.update();
    }
    
   ROS_INFO("Opened joystick: %s. deadzone_: %f.", joy_dev_.c_str(), deadzone_);
   return joy_fd;
}   

void Joystick::devclose(){
        if(joy_fd != -1){
                close(joy_fd);
                joy_fd = -1;
            }
          joy_fd = -1;
}

void Joystick::procReadPub()
{


        js_event event;
        struct timeval tv;
        fd_set set;
       
        event_count_ = 0;
        pub_count_ = 0;
        
        lastDiagTime_ = ros::Time::now().toSec();    


            open_ = false;
           // diagnostic_.force_update();
            devopen();
            if(joy_fd<0){
                    ROS_ERROR("Couldn't open joystick %s. Will retry every second.", joy_dev_.c_str());
                    return;
                }
            open_ = true;
            //diagnostic_.force_update();

            tv.tv_sec = 0;
            tv.tv_usec =1000*50;   
            

            while (nh_.ok()) 
                {
                  ros::spinOnce();
                  tv.tv_sec = 0;
                  tv.tv_usec =1000*50;                     
                  bool publish_now = false;
                  FD_ZERO(&set);
                  FD_SET(joy_fd, &set);
                  switch(select(joy_fd+1, &set, NULL, NULL, &tv)) //select使用
                    {
                        case -1: 
                                //tv.tv_sec = 0;
                                //tv.tv_usec =0;                   
                                break; //select错误，退出程序 
                         case 0:
                                   if (read(joy_fd, &event, sizeof(js_event)) == -1 && errno != EAGAIN){
                                               
                                               break; // Joystick is probably closed. Definitely occurs.
                                       }  
                                   //ROS_INFO("Read data...");
                                   joy_msg.header.stamp = ros::Time().now();
                                   event_count_++; 
                                   publish_now = recordjoyinput(event);

                                break; //再次轮询 
                                
                          default: 
                               if (FD_ISSET(joy_fd, &set)){
                                   if (read(joy_fd, &event, sizeof(js_event)) == -1 && errno != EAGAIN){
                                               break; // Joystick is probably closed. Definitely occurs.
                                       }
                                    //ROS_INFO("Read data...");
                                    joy_msg.header.stamp = ros::Time().now();
                                  event_count_++; 
                                    publish_now = recordjoyinput(event);
                                }
                               break;

                    }
                                                   
                  if (publish_now)
                  {
                    // Assume that all the JS_EVENT_INIT messages have arrived already.
                    // This should be the case as the kernel sends them along as soon as
                    // the device opens.
                   // std::cout<<"pub_count_="<<pub_count_;
                  //  std::cout<<" event_count_="<<event_count_<<std::endl;
                   if(!check_joymsg_init()){
                    #if DUBUG_ECHO
                    print_joymsg();
                    #endif
                    pub_.publish(joy_msg);
                    publish_now = false;
                   pub_count_=event_count_;
                  // pub_count_=0;
                   // event_count_=0;
                    }
                   else{
                   publish_now = false;
                   if((pub_count_+1)==event_count_){
                           #if DUBUG_ECHO
                          print_joymsg();
                           #endif
                         pub_.publish(joy_msg);

                         
                    }
                        pub_count_=0;
                        event_count_=1;
                    }

                  }
                  
              
                 // diagnostic_.update();
                } // End of joystick open loop.
              devclose();
              ros::spinOnce();
              if (nh_.ok()){
                    ROS_ERROR("Connection to joystick device lost unexpectedly. Will reopen.");
                }

}

void Joystick::print_joymsg(){
    
    std::string buttonsstr;
    for(int index=0;index< joy_msg.buttons.size();index++){
            boost::format fmt("buttons[%d]=%f");
            fmt %index;
            fmt % joy_msg.buttons[index];
            buttonsstr+=fmt.str()+",";
        }
    //std::cout<<str<<std::endl;str.clear();
     std::string axesstr;
    for(int index=0;index< joy_msg.axes.size();index++){
            boost::format fmt("axes[%d]=%f");
            fmt %index;
            fmt %joy_msg.axes[index];
            axesstr+=fmt.str()+",";
        }    

    //std::cout<<str<<std::endl;
     ROS_INFO("%s\n%s",buttonsstr.c_str(),axesstr.c_str());
    
}

bool Joystick::recordjoyinput( js_event event){

    // Parameter conversions
    double autorepeat_interval = 1 / autorepeat_rate_;
    double scale = -1. / (1. - deadzone_) / 32767.;
    double unscaled_deadzone = 32767. * deadzone_;

    bool publish_now = false;
    switch(event.type){
            case JS_EVENT_BUTTON:
            case JS_EVENT_BUTTON | JS_EVENT_INIT:
                       if(event.number >= joy_msg.buttons.size()){
                                int old_size = joy_msg.buttons.size();
                                 //示调整容器vv的大小为n，扩容后的每个元素的值为element，默认为0
                                 joy_msg.buttons.resize(event.number+1);
                                for(unsigned int i=old_size;i<joy_msg.buttons.size();i++){
                                            joy_msg.buttons[i] = 0.0;
                                        }
                            }
                       //获取读数
                        joy_msg.buttons[event.number] = (event.value ? 1 : 0);
                        // For initial events, wait a bit before sending to try to catch
                        // all the initial events.
                        if (!(event.type & JS_EVENT_INIT)){
                                    publish_now = true;
                           }
                        break;
               case JS_EVENT_AXIS:
               case JS_EVENT_AXIS | JS_EVENT_INIT:
                           if(event.number >= joy_msg.axes.size()){
                                    int old_size = joy_msg.axes.size();
                                    //示调整容器vv的大小为n，扩容后的每个元素的值为element，默认为0
                                    joy_msg.axes.resize(event.number+1);
                                    for(unsigned int i=old_size;i<joy_msg.axes.size();i++)
                                    joy_msg.axes[i] = 0.0;
                               }
  
                            if (!(event.type & JS_EVENT_INIT)) {
                                            double val = event.value;
                                            // Allows deadzone to be "smooth"
                                            if (val > unscaled_deadzone)
                                                        val -= unscaled_deadzone;
                                            else if (val < -unscaled_deadzone)
                                                        val += unscaled_deadzone;
                                            else
                                                         val = 0;
                                            // joy_msg.axes[event.number] = val * scale;
                                            joy_msg.axes[event.number] = val ;
                                            publish_now = true;
                                }
                            // Will wait a bit before sending to try to combine events.    
                        break;
                default:
                        ROS_WARN("joy_node: Unknown event type. Please file a ticket. time=%u, value=%d, type=%Xh, number=%d", event.time, event.value, event.type, event.number);
                        break;
         }
        return publish_now;


}

int Joystick::check_joymsg_init(){
    
   // std::string buttonsstr;
     //static sensor_msgs::Joy joy_msg; 
    for(int index=0;index< joy_msg.buttons.size();index++){
           // boost::format fmt("buttons[%d]=%f");
          //  fmt %index;
           // fmt % joy_msg.buttons[index];
           // buttonsstr+=fmt.str()+",";

            if( fabs(joy_msg.buttons[index])>0.1){
                    return 0;
                }
        }
    //std::cout<<str<<std::endl;str.clear();
    // std::string axesstr;
    for(int index=0;index< joy_msg.axes.size();index++){
            //boost::format fmt("axes[%d]=%f");
           // fmt %index;
           // fmt %joy_msg.axes[index];
            //axesstr+=fmt.str()+",";
             if( (fabs(joy_msg.axes[index])>0.1)&&(index!=4)&&(index!=5)){
                    return 0;
                }
        }    
    return 1;
    //std::cout<<str<<std::endl;
    // ROS_INFO("%s\n%s",buttonsstr.c_str(),axesstr.c_str());
    
}


