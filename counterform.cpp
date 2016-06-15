#include "counterform.h"
#include "ui_counterform.h"
#include "communicationsdialog.h"

CounterForm::CounterForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CounterForm)
{
    ui->setupUi(this);
    ui->countTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->countTableWidget->setHorizontalHeaderLabels(QStringList() << "Picture" << ui->mayflyCheckBox->text() << ui->chironomidaeCheckBox->text());

    ui->imageGraphicsView->setScene(&scene);
    ui->imageGraphicsView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);

    pen.setWidth(5);
    rightButtonClicked = false;
}

CounterForm::~CounterForm()
{
    delete ui;
}

void CounterForm::slotMouseMoved(QPointF pos)
{
    //here comes the drag event
}

void CounterForm::slotRightButtonPressed()
{
    double size = ui->zoomVerticalSlider->value() / 100.0;
    painter.begin(&mask);
    pen.setColor(Qt::transparent);
    painter.setPen(pen);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawEllipse(lastPos.x() - (penSize / size / 2.0), lastPos.y() - (penSize / size / 2.0), penSize / size, penSize / size);
    painter.end();
    refreshImage();

    if (rightButtonClicked == false){
        if(ui->mayflyCheckBox->isChecked())
            mayflyNo--;
            ui->countTableWidget->setItem(ui->countTableWidget->rowCount()-1, 1, new QTableWidgetItem(QString::number(mayflyNo)));

        if(ui->chironomidaeCheckBox->isChecked())
            chirNo--;
            ui->countTableWidget->setItem(ui->countTableWidget->rowCount()-1, 2, new QTableWidgetItem(QString::number(chirNo)));
    }
    rightButtonClicked = true;
}

void CounterForm::slotLeftButtonPressed(QPointF pos)
{
    lastPos = pos;
    double size = ui->zoomVerticalSlider->value() / 100.0;
    painter.begin(&mask);
    if(ui->mayflyCheckBox->isChecked()){
        pen.setColor(Qt::blue);
        mayflyNo++;
        ui->countTableWidget->setItem(ui->countTableWidget->rowCount()-1, 1, new QTableWidgetItem(QString::number(mayflyNo)));
    }
    if(ui->chironomidaeCheckBox->isChecked()){
        pen.setColor(Qt::green);
        chirNo++;
        ui->countTableWidget->setItem(ui->countTableWidget->rowCount()-1, 2, new QTableWidgetItem(QString::number(chirNo)));
    }
    painter.setPen(pen);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawEllipse(pos.x() - (penSize / size / 2.0), pos.y() - (penSize / size / 2.0), penSize / size, penSize / size);
    painter.end();
    refreshImage();

    rightButtonClicked = false;
}

void CounterForm::slotWheelScrolledUp()
{
    if(penSize < ui->imageGraphicsView->height()/2.0){
        penSize *= 1.1;
        setCursorImage(penSize);
    }
}

void CounterForm::slotWheelScrolledDown()
{
    if(penSize > 15){
        penSize *= 0.9;
        setCursorImage(penSize);
    }
}

void CounterForm::slotDialogAccepted()
{
    image.save(openFileName.remove(".JPG").append("_counted.jpg"));
    timer.start(1000);
    ui->savePushButton->setStyleSheet("QPushButton{color: red}");
    ui->savePushButton->setText("Image Saved");

    connect(&timer, SIGNAL(timeout()), this, SLOT(slotTimeOut()));

}

void CounterForm::slotExportNewFile()
{
    QString outFileName = QFileDialog::getSaveFileName(this, QDir::currentPath());
    if (!outFileName.endsWith(".csv"))
        outFileName.append(".csv");
    exportFile.setFileName(outFileName);
    if(!exportFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug("baj");
        return;
    }
    exportStream.setDevice(&exportFile);
    exportStream << "Picture\t" << ui->mayflyCheckBox->text() << "\t" << ui->chironomidaeCheckBox->text() << "\n";

    for(int i = 0; i < ui->countTableWidget->rowCount(); i++)
        exportStream << ui->countTableWidget->item(i, 0)->text() << "\t" << ui->countTableWidget->item(i, 1)->text() << "\t" << ui->countTableWidget->item(i, 2)->text() << "\n";

    exportFile.close();
}

