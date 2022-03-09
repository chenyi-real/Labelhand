/*
 * author : fang
 * date   : 2022-3
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFileDialog>
#include <QDebug>
#define cout qDebug()
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pointMark = QVector<QPoint>(21);
    connect(ui->pictureLabel,&MyLabel::sendPoint,this,&MainWindow::receivePoint);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    openFileName = QFileDialog::getExistingDirectory(
                this, "choose src Directory",
                 "F:/hands");
    QDir dir(openFileName); //获取文件夹下所有文件
    //获取文件夹下所有文件(文件夹+文件)
    QFileInfoList list = dir.entryInfoList();
    QString filter;
    //必须保证图片和数据集名字相同
    for(int i = 0; i < list.size(); i++)
    {
        filter = list[i].suffix();  //后缀名
        if(filter != "jpg")
            continue;
        QStringList lt = list[i].fileName().split(".");
        allImageName.append(lt[0]);
    }
    if(allImageName.empty()) {
        return ;
    }
    pictureNum = 0;
    show_picture();     //加载图片
}

void MainWindow::show_picture()
{
    if(pictureNum < 0 || pictureNum >= allImageName.size())
        return ;
    updateNameLabel();
    image = QPixmap(QString("%1/%2.jpg").arg(openFileName).arg(allImageName[pictureNum]));
    image_size = image.size();  //获取图片尺寸
    ui->pictureLabel->mySetPixmap(image);     //显示图片
//    ui->pictureLabel->setPixmap(image);
//    ui->pictureLabel->setScaledContents(true);      //自适应

    readJsonFile();   //读json文件
    ui->pictureLabel->getPointMark(pointMark, image_size);
    updateListWidget();   //更新显示
}

void MainWindow::readJsonFile()
{
    //读json文件
    QFile file(QString("%1/%2.json").arg(openFileName).arg(allImageName[pictureNum]));
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    file.close();
    //使用json文件对象加载字符串
    QJsonDocument doc = QJsonDocument::fromJson(data);
    for(int i = 0; i < 21; ++i) {
        QPoint point;
        point.setX(doc["info"][0]["pts"][QString::number(i)]["x"].toInt());
        point.setY(doc["info"][0]["pts"][QString::number(i)]["y"].toInt());
        pointMark[i] = point;
    }
}

void MainWindow::writeJsonFile()
{
    QString path = QString("%1/%2.json").arg(openFileName).arg(allImageName[pictureNum]);
    //写json文件
    QFile file(path);
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray data = file.readAll();
    file.close();
    //使用json文件对象加载字符串
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    QJsonArray infoArr = obj.value("info").toArray();
    QJsonObject infoObj = infoArr[0].toObject();
    QJsonObject ptsObj = infoObj.value("pts").toObject();
    for(int i = 0; i < 21; ++i) {
        QJsonObject numObj = ptsObj.value(QString::number(i)).toObject();
        numObj["x"] = pointMark[i].rx();
        numObj["y"] = pointMark[i].ry();
        ptsObj[QString::number(i)] = numObj;
    }
    infoObj["pts"] = ptsObj;
    infoArr[0] = infoObj;
    obj["info"] = infoArr;
    doc.setObject(obj);

    file.setFileName(path);
    file.open(QFile::WriteOnly | QFile::Text);
    file.write(doc.toJson());
    file.close();
    ui->nameLabel->setText("保存成功！");
}

void MainWindow::updateListWidget()
{
    //更新ListWidget
    ui->listWidget->clear();
    for(int i = 0; i < 21; ++i) {
        ui->listWidget->addItem(QString("%1 : %2    %3").arg(QString::number(i + 1))
                                .arg(QString::number(pointMark[i].rx())).arg(QString::number(pointMark[i].ry())));
    }
//    drawPointOnPicture();
}

void MainWindow::drawPointOnPicture()
{
    QPixmap img(image);
    QPainter painter(&img);             // 创建QPainter一个对象
    QPen pen;

    pen.setColor(Qt::red);           // 设置画笔颜色
    painter.setPen(pen);                // 设置画笔
    painter.setBrush(QColor(255,0,0));  //设置画刷
    // 画多个点
    for (int i = 0; i < 21; ++i)
    {
        painter.drawEllipse(pointMark[i], 3, 3);
    }
    // 连线
    QPoint last;
    // 拇指
    for(int i = 0; i < 21; ++i) {
        if(i != 0) {
            QPoint point = pointMark[i];
            QLineF line(last, point);
            painter.drawLine(line);
            last = point;
        }
        if(i % 4 == 0) {
            if(i != 20) pen.setColor(lineColor[i / 4]);
            painter.setPen(pen);
            last = pointMark[0];
        }
    }
    ui->pictureLabel->setPixmap(img);
}

void MainWindow::on_nextButton_clicked()
{
    if(pictureNum >= allImageName.size() - 1)
        return;
    pictureNum++;
    show_picture();
}

void MainWindow::receivePoint(QVector<QPoint> p)
{
    pointMark = p;
    updateListWidget();
//    writeJsonFile();
}

void MainWindow::on_preButton_clicked()
{
    if(pictureNum <= 0)
        return;
    pictureNum--;
    show_picture();
}

void MainWindow::updateNameLabel()
{
    ui->nameLabel->setText(QString("%1 / %2 , %3").arg(QString::number(pictureNum + 1))
                           .arg(QString::number(allImageName.size())).arg(allImageName[pictureNum] + ".jpg"));
}

void MainWindow::on_saveButton_clicked()
{
    writeJsonFile();
}
