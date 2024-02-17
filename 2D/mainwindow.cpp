
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , img(600, 600, QImage::Format_RGB32)
    , img2(600, 600, QImage::Format_RGB32)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    img.load("C://Users/Konrad/Pictures/earth.jpg");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(0,0,img2);
}

void MainWindow::transformation() {
    transformationM = translationM * bendingM * scalingM * rotationM;
    std::cout << transformationM;
    auto color1 = [&](int a, int b)
    {
        arma::vec v{(float)a, (float)b, 1};
        arma::vec res = transformationM * v;
        int x = (int)res[0];
        int y = (int)res[1];
        if(x >= 0 && x < 600 && y >= 0 && y < 600) return img.pixelColor(x, y);
        else return img.pixelColor(0,0);
    };

    for(int i = 0; i < 600; i++){
        for(int j = 0; j < 600; j++) {
            img2.setPixelColor(i,j,color1(i,j));
        }
    }
    update();
}


void MainWindow::on_bendX_valueChanged(int value)
{
    BX = value / 100.0;
    bendingM(0,1) = BX;
    transformation();
}


void MainWindow::on_bendY_valueChanged(int value)
{
    BY = value / 100.0;
    bendingM(1,0) = BY;
    transformation();
}


void MainWindow::on_rotationSlider_valueChanged(int value)
{
    rotationM = arma::eye(3,3);

    R = value*M_PI/10;
    rotationM(0,0) = cos(R);
    rotationM(0,1) = sin(R);
    rotationM(1,0) = -sin(R);
    rotationM(1,1) = cos(R);

    //wykonanie dodatkowych translacji dla przesuniecia wzgledem srodka ekranu
    arma::mat Tminus = arma::eye(3,3);
    arma::mat Tplus = arma::eye(3,3);
    Tminus(0,2) = -600/2;
    Tminus(1,2) = -600/2;
    Tplus(0,2) = 600/2;
    Tplus(1,2) = 600/2;

    rotationM = Tplus * rotationM * Tminus;
    transformation();
}


void MainWindow::on_scalingX_valueChanged(int value)
{
    SX = value/50.0f;
    scalingM(0,0) = 1.0f/SX;
    transformation();
}


void MainWindow::on_skalingY_valueChanged(int value)
{
    SY = value/50.0f;
    scalingM(1,1) = 1.0f/SY;
    transformation();
}


void MainWindow::on_translationX_valueChanged(int value)
{
    TX = value;
    translationM(0,2) = -TX;
    transformation();
}


void MainWindow::on_translationY_valueChanged(int value)
{
    TY = value;
    translationM(1,2) = -TY;
    transformation();
}

