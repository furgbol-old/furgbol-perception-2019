#include "system.h"
System::System()
{
    ball = new Ball();

    for(int i = 0 ; i < NUM_MAX_ROBOTS ; i++){
        team.push_back(new TeamRobot(i));
        enemy.push_back(new EnemyRobot(i));
    }

    ai_manager = new AIManager(team);
    serial = new SerialFeedback(team);
    vision_manager = new VisionManager(ball, team, enemy);

    ai_send = new QUdpSocket(this);
    ai_receive = new QUdpSocket(this);
    //vision = new QUdpSocket(this);
    referee = new QUdpSocket(this);

    resetRefereeData();

    Config::load();

    cout<<"\nDECLARANDO SLOTS E SINAIS..."<<endl;
    cout<<"IP: "<<Config::network.ai_ip<<"\tPORTA: "<<Config::network.ai_receive_port<<endl;
    connect(ai_receive, SIGNAL(readyRead()), this, SLOT(readAIData()));
//    cout<<"IP: "<<Config::network.vision_ip<<"\tPORTA: "<<Config::network.vision_port<<endl;
//    connect(vision, SIGNAL(readyRead()), this, SLOT(readVisionData()));
    cout<<"IP: "<<Config::network.referee_ip<<"\tPORTA: "<<Config::network.referee_port<<endl;
    connect(referee, SIGNAL(readyRead()), this, SLOT(readRefereeData()));

    vision_receiver_ = std::make_unique<furgbol::io::MulticastReceiver>("0.0.0.0", Config::network.vision_ip, Config::network.vision_port);
}
System::~System()
{
    stop();

    delete ai_manager;
    delete serial;
    delete ai_manager;
}

void System::start()
{
    bool ans;

    cout<<"\nREALIZANDO CONEXÕES..."<<endl;

    ans = ai_receive->bind(Config::network.ai_receive_port, QUdpSocket::ShareAddress);
    if(ans) {
        cout<<"Pronto para receber dados da AI."<<endl;
    }
    else cout<<"Nao foi possivel conectar na AI (receiver)."<<endl;

    ans = referee->bind(QHostAddress::AnyIPv4, Config::network.referee_port, QUdpSocket::ShareAddress);
    if(ans) {
        referee->joinMulticastGroup(Config::network.multi_referee);
        cout<<"Pronto para receber dados do Referee."<<endl;
    }
    else cout<<"Nao foi possivel conectar no Referee."<<endl;

//    ans = vision->bind(QHostAddress::AnyIPv4, Config::network.vision_port, QUdpSocket::ShareAddress);
//    if(ans) {
//        vision->joinMulticastGroup(Config::network.multi_vision);
//        cout<<"Pronto para receber dados da SSL Vision."<<endl;
//    }
//    else cout<<"Nao foi possivel conectar na SSL Vision."<<endl;

    serial->connect();
    ans = serial->connected();
    if(ans){
        cout<<"Pronto para receber dados por serial"<<endl;
    }
    else{
        cout<<"Nao foi possivel iniciar a comunicação serial"<<endl;
    }
    Clock::setStartTime();
    vision_subscription_ = vision_receiver_->datagram()
        .subscribe_on(rxcpp::observe_on_new_thread())
        .subscribe([this](std::string datagram) {
            on_vision_data_(std::move(datagram)); });
    vision_manager_subscription_ = vision_manager->read_all_cameras.get_observable()
            .subscribe_on(rxcpp::observe_on_new_thread())
            .subscribe([this](long) {
                sendAIData(); });
}
void System::stop()
{
    cout<<"CONEXÕES FINALIZADAS."<<endl;
    if(ai_receive->BoundState) ai_receive->close();
    if(referee->BoundState) {
        referee->leaveMulticastGroup(Config::network.multi_referee);
        referee->close();
    }
//    if(vision->BoundState) {
//        vision->leaveMulticastGroup(Config::network.multi_vision);
//        vision->close();
//    }
    if(serial->connected()){
        serial->disconnect();
    }

    vision_subscription_.unsubscribe();
}

void System::on_vision_data_(const std::string& datagram) {
    SSL_WrapperPacket wrapper;
    if (wrapper.ParseFromString(datagram)) {
        if (wrapper.has_detection()) {
            SSL_DetectionFrame detection;
            detection.CopyFrom(wrapper.detection());
            vision_manager->readVisionData(detection);
        }
    } else {
        cout << "error parsing vision datagram" << endl;
    }
}

