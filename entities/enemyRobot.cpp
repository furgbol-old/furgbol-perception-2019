#include "enemyRobot.h"

/*Constructor*/
EnemyRobot::EnemyRobot(int _id): Robot(_id)
{
    state = Mat_<float>(3,1,0.f);
}

/*Setters*/
void EnemyRobot::setVisionData(visionRobot& vision_robot)
{
    //cout<<vision_robot.pose[0][0]<<" "<<vision_robot.pose[1][0]<<" "<<vision_robot.pose[2][0]<<endl;
    state = vision_robot.pose.clone();
    confidence = vision_robot.confidence;
}
