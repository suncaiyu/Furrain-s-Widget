#include "Widget.h"
#include "ui_Widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    mTimeline = std::make_shared<Timeline>();
    mTimeline->show();
}

Widget::~Widget()
{
    delete ui;
}

