#include "counterform.h"
#include "ui_counterform.h"
#include "communicationsdialog.h"

CounterForm::CounterForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CounterForm)
{
    ui->setupUi(this);

    zoom = 1;

    connect(ui->imageGraphicsView, &ImageGraphicsViewForm::signalWheelScrolledDown, this, &CounterForm::slotWheelScrolledDown);
    connect(ui->imageGraphicsView, &ImageGraphicsViewForm::signalWheelScrolledUp, this, &CounterForm::slotWheelScrolledUp);
    connect(ui->imageGraphicsView, &ImageGraphicsViewForm::signalMouseMoved, this, &CounterForm::slotMouseMoved);

    ui->countTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->countTableWidget->setHorizontalHeaderLabels(QStringList() << "Picture" << ui->mayflyCheckBox->text() << ui->chironomidaeCheckBox->text());

    ui->imageGraphicsView->setScene(&scene);
    ui->imageGraphicsView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);

    pen.setWidth(5);
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
    painter.begin(&mask);
    pen.setColor(Qt::transparent);
    painter.setPen(pen);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    QPointF toRemove;
    double currentSize, currentPenSize;

    if(ui->mayflyCheckBox->isChecked() && !group1PosList.isEmpty()){
        mayflyNo--;
        toRemove = group1PosList.takeLast();
        currentSize = size1List.takeLast();
        currentPenSize = penSize1List.takeLast();
        ui->countTableWidget->setItem(ui->countTableWidget->rowCount()-1, 1, new QTableWidgetItem(QString::number(mayflyNo)));
    }
    else if(ui->chironomidaeCheckBox->isChecked() && !group2PosList.isEmpty()){
        chirNo--;
        toRemove = group2PosList.takeLast();
        currentSize = size2List.takeLast();
        currentPenSize = penSize2List.takeLast();
        ui->countTableWidget->setItem(ui->countTableWidget->rowCount()-1, 2, new QTableWidgetItem(QString::number(chirNo)));
    }
    else
        return;

    painter.drawEllipse(toRemove.x() - (currentPenSize / currentSize / 2.0), toRemove.y() - (currentPenSize / currentSize / 2.0), currentPenSize / currentSize, currentPenSize / currentSize);
    painter.end();
    refreshImage();
}

void CounterForm::slotLeftButtonPressed(QPointF pos)
{
    double size = zoom;
    painter.begin(&mask);
    if(ui->mayflyCheckBox->isChecked()){
        pen.setColor(Qt::blue);
        mayflyNo++;
        group1PosList.append(pos);
        size1List.append(size);
        penSize1List.append(penSize);
        ui->countTableWidget->setItem(ui->countTableWidget->rowCount()-1, 1, new QTableWidgetItem(QString::number(mayflyNo)));
    }
    if(ui->chironomidaeCheckBox->isChecked()){
        pen.setColor(Qt::green);
        chirNo++;
        group2PosList.append(pos);
        size2List.append(size);
        penSize2List.append(penSize);
        ui->countTableWidget->setItem(ui->countTableWidget->rowCount()-1, 2, new QTableWidgetItem(QString::number(chirNo)));
    }
    painter.setPen(pen);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawEllipse(pos.x() - (penSize / size / 2.0), pos.y() - (penSize / size / 2.0), penSize / size, penSize / size);
    painter.end();
    refreshImage();
}

void CounterForm::slotWheelScrolledUp()
{
    if(zoom < 3)
        setImageSize(1.1);
}

void CounterForm::slotWheelScrolledDown()
{
    if(zoom > 0.1)
        setImageSize(0.9);
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
        connect(ui->imageGraphicsView, &ImageGraphicsViewForm::signalLeftButtonPressed, this, &CounterForm::slotLeftButtonPressed);
        connect(ui->imageGraphicsView, &ImageGraphicsViewForm::signalRightButtonPressed, this, &CounterForm::slotRightButtonPressed);

    }
    else{
        ui->imageGraphicsView->unsetCursor();
        disconnect(ui->imageGraphicsView, &ImageGraphicsViewForm::signalLeftButtonPressed, this, &CounterForm::slotLeftButtonPressed);
        disconnect(ui->imageGraphicsView, &ImageGraphicsViewForm::signalRightButtonPressed, this, &CounterForm::slotRightButtonPressed);
    }


}

