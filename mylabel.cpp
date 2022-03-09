/*
 * author : fang
 * date   : 2022-3
 */
#include "mylabel.h"
#include <QDebug>
#define cout qDebug()
#include <QMouseEvent>

MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{
    pointMark = QVector<QPoint>(21);
    qPointMark = QVector<QPoint>(21);
}

void MyLabel::enterEvent(QEvent *)
{

}
//鼠标按下事件
void MyLabel::mousePressEvent(QMouseEvent *ev)
{
    //鼠标左键按下  打印信息
    if(ev->button() == Qt::LeftButton)
    {
        setMouseTracking(true);
        QPoint point(ev->x(), ev->y());
        dotNum = getMouseMatchPoint(point);
        if(dotNum != -1) {
            mouseMoving = true;
            cout << QString("当前选中了点%1").arg(QString::number(dotNum));
        }
    }
}
//鼠标释放事件
void MyLabel::mouseReleaseEvent(QMouseEvent *ev)
{

    if(ev->button() == Qt::LeftButton)
    {
        setMouseTracking(false);
        mouseMoving = false;
        pointMark[dotNum].setX(qPointMark[dotNum].rx() * imageSize.rwidth() / this->size().rwidth());
        pointMark[dotNum].setY(qPointMark[dotNum].ry() * imageSize.rheight() / this->size().rheight());
        emit sendPoint(pointMark);
    }

}
//鼠标移动事件
void MyLabel::mouseMoveEvent(QMouseEvent *ev)
{
    if(ev->buttons() == Qt::LeftButton && dotNum != -1)
    {
        qPointMark[dotNum].setX(ev->x() + dotDiff.rx());
        qPointMark[dotNum].setY(ev->y() + dotDiff.ry());
        update();
    }
}

void MyLabel::paintEvent(QPaintEvent *ev)
{
    if(image.isNull())    return;
    if(mouseMoving == false)    setPointMark();//点在移动的时候不更新坐标
    QLabel::paintEvent(ev);
    QPainter painter(this);             // 创建QPainter一个对象
    drawLine(painter);          //画线
}



void MyLabel::mySetPixmap(QPixmap & img)
{
    image = img;
    this->setPixmap(img);
    this->setScaledContents(true);
}

void MyLabel::getPointMark(QVector<QPoint> p, QSize imgSize)
{
    pointMark = p;
    imageSize = imgSize;
    setPointMark();
}

void MyLabel::setPointMark()
{
    for(int i = 0; i < 21; ++i) {
        qPointMark[i].setX(pointMark[i].rx() * this->size().rwidth() / imageSize.rwidth());
        qPointMark[i].setY(pointMark[i].ry() * this->size().rheight() / imageSize.height());
    }
}

int MyLabel::getMouseMatchPoint(QPoint &point)
{
    for(int i = 0; i < 21; ++i) {
        if((qPointMark[i].rx() + dotRadius >= point.rx() && qPointMark[i].rx() - dotRadius <= point.rx())
                &&(qPointMark[i].ry() + dotRadius >= point.ry() && qPointMark[i].ry() - dotRadius <= point.ry())){
            dotDiff.setX(point.x() - qPointMark[i].x());
            dotDiff.setY(point.y() - qPointMark[i].y());
            return i;
        }
    }
    return -1;
}

void MyLabel::drawLine(QPainter &painter)
{
    QPen pen;
    pen.setColor(Qt::red);           // 设置画笔颜色
    painter.setPen(pen);                // 设置画笔
    painter.setBrush(QColor(255,0,0));  //设置画刷
    // 画多个点
    for (int i = 0; i < 21; ++i)
    {
        painter.drawEllipse(qPointMark[i], dotRadius, dotRadius);
    }
    // 连线
    QPoint last;
    // 拇指
    for(int i = 0; i < 21; ++i) {
        if(i != 0) {
            QPoint point = qPointMark[i];
            QLineF line(last, point);
            painter.drawLine(line);
            last = point;
        }
        if(i % 4 == 0) {
            if(i != 20) pen.setColor(lineColor[i / 4]);
            painter.setPen(pen);
            last = qPointMark[0];
        }
    }
}


//事件分发器 :用途，分发事件
bool MyLabel::event(QEvent *e)
{
    //其他事件  抛给父类去处理
    return QLabel::event(e);
}

