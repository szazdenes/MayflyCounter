#include "exportdialog.h"
#include "ui_exportdialog.h"

ExportDialog::ExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::on_newPushButton_clicked()
{
    this->close();
    emit signalExportNewFile();
}

void ExportDialog::on_appendPushButton_clicked()
{
    this->close();
    emit signalExportAppendFile();
}
