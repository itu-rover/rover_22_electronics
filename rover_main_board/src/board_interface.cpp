#include "board_interface.h"

BoardInterface::BoardInterface(int arr_len, int str_msg_len, int str_piece_len, float mapping_coef){
    _array_length=arr_len;
    _str_msg_length=str_msg_len;
    _str_piece_len=str_piece_len;
    _radian_map_coef=mapping_coef;

    command_to_send=(float *)malloc(sizeof(float)*_array_length);
    feedback_to_send=(float *)malloc(sizeof(float)*_array_length);

    feedback_arr.data=(float *)malloc(sizeof(float)*_array_length);
    feedback_arr.data_length=_array_length;

    command_arr.data=(float *)malloc(sizeof(float)*_array_length);
    command_arr.data_length=_array_length;
}

BoardInterface::~BoardInterface(){
    free(command_to_send);
    free(feedback_to_send);
    free(feedback_arr.data);
    free(command_arr.data);
}

void BoardInterface::assignFeedbackArr(std_msgs::Float64MultiArray &goal_arr){
    goal_arr=feedback_arr;
}

void BoardInterface::assignCommandArr(const std_msgs::Float64MultiArray &given_arr){
    command_arr=given_arr;
}

std_msgs::Float64MultiArray BoardInterface::returnFeedbackMultiArray(){
    return feedback_arr;
}

void BoardInterface::multiArrToArr(const std_msgs::Float64MultiArray &given_arr, float destination_arr[]){
    for(int i=0;i<_array_length;i++){
        destination_arr[i]=given_arr.data[i];
    }
}

void BoardInterface::arrToMultiArr(const float given_arr[], std_msgs::Float64MultiArray &destination_arr){
    for(int i=0;i<_array_length;i++){
        destination_arr.data[i]=given_arr[i];
    }
}

void BoardInterface::assignStrFeedback(const String &enc_str){
    str_feedback=enc_str;
}

String BoardInterface::returnCommandString(){
    return str_command;
}

float BoardInterface::detectDirection(char direction_byte){
    if(direction_byte=='0'){
        return -1.0;
    }
    else{
        return 1.0;
    }
}





