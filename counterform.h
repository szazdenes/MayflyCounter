#ifndef COUNTERFORM_H
#define COUNTERFORM_H

#include "addgroupsdialog.h"
#include "exportdialog.h"

#include <QWidget>
#include <QGraphicsScene>
#include <QPainter>
#include <QTimer>
#include <QFileDialog>
#include <QCursor>
#include <QTextStream>

namespace Ui {
class CounterForm;
}

class CounterForm : public QWidget
{
    Q_OBJECT

public:
    explicit CounterForm(QWidget *parent = 0);
    ~CounterForm();


public slots:
    void slotMouseMoved(QPointF pos);
    void slotRightButtonPressed();
    void slotLeftButtonPressed(QPointF pos);
    void slotWheelScrolledUp();
    void slotWheelScrolledDown();
    void slotDialogAccepted();
    void slotExportNewFile();
    void slotExportAppendFile();

private slots:
    void on_mayflyCheckBox_toggled(bool checked);
    void on_chironomidaeCheckBox_toggled(bool checked);
    void on_loadPushButton_clicked();
    void on_zoomVerticalSlider_valueChanged(int value);
    void on_originalSizePushButton_clicked();
    void on_fiToWindowPushButton_clicked();
    void on_savePushButton_clicked();
    void on_exportPushButton_clicked();
    void slotTimeOut();
    void on_addGroupPushButton_clicked();
    void slotFirstGroupSet(QString firstGroup);
    void slotSecondGroupSet(QString secondGroup);

private:
    Ui::CounterForm *ui;

    QString openFileName;
    QImage image, currentImage;
    QImage mask, mask2;
    QGraphicsScene scene;
    double penSize;
    QPainter painter;
    QPen pen;
    QPointF lastPos;
    QCursor cursor;
    double mayflyNo;
    double chirNo;
    bool rightButtonClicked;
    QTimer timer;
    double zoom;

    QFile exportFile;
    QTextStream exportStream;

    void refreshImage();
    void setCursorImage(double size);
    void setImageSize(double ratio);
};

#endif // COUNTERFORM_H
