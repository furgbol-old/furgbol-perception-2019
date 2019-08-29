#ifndef CONTROLCONFIGDIALOG_H
#define CONTROLCONFIGDIALOG_H

#include <QDialog>

#include "definitions.h"
#include "config.h"

namespace Ui {
class ControlConfigDialog;
}

class ControlConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ControlConfigDialog(QWidget *parent = 0);
    ~ControlConfigDialog();

private slots:
    void on_ControlConfigDialog_accepted();

private:
    Ui::ControlConfigDialog *ui;
};

#endif // CONTROLCONFIGDIALOG_H