void CounterForm::slotExportAppendFile()
{
    QString outFileName = QFileDialog::getOpenFileName(this, QDir::currentPath());
    exportFile.setFileName(outFileName);
    if(!exportFile.open(QIODevice::Append | QIODevice::Text)) {
        qDebug("baj");
        return;
    }
    exportStream.setDevice(&exportFile);

    QFile readFile(outFileName);
    if(!readFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug("baj");
        return;
    }
    QTextStream readStream(&readFile);
    QStringList dataList;
    while(!readStream.atEnd()){
        QString currentline = readStream.readLine();
        dataList.append(currentline);
    }
    readFile.close();

    for(int i = 0; i < ui->countTableWidget->rowCount(); i++){
        QString currentStream = QString(ui->countTableWidget->item(i, 0)->text() + "\t" + ui->countTableWidget->item(i, 1)->text() + "\t" + ui->countTableWidget->item(i, 2)->text());
        if(!dataList.contains(currentStream))
            exportStream << currentStream + "\n";
    }

    exportFile.close();
}

void CounterForm::on_mayflyCheckBox_toggled(bool checked)
{
    if(checked == true){
        ui->mayflyCheckBox->setChecked(checked);
        ui->chironomidaeCheckBox->setChecked(false);
        setCursorImage(penSize);
        connect(ui->imageGraphicsView, SIGNAL(signalMouseMoved(QPointF)), this, SLOT(slotMouseMoved(QPointF)));
        connect(ui->imageGraphicsView, SIGNAL(signalLeftButtonPressed(QPointF)), this, SLOT(slotLeftButtonPressed(QPointF)));
        connect(ui->imageGraphicsView, SIGNAL(signalRightButtonPressed()), this, SLOT(slotRightButtonPressed()));
        connect(ui->imageGraphicsView, SIGNAL(signalWheelScrolledDown()), this, SLOT(slotWheelScrolledDown()));
        connect(ui->imageGraphicsView, SIGNAL(signalWheelScrolledUp()), this, SLOT(slotWheelScrolledUp()));
    }
    else{
        ui->imageGraphicsView->unsetCursor();
        disconnect(ui->imageGraphicsView, SIGNAL(signalMouseMoved(QPointF)), this, SLOT(slotMouseMoved(QPointF)));
        disconnect(ui->imageGraphicsView, SIGNAL(signalLeftButtonPressed(QPointF)), this, SLOT(slotLeftButtonPressed(QPointF)));
        disconnect(ui->imageGraphicsView, SIGNAL(signalRightButtonPressed()), this, SLOT(slotRightButtonPressed()));
        disconnect(ui->imageGraphicsView, SIGNAL(signalWheelScrolledDown()), this, SLOT(slotWheelScrolledDown()));
        disconnect(ui->imageGraphicsView, SIGNAL(signalWheelScrolledUp()), this, SLOT(slotWheelScrolledUp()));
    }


}

void CounterForm::on_chironomidaeCheckBox_toggled(bool checked)
{

    if(checked == true){
        ui->chironomidaeCheckBox->setChecked(checked);
        ui->mayflyCheckBox->setChecked(false);
        setCursorImage(penSize);
        connect(ui->imageGraphicsView, SIGNAL(signalMouseMoved()), this, SLOT(slotMouseMoved()));
        connect(ui->imageGraphicsView, SIGNAL(signalLeftButtonPressed(QPointF)), this, SLOT(slotLeftButtonPressed(QPointF)));
        connect(ui->imageGraphicsView, SIGNAL(signalRightButtonPressed()), this, SLOT(slotRightButtonPressed()));
        connect(ui->imageGraphicsView, SIGNAL(signalWheelScrolledDown()), this, SLOT(slotWheelScrolledDown()));
        connect(ui->imageGraphicsView, SIGNAL(signalWheelScrolledUp()), this, SLOT(slotWheelScrolledUp()));
    }
    else{
        ui->imageGraphicsView->unsetCursor();
        disconnect(ui->imageGraphicsView, SIGNAL(signalMouseMoved()), this, SLOT(slotMouseMoved()));
        disconnect(ui->imageGraphicsView, SIGNAL(signalLeftButtonPressed(QPointF)), this, SLOT(slotLeftButtonPressed(QPointF)));
        disconnect(ui->imageGraphicsView, SIGNAL(signalRightButtonPressed()), this, SLOT(slotRightButtonPressed()));
        disconnect(ui->imageGraphicsView, SIGNAL(signalWheelScrolledDown()), this, SLOT(slotWheelScrolledDown()));
        disconnect(ui->imageGraphicsView, SIGNAL(signalWheelScrolledUp()), this, SLOT(slotWheelScrolledUp()));
    }
}

