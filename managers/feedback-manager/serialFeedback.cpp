#include "serialFeedback.h"
using namespace boost;

SerialFeedback::SerialFeedback(vector<TeamRobot*> robots): connectionFlag(false), io_svc(), serial(io_svc){
    this->robots = robots;
}

SerialFeedback::~SerialFeedback(){
    this->disconnect();
}

bool SerialFeedback::connect(){
    try{
        serial.open(SERIAL_PORT);
        serial.set_option(asio::serial_port_base::baud_rate(SERIAL_BAUD_RATE));
        serial.set_option(asio::serial_port_base::character_size(8));
    } catch(system::system_error e){
        cout << e.what() << endl;
    }

    if(serial.is_open()){
        connectionFlag = true;
        reader = std::thread(&SerialFeedback::readData, this);
        fill(current_data.begin(), current_data.end(), 0);
    }else{
        cout << "Erro na conexão serial" << endl;
    }
}

bool SerialFeedback::disconnect(){
    connectionFlag = false;
    serial.close();
    reader.join();
}

bool SerialFeedback::connected(){
    return connectionFlag;
}

void SerialFeedback::readData(){
    unsigned char received;
    unsigned char data[SERIAL_BUFFER_SIZE];
    system::error_code ec;
    while(connected()){
        serial.read_some(asio::buffer(&received, 1), ec);
        if(received >= 128){
            data[0] = received - 128;
            serial.read_some(asio::buffer(data+1, SERIAL_BUFFER_SIZE-1), ec);
            current_data.assign(data, data + SERIAL_BUFFER_SIZE);
            parseData(current_data);
        }

    }
}

void SerialFeedback::parseData(vector<unsigned char> received_data){
    int id, v1, v2, v3, v4, bc, cc;

    id = received_data[0];
    v1 = received_data[1];
    v2 = received_data[2];
    v3 = received_data[3];
    v4 = received_data[4];

    bc = received_data[6];
    cc = received_data[7];

    if(received_data[5] & 0x1){
        v1 *= -1;
    }
    if(received_data[5] & 0x2){
        v2 *= -1;
    }
    if(received_data[5] & 0x4){
        v3 *= -1;
    }
    if(received_data[5] & 0x8){
        v4 *= -1;
    }

    if(!isValid(id, v1, v2, v3, v4, bc, cc))
        return;

    kinematics();
    vector<float> velocities_vector = {v1, v2, v3, v4};
    Mat wheels_velocity = (Mat_<float>(4, 1) << velocities_vector[0], velocities_vector[1], velocities_vector[2], velocities_vector[3]);
    Mat_<float> velocity = Mat_<float>(3,1);

    velocity = M * wheels_velocity;

    feedbackRobot parsed_robot;
    parsed_robot.velocity = velocity;
    parsed_robot.batery_charge = bc;
    parsed_robot.capacitor_charge = cc;

    robots[0]->setFeedbackData(parsed_robot);
    printf("%d %d %d %d %d %d %d\n", id, v1, v2, v3, v4, bc, cc);
    fflush(stdout);
}

bool SerialFeedback::isValid(int id, int v1, int v2, int v3, int v4, int bc, int cc){
    if(id < 0 || id > 11)
        return false;
    if(v1 < -127 || v1 > 127)
        return false;
    if(v2 < -127 || v2 > 127)
        return false;
    if(v3 < -127 || v3 > 127)
        return false;
    if(v4 < -127 || v4 > 127)
        return false;
    if(bc < 0 || bc > 100)
        return false;
    if(cc < 0 || cc > 100)
        return false;
    return true;
}

bool SerialFeedback::quit(){
    return quitFlag;
}

vector<unsigned char> SerialFeedback::getCurrent_data(){
    return current_data;
}

void SerialFeedback::kinematics(){
   //! Estas medidas estão em metros.

   /// O raio da roda é utilizado para determinar as velocidades das rodas e o raio do robo
   /// para se obter a velocidade angular do robo
   float raioDaRoda = 0.0275;
   float raioRobo = 0.080;

   //--------MODELO CINEMÁTICO DO ROBO-----------
   float alpha1, alpha2;

   //angulos de ataque das rodas
   alpha1 = 45.0*M_PI/180.0;   //45°
   alpha2 = 37.0*M_PI/180.0;   //37°

   //inicializa a matriz M
   /// Aqui é quando inicializamos a matriz M especificado utilizando os valores do robo
   M = cv::Mat_<float>(3,4);
   M[0][0] = -cos(alpha1);     M[0][1] = -cos(alpha2);     M[0][2] =  cos(alpha2);      M[0][3] = cos(alpha1);
   M[1][0] =  sin(alpha1);     M[1][1] = -sin(alpha2);     M[1][2] = -sin(alpha2);      M[1][3] = sin(alpha1);
   M[2][0] = -1/raioRobo;      M[2][1] = -1/raioRobo;      M[2][2] =  -1/raioRobo;      M[2][3] = -1/raioRobo;

   M = raioDaRoda * M;     //isto é feito para calcular a velocidade angular das rodas posteriormente, e não a linear

}
