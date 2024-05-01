#ifndef CLEANNOTE_H
#define CLEANNOTE_H

#include <QWidget>

namespace Ui {
class CleanNote;
}

class CleanNote : public QWidget
{
    Q_OBJECT

public:
    explicit CleanNote(QWidget *parent = nullptr);
    ~CleanNote();

private slots:
    void on_horizontalSlider_sliderReleased();

private:
    Ui::CleanNote *ui;


signals:
    void on_clean();
};

#endif // CLEANNOTE_H
