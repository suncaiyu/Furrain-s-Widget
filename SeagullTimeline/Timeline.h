#ifndef TIMELINE_H
#define TIMELINE_H
#include <QAbstractScrollArea>
#include <QScrollBar>
// 一个简单的eid事件轴(其实还不算时间轴...)
class TimelineScrollBar : public QScrollBar
{
    Q_OBJECT
public:
    explicit TimelineScrollBar(QWidget *parent = nullptr) : QScrollBar(parent) {};
    explicit TimelineScrollBar(Qt::Orientation ot, QWidget *parent = nullptr) : QScrollBar(ot, parent){};
    virtual ~TimelineScrollBar(){};
protected:
    void enterEvent(QEvent *e) override {
        MouseEnterSignal();
    };

signals:
    void MouseEnterSignal();
};

class Timeline : public QAbstractScrollArea
{
    Q_OBJECT
public:
    Timeline(QWidget *parent = nullptr);
    virtual ~Timeline();

protected:
    void resizeEvent(QResizeEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void paintEvent(QPaintEvent *) override;
    void wheelEvent(QWheelEvent *e) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

private:
    void Layout();
    void drawLabelText(QPainter &p);
    void drawMouseCursor(QPainter &p);
    void drawAxixBounding(QPainter &p);
    void drawToolbar(QPainter &p);
    qreal GetPosByValue(const qreal value);
    qreal GetValueByPos(const qreal pos);
    int GetIntValue(const qreal value);

    qreal mZoom = 1.0;
    qreal mPan = 0.0;
    int mMaxEid = 889;
    // 显示的eidlabel的步长(隔多少显示一个label)
    int mEidLabelStep = 1;
    // 这个窗口(viewport)的width
    int mAreaWidth;
    int mEidLabelWidth;
    // 放大后(乘上zoom系数后)，虚拟的width
    qreal mVirtualWidth;
    QFont mFont;
    int mMaxEidTextWidth;
    // 每一个像素点代表多少长度的eid
    qreal mPerPixel;
    QPointF mPos;
    std::shared_ptr<TimelineScrollBar> mScrollBar;
    bool mShowCursor = true;
    bool mIsMousePress = false;
    QPointF mPressPoint;
};

#endif // TIMELINE_H
