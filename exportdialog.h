#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = 0);
    ~ExportDialog();

signals:
    void signalExportNewFile();
    void signalExportAppendFile();

private slots:
    void on_newPushButton_clicked();

    void on_appendPushButton_clicked();

private:
    Ui::ExportDialog *ui;
};

#endif // EXPORTDIALOG_H
