#ifndef CAPTCHA_H
#define CAPTCHA_H

#include <QWidget>

namespace Ui {
class Captcha;
}

class Captcha : public QWidget
{
    Q_OBJECT

public:
    explicit Captcha(int height, int number, QWidget *parent = nullptr);
    ~Captcha();
    QString GetCode();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::Captcha *ui;
    QString m_code = "";
    int m_amount = 0;
    int m_pointsize = 0;
    void Reset();
    QVector<QRect> perRect;
};

#endif // CAPTCHA_H
