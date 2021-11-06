#include <Arduino.h>

#include <ros.h>
#include <std_msgs/Float64MultiArray.h>

#define ARRAY_LEN 3

ros::NodeHandle nh;

void processData();
void commandCallback(const std_msgs::Float64MultiArray &command_msg);

std_msgs::Float64MultiArray feedback_arr;
std_msgs::Float64MultiArray command_arr;

ros::Publisher feedback_pub("encoder_feedback",&feedback_arr);
ros::Subscriber<std_msgs::Float64MultiArray> command_sub("command_topic",&commandCallback);

void setup(){
    nh.initNode();

    feedback_arr.data = (float*)malloc(sizeof(float)*ARRAY_LEN);
    feedback_arr.data_length = ARRAY_LEN;

    command_arr.data = (float*)malloc(sizeof(float)*ARRAY_LEN);
    command_arr.data_length = ARRAY_LEN;

    nh.advertise(feedback_pub);
    nh.subscribe(command_sub);

}

void loop(){
    nh.spinOnce();

    processData();
    delay(1);
}

/* --------------- FUNCTION DEFINITIONS --------------- */

void processData(){
    for (int i=0; i<ARRAY_LEN;i++){
        feedback_arr.data[i] = command_arr.data[i]*2;
    }
}

void commandCallback(const std_msgs::Float64MultiArray &command_msg){
    for (int i=0; i<ARRAY_LEN; i++){
        command_arr.data[i] = command_msg.data[i];
    }
    nh.loginfo("New command!");
    feedback_pub.publish(&feedback_arr);
}
