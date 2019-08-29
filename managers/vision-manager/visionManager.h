#ifndef VISIONMANAGER_H
#define VISIONMANAGER_H
#include <messages_robocup_ssl_detection.pb.h>
#include <messages_robocup_ssl_geometry.pb.h>
#include <messages_robocup_ssl_wrapper.pb.h>
#include <messages_vision_manager.pb.h>

#include "definitions.h"
#include "ball.h"
#include "enemyRobot.h"
#include "teamRobot.h"
#include "config.h"

class VisionManager
{
private:
    Ball* ball;
    vector<TeamRobot*> team;
    vector<EnemyRobot*> enemy;

    visionBall ball_v;
    vector<visionRobot> blue_v;
    vector<visionRobot> yellow_v;

    vector<bool> read_cameras;
    long frame_number;
    double delay;

    bool ready;

    bool allFieldRead();
    void resetVisionData();
    void updateEntities();

public:
    VisionManager(Ball* _ball, vector<TeamRobot*> _team, vector<EnemyRobot*> _enemy);

    void mountVisionPackage(VisionPackage& package);

    bool readVisionData(const SSL_DetectionFrame& package);
};

#endif // VISIONMANAGER_H
