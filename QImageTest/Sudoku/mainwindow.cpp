#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    wb_ = false;
    timer_ = new QTimer(this);
    timer_->setInterval(ui->spinBox->value());
    timer_->connect(timer_, SIGNAL(timeout()), this, SLOT(on_pushButton_clicked()));
    on_pushButton_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QImage image(256, 256, QImage::Format_Indexed8);

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    if(wb_)
        for(int i = 0; i < 256; i++){
            int temp = qrand() % 256;
            image.setColor(i, qRgb(temp,temp,temp));
        }
    else
        for(int i = 0; i < 256; i++)
            image.setColor(i, qRgb(qrand() % 256,qrand() % 256,qrand() % 256));

    int count = 0;
    for(int i = 0; i < 255; i+=2){
        for(int j = 0; j < 255; j+=2){
            count = qrand() % 256;
            image.setPixel(i, j, count);
            image.setPixel(i, j+1, count);
            image.setPixel(i+1, j, count);
            image.setPixel(i+1, j+1, count);
        }
    }
    ui->label->setPixmap(QPixmap::fromImage(image));
    return;
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    wb_ = arg1 > 0 ? false : true;
}

void MainWindow::on_pushButton_2_clicked()
{

    timer_->start();
}

void MainWindow::on_pushButton_3_clicked()
{
    timer_->stop();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    timer_->setInterval(arg1);
}
