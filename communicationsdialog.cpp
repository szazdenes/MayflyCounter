#include "communicationsdialog.h"
#include "ui_communicationsdialog.h"

CommunicationsDialog::CommunicationsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommunicationsDialog)
{
    ui->setupUi(this);
}

CommunicationsDialog::~CommunicationsDialog()
{
    delete ui;
}
