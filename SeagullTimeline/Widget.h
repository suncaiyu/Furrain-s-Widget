#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "Timeline.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    std::shared_ptr<Timeline> mTimeline;
};
#endif // WIDGET_H
