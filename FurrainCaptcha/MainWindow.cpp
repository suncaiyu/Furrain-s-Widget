#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Captcha.h"
#include <QtDebug>
Captcha *c;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    c = new  Captcha(40, 5, this);
    c->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString txt = ui->lineEdit->text();
    if (txt == c->GetCode()) {
        qDebug() << "OK";
    } else {
        qDebug("%s","False");
    }
}
