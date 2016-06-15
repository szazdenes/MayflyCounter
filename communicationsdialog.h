#ifndef COMMUNICATIONSDIALOG_H
#define COMMUNICATIONSDIALOG_H

#include <QDialog>

namespace Ui {
class CommunicationsDialog;
}

class CommunicationsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CommunicationsDialog(QWidget *parent = 0);
    ~CommunicationsDialog();

private:
    Ui::CommunicationsDialog *ui;
};

#endif // COMMUNICATIONSDIALOG_H
