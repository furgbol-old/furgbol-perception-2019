#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "clock.h"

//QT includes
#include <QUdpSocket>

//OpenCV includes
#include <opencv/cv.h>
#include <opencv2/video/tracking.hpp>
using namespace cv;

//STL includes
#include <iostream>
#include <vector>
#include <string>
#include <thread>
using namespace std;

//Boost includes
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>

//OpenGL includes

//ProtoLIB includes
#include "messages_data_manager.pb.h"

//System defaults
const string CONFIG_PATH = "../data-manager-v2/config.json";
const string DEFAULTS_PATH = "../data-manager-v2/defaults.json";

//NetworkConfig defaults
const string AI_IP = "127.0.0.1";
const int AI_SEND_PORT = 8765;
const int AI_RECEIVE_PORT = 5678;

const string VISION_IP = "224.5.23.2";
const int VISION_PORT = 10006;

const string REFEREE_IP = "224.5.23.1";
const int REFEREE_PORT = 1883;

//Serial defaults
const int SERIAL_BAUD_RATE = 115200;
const int SERIAL_BUFFER_SIZE = 8;
const string SERIAL_PORT = "/dev/ttyUSB0";

//GeometryConfig defaults
enum FieldSize {
    DoubleSize, SingleSize
};

//Vision deafults
const int FRAMES_FOUNDED = 5;
const int FRAMES_NOT_FOUNDED = 50;

const int NUM_CAMERAS = 4;

const int NUM_MAX_ROBOTS = 12;

#endif // DEFINITIONS_H
