#include "geometryconfigdialog.h"
#include "ui_geometryconfigdialog.h"

GeometryConfigDialog::GeometryConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeometryConfigDialog)
{
    ui->setupUi(this);
    setFixedSize(width(), height());

    ui->fieldLengthInput->setText(QString::number(Config::geometry.field_length));
    ui->fieldWidthInput->setText(QString::number(Config::geometry.field_width));
    ui->centralRadiusInput->setText(QString::number(Config::geometry.central_radius));
    ui->areaLineInput->setText(QString::number(Config::geometry.area_line));
    ui->areaRadiusInput->setText(QString::number(Config::geometry.area_radius));
    ui->goalWidthInput->setText(QString::number(Config::geometry.goal_width));
    ui->goalDepthInput->setText(QString::number(Config::geometry.goal_depth));
    ui->robotRadiusInput->setText(QString::number(Config::geometry.robot_radius));
    ui->chordDistInput->setText(QString::number(Config::geometry.robot_chord_dist));
    ui->ballRadiusInput->setText(QString::number(Config::geometry.ball_radius));
}

GeometryConfigDialog::~GeometryConfigDialog()
{
    delete ui;
}

void GeometryConfigDialog::on_GeometryConfigDialog_accepted()
{
    Config::geometry.field_length = ui->fieldLengthInput->text().toInt();
    Config::geometry.field_width = ui->fieldWidthInput->text().toInt();
    Config::geometry.central_radius = ui->centralRadiusInput->text().toInt();
    Config::geometry.area_line = ui->areaLineInput->text().toInt();
    Config::geometry.area_radius = ui->areaRadiusInput->text().toInt();
    Config::geometry.goal_width = ui->goalWidthInput->text().toInt();
    Config::geometry.goal_depth = ui->goalDepthInput->text().toInt();
    Config::geometry.robot_radius = ui->robotRadiusInput->text().toInt();
    Config::geometry.robot_chord_dist = ui->chordDistInput->text().toInt();
    Config::geometry.ball_radius = ui->ballRadiusInput->text().toInt();
    Config::geometry.save();
}