void CounterForm::on_loadPushButton_clicked()
{
    openFileName = QFileDialog::getOpenFileName(this, QDir::currentPath());
    currentImage = QImage(openFileName);
    mask = QImage(currentImage.size(), QImage::Format_ARGB32);
    mask.fill(Qt::transparent);
    mask2 = QImage(ui->imageGraphicsView->viewport()->size(), QImage::Format_ARGB32);
    mask2.fill(Qt::transparent);
    penSize = (double)currentImage.height() / 50.0;

    refreshImage();
    if(ui->mayflyCheckBox->isChecked() || ui->chironomidaeCheckBox->isChecked())
        setCursorImage(penSize);

    QString picname = openFileName.split("/").last();
    mayflyNo = 0;
    chirNo = 0;

    ui->countTableWidget->insertRow(ui->countTableWidget->rowCount());
    ui->countTableWidget->setItem(ui->countTableWidget->rowCount()-1, 0, new QTableWidgetItem(picname));
    ui->countTableWidget->setItem(ui->countTableWidget->rowCount()-1, 1, new QTableWidgetItem(QString::number(mayflyNo)));
    ui->countTableWidget->setItem(ui->countTableWidget->rowCount()-1, 2, new QTableWidgetItem(QString::number(chirNo)));
}

void CounterForm::refreshImage()
{
    image = QImage(currentImage.size(), QImage::Format_ARGB32_Premultiplied);

    QPainter painter(&image);
    painter.drawImage(0, 0, currentImage);
    painter.drawImage(0, 0, mask);
    painter.end();

    scene.clear();
    scene.addPixmap(QPixmap::fromImage(image));
}

void CounterForm::setCursorImage(double size)
{
    QImage cursorImage = QImage(size, size, QImage::Format_ARGB32);
    QPainter cursorPainter(&cursorImage);
    cursorPainter.setCompositionMode(QPainter::CompositionMode_Source);
    cursorPainter.fillRect(cursorImage.rect(), Qt::transparent);
    if(ui->mayflyCheckBox->isChecked())
        cursorPainter.setPen(Qt::blue);
    if(ui->chironomidaeCheckBox->isChecked())
        cursorPainter.setPen(Qt::green);
    cursorPainter.drawEllipse(0, 0, size, size);
    cursorPainter.end();
    cursor = QCursor(QPixmap::fromImage(cursorImage));
    ui->imageGraphicsView->setCursor(cursor);
}

void CounterForm::on_zoomVerticalSlider_valueChanged(int value)
{
    QMatrix matrix = ui->imageGraphicsView->matrix().inverted();
    QRectF visibleRect = matrix.mapRect(ui->imageGraphicsView->viewport()->rect());
    QRectF rect = ui->imageGraphicsView->viewport()->rect();

    double zoom = qMin<double>(visibleRect.width() / rect.width(), visibleRect.height() / rect.height());
    zoom *= value / 100.0;

    ui->imageGraphicsView->scale(zoom, zoom);
}

void CounterForm::on_originalSizePushButton_clicked()
{
    ui->zoomVerticalSlider->setValue(100);
}

void CounterForm::on_fiToWindowPushButton_clicked()
{
    QRectF imageRect = currentImage.rect();
    QRectF rect = ui->imageGraphicsView->viewport()->rect();
    double zoom = qMin<double>(rect.width() / imageRect.width(), rect.height() / imageRect.height());

    ui->zoomVerticalSlider->setValue(zoom * 100.0);
}

void CounterForm::on_savePushButton_clicked()
{
    CommunicationsDialog dialog;
    connect(&dialog, SIGNAL(accepted()), this, SLOT(slotDialogAccepted()));
    dialog.exec();
}

void CounterForm::on_exportPushButton_clicked()
{
    ExportDialog dialog(this);
    connect(&dialog, SIGNAL(signalExportNewFile()), this, SLOT(slotExportNewFile()));
    connect(&dialog, SIGNAL(signalExportAppendFile()), this, SLOT(slotExportAppendFile()));
    dialog.exec();
}

void CounterForm::slotTimeOut()
{
    ui->savePushButton->setStyleSheet("default");
    ui->savePushButton->setText("Save Image");
    timer.stop();
}

void CounterForm::on_addGroupPushButton_clicked()
{
    AddGroupsDialog dialog(this);
    connect(&dialog, SIGNAL(signalFirstGroupSet(QString)), this, SLOT(slotFirstGroupSet(QString)));
    connect(&dialog, SIGNAL(signalSecondGroupSet(QString)), this, SLOT(slotSecondGroupSet(QString)));
    dialog.exec();
}

void CounterForm::slotFirstGroupSet(QString firstGroup)
{
    ui->mayflyCheckBox->setText(firstGroup);
    ui->countTableWidget->setHorizontalHeaderLabels(QStringList() << "Picture" << ui->mayflyCheckBox->text() << ui->chironomidaeCheckBox->text());
}

void CounterForm::slotSecondGroupSet(QString secondGroup)
{
    ui->chironomidaeCheckBox->setText(secondGroup);
    ui->countTableWidget->setHorizontalHeaderLabels(QStringList() << "Picture" << ui->mayflyCheckBox->text() << ui->chironomidaeCheckBox->text());

}
