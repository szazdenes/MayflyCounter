#ifndef MAYFLYCOUNTER_H
#define MAYFLYCOUNTER_H

#include <QMainWindow>

namespace Ui {
class MayflyCounter;
}

class MayflyCounter : public QMainWindow
{
    Q_OBJECT

public:
    explicit MayflyCounter(QWidget *parent = 0);
    ~MayflyCounter();

private:
    Ui::MayflyCounter *ui;
};

#endif // MAYFLYCOUNTER_H
