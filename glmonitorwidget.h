#ifndef GLMONITORWIDGET_H
#define GLMONITORWIDGET_H

#include <QOpenGLWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <QVector>
#include <QtMath>

#include <iostream>

#include "config.h"
#include "ball.h"
#include "teamRobot.h"
#include "enemyRobot.h"

using namespace std;

class GLShape
{
public:
    virtual void compile() = 0;
    virtual void paint(QPainter *p) = 0;
};

class FieldShape : public GLShape
{
public:
    FieldShape();
    void compile();
    void paint(QPainter *p);
private:
    QRect borders;
    QLine center_line_l;
    QLine center_line_w;
    QPoint central;
    int central_radius;
    QRect area_arc_rect;
    QLine area_line;
    vector<QLine> goal;
};

class BallShape : public GLShape
{
public:
    BallShape(Ball *_ball = nullptr);
    void compile();
    void paint(QPainter *p);
private:
    Ball *ball;
    int radius;
};

class RobotShape : GLShape
{
public:
    RobotShape(Robot *_robot = nullptr);
    void compile();
    void paint(QPainter *p);
    void paintId(QPainter *p);
    void paintPathPlanning(QPainter *p);

    int getRadius() { return radius; }
private:
    Robot *robot;
    int radius;
    int chord_dist;
    QRect chord_rect;
};

class GLMonitorWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit GLMonitorWidget(QWidget *parent = nullptr);
    ~GLMonitorWidget();
    void initializeGL();
    void paintEvent(QPaintEvent *e);

    void init(Ball *_ball, vector<TeamRobot *> _team_robots, vector<EnemyRobot *> _enemy_robots);
    void setShowPathPlanning(bool arg1) { show_path_planning = arg1; }
    void setShowId(bool arg1) { show_id = arg1; }

private:
    QTimer timer;
    double ratio;

    bool show_id;
    bool show_path_planning;

    /*Shapes*/
    FieldShape field_shape;
    BallShape ball_shape;
    vector<RobotShape> team_robots_shapes;
    vector<RobotShape> enemy_robots_shapes;

    /*Entities*/
    Ball *ball;
    vector<TeamRobot *> team_robots;
    vector<EnemyRobot *> enemy_robots;
};

#endif // GLMONITORWIDGET_H
