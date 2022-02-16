#include <Arduino.h>
#include "drive_interface.h"
#include "HardwareSerial.h"
#include "ros.h"
#include "std_msgs/Float64MultiArray.h"

#define ARRAY_LEN 6
#define MSG_LEN ARRAY_LEN*4 + 2

int incoming_byte;
String incoming_str;
bool receive_cnt_flag;

HardwareSerial DriveSerial(PC5, PC4);

std_msgs::Float64MultiArray ros_array;
std_msgs::Float64MultiArray drive_published_feedback;
std_msgs::Float64MultiArray comingMultiArray;

DriveInterface DriveSystem(ARRAY_LEN, MSG_LEN, 0, 0);

ros::NodeHandle nh;
ros::Publisher driveFeedbackPub("drive_feedback_topic", &drive_published_feedback);
ros::Subscriber <std_msgs::Float64MultiArray> joySub("multi_array_topic", &multiArrayCallback);
void multiArrayCallback(const std_msgs::Float64MultiArray &comingMultiArray);

void DriveFeedbackListener(void);

void setup() {
  nh.initNode();
  nh.advertise(driveFeedbackPub);
  nh.subscribe(joySub);

  DriveSerial.begin(9600);
  ros_array.data=(float *)malloc(sizeof(float)*ARRAY_LEN);  
  ros_array.data_length=ARRAY_LEN;
  
  drive_published_feedback.data=(float *)malloc(sizeof(float)*ARRAY_LEN);  
  drive_published_feedback.data_length=ARRAY_LEN;
  
  ros_array.data[0]=0.55;
  ros_array.data[1]=-0.05;
  ros_array.data[2]=0.37;
  ros_array.data[3]=-0.00;
  ros_array.data[1]=-0.05;
  ros_array.data[2]=0.37;
}

void loop() {
  nh.spinOnce();
  
  /*
  //Drive SxxxF Sender
  DriveSystem.assignCommandArr(ros_array);
  DriveSystem.multiArrToArr();
  DriveSerial.println(DriveSystem.generateMCUMessage());
  //delay(500);
  */
  
  //Drive AxxxB Receiver
  DriveFeedbackListener();

}

void DriveFeedbackListener(void){
     
    if (DriveSerial.available() > 0){
   
        incoming_byte = DriveSerial.read();
        if (incoming_byte == 'A'){
            incoming_str = "";
            incoming_str += (char) incoming_byte;
            receive_cnt_flag = true;
            return;
        }
        else if (receive_cnt_flag = true && incoming_byte != 'B'){
            incoming_str += (char) incoming_byte;
        }
        else if (incoming_byte == 'B'){
            incoming_str += (char) incoming_byte;
            DriveSystem.getThrustings(incoming_str);
            DriveSerial.println(incoming_str);

            for(int i=0;i<ARRAY_LEN;i++){
              drive_published_feedback.data[i]=DriveSystem.returnFeedbackMultiArr().data[i];
            }
            driveFeedbackPub.publish(&drive_published_feedback);
            
            incoming_str = "";
            receive_cnt_flag = false;
        }   
    }
}

void multiArrayCallback(const std_msgs::Float64MultiArray &comingMultiArray){
  DriveSystem.assignCommandArr(comingMultiArray);
  DriveSystem.multiArrToArr();
  DriveSerial.println(DriveSystem.generateMCUMessage());
  delay(50);
}
