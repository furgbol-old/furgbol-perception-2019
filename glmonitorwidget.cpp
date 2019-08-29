#include "glmonitorwidget.h"

FieldShape::FieldShape()
{

}

void FieldShape::compile()
{
    borders = QRect(0, 0, Config::geometry.field_length, Config::geometry.field_width);
    center_line_l = QLine(-Config::geometry.field_length/2, 0, Config::geometry.field_length/2, 0);
    center_line_w = QLine(0, -Config::geometry.field_width/2, 0, Config::geometry.field_width/2);
    central = QPoint(0, 0);
    central_radius = Config::geometry.central_radius;
    area_arc_rect = QRect(-((Config::geometry.field_length/2)+1000), -((Config::geometry.area_line/2)+Config::geometry.area_radius),
                          Config::geometry.area_radius*2, Config::geometry.area_radius*2);
    area_line = QLine(-((Config::geometry.field_length/2)-Config::geometry.area_radius), -(Config::geometry.area_line/2),
                      -((Config::geometry.field_length/2)-Config::geometry.area_radius), (Config::geometry.area_line/2));
    goal = vector<QLine>{QLine(-((Config::geometry.field_length/2)+Config::geometry.goal_depth), -(Config::geometry.goal_width/2),
                               -((Config::geometry.field_length/2)+Config::geometry.goal_depth), (Config::geometry.goal_width/2)),
                        QLine(((Config::geometry.field_length/2)+Config::geometry.goal_depth), -(Config::geometry.goal_width/2),
                              ((Config::geometry.field_length/2)+Config::geometry.goal_depth), (Config::geometry.goal_width/2)),
                        QLine(-((Config::geometry.field_length/2)+Config::geometry.goal_depth), -(Config::geometry.goal_width/2),
                              -(Config::geometry.field_length/2), -(Config::geometry.goal_width/2)),
                        QLine(-((Config::geometry.field_length/2)+Config::geometry.goal_depth), (Config::geometry.goal_width/2),
                              -(Config::geometry.field_length/2), (Config::geometry.goal_width/2)),
                        QLine(((Config::geometry.field_length/2)+1), -(Config::geometry.goal_width/2),
                              ((Config::geometry.field_length/2)+Config::geometry.goal_depth), -(Config::geometry.goal_width/2)),
                        QLine(((Config::geometry.field_length/2)+1), (Config::geometry.goal_width/2),
                              ((Config::geometry.field_length/2)+Config::geometry.goal_depth), (Config::geometry.goal_width/2))};
}

void FieldShape::paint(QPainter *p)
{
    p->save();
    p->translate(-Config::geometry.field_length/2.0, -Config::geometry.field_width/2.0);
    p->drawRect(borders);
    p->restore();
    p->drawLine(center_line_l);
    p->drawLine(center_line_w);
    p->drawEllipse(central, central_radius, central_radius);
    p->drawArc(area_arc_rect, 0, 16 * 90);
    p->drawLine(area_line);
    p->save();
    p->scale(1.0, -1.0);
    p->drawArc(area_arc_rect, 0, 16 * 90);
    p->scale(-1.0, 1.0);
    p->drawArc(area_arc_rect, 0, 16 * 90);
    p->scale(1.0, -1.0);
    p->drawArc(area_arc_rect, 0, 16 * 90);
    p->drawLine(area_line);
    p->restore();
    for (QLine goal_line : goal) p->drawLine(goal_line);
}

BallShape::BallShape(Ball *_ball)
{
    ball = _ball;
}

void BallShape::compile()
{
    radius = Config::geometry.ball_radius;
}

void BallShape::paint(QPainter *p)
{
    p->save();
    if (ball) p->drawEllipse(ball->getX(), -ball->getY(), radius, radius);
    else p->drawEllipse(0, 0, radius, radius);
    p->restore();
}

RobotShape::RobotShape(Robot *_robot)
{
    robot = _robot;
}

void RobotShape::compile()
{
    radius = Config::geometry.robot_radius;
    chord_dist = Config::geometry.robot_chord_dist;
    chord_rect = QRect(-Config::geometry.robot_radius, -Config::geometry.robot_radius, 2*Config::geometry.robot_radius, 2*Config::geometry.robot_radius);
}

void RobotShape::paint(QPainter *p)
{
    p->save();
    if (robot->found()) {
        p->translate(robot->getX(), -robot->getY());
        p->rotate(-qRadiansToDegrees(robot->getTheta()));
        p->drawChord(QRect(-Config::geometry.robot_radius, -Config::geometry.robot_radius, 2*Config::geometry.robot_radius, 2*Config::geometry.robot_radius), 16*qRadiansToDegrees(qAcos((Config::geometry.robot_chord_dist*1.0)/Config::geometry.robot_radius)), 16*(360-qRadiansToDegrees(qAcos(Config::geometry.robot_chord_dist/Config::geometry.robot_radius))));
    }
    p->restore();
}

