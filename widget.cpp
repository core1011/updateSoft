#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    updatetask *tmp = new updatetask();
    tmp->start();
}

Widget::~Widget()
{
    delete ui;
}
