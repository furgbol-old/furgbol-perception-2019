#ifndef BALL_H
#define BALL_H

#include "definitions.h"
#include "messages_vision_manager.pb.h"

struct visionBall{
    visionBall(): confidence(0.f), found(false){ pose = Mat_<float>(3,1); }
    Mat_<float> pose;
    float confidence;
    bool found;
};

class Ball
{
private:
    /*Final data*/
    Mat_<float> pose;
    float confidence;

public:
    Ball();

    operator BallPackage() const;

    /*Setters*/
    void setVisionData(visionBall& vision_ball);

    /*Getters*/
    float getX();
    float getY();
    Mat_<float> getPose();
    float getConfibility();
};
#endif // BALL_H
