#ifndef DIALOGCLEAN_H
#define DIALOGCLEAN_H

#include <QDialog>

#include "canvaswin.h"
namespace Ui {
class DialogClean;
}

class DialogClean : public QDialog
{
    Q_OBJECT

public:
    explicit DialogClean(QWidget *parent = nullptr);
    ~DialogClean();
    bool event(QEvent*);

private slots:
    void on_horizontalSlider_sliderReleased();

private:
    Ui::DialogClean *ui;
};

#endif // DIALOGCLEAN_H
