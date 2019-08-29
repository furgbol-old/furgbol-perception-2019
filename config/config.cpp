#include "config.h"

NetworkConfig Config::network;
GeometryConfig Config::geometry;
ControlConfig Config::control;

NetworkConfig::NetworkConfig()
{
}

void NetworkConfig::load(const json file)
{
    vision_ip = file["vision"]["ip"].get<string>();
    vision_port = file["vision"]["port"].get<int>();

    referee_ip = file["referee"]["ip"].get<string>();
    referee_port = file["referee"]["port"].get<int>();

    ai_ip = file["ai"]["ip"].get<string>();
    ai_send_port = file["ai"]["send_port"].get<int>();
    ai_receive_port = file["ai"]["receive_port"].get<int>();

    multi_vision = QHostAddress(QString::fromStdString(vision_ip));
    multi_referee = QHostAddress(QString::fromStdString(referee_ip));
    ai = QHostAddress(QString::fromStdString(ai_ip));
}

void NetworkConfig::save()
{
    ifstream json_in(CONFIG_PATH);
    json json_config;
    json_in >> json_config;
    json_in.close();

    json_config["network"]["vision"]["ip"] = vision_ip;
    json_config["network"]["vision"]["port"] = vision_port;
    json_config["network"]["referee"]["ip"] = referee_ip;
    json_config["network"]["referee"]["port"] = referee_port;
    json_config["network"]["ai"]["ip"] = ai_ip;
    json_config["network"]["ai"]["send_port"] = ai_send_port;
    json_config["network"]["ai"]["receive_port"] = ai_receive_port;

    ofstream json_out(CONFIG_PATH);
    json_out << setw(4) << json_config << endl;
    json_out.close();
}

ostream& operator <<(ostream& out, NetworkConfig network){
}

GeometryConfig::GeometryConfig()
{
}

void GeometryConfig::load(const json file)
{
    field_length = file["field"]["length"].get<int>();
    field_width = file["field"]["width"].get<int>();
    central_radius = file["central_radius"].get<int>();
    goal_width = file["goal"]["width"].get<int>();
    goal_depth = file["goal"]["depth"].get<int>();
    area_line = file["area"]["line"].get<int>();
    area_radius = file["area"]["radius"].get<int>();
    ball_radius = file["ball"]["radius"].get<int>();
    robot_radius = file["robot"]["radius"].get<int>();
    robot_chord_dist = file["robot"]["chord_dist"].get<int>();
}

void GeometryConfig::save()
{
    ifstream json_in(CONFIG_PATH);
    json json_config;
    json_in >> json_config;
    json_in.close();

    json_config["geometry"]["field"]["length"] = field_length;
    json_config["geometry"]["field"]["width"] = field_width;
    json_config["geometry"]["central_radius"] = central_radius;
    json_config["geometry"]["goal"]["width"] = goal_width;
    json_config["geometry"]["goal"]["depth"] = goal_depth;
    json_config["geometry"]["area"]["line"] = area_line;
    json_config["geometry"]["area"]["radius"] = area_radius;
    json_config["geometry"]["ball"]["radius"] = ball_radius;
    json_config["geometry"]["robot"]["radius"] = robot_radius;
    json_config["geometry"]["robot"]["chord_dist"] = robot_chord_dist;

    ofstream json_out(CONFIG_PATH);
    json_out << setw(4) << json_config << endl;
    json_out.close();
}

ostream& operator <<(ostream& out, GeometryConfig network){
}

ControlConfig::ControlConfig(): field_side(ControlPackage::Left), team_color(ControlPackage::Yellow), id_gk(0),
    yellow_name("Yellow Team"), blue_name("Blue Team"), yellow_score(0), blue_score(0)
{

}

Config::Config()
{

}

void Config::load()
{
    cout << "CARREGANDO CONFIGURAÇÕES..." << endl;
    fstream json_file(CONFIG_PATH);
    json json_config;
    json_file >> json_config;

    cout << "NETWORK...";
    network.load(json_config["network"]);
    cout << "DONE!" << endl;

    cout << "GEOMETRY...";
    geometry.load(json_config["geometry"]);
    cout << "DONE!" << endl;

    cout << "CONTROL...";
    cout << control.field_side << " " << control.team_color << " " << control.id_gk << " ";
    cout << "DONE!" << endl;

    cout << "CONFIGURAÇÕES CARREGADAS." << endl;
}