void CounterForm::on_chironomidaeCheckBox_toggled(bool checked)
{

    if(checked == true){
        ui->chironomidaeCheckBox->setChecked(checked);
        ui->mayflyCheckBox->setChecked(false);
        setCursorImage(penSize);
        connect(ui->imageGraphicsView, &ImageGraphicsViewForm::signalLeftButtonPressed, this, &CounterForm::slotLeftButtonPressed);
        connect(ui->imageGraphicsView, &ImageGraphicsViewForm::signalRightButtonPressed, this, &CounterForm::slotRightButtonPressed);
    }
    else{
        ui->imageGraphicsView->unsetCursor();
        disconnect(ui->imageGraphicsView, &ImageGraphicsViewForm::signalLeftButtonPressed, this, &CounterForm::slotLeftButtonPressed);
        disconnect(ui->imageGraphicsView, &ImageGraphicsViewForm::signalRightButtonPressed, this, &CounterForm::slotRightButtonPressed);
    }
}

void CounterForm::on_loadPushButton_clicked()
{
    group1PosList.clear();
    group2PosList.clear();
    size1List.clear();
    size2List.clear();
    penSize1List.clear();
    penSize2List.clear();

    ui->imageGraphicsView->scale(1.0/zoom, 1.0/zoom);
    zoom =1;
    openFileName = QFileDialog::getOpenFileName(this, QDir::currentPath());
    currentImage = QImage(openFileName);
    mask = QImage(currentImage.size(), QImage::Format_ARGB32);
    mask.fill(Qt::transparent);
    mask2 = QImage(ui->imageGraphicsView->viewport()->size(), QImage::Format_ARGB32);
    mask2.fill(Qt::transparent);
    penSize = (double)currentImage.height() / 50.0;

    ui->zoomVerticalSlider->setMinimum(10);
    ui->zoomVerticalSlider->setMaximum((int)(ui->imageGraphicsView->height()/2.0));
    ui->zoomVerticalSlider->setValue((int)((double)currentImage.height() / 50.0));

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

void CounterForm::setImageSize(double ratio)
{
    ui->imageGraphicsView->scale(ratio, ratio);
    zoom *= ratio;
}

void CounterForm::on_zoomVerticalSlider_valueChanged(int value)
{
    if(ui->mayflyCheckBox->isChecked() || ui->chironomidaeCheckBox->isChecked()){
        penSize = (double)value;
        setCursorImage(penSize);
    }
}

void CounterForm::on_originalSizePushButton_clicked()
{
    ui->imageGraphicsView->scale(1.0/zoom, 1.0/zoom);
    zoom = 1;
}

void CounterForm::on_fiToWindowPushButton_clicked()
{
    ui->imageGraphicsView->scale(1.0/zoom, 1.0/zoom);

    QRectF imageRect = currentImage.rect();
    QRectF rect = ui->imageGraphicsView->viewport()->rect();
    double fitSize = qMin<double>(rect.width() / imageRect.width(), rect.height() / imageRect.height());

    ui->imageGraphicsView->scale(fitSize, fitSize);
    zoom = fitSize;
}

void CounterForm::on_savePushButton_clicked()
{
    CommunicationsDialog dialog;
    connect(&dialog, &CommunicationsDialog::accepted, this, &CounterForm::slotDialogAccepted);
    dialog.exec();
}

void CounterForm::on_exportPushButton_clicked()
{
    ExportDialog dialog(this);
    connect(&dialog, &ExportDialog::signalExportNewFile, this, &CounterForm::slotExportNewFile);
    connect(&dialog, &ExportDialog::signalExportAppendFile, this, &CounterForm::slotExportAppendFile);
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
    connect(&dialog, &AddGroupsDialog::signalFirstGroupSet, this, &CounterForm::slotFirstGroupSet);
    connect(&dialog, &AddGroupsDialog::signalSecondGroupSet, this, &CounterForm::slotSecondGroupSet);
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
