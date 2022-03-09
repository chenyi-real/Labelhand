/*
 * author : fang
 * date   : 2022-3
 */
#ifndef MYLABEL_H
#define MYLABEL_H
#include <QLabel>
#include <QPixmap>
#include "mainwindow.h"


class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = 0);
    void enterEvent(QEvent *) override;
    //鼠标按下事件
    void mousePressEvent(QMouseEvent *ev) override;
    //鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *ev) override;
    //鼠标移动事件
    void mouseMoveEvent(QMouseEvent *ev) override;
    //重写绘制事件
    virtual void paintEvent(QPaintEvent *event) override;

    void mySetPixmap(QPixmap &);
    void getPointMark(QVector<QPoint> p, QSize imgSize);
    void setPointMark();
    int getMouseMatchPoint(QPoint& point);
    //画线
    void drawLine(QPainter& painter);
    //事件分发器
    bool event(QEvent *e) override;
private:
    QPixmap image;
    QSize imageSize;
    QVector<QPoint> qPointMark; //转换后pointmark
    QVector<QPoint> pointMark;
    QColor lineColor[5] = {Qt::yellow, Qt::white, Qt::blue, Qt::green, Qt::gray};
    int dotRadius = 5;
    QPoint dotDiff;
    int dotNum;
    bool mouseMoving = false;
signals:
    void sendPoint(QVector<QPoint>);
public slots:
};

#endif // MYLABEL_H
