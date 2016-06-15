#ifndef ADDGROUPSDIALOG_H
#define ADDGROUPSDIALOG_H

#include <QDialog>

namespace Ui {
class AddGroupsDialog;
}

class AddGroupsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddGroupsDialog(QWidget *parent = 0);
    ~AddGroupsDialog();

signals:
    void signalFirstGroupSet(QString firstGroup);
    void signalSecondGroupSet(QString secondGroup);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddGroupsDialog *ui;
};

#endif // ADDGROUPSDIALOG_H