void RobotShape::paintId(QPainter *p)
{
    p->save();
    if (robot->found()) {
        p->translate(robot->getX() + Config::geometry.robot_radius, -robot->getY() + Config::geometry.robot_radius);
        p->drawText(0, 0, QString::number(robot->getId()));
    }
    p->restore();
}

void RobotShape::paintPathPlanning(QPainter *p)
{
    p->save();
    if (robot->found()) {
        TeamRobot *aux_robot = dynamic_cast<TeamRobot *>(robot);
        QVector<QPointF> points;
        for (pair<float, float> point : aux_robot->getPath()) points.append(QPointF(point.first, -point.second));
        if (!points.isEmpty()) {
            p->drawLine(QPointF(robot->getX(), -robot->getY()), points.first());
            for (int i = 0; i < points.size() - 1; ++i) p->drawLine(points[i], points[i+1]);
        }
        p->translate(aux_robot->getTarget().first, -aux_robot->getTarget().second);
        p->rotate(-qRadiansToDegrees(robot->getTheta()));
        p->drawChord(QRect(-Config::geometry.robot_radius, -Config::geometry.robot_radius, 2*Config::geometry.robot_radius, 2*Config::geometry.robot_radius), 16*qRadiansToDegrees(qAcos((Config::geometry.robot_chord_dist*1.0)/Config::geometry.robot_radius)), 16*(360-qRadiansToDegrees(qAcos(Config::geometry.robot_chord_dist/Config::geometry.robot_radius))));
    }
    p->restore();
}

GLMonitorWidget::GLMonitorWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    show_id = false;
    show_path_planning = false;
}

GLMonitorWidget::~GLMonitorWidget()
{

}

void GLMonitorWidget::initializeGL()
{
    ratio = height() / 7400.0;
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
}

void GLMonitorWidget::paintEvent(QPaintEvent *e)
{
    QPainter p;
    p.begin(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(e->rect(), QColor(35, 135, 45));
    p.translate(width() / 2, height() / 2);
    p.scale(ratio, ratio);

    QPen field_pen(QColor(255, 255, 255));
    field_pen.setWidthF(1/ratio);
    p.setPen(field_pen);
    field_shape.paint(&p);

    QPen ball_pen(QColor(240, 140, 40));
    QBrush ball_brush(QColor(240, 140, 40));
    ball_pen.setWidthF(1/ratio);
    p.setPen(ball_pen);
    p.setBrush(ball_brush);
    ball_shape.paint(&p);

    QBrush team_brush = (Config::control.team_color == ControlPackage::Yellow) ? QBrush(QColor(245, 245, 15)) : QBrush(QColor(15, 95, 245));
    QBrush enemy_brush = (Config::control.team_color == ControlPackage::Yellow) ? QBrush(QColor(15, 95, 245)) : QBrush(QColor(245, 245, 15));
    p.setPen(Qt::NoPen);
    p.setBrush(team_brush);
    for (RobotShape rs : team_robots_shapes) rs.paint(&p);
    p.setBrush(enemy_brush);
    for (RobotShape rs : enemy_robots_shapes) rs.paint(&p);
    if (show_id) {
        QPen id_pen(QColor(255, 255, 255));
        id_pen.setWidth(1/ratio);
        p.setPen(id_pen);
        p.setFont(QFont(p.font().family(), 100));
        for (RobotShape rs : team_robots_shapes) rs.paintId(&p);
        for (RobotShape rs : enemy_robots_shapes) rs.paintId(&p);
    }
    if (show_path_planning) {
        QPen path_planning_pen(QColor(255, 0, 0));
        path_planning_pen.setWidth(1/ratio);
        p.setPen(path_planning_pen);
        p.setBrush(Qt::NoBrush);
        for (RobotShape rs : team_robots_shapes) rs.paintPathPlanning(&p);
    }

    p.end();
}

void GLMonitorWidget::init(Ball *_ball, vector<TeamRobot *> _team_robots, vector<EnemyRobot *> _enemy_robots)
{
    ball = _ball;
    team_robots = _team_robots;
    enemy_robots = _enemy_robots;

    ball_shape = BallShape(_ball);
    for (TeamRobot *tr : team_robots) team_robots_shapes.push_back(RobotShape(tr));
    for (EnemyRobot *er : enemy_robots) enemy_robots_shapes.push_back(RobotShape(er));

    field_shape.compile();
    ball_shape.compile();
    for (RobotShape rs : team_robots_shapes) rs.compile();
    for (RobotShape rs : enemy_robots_shapes) rs.compile();

    timer.start(40);
}
