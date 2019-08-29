#ifndef SERIALFEEDBACK_H
#define SERIALFEEDBACK_H

#include "definitions.h"
#include "teamRobot.h"

class SerialFeedback{

private:
    vector<unsigned char> current_data;
    vector<TeamRobot*> robots;

    bool quitFlag;
    bool connectionFlag;

    std::thread reader;
    boost::asio::io_service io_svc;
    boost::asio::serial_port serial;


    Mat_<float> M;
    void kinematics();

    void readData();
    void parseData(vector<unsigned char> received_data);
    bool isValid(int id, int v1, int v2, int v3, int v4, int bc, int cc);
public:
    SerialFeedback(vector<TeamRobot*> robots);
    ~SerialFeedback();

    bool quit();
    bool connected();

    bool connect();
    bool disconnect();

    vector<unsigned char> getCurrent_data();
    void setRobots(vector<TeamRobot*>);
};

#endif // SERIALFEEDBACK_H
