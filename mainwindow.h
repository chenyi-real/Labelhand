/*
 * author : fang
 * date   : 2022-3
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QPixmap>
#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QListWidget>
#include <QPainter>
#include <QPen>
#include <QRectF>
#include <QLineF>
#include <QColor>
#include <QVector2D>
#include <QVector>
#include <QPoint>
#include <QJsonArray>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void show_picture();
    void readJsonFile();
    void writeJsonFile();
    void updateListWidget();
    void drawPointOnPicture();
    void on_nextButton_clicked();
    void receivePoint(QVector<QPoint>);

    void on_preButton_clicked();
    void updateNameLabel();

    void on_saveButton_clicked();

private:
    Ui::MainWindow *ui;
    QString openFileName = nullptr;   //文件夹名称
    QStringList allImageName;
    int pictureNum = 0;        //当前操作图片
    QPixmap image;
    QSize image_size;
    QVector<QPoint> pointMark;
    QColor lineColor[5] = {Qt::yellow, Qt::white, Qt::blue, Qt::green, Qt::gray};
};
#endif // MAINWINDOW_H
