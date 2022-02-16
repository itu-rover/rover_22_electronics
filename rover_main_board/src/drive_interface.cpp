#include "drive_interface.h"

float DriveInterface::mapData(float x){
    return 255*x;
}

String DriveInterface::generateString(float x){
    String processedString = String((mapData(x)));
    while(processedString.length()<3){
        processedString = "0" + processedString;
    }
    return DriveInterface::detectDirection(x) + processedString;
}

String DriveInterface::generateMCUMessage(void){
    String sentString = "S";
    for(int i=0;i<_array_length;i++){
        sentString+=generateString(command_to_send[i]);
    }
    sentString+='F';
    return sentString;
}

void DriveInterface::getThrustings(void){
    String leftThrusting, rightThrusting;
    if(command_arr.data[0]=='A' && command_arr.data[_str_msg_length-1]=='B'){
        static int counter=0;
        while(counter<4){
            leftThrusting+=command_arr.data[counter+1];
            counter++;
        }
        counter=0;
        while(counter<4){
            rightThrusting+=command_arr.data[counter+5];
            counter++;
        }
    }
    return ;
}

void DriveInterface::getForwardSteerings(void){
    String leftForwardSteering, rightForwardSteering;
    if(command_arr.data[0]=='A' && command_arr.data[_str_msg_length-1]=='B'){
        static int counter=0;
        while(counter<4){
            leftForwardSteering+=command_arr.data[counter+9];
            counter++;
        }
        counter=0;
        while(counter<4){
            rightForwardSteering+=command_arr.data[counter+13];
            counter++;
        }
    }
    return ;
}

void DriveInterface::getBackwardSteerings(void){
    String leftBackwardSteering, rightBackwardSteering;
    if(command_arr.data[0]=='A' && command_arr.data[_str_msg_length-1]=='B'){
        static int counter=0;
        while(counter<4){
            leftBackwardSteering+=command_arr.data[counter+17];
            counter++;
        }
        counter=0;
        while(counter<4){
            rightBackwardSteering+=command_arr.data[counter+21];
            counter++;
        }
    }
    return ;
}
