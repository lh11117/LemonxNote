#include "dialogclean.h"
#include "ui_dialogclean.h"

DialogClean::DialogClean(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogClean)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    connect(ui->horizontalSlider, &QSlider::valueChanged, [=]{
        if (ui->horizontalSlider->value() >= 97) {
            ui->label->setText(tr("Release to clear"));
        } else {
            ui->label->setText(tr(">>> Swipe to clear  >>>"));
        }
    });
}

DialogClean::~DialogClean()
{
    delete ui;
}

void DialogClean::on_horizontalSlider_sliderReleased()
{
    if (ui->horizontalSlider->value() < 97) {
        ui->horizontalSlider->setValue(0);
    } else {
        this->close();
    }
}

bool DialogClean::event(QEvent * event)
{
    if (QEvent::ActivationChange == event->type()) {
        if (QApplication::activeWindow() != this){
            if (ui->horizontalSlider->value() < 97) {
                this->close();
            } else {
                accept();
                this->close();
            }
        }
    }

    return QWidget::event(event);
}
