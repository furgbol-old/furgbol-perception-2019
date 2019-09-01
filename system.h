#ifndef SYSTEM_H
#define SYSTEM_H

#include <memory>

#include <QObject>

#include "config.h"
#include "definitions.h"
#include "visionManager.h"
#include "serialFeedback.h"
#include "aiManager.h"
#include "feedbackManager.h"
#include "teamRobot.h"
#include "enemyRobot.h"
#include "ball.h"

#include <rxcpp/rx.hpp>
#include <furgbol-core/io/multicast_receiver.h>
#include <ssl-refbox-proto/referee.pb.h>
#include <furgbol-core/proto/messages_data_manager.pb.h>

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

    std::unique_ptr<furgbol::io::MulticastReceiver> vision_receiver_;
    rxcpp::composite_subscription vision_subscription_;
    rxcpp::composite_subscription vision_manager_subscription_;
    void on_vision_data_(const std::string&);

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
