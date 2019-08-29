#include "controlconfigdialog.h"
#include "ui_controlconfigdialog.h"

ControlConfigDialog::ControlConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ControlConfigDialog)
{
    ui->setupUi(this);
    setFixedSize(width(), height());

    if (Config::control.field_side == ControlPackage::Left) ui->leftSideRadioButton->setChecked(true);
    else ui->rightSideRadioButton->setChecked(true);
    ui->gkIdSpinBox->setValue(Config::control.id_gk);
    if (Config::control.team_color == ControlPackage::Yellow) ui->yellowRadioButton->setChecked(true);
    else ui->blueRadioButton->setChecked(true);
    ui->yellowNameInput->setText(QString::fromStdString(Config::control.yellow_name));
    ui->blueNameInput->setText(QString::fromStdString(Config::control.blue_name));
}

ControlConfigDialog::~ControlConfigDialog()
{
    delete ui;
}

void ControlConfigDialog::on_ControlConfigDialog_accepted()
{
    Config::control.field_side = (ui->leftSideRadioButton->isChecked()) ? ControlPackage::Left : ControlPackage::Right;
    Config::control.id_gk = ui->gkIdSpinBox->value();
    Config::control.team_color = (ui->yellowRadioButton->isChecked()) ? ControlPackage::Yellow : ControlPackage::Blue;
    Config::control.yellow_name = ui->yellowNameInput->text().toStdString();
    Config::control.blue_name = ui->blueNameInput->text().toStdString();
}
