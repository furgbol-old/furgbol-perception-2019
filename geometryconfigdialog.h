#ifndef GEOMETRYCONFIGDIALOG_H
#define GEOMETRYCONFIGDIALOG_H

#include <QDialog>

#include "config.h"

namespace Ui {
class GeometryConfigDialog;
}

class GeometryConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GeometryConfigDialog(QWidget *parent = 0);
    ~GeometryConfigDialog();

private slots:
    void on_GeometryConfigDialog_accepted();

private:
    Ui::GeometryConfigDialog *ui;
};

#endif // GEOMETRYCONFIGDIALOG_H
