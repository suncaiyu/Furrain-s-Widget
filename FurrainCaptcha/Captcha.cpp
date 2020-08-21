#include "Captcha.h"
#include "ui_Captcha.h"
#include <QPainter>
#include <QDebug>
#include <QTime>
#include <QTimer>
QString randstr(const int len)
{
    QString str;
    srand(time(NULL));
    int i;
    for (i = 0; i < len; ++i)
    {
        switch ((rand() % 3))
        {
        case 1:
            str[i] = 'A' + rand() % 26;
            break;
        case 2:
            str[i] = 'a' + rand() % 26;
            break;
        default:
            str[i] = '0' + rand() % 10;
            break;
        }
    }
    return str;
}

Captcha::Captcha(int height, int num, QWidget *parent) :
      QWidget(parent),m_amount(num),
      ui(new Ui::Captcha)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(m_amount * height, height));
    Reset();
    QTimer *t = new QTimer(this);
    connect(t, &QTimer::timeout, this, [this] { update(); });
    t->start(150);
}

Captcha::~Captcha()
{
    delete ui;
}


void Captcha::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.fillRect(rect(), Qt::white);
    qsrand(QTime::currentTime().second() * 1000 + QTime::currentTime().msec());
    for(int i=0; i<100*m_amount; i++)
    {
        p.setPen(QColor(qrand()%255,qrand()%255,qrand()%255));
        p.drawPoint(0 + qrand() % width(), 0 + qrand() % height());
    }

    for (int i = 0; i < m_amount * 2; i++) {
        QPoint pt1, pt2;
        pt1.setX(qrand() % this->rect().width());
        pt1.setY(0); //顶部的点

        pt2.setX(qrand() % width());
        pt2.setY(height()); //底部的点

        if (i % 2 == 0) {
            QPen pen = p.pen();
            pen.setStyle(Qt::PenStyle::DashDotLine);
            p.setPen(pen);
            p.drawLine(pt2, pt1);
            }
            else
            {
                QPen pen = p.pen();
                pen.setStyle(Qt::PenStyle::SolidLine);
                p.setPen(pen);
                p.drawLine(pt1, pt2);
            }
        }

    QFont font;
    font.setFamily("Microsoft YaHei");
    QFontMetrics fm = p.fontMetrics();
    for (int i = 0; i < m_amount; i++) {
        p.setPen(QColor(qrand()%255, qrand()%255, qrand()%255));
        // inch = 72  dpi = 96
        // 显示大小 = dpi * pointsize / inch
        font.setPointSize(m_pointsize);
        p.setFont(font);
        int charWidth = fm.width(m_code.at(i)) + fm.ascent();
        int charHeight = fm.ascent() + fm.descent() + fm.ascent();
        int maxchangewidth = perRect[i].width() - charWidth;
        int maxchangeHeight = perRect[i].height() - charHeight;
        QPoint pt(perRect[i].x() + qrand() % maxchangewidth, charHeight + rand() % maxchangeHeight);
        p.drawText(pt, m_code.at(i));
    }
}


void Captcha::Reset()
{
    m_code = randstr(m_amount);
}

void Captcha::resizeEvent(QResizeEvent *event)
{
    for (int i = 0; i < m_amount; i++) {
        QRect per = QRect(0 + (width() / m_amount) * i, 0, width() / m_amount, height());
        perRect.push_back(per);
    }
    m_pointsize = height() / 2 * 72 / 96;
}

void Captcha::mousePressEvent(QMouseEvent *event)
{
    Reset();
}

QString Captcha::GetCode()
{
    return m_code;
}
