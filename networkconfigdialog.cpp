#include "networkconfigdialog.h"
#include "ui_networkconfigdialog.h"

NetworkConfigDialog::NetworkConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetworkConfigDialog)
{
    ui->setupUi(this);
    setFixedSize(width(), height());

    ui->visionIpInput->setText(QString::fromStdString(Config::network.vision_ip));
    ui->visionPortInput->setText(QString::number(Config::network.vision_port));
    ui->refereeIpInput->setText(QString::fromStdString(Config::network.referee_ip));
    ui->refereePortInput->setText(QString::number(Config::network.referee_port));
    ui->aiIpInput->setText(QString::fromStdString(Config::network.ai_ip));
    ui->aiSPortInput->setText(QString::number(Config::network.ai_send_port));
    ui->aiRPortInput->setText(QString::number(Config::network.ai_receive_port));
}

NetworkConfigDialog::~NetworkConfigDialog()
{
    delete ui;
}

void NetworkConfigDialog::on_defaultsButton_clicked()
{
    ui->visionIpInput->setText(QString::fromStdString(VISION_IP));
    ui->visionPortInput->setText(QString::number(VISION_PORT));
    ui->refereeIpInput->setText(QString::fromStdString(REFEREE_IP));
    ui->refereePortInput->setText(QString::number(REFEREE_PORT));
    ui->aiIpInput->setText(QString::fromStdString(AI_IP));
    ui->aiSPortInput->setText(QString::number(AI_SEND_PORT));
    ui->aiRPortInput->setText(QString::number(AI_RECEIVE_PORT));
}

void NetworkConfigDialog::on_NetworkConfigDialog_accepted()
{
    Config::network.vision_ip = ui->visionIpInput->text().toStdString();
    Config::network.vision_port = ui->visionPortInput->text().toInt();
    Config::network.referee_ip = ui->refereeIpInput->text().toStdString();
    Config::network.referee_port = ui->refereePortInput->text().toInt();
    Config::network.ai_ip = ui->aiIpInput->text().toStdString();
    Config::network.ai_send_port = ui->aiSPortInput->text().toInt();
    Config::network.ai_receive_port = ui->aiRPortInput->text().toInt();
    Config::network.save();
}
