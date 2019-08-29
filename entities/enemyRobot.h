#ifndef ENEMYROBOT_H
#define ENEMYROBOT_H

#include "robot.h"

class EnemyRobot : public Robot
{
public:
    EnemyRobot(int _id);

    /*Setters*/
    void setVisionData(visionRobot& vision_robot);

};

#endif // ENEMYROBOT_H
