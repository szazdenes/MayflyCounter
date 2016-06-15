#ifndef IMAGEGRAPHICSSCENEFORM_H
#define IMAGEGRAPHICSSCENEFORM_H

#include <QWidget>

namespace Ui {
class ImageGraphicsSceneForm;
}

class ImageGraphicsSceneForm : public QWidget
{
    Q_OBJECT

public:
    explicit ImageGraphicsSceneForm(QWidget *parent = 0);
    ~ImageGraphicsSceneForm();

private:
    Ui::ImageGraphicsSceneForm *ui;
};

#endif // IMAGEGRAPHICSSCENEFORM_H
