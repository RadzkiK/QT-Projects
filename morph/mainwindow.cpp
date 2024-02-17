#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qfiledialog.h>
#include <qpainter.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , img(500, 500, QImage::Format_RGB32)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    threshold = 127;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(0,0,img);
}


bool MainWindow::inImage(int x, int y)
{
    if (y < 0 || y > img.height() - 1 || x < 0 || x > img.width() - 1) return false;
    else return true;
}

void MainWindow::drawPixel(int x, int y, int r, int g, int b)
{
    if (y < 0 || y > img.height() - 1 || x < 0 || x > img.width() - 1) return;
    uchar *pix = img.scanLine(y);
    pix[x*4 + 0] = b;
    pix[x*4 + 1] = g;
    pix[x*4 + 2] = r;
    update();
}

void MainWindow::drawPixel(QImage im, int x, int y, int r, int g, int b)
{
    if (y < 0 || y > im.height() - 1 || x < 0 || x > im.width() - 1) return;
    uchar *pix = im.scanLine(y);
    pix[x*4 + 0] = b;
    pix[x*4 + 1] = g;
    pix[x*4 + 2] = r;
    update();
}

QColor MainWindow::getCol(int x, int y)
{
    if (y < 0 || y > img.height() - 1 || x < 0 || x > img.width() - 1) return Qt::white; // w razie wyjscia poza obraz zwroc bialy kolor(kolor tla)
    uchar *pix = img.scanLine(y);
    QColor actual;
    actual.setRed(pix[x*4 + 2]);
    actual.setGreen(pix[x*4 + 1]);
    actual.setBlue(pix[x*4 + 0]);
    return actual;
}

void MainWindow::toMonochrome()
{
    int y;
    for(int j = 0; j < img.height(); j++)
    {
        for(int i = 0; i < img.width(); i++)
        {
            y = (getCol(i, j).blue() + getCol(i, j).green() + getCol(i, j).red()) / 3;
            drawPixel(i, j, y, y, y);
        }
    }
    update();
}

void MainWindow::toBinary()
{
    for(int j = 0; j < img.height(); j++)
    {
        for(int i = 0; i < img.width(); i++)
        {
            if(getCol(i, j).blue() < getThreshold()) drawPixel(i, j, 0, 0 ,0);
            else drawPixel(i, j, 255, 255, 255);
        }
    }
    update();
}

void MainWindow::negative()
{
    for(int j = 0; j < img.height(); j++)
    {
        for(int i = 0; i < img.width(); i++)
        {
            if(getCol(i, j).blue() == 255) drawPixel(i, j, 0, 0 ,0);
            else drawPixel(i, j, 255, 255, 255);
        }
    }
    update();
}

void MainWindow::dilation()
{
    toMonochrome();
    toBinary();
    update();
    QImage tmp = QImage(500, 500, QImage::Format_RGB32);
    QColor bg = QColor(Qt::white).rgb(); // ustalamy ze kolorem tla jest kolor bialy, zatem piksele 'zapalone' beda mialy kolor czarny
    tmp.fill(bg);

    for(int j = 0; j < img.height(); j++)
    {
        for(int i = 0; i < img.width(); i++)
        {
                if((inImage(i + 1, j) && getCol(i + 1, j) == QColor(Qt::black)) ||
                   (inImage(i - 1, j) && getCol(i - 1, j) == QColor(Qt::black)) ||
                   (inImage(i + 1, j + 1) && getCol(i + 1, j + 1) == QColor(Qt::black)) ||
                   (inImage(i - 1, j - 1) && getCol(i - 1, j - 1) == QColor(Qt::black)) ||
                   (inImage(i + 1, j - 1) && getCol(i + 1, j - 1) == QColor(Qt::black)) ||
                   (inImage(i - 1, j + 1) && getCol(i - 1, j + 1) == QColor(Qt::black)) ||
                   (inImage(i, j + 1) && getCol(i, j + 1) == QColor(Qt::black)) ||
                   (inImage(i, j - 1) && getCol(i, j - 1) == QColor(Qt::black)) ||
                        (inImage(i, j) && getCol(i, j) == QColor(Qt::black)))
                    {
                        //drawPixel(tmp, i, j, 255, 255, 255);
                        tmp.setPixel(i, j, Qt::black);
                    }
        }
    }
    img = tmp.copy();
    update();
}

void MainWindow::erode()
{
    toMonochrome();
    toBinary();
    negative();
    dilation();
    negative();
    update();
}

void MainWindow::close()
{
    dilation();
    erode();
    update();
}

void MainWindow::open()
{
    erode();
    dilation();
    update();
}


void MainWindow::loadPicture()
{
    QString path = QFileDialog::getOpenFileName(this,"Wczytaj obrazek","C://");
    if(img.load(path) == false) { qDebug("Nie udalo sie wczytac obrazka"); exit(0); }
}



void MainWindow::on_loadButton_clicked()
{
    loadPicture();
    update();
}


void MainWindow::on_dilationButton_clicked()
{
    dilation();
}


void MainWindow::on_erodeButton_clicked()
{
    erode();
}


void MainWindow::on_closeButton_clicked()
{
    close();
}


void MainWindow::on_openButton_clicked()
{
    open();
}


void MainWindow::on_thresholdSlider_valueChanged(int value)
{
    setThreshold(value);
    update();
}

void MainWindow::setThreshold(int i)
{
    if(i >=0 && i <= 255) threshold = i;
    update();
}

int MainWindow::getThreshold()
{
    return threshold;
}


void MainWindow::on_monButton_clicked()
{
    toMonochrome();
}


void MainWindow::on_biButton_clicked()
{
    toBinary();
}