void System::sendAIData()
{
    QByteArray data;
    DataPackage package;
    VisionPackage vision_data;
    int ans;

    vision_manager->mountVisionPackage(vision_data);
    package.mutable_vision()->CopyFrom(vision_data);
    RobotPackage robot;
    cout<<"---team---"<<endl;
    for(int i = 0 ; i < vision_data.team_size() ; i++){
        robot = vision_data.team(i);
        cout<<"ID: "<<robot.id()<<endl;
        cout<<"x: "<<robot.x()<<" y: "<<robot.y()<<" o: "<<robot.orientation()<<endl;
    }
    cout<<"---enemy---"<<endl;
    for(int i = 0 ; i < vision_data.enemy_size() ; i++){
        robot = vision_data.enemy(i);
        cout<<"ID: "<<robot.id()<<endl;
        cout<<"x: "<<robot.x()<<" y: "<<robot.y()<<" o: "<<robot.orientation()<<endl;;
    }
    package.mutable_referee()->CopyFrom(referee_data);
    ControlPackage* control_data = package.mutable_control();
    control_data->set_field_side(Config::control.field_side);
    control_data->set_team_color(Config::control.team_color);
    control_data->set_id_gk(Config::control.id_gk);

    data.resize(package.ByteSize());
    package.SerializeToArray(data.data(), data.size());
    ans = ai_send->writeDatagram(data, Config::network.ai, Config::network.ai_send_port);
    if(ans != -1) cout<<"Dado com "<<ans<<" bytes foi enviado com sucesso."<<endl;
    else cout<<"Não foi possível enviar o dado."<<endl;
}

void System::readAIData()
{
    if(ai_receive->hasPendingDatagrams()){
        cout<<"Dados da AI."<<endl;
        QByteArray data;
        AIDataManagerPackage package;

        data.resize(ai_receive->pendingDatagramSize());
        ai_receive->readDatagram(data.data(), data.size());
        package.ParseFromArray(data.data(), data.size());

        ai_manager->readAIData(package);
        cout<<"Leitura finalizada."<<endl;
    }
}
void System::readVisionData()
{
    if(vision->hasPendingDatagrams()) {
        cout<<"Dados da Visão."<<endl;
        QByteArray data;
        SSL_WrapperPacket package;

        data.resize(vision->pendingDatagramSize());
        vision->readDatagram(data.data(), data.size());
        package.ParseFromArray(data.data(), data.size());

        if(package.has_detection()) {
            SSL_DetectionFrame frame_package;
            frame_package.CopyFrom(package.detection());

            if(vision_manager->readVisionData(frame_package)) {
                sendAIData();
            }
        }
        cout<<"Leitura finalizada."<<endl;
       }
}
void System::readRefereeData()
{
    if(referee->hasPendingDatagrams()) {
        cout<<"Lendo dados do Referee."<<endl;
        QByteArray data;
        data.resize(referee->pendingDatagramSize());
        referee->readDatagram(data.data(), data.size());
        referee_data.ParseFromArray(data.data(), data.size());
        cout << referee_data.command() << endl;
        cout<<"Leitura finalizada."<<endl;
        int yellow_score = referee_data.yellow().score();
        int blue_score = referee_data.blue().score();
        if (yellow_score != Config::control.yellow_score || blue_score != Config::control.blue_score) {
            Config::control.yellow_score = yellow_score;
            Config::control.blue_score = blue_score;
            emit scoreChanged(yellow_score, blue_score);
        }
    }
}

void System::resetRefereeData(){
    referee_data.set_command(SSL_Referee::HALT);
    referee_data.set_packet_timestamp(1);
    referee_data.set_stage(SSL_Referee::NORMAL_FIRST_HALF_PRE);
    referee_data.set_command_counter(0);
    referee_data.set_command_timestamp(1);

    SSL_Referee_TeamInfo* yellow_team = referee_data.mutable_yellow();
    yellow_team->set_name("");
    yellow_team->set_score(0);
    yellow_team->set_red_cards(0);
    yellow_team->set_yellow_cards(0);
    yellow_team->set_timeouts(4);
    yellow_team->set_timeout_time(300000000);
    yellow_team->set_goalie(0);

    SSL_Referee_TeamInfo* blue_team = referee_data.mutable_blue();
    blue_team->set_name("");
    blue_team->set_score(0);
    blue_team->set_red_cards(0);
    blue_team->set_yellow_cards(0);
    blue_team->set_timeouts(4);
    blue_team->set_timeout_time(300000000);
    blue_team->set_goalie(0);
}

void readFeedbackData()
{

}
