#ifndef TEAMROBOT_H
#define TEAMROBOT_H
//#include "messages_ai_data_manager.pb.h"
#include <furgbol-core/proto/messages_ai_data_manager.pb.h>
#include "robot.h"

enum KickType{
    LOW, HIGH
};

struct feedbackRobot{
    feedbackRobot(): capacitor_charge(0), batery_charge(0){ velocity = Mat_<float>(3,1); }
    Mat_<float> velocity;
    int capacitor_charge;
    int batery_charge;
};

class TeamRobot: public Robot
{
private:
    /*kalman data*/
    Mat_<float> observation;
    Mat_<float> deltat_state;
    Mat H;

    /*feedback-manager data*/
    float capacitor_charge;
    float batery_charge;
    float orientation;

    /*ai-manager data*/
    string skill;
    string tactic;
    pair<float, float> target;
    vector< pair<float, float> > path;
    Mat_<float> command;
    KickType kick_type;
    float kick_power;
    float dribbler_velocity;

public:
    TeamRobot(int _id);

    /*Setters*/
    void setVisionData(visionRobot& vision_robot);
    void setFeedbackData(feedbackRobot& feedback_robot);
    void setAIData(AIRobotPackage& ai_robot);

    /*Getters*/
    float getCapacitorCharge();
    float getBateryCharge();
    pair<float, float> getTarget() { return target; }
    vector< pair<float, float> > getPath() { return path; }

    void kalmanFilter();
};

#endif // TEAMROBOT_H
