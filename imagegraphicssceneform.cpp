#include "imagegraphicssceneform.h"
#include "ui_imagegraphicssceneform.h"

ImageGraphicsSceneForm::ImageGraphicsSceneForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageGraphicsSceneForm)
{
    ui->setupUi(this);
}

ImageGraphicsSceneForm::~ImageGraphicsSceneForm()
{
    delete ui;
}
