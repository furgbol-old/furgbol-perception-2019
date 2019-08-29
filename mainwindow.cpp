#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    configActionGroup = new QActionGroup(this);
    configActionGroup->addAction(ui->actionNetwork);
    configActionGroup->addAction(ui->actionGeometry);
    configActionGroup->addAction(ui->actionControl);
    system = new System();
    ui->monitorWidget->init(system->getBall(), system->getTeamRobots(), system->getEnemyRobots());
    updateControlData();
    connect(system, SIGNAL(scoreChanged(int, int)), this, SLOT(updateScore(int, int)));
}

MainWindow::~MainWindow()
{
    delete system;
    delete ui;
}

void MainWindow::on_actionStart_toggled(bool arg1)
{
    if (arg1)
        system->start();
    else
        system->stop();
    configActionGroup->setEnabled(!arg1);
}

void MainWindow::on_actionNetwork_triggered()
{
    NetworkConfigDialog netConfigDialog(this);
    netConfigDialog.setWindowModality(Qt::WindowModal);
    netConfigDialog.exec();
}

void MainWindow::on_actionControl_triggered()
{
    ControlConfigDialog controlConfigDialog(this);
    controlConfigDialog.setWindowModality(Qt::WindowModal);
    if (controlConfigDialog.exec() == QDialog::Accepted) {
        updateControlData();
        return;
    }
}

void MainWindow::on_actionShow_ID_toggled(bool arg1)
{
    ui->monitorWidget->setShowId(arg1);
}

void MainWindow::on_actionPathPlanning_toggled(bool arg1)
{
    ui->monitorWidget->setShowPathPlanning(arg1);
}

void MainWindow::updateScore(int yellow_score, int blue_score)
{
    ui->yellowTeamScoreLabel->setText(QString::number(yellow_score));
    ui->blueTeamScoreLabel->setText(QString::number(blue_score));
}

void MainWindow::updateControlData()
{
    ui->yellowTeamNameLabel->setText(QString::fromStdString(Config::control.yellow_name));
    ui->blueTeamNameLabel->setText(QString::fromStdString(Config::control.blue_name));
}

void MainWindow::on_actionGeometry_triggered()
{
    GeometryConfigDialog geomConfigDialog(this);
    geomConfigDialog.setWindowModality(Qt::WindowModal);
    if (geomConfigDialog.exec() == QDialog::Accepted) {
        ui->monitorWidget->update();
        return;
    }
}
