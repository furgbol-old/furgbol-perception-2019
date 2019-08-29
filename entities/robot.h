#ifndef ROBOT_H
#define ROBOT_H

#include "messages_vision_manager.pb.h"
#include "definitions.h"

struct visionRobot{
    visionRobot(): time(0.f), confidence(0.f), found(false){ pose = Mat_<float>(3,1,0.f); }
    Mat_<float> pose;
    float time;
    float confidence;
    bool found;
};

class Robot
{
private:
    int id; //Id of the robot

    /*Control data*/
    int frames_in; //number of frames inside the field
    int frames_out; //number of frames outside the field

protected:
    /*Final data*/
    float confidence;
    Mat_<float> state;
    Mat_<float> velocity;

public:
    Robot(int id);

    operator RobotPackage() const;

    /*Getters*/
    int getId();
    float getX();
    float getY();
    float getTheta();
    Mat_<float> getState();
    float getConfidence();

    /*Setters*/
    virtual void setVisionData(visionRobot &vision_robot) = 0;

    /*Control methods*/
    bool found(); //verify if the robot is playing
    void frameInside(); //increases frames_in and put 0 on frames_out
    void frameOutside(); //increases frames_out and put 0 on frames_in
};

#endif // ROBOT_H
