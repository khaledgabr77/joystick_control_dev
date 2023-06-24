#include <iostream>
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Joy.h"
#include <vector>

class JoystickNode {
private:
    ros::Publisher  vel_pub;
    ros::Subscriber joy_subscriber;
    geometry_msgs::Twist vel_msg;

    std::vector<float> axes;
    std::vector<float> buttons;

    bool data_recived=false;

    float linear_speed=0.25;
    float angular_speed=0.5;

    std::string cmd_vel_topic="cmd_vel";

    int index1=1;
    int index2=0;
    int index3=0;
    int index4=1;
    int index5=3;
    int index6=2;

    public:

    JoystickNode(){
        ros::NodeHandle n;
        ros::NodeHandle nh_private("~");
        nh_private.param("cmd_vel_topic", cmd_vel_topic);
        nh_private.param("linear_direction_index", index1);
        nh_private.param("angular_direction_index", index2);
        nh_private.param("linear_speed_increase_index", index3);
        nh_private.param("linear_speed_decrease_index", index4);
        nh_private.param("angular_speed_increase_index", index5);
        nh_private.param("angular_speed_decrease_index", index6);

        vel_pub = n.advertise<geometry_msgs::Twist>(cmd_vel_topic, 10); 
        joy_subscriber = n.subscribe("joy",10, &JoystickNode::joy_callback, this);


    }


    void joy_callback(const sensor_msgs::Joy::ConstPtr&  msg){

        data_recived=true;

        axes = msg->axes;
        buttons = msg->buttons;

    }

    void direction_control(){

       if (data_recived)
        {  

            vel_msg.linear.x  =  axes[index1]* linear_speed ;
            vel_msg.angular.z =  axes[index2]* angular_speed; 
            
            vel_pub.publish(vel_msg);

        }
    }

    void speed_control(){

       if (data_recived)
        {  
            if (buttons[index3]==1)
            {
                 linear_speed +=0.2;
            }

            else if (buttons[index4]==1 && linear_speed>0.2)
            {
                 linear_speed -=0.2;
            }   

            else if (buttons[index5]==1)
            {
                 angular_speed +=0.2;
            }    
                      
            else if (buttons[index6]==1&& angular_speed> 0.2)
            {
                 angular_speed -=0.2;
            }    
       
        
        vel_pub.publish(vel_msg);

        }
    }
          
    void printData() {
        if (data_received) {
            std::cout << "axes_array: ";
            for (const auto& axis : axes) {
                std::cout << axis << " ";
            }
            std::cout << std::endl;

            std::cout << "buttons_array: ";
            for (const auto& button : buttons) {
                std::cout << button << " ";
            }
            std::cout << std::endl;
        }
    }
};

int main (int argc, char **argv)
{
    ros::init(argc, argv, "joystick_node");
    

    JoystickNode joystick;
    ros::Rate loop_rate(10);

    while (ros::ok())
    {
        joystick.speed_control();
        joystick.direction_control();
        
    //*****print data for debuging******
        // joystick.print_data();

        ros::spinOnce();
        loop_rate.sleep();
    }
    
}
