#include "visionManager.h"

/*Constructor*/
VisionManager::VisionManager(Ball* _ball, vector<TeamRobot*> _team, vector<EnemyRobot*> _enemy): ball(_ball), team(_team), enemy(_enemy)
{
    blue_v = vector<visionRobot>(NUM_MAX_ROBOTS);
    yellow_v = vector<visionRobot>(NUM_MAX_ROBOTS);
    resetVisionData();
}

bool VisionManager::allFieldRead()
{
    vector<bool>::iterator it;
    for(it = read_cameras.begin();it!=read_cameras.end();it++){
        if(*it==false){
            return false;
        }
    }
    return true;
}

bool VisionManager::readVisionData(const SSL_DetectionFrame& package)
{
    /*Camera*/
    int cam_id = package.camera_id();
    if(cam_id<NUM_CAMERAS){
        read_cameras[cam_id] = true;
    }
    else return false;

    cout<<"Lendo a camera: "<<cam_id<<endl;

    /*Ball*/
    SSL_DetectionBall aux_ball;
    int num_balls;
    num_balls = package.balls_size();
    int i;
    for(i = 0 ; i<num_balls ; i++){
        aux_ball = package.balls(i);
        if(aux_ball.confidence()>ball_v.confidence){
            ball_v.pose[0][0] = aux_ball.x();
            ball_v.pose[1][0] = aux_ball.y();
            ball_v.pose[2][0] = aux_ball.z();
            ball_v.confidence = aux_ball.confidence();
            ball_v.found = true;
        }
    }

    /*Robots*/
    SSL_DetectionRobot aux_robot;
    int num_robots;
    int robot_id;
    num_robots = package.robots_blue_size();
    for(i = 0 ; i<num_robots ; i++){
        aux_robot = package.robots_blue(i);
        robot_id = aux_robot.robot_id();
        if(aux_robot.confidence()>blue_v[robot_id].confidence){
            blue_v[robot_id].pose[0][0] = aux_robot.x();
            blue_v[robot_id].pose[1][0] = aux_robot.y();
            blue_v[robot_id].pose[2][0] = aux_robot.orientation();
            blue_v[robot_id].time = Clock::calculateDeltaT();
            blue_v[robot_id].confidence = aux_robot.confidence();
            blue_v[robot_id].found = true;
        }
    }

    num_robots = package.robots_yellow_size();
    for(i = 0 ; i<num_robots ; i++){
        aux_robot = package.robots_yellow(i);
        robot_id = aux_robot.robot_id();
        if(aux_robot.confidence()>yellow_v[robot_id].confidence){
            yellow_v[robot_id].pose[0][0] = aux_robot.x();
            yellow_v[robot_id].pose[1][0] = aux_robot.y();
            yellow_v[robot_id].pose[2][0] = aux_robot.orientation();
            //cout<<aux_robot.orientation()<<endl;
            yellow_v[robot_id].time = Clock::calculateDeltaT();
            yellow_v[robot_id].confidence = aux_robot.confidence();
            yellow_v[robot_id].found = true;
        }
    }

    if(allFieldRead()){
        frame_number = package.frame_number();
        delay = package.t_sent() - package.t_capture();
        updateEntities();
        resetVisionData();
        read_all_cameras.get_subscriber().on_next(frame_number);
        return true;
    }
    else return false;
}

void VisionManager::resetVisionData()
{
    read_cameras = vector<bool>(NUM_CAMERAS, false);

    for(int i = 0 ; i < NUM_MAX_ROBOTS ; i++){
        blue_v[i] = visionRobot();
        yellow_v[i] = visionRobot();
    }

    ball_v = visionBall();
}

void VisionManager::updateEntities()
{
    /*Ball*/
    if(ball_v.found){
        //cout<<"-ball-"<<endl;
        ball->setVisionData(ball_v);
    }
    /*Robots*/
    //TEAM_COLOR going to be picked up from the interface
    if(Config::control.team_color == ControlPackage::Blue){
        int i;
        for(i = 0 ; i<NUM_MAX_ROBOTS ; i++){
            if(blue_v[i].found){
                //cout<<"-team-"<<endl;
                //cout<<"robot: "<<i<<endl;
                team[i]->setVisionData(blue_v[i]);
                team[i]->kalmanFilter();
                team[i]->frameInside();
            }
            else{
                team[i]->frameOutside();
            }
        }

        for(i = 0 ; i<NUM_MAX_ROBOTS ; i++){
            if(yellow_v[i].found){
                //cout<<"-enemy-"<<endl;
                //cout<<"robot: "<<i<<endl;
                enemy[i]->setVisionData(yellow_v[i]);
                enemy[i]->frameInside();
            }
            else{
                enemy[i]->frameOutside();
            }
        }
    }
    else{
        int i;
        for(i = 0 ; i<NUM_MAX_ROBOTS ; i++){
            if(yellow_v[i].found){
                //cout<<"-team-"<<endl;
                //cout<<"robot: "<<i<<endl;
                team[i]->setVisionData(yellow_v[i]);
                team[i]->kalmanFilter();
                team[i]->frameInside();
            }
            else{
                team[i]->frameOutside();
            }
        }

        for(i = 0 ; i<NUM_MAX_ROBOTS ; i++){
            if(blue_v[i].found){
                //cout<<"-enemy-"<<endl;
                //cout<<"robot: "<<i<<endl;
                enemy[i]->setVisionData(blue_v[i]);
                enemy[i]->frameInside();
            }
            else{
                enemy[i]->frameOutside();
            }
        }

    }
}

void VisionManager::mountVisionPackage(VisionPackage& package)
{
    package.set_frame_number(frame_number);
    package.set_delay(delay);

    //package.ball() = *(ball);
    *(package.mutable_ball()) = *(ball);

    for(int i = 0 ; i < NUM_MAX_ROBOTS ; i++){
        if(team[i]->found())
            *(package.add_team()) = *(team[i]);;

        if(enemy[i]->found())
            *(package.add_enemy()) = *(enemy[i]);
    }
}
