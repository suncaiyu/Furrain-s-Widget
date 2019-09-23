#ifndef FurrainPushButton_H
#define FurrainPushButton_H
#include <QPushButton>
#include <QEnterEvent>
#include <QPropertyAnimation>


class FurrainPushButton:public QPushButton
{
    Q_OBJECT
	Q_ENUMS (AniModde)
    Q_PROPERTY(int xx READ getxx WRITE setxx)
    Q_PROPERTY(QColor fadeoutColor READ getfadeoutColor WRITE setfadeoutColor)
public:
    FurrainPushButton(QWidget *parent, QColor color = QColor(255,194,123,155), int mode = 0);
    ~FurrainPushButton();

    enum AniModde{
        PROGRESS_MODE,
        FADEOUT_MODE
    };
	    void setMode(AniModde mode);
    void setTargetColor(QColor color);
	    void setfadeoutColor(QColor color);
    QColor getfadeoutColor();
protected:
    void paintEvent(QPaintEvent *event);
    QRect myRect;
    virtual void  enterEvent(QEvent * e);
    virtual void  leaveEvent(QEvent * e);
private:
    int xx;
    int getxx();
    void setxx(int x);
    int mode;
    QColor fadeoutColor;
    QPropertyAnimation *aniProgress;
    QPropertyAnimation *aniFadeout;
    QTimer *timer;
    QColor targetColor;

};

#endif // FurrainPushButton_H
