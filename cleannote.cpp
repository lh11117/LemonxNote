#include "cleannote.h"
#include "ui_cleannote.h"

CleanNote::CleanNote(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CleanNote)
{
    ui->setupUi(this);
}

CleanNote::~CleanNote()
{
    delete ui;
}

void CleanNote::on_horizontalSlider_sliderReleased()
{
    if (ui->horizontalSlider->value() < 97) {
        ui->horizontalSlider->setValue(0);
    } else {
        emit on_clean();
        this->close();
    }
}
