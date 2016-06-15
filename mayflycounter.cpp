#include "mayflycounter.h"
#include "ui_mayflycounter.h"

MayflyCounter::MayflyCounter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MayflyCounter)
{
    ui->setupUi(this);
}

MayflyCounter::~MayflyCounter()
{
    delete ui;
}
