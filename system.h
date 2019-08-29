#ifndef SYSTEM_H
#define SYSTEM_H

#include <QObject>

#include "config.h"
#include "definitions.h"
#include "visionManager.h"
#include "serialFeedback.h"
#include "aiManager.h"
#include "feedbackManager.h"
//#include "messages_data_manager.pb.h"
//#include "messages_robocup_ssl_referee.pb.h"
#include <ssl-refbox-proto/referee.pb.h>
#include <furgbol-core/proto/messages_data_manager.pb.h>
#include "teamRobot.h"
#include "enemyRobot.h"
#include "ball.h"

class System: public QObject
{
    Q_OBJECT
private:
    QUdpSocket *ai_send;
    QUdpSocket *ai_receive;
    QUdpSocket *vision;
    QUdpSocket *referee;

    Ball *ball;
    vector<TeamRobot*> team;
    vector<EnemyRobot*> enemy;

    AIManager *ai_manager;
    SerialFeedback *serial;

    VisionManager *vision_manager;

    SSL_Referee referee_data;

private slots:
    void readAIData();
    void readVisionData();
    void readRefereeData();

signals:
    void scoreChanged(int yellow_score, int blue_score);

public:
    System();
    ~System();

    Ball *getBall() { return ball; }
    vector<TeamRobot *> getTeamRobots() { return team; }
    vector<EnemyRobot *> getEnemyRobots() { return enemy; }

    void sendAIData();
    void resetRefereeData();

    void start();
    void stop();
};

#endif // SYSTEM_H
