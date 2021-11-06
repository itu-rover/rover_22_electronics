#include <Arduino.h>

#include <ros.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/Float64.h>

#define ARRAY_LEN 1

ros::NodeHandle nh;
//std_msgs::Float64MultiArray command_arr;
//std_msgs::Float64MultiArray feedback_arr;

std_msgs::Float64 command;
std_msgs::Float64 feedback;
float temp_arr[1] = {0.0};

void commandCallback(const std_msgs::Float64 &command_msg);
void processCommand(const std_msgs::Float64 &command_arr);

ros::Publisher feedback_pub("encoder_feedback",&feedback);
ros::Subscriber<std_msgs::Float64> command_sub("position_command",&commandCallback);

void setup(){
    nh.initNode();
    nh.advertise(feedback_pub);
    nh.subscribe(command_sub);
}

void loop(){
    nh.spinOnce();
    //feedback_pub.publish(&feedback_arr);
    delay(1);
}


void commandCallback(const std_msgs::Float64 &command_msg){
    /*
    for (int i=0; i<ARRAY_LEN; i++){
        temp_arr[i] = command_msg.data[i];
    }
    nh.loginfo("New command!");
    feedback_pub.publish(&feedback_arr);
    */
   command.data = command_msg.data;
   nh.loginfo("New command!\n");
   processCommand(command);
   feedback_pub.publish(&feedback);
   
}

void processCommand(const std_msgs::Float64 &command){
    /*
    float arr[1] = {0.0};
    arr[0] = command_arr.data[0];
    arr[0] *= 2;
    feedback_arr.data = arr;
    */
   float processed_data = command.data;
   processed_data *= 2;
   feedback.data = processed_data;
}
