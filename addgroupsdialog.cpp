#include "addgroupsdialog.h"
#include "ui_addgroupsdialog.h"

AddGroupsDialog::AddGroupsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddGroupsDialog)
{
    ui->setupUi(this);

}

AddGroupsDialog::~AddGroupsDialog()
{
    delete ui;
}

void AddGroupsDialog::on_buttonBox_accepted()
{
    if(!ui->lineEdit->text().isEmpty())
        emit signalFirstGroupSet(ui->lineEdit->text());
    if(!ui->lineEdit_2->text().isEmpty())
        emit signalSecondGroupSet(ui->lineEdit_2->text());
}
