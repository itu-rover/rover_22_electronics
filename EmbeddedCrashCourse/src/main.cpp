#include <Arduino.h>

#include <SoftwareSerial.h>
#include <HardwareSerial.h>

#include <ros.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/String.h>

/* ----------------- MACROS AND CONSTANTS --------------------- */
#define ARRAY_LEN 1
#define STR_MSG_LEN 6
#define TRUE 1
#define FALSE 0


const float radian_map_coef = 300;
/* ------------------------------------------------------------ */

ros::NodeHandle nh;

/* --------------- FUNCTION DECLERATIONS --------------- */

void processData();
void commandCallback(const std_msgs::Float64MultiArray &command_msg);

void generateStrMsg();
String floatToStr(float radian_data);

void readDataFromMCU();
void parseEncoderFeedback();
float detectDirection(char direction_byte);
float decodeRadians(String mapped_radian);
void publishEncoderFeedback();


/* ---------------- GLOBAL VARIABLES -------------------- */

std_msgs::Float64MultiArray feedback_arr;
std_msgs::Float64MultiArray command_arr;

std_msgs::String debug_str;
char debug_ch[50];

float commands_to_send[ARRAY_LEN];
float enc_feedback_to_send[ARRAY_LEN] = {1.57};

String str_angle_command;
String str_enc_feedback = "A0150B";

int incoming_byte;
char str_arr[STR_MSG_LEN+1];

int receive_cnt_flag = FALSE;


/* ------------------------------------------------------- */


ros::Publisher feedback_pub("encoder_feedback",&feedback_arr);
ros::Subscriber<std_msgs::Float64MultiArray> command_sub("command_topic",&commandCallback);

ros::Publisher debug_pub("debug_topic",&debug_str);

/* ------------- SERIAL CONFIG FOR OTHER STMS -----------*/
HardwareSerial armSerial(PB11,PB10); // RX, TX


void setup(){
    nh.initNode();

    feedback_arr.data = (float*)malloc(sizeof(float)*ARRAY_LEN);
    feedback_arr.data_length = ARRAY_LEN;

    command_arr.data = (float*)malloc(sizeof(float)*ARRAY_LEN);
    command_arr.data_length = ARRAY_LEN;

    nh.advertise(feedback_pub);
    nh.subscribe(command_sub);

    nh.advertise(debug_pub);

    // Serial inits
    armSerial.begin(57600);

}

void loop(){
    nh.spinOnce();

    processData();
    readDataFromMCU();
    publishEncoderFeedback();

    delay(50);

    
}

/* --------------- FUNCTION DEFINITIONS --------------- */

void readDataFromMCU(){
     
    if (armSerial.available() > 0){
        incoming_byte = armSerial.read();
        if (incoming_byte == 'A'){
            str_enc_feedback = "";
            str_enc_feedback += (char) incoming_byte;
            return;
        }
        else if (receive_cnt_flag = TRUE && incoming_byte != 'B'){
            str_enc_feedback += (char) incoming_byte;
        }
        else if (incoming_byte == 'B'){
            str_enc_feedback += (char) incoming_byte;
            str_enc_feedback.toCharArray(str_arr,STR_MSG_LEN+1);
            nh.loginfo(str_arr);

            
            parseEncoderFeedback();
            publishEncoderFeedback();

            str_enc_feedback = "";
        }   

    }
}

void publishEncoderFeedback(){

    for (int i = 0; i < ARRAY_LEN; i++){
        feedback_arr.data[i] = enc_feedback_to_send[i];
    }
    feedback_pub.publish(&feedback_arr);

}

//------------------  WIP  ------------------//
void parseEncoderFeedback(){

  if (str_enc_feedback[0] == 'A' && str_enc_feedback[STR_MSG_LEN-1] == 'B'){
    float direction;
    float angle;
    String mapped;
    int counter = 0;
    for (int i = 0; i < ARRAY_LEN; i += 4){
      mapped = "";
      direction = detectDirection(str_enc_feedback[i+1]);
      mapped += str_enc_feedback[i+2] + str_enc_feedback[i+3] + str_enc_feedback[i+4];
      angle = decodeRadians(mapped);
      enc_feedback_to_send[counter] = direction*angle;
      counter++;
    }

  }

  return;
}

float detectDirection(char direction_byte){
  if (direction_byte == '0'){
    return -1.0;
  }
  else{
    return 1.0;
  }
}

float decodeRadians(String mapped_radian){
  float in_radians = mapped_radian.toFloat();

  in_radians = in_radians/radian_map_coef;

  return in_radians;
}

//------------------  WIP  ------------------//

void processData(){
    for (int i=0; i<ARRAY_LEN;i++){
        feedback_arr.data[i] = command_arr.data[i]*2;
    }
}

void commandCallback(const std_msgs::Float64MultiArray &command_msg){
    for (int i=0; i<ARRAY_LEN; i++){
        commands_to_send[i] = command_msg.data[i];
    }
    nh.loginfo("New command!");
    


    generateStrMsg();
    str_angle_command.toCharArray(debug_ch,50);
    debug_str.data = debug_ch;
    debug_pub.publish(&debug_str);
    
    armSerial.println(str_angle_command);
    nh.loginfo("Sent\n\n");
}

void generateStrMsg(){
    str_angle_command = "S";

    for (int i=0; i<ARRAY_LEN; i++){
        str_angle_command += floatToStr(commands_to_send[i]);
    }

    str_angle_command += "F";

}

String floatToStr(float radian_data){
    String mapped_angle;
    if (radian_data > 0){
        mapped_angle = "1";
    }
    else{
        mapped_angle = "0";
    }
    radian_data = abs(radian_data);
    int encoded_radian = radian_data * radian_map_coef;

    String encoded_str = String(encoded_radian);

    while (encoded_str.length() < 3){
        encoded_str = "0" + encoded_str;
    }

    mapped_angle += encoded_str;

    return mapped_angle;
}
