#include "Timeline.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QScrollBar>

namespace  {
constexpr int PADDING_LEFT = 100; //左侧可能会画一些toolbar
}
Timeline::Timeline(QWidget *parent) : QAbstractScrollArea(parent)
{
    mFont.setFamily("SimSun");
    mFont.setPointSize(18);
    setMouseTracking(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    mScrollBar = std::make_shared<TimelineScrollBar>(Qt::Orientation::Horizontal);
    setHorizontalScrollBar(mScrollBar.get());
    QObject::connect(mScrollBar.get(), &QScrollBar::valueChanged,
                     [this](int value) { mPan = -value; });
    connect(mScrollBar.get(), &TimelineScrollBar::MouseEnterSignal, this, [this](){
        mShowCursor = false;
        viewport()->update();
    });
    setWindowTitle(tr("Timeline"));
}

Timeline::~Timeline()
{

}

void Timeline::resizeEvent(QResizeEvent *)
{
    Layout();
}

void Timeline::paintEvent(QPaintEvent *)
{
    QPainter p(viewport());
    drawAxixBounding(p);
    drawLabelText(p);
    if (mShowCursor) {
        drawMouseCursor(p);
    }
    drawToolbar(p);
}

void Timeline::wheelEvent(QWheelEvent *e)
{
    qreal prevZoom = mZoom;
    int delta = e->delta();
    if (delta > 0 && mZoom < 100) {
        mZoom++;
    } else if (delta < 0 && mZoom > 1) {
        mZoom--;
    }
    qreal zoomDelta = mZoom / prevZoom;
    qreal newPan = mPan;
    newPan -= e->x();
    newPan = newPan * zoomDelta;
    newPan += e->x();
    mPan = qBound(-mEidLabelWidth * (mZoom - 1.0), newPan, 0.0);
    Layout();
}

void Timeline::mousePressEvent(QMouseEvent *e)
{
    mIsMousePress = true;
    mPressPoint = e->pos();
}

void Timeline::mouseMoveEvent(QMouseEvent *e)
{
    if (e->x() >= PADDING_LEFT){
        mPos = e->pos();
    }
    mShowCursor = true;
    if (mIsMousePress) {
        QPointF currentPoint = e->pos();
        int distance = currentPoint.x() - mPressPoint.x();
        mPan += distance;
        mPan = qBound(-mEidLabelWidth * (mZoom - 1.0), mPan, 0.0);
        mPressPoint = currentPoint;
    }
    Layout();
}

void Timeline::mouseReleaseEvent(QMouseEvent *)
{
    mIsMousePress = false;
}

void Timeline::Layout()
{
    QFontMetrics fm(mFont);
    mMaxEidTextWidth = fm.horizontalAdvance(QString::number(mMaxEid)) * 2;
    mAreaWidth = viewport()->width();
    mEidLabelWidth = mAreaWidth - PADDING_LEFT;
    mVirtualWidth = (qreal)mEidLabelWidth * mZoom;
    int unitystep = 1;
    int unitystep2 = 1;
    mEidLabelStep = unitystep * unitystep2;
    while (mVirtualWidth > 0 && ((qreal)mMaxEid / (qreal)mEidLabelStep * mMaxEidTextWidth > mVirtualWidth)) {
        // step = 1 2 5 10 20 50 100 200 500 ...
        if (unitystep == 1) {
            unitystep = 2;
        } else if (unitystep == 2) {
            unitystep =5;
        } else if (unitystep == 5) {
            unitystep = 1;
            unitystep2 *= 10;
        }
        mEidLabelStep =unitystep * unitystep2;
    }
    mPerPixel = (qreal)mMaxEid / mVirtualWidth;
    int savedPan = mPan;

    horizontalScrollBar()->setRange(0, mVirtualWidth - mEidLabelWidth);
    horizontalScrollBar()->setSingleStep(mEidLabelStep);
    horizontalScrollBar()->setPageStep(mAreaWidth);
    horizontalScrollBar()->setValue(-savedPan);
    viewport()->update();

}

void Timeline::drawLabelText(QPainter &p)
{
    QFontMetrics fm(mFont);
    for (int i = 0; i <= mMaxEid; i += mEidLabelStep) {
        QRectF rc(GetPosByValue(i), 0, mMaxEidTextWidth, fm.height());
        rc.adjust(-mMaxEidTextWidth / 2, 0, -mMaxEidTextWidth / 2, 0);
        if (rc.right() < 0) {
            continue;
        }
        if (rc.left() > mAreaWidth) {
            break;
        }
        p.drawText(rc, Qt::AlignCenter, QString::number(i));
    }
}

void Timeline::drawMouseCursor(QPainter &p)
{
    QFontMetrics fm(mFont);
    qreal value = GetValueByPos(mPos.x());
    int intValue = GetIntValue(value);
    qreal posX = GetPosByValue(intValue);
    QRectF rc(posX, 0, mMaxEidTextWidth, fm.height());
    rc.adjust(-mMaxEidTextWidth / 2, 0, -mMaxEidTextWidth / 2, 0);
    p.save();
    p.setBrush(QColor(128, 200, 84));
    p.drawRect(rc);
    p.drawText(rc, Qt::AlignCenter, QString::number(intValue));
    p.restore();
}

void Timeline::drawAxixBounding(QPainter &p)
{
    QFontMetrics fm(mFont);
    p.drawRect(QRect(0, 0, mAreaWidth, fm.height()));
}

void Timeline::drawToolbar(QPainter &p)
{
    QFontMetrics fm(mFont);
    QRectF eidRc(0, 0, PADDING_LEFT, fm.height());
    p.save();
    p.setBrush(Qt::white);
    p.drawRect(eidRc);
    p.drawText(eidRc, Qt::AlignCenter, "EID : ");
    p.restore();
}

qreal Timeline::GetPosByValue(const qreal value)
{
    // value = (pos.x - mPan) * mPerPixel;
    // value / mPerPixel = pos.x - mPan;
    return value / mPerPixel + mPan + PADDING_LEFT;
}

qreal Timeline::GetValueByPos(const qreal pos)
{
    return (pos - (mPan + PADDING_LEFT)) * mPerPixel;
}

int Timeline::GetIntValue(const qreal value)
{
    if (value - (int)value >= 0.5) {
        return (int)value + 1;
    } else {
        return (int)value;
    }
}
