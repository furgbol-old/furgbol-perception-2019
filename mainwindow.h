#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>

#include "networkconfigdialog.h"
#include "geometryconfigdialog.h"
#include "controlconfigdialog.h"
#include "system.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionStart_toggled(bool arg1);
    void on_actionNetwork_triggered();
    void on_actionControl_triggered();
    void on_actionGeometry_triggered();
    void on_actionShow_ID_toggled(bool arg1);
    void on_actionPathPlanning_toggled(bool arg1);
    void updateScore(int yellow_score, int blue_score);

private:
    Ui::MainWindow *ui;
    QActionGroup *configActionGroup;
    System *system;

    void updateControlData();
};

#endif // MAINWINDOW_H
