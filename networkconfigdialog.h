#ifndef NETWORKCONFIGDIALOG_H
#define NETWORKCONFIGDIALOG_H

#include <QDialog>
#include <QString>

#include "definitions.h"
#include "config.h"

namespace Ui {
class NetworkConfigDialog;
}

class NetworkConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetworkConfigDialog(QWidget *parent = 0);
    ~NetworkConfigDialog();

private slots:
    void on_NetworkConfigDialog_accepted();

    void on_defaultsButton_clicked();

private:
    Ui::NetworkConfigDialog *ui;
};

#endif // NETWORKCONFIGDIALOG_H
