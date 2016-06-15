#ifndef IMAGEGRAPHICSVIEWFORM_H
#define IMAGEGRAPHICSVIEWFORM_H

#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>

namespace Ui {
class ImageGraphicsViewForm;
}

class ImageGraphicsViewForm : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ImageGraphicsViewForm(QWidget *parent = 0);
    ~ImageGraphicsViewForm();

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

signals:
    void signalLeftButtonPressed(QPointF pos);
    void signalMouseMoved(QPointF pos);
    void signalRightButtonPressed();
    void signalWheelScrolledUp();
    void signalWheelScrolledDown();

private:
    Ui::ImageGraphicsViewForm *ui;
};

#endif // IMAGEGRAPHICSVIEWFORM_H
