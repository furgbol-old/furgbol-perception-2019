#include "robot.h"

/*Constructor*/
Robot::Robot(int _id): id(_id), frames_in(0), frames_out(0), confidence(0.f){}

Robot::operator RobotPackage() const
{
    RobotPackage package;
    package.set_id(id);
    package.set_x(state[0][0]);
    package.set_y(state[1][0]);
    package.set_orientation(state[2][0]);
    return package;
}

/*Getters*/
int Robot::getId(){
    return id;
}

float Robot::getX()
{
    return state[0][0];
}

float Robot::getY()
{
    return state[1][0];
}

float Robot::getTheta()
{
    return state[2][0];
}

Mat_<float> Robot::getState(){
    return state.clone();
}

float Robot::getConfidence(){
    return confidence;
}

/*Control methods*/
bool Robot::found()
{
    if(frames_out>0 && frames_out<FRAMES_NOT_FOUNDED){
        return true;
    }
    else if(frames_in>=FRAMES_FOUNDED){
        return true;
    }
    else{
        return false;
    }
}

void Robot::frameInside()
{
    frames_out = 0;
    frames_in++;
}

void Robot::frameOutside()
{
    frames_in = 0;
    frames_out++;
}
