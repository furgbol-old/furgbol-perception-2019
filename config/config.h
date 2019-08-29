#ifndef CONFIG_H
#define CONFIG_H

#include "single_include/nlohmann/json.hpp"
#include "definitions.h"
//#include "messages_control.pb.h"
#include <furgbol-core/proto/messages_control.pb.h>

#include <fstream>
#include <string>
#include <iomanip>

using namespace std;
using json = nlohmann::json;

class Config;

class NetworkConfig {
public:
    NetworkConfig();
    void load(const json file);
    void save();

    string vision_ip;
    int vision_port;
    QHostAddress multi_vision;

    string referee_ip;
    int referee_port;
    QHostAddress multi_referee;

    string ai_ip;
    int ai_send_port;
    int ai_receive_port;
    QHostAddress ai;
};

class GeometryConfig {
public:
    GeometryConfig();
    void load(const json file);
    void save();

    friend ostream& operator <<(ostream& out, GeometryConfig network);

    int field_length;
    int field_width;
    int central_radius;
    int goal_width;
    int goal_depth;
    int area_line;
    int area_radius;
    int ball_radius;
    int robot_radius;
    int robot_chord_dist;
};

class ControlConfig {
public:
    ControlConfig();
    ControlPackage::FieldSide field_side;
    ControlPackage::TeamColor team_color;
    int id_gk;
    string yellow_name;
    string blue_name;
    int yellow_score;
    int blue_score;
};

class Config {
public:
    Config();
    static void load();

    static NetworkConfig network;
    static GeometryConfig geometry;
    static ControlConfig control;
};

#endif // CONFIG_H
