#include "imagegraphicsviewform.h"
#include "ui_imagegraphicsviewform.h"

ImageGraphicsViewForm::ImageGraphicsViewForm(QWidget *parent) :
    QGraphicsView(parent),
    ui(new Ui::ImageGraphicsViewForm)
{
    ui->setupUi(this);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

ImageGraphicsViewForm::~ImageGraphicsViewForm()
{
    delete ui;
}

void ImageGraphicsViewForm::mousePressEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) != 0)
        emit signalLeftButtonPressed(mapToScene(event->pos()));
    if((event->buttons() & Qt::RightButton) != 0)
        emit signalRightButtonPressed();
    else
        QGraphicsView::mousePressEvent(event);
}

void ImageGraphicsViewForm::mouseMoveEvent(QMouseEvent *event)
{
    emit signalMouseMoved(mapToScene(event->pos()));
}

void ImageGraphicsViewForm::wheelEvent(QWheelEvent *event)
{
    if(event->delta() > 0)
        emit signalWheelScrolledUp();
    if(event->delta() < 0)
        emit signalWheelScrolledDown();
    else
        QGraphicsView::wheelEvent(event);
}
