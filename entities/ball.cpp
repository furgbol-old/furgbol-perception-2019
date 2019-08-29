#include "ball.h"

/*Constructor*/
Ball::Ball(): confidence(0.f)
{
    pose = Mat_<float>(3,1);
}

Ball::operator BallPackage() const
{
    BallPackage package;
    package.set_x(pose[0][0]);
    package.set_y(pose[1][0]);
    package.set_z(pose[2][0]);
    return package;
}

/*Setters*/
void Ball::setVisionData(visionBall& vision_ball)
{
    pose = vision_ball.pose.clone();

    //cout<<"x = "<<pose[0][0]<<" y = "<<pose[1][0]<<" z = "<<pose[2][0]<<endl;
    confidence = vision_ball.confidence;
}

/*Getters*/
float Ball::getX()
{
    return pose[0][0];
}

float Ball::getY()
{
    return pose[1][0];
}

Mat_<float> Ball::getPose()
{
    return pose.clone();
}

float Ball::getConfibility()
{
    return confidence;
}

