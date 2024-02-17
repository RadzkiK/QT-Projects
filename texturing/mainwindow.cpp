
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMouseEvent>
#include <qpainter.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    img(500, 500, QImage::Format_RGB32),
    img2(500, 500, QImage::Format_RGB32),
    img_tmp(500, 500, QImage::Format_RGB32),
    img2_tmp(500, 500, QImage::Format_RGB32),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    img.fill(0);
    img2.fill(0);
    img_tmp.fill(0);
    img2_tmp.fill(0);
    img.load("C://Users/Konrad/Pictures/earth.jpg");
    //img = img.scaled(500,500);
    img_tmp = img;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    p.drawImage(0,0,img);
    p.drawImage(510,0,img2);
    update();
}

QColor MainWindow::getCol(QPoint p)
{
    QColor actual;
    if(p.x() >= 0 && p.x() < 500 && p.y() >= 0 && p.y() < 500)
    {
        uchar *pix = img.scanLine(p.y());
        actual.setRed(pix[p.x()*4 + 2]);
        actual.setGreen(pix[p.x()*4 + 1]);
        actual.setBlue(pix[p.x()*4 + 0]);
    } else if( p.x() >= 510 && p.x() < 1010 && p.y() >= 0 && p.y() < 500)
    {
        uchar *pix = img2.scanLine(p.y());
        actual.setRed(pix[p.x()*4 + 2]);
        actual.setGreen(pix[p.x()*4 + 1]);
        actual.setBlue(pix[p.x()*4 + 0]);
    }
    return actual;
}

void MainWindow::putPixel(int x, int y)
{
    uchar *pix = img.scanLine(y);
    if(x >= 0 && x < 500 && y >= 0 && y < 500)
    {
        pix[4*x] = c.blue();
        pix[4*x+1] = c.green();
        pix[4*x+2] = c.red();
    }
    update();
}

void MainWindow::putPixel(int x, int y, QColor col)
{
    uchar *pix = img.scanLine(y);
    if(x >= 0 && x < 500 && y >= 0 && y < 500)
    {
        pix[4*x] = col.blue();
        pix[4*x+1] = col.green();
        pix[4*x+2] = col.red();
    }
    update();
}

void MainWindow::putPixelOnImg2(int x, int y)
{
    uchar *pix = img2.scanLine(y);
    if(x >= 0 && x < 500 && y >= 0 && y < 500)
    {
        pix[4*x] = c.blue();
        pix[4*x+1] = c.green();
        pix[4*x+2] = c.red();
    }
    update();
}

void MainWindow::putPixelOnImg2(int x, int y, QColor col)
{
    uchar *pix = img2.scanLine(y);
    if(x >= 0 && x < 500 && y >= 0 && y < 500)
    {
        pix[4*x] = col.blue();
        pix[4*x+1] = col.green();
        pix[4*x+2] = col.red();
    }
    update();
}

void MainWindow::drawLine(int x0, int y0, int x1, int y1)
{
    int deltax = abs(x1 - x0);
    int deltay = abs(y1 - y0);
    int err = 0;
    int y = y0;
    int x = x0;
    if(deltay < deltax)
    {
        while(x != x1)
        {
            putPixel(x, y);
            err = err + deltay;
            if (2 * err >= deltax)
            {
                if(y1 >= y0) y++;
                else if(y1 < y0) y--;
                err = err - deltax;
            }
            if(x1 >= x0) x++;
            else if(x1 < x0) x--;
        }
    }
    else
    {
        while(y != y1)
        {
            putPixel(x, y);
            err = err + deltax;
            if (2 * err >= deltay)
            {
                if(x1 >= x0) x++;
                else if(x1 < x0) x--;
                err = err - deltay;
            }
            if(y1 >= y0) y++;
            else if(y1 < y0) y--;
        }
    }
    update();
}

void MainWindow::drawLineOnImg2(int x0, int y0, int x1, int y1)
{
    int deltax = abs(x1 - x0);
    int deltay = abs(y1 - y0);
    int err = 0;
    int y = y0;
    int x = x0;
    if(deltay < deltax)
    {
        while(x != x1)
        {
            putPixelOnImg2(x, y);
            err = err + deltay;
            if (2 * err >= deltax)
            {
                if(y1 >= y0) y++;
                else if(y1 < y0) y--;
                err = err - deltax;
            }
            if(x1 >= x0) x++;
            else if(x1 < x0) x--;
        }
    }
    else
    {
        while(y != y1)
        {
            putPixelOnImg2(x, y);
            err = err + deltax;
            if (2 * err >= deltay)
            {
                if(x1 >= x0) x++;
                else if(x1 < x0) x--;
                err = err - deltay;
            }
            if(y1 >= y0) y++;
            else if(y1 < y0) y--;
        }
    }
    update();
}

void MainWindow::draw()
{
    if(points.size() > 0 && points.size() <= 3)
    {
        for(int i = 0; i < points.size(); i++)
        {
            drawLine(points[i].x(), points[i].y(), points[(i+1)%points.size()].x(), points[(i+1)%points.size()].y());
            drawPoint(QPoint(points[i].x(), points[i].y()), Qt::red);
        }
    }
    if(points2.size() > 0 && points2.size() <= 3)
    {
        for(int i = 0; i < points2.size(); i++)
        {
            drawLineOnImg2(points2[i].x(), points2[i].y(), points2[(i+1)%points2.size()].x(), points2[(i+1)%points2.size()].y());
            drawPointOnImg2(QPoint(points2[i].x(), points2[i].y()), Qt::red);
        }
    }
    if(points.size() == 3 && points2.size() == 3)
    {
        texture();
    }
    update();
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
//    if(position.x() >= 0 && position.x() < 500 && position.y() >= 0 && position.x() < 500)
//    {
//        img_tmp = img;
//    } else if(position.x() >= 510 && position.x() < 1010 && position.y() >= 0 && position.x() < 500)
//    {
//        img2_tmp = img2;
//    }
    img_tmp = img.copy();
    QPoint position = e -> pos();
    x0 = position.x();
    y0 = position.y();
    img = img_tmp.copy();
    if(e->button() == Qt::RightButton)
    {
        deletePoint(position);
        img.load("C://Users/Konrad/Pictures/earth.jpg");
    }
    else if(e->button() == Qt::MiddleButton)
    {
        if(position.x() >= 0 && position.x() < 500 && position.y() >= 0 && position.y() < 500)
        {
            if(!points.empty())
            {
                for(int it = 0; it <= points.size()-1; ++it)
                {
                    if(sqrt(pow(x0 - points[it].x(), 2) + pow(y0 - points[it].y(), 2)) <= 2)
                    {
                        iterat = it;
                        copy = points;
                    }
                }
            }
        } else if(position.x() >= 510 && position.x() < 1010 && position.y() >= 0 && position.y() < 500)
        {
            if(!points2.empty())
            {
                for(int it = 0; it <= points2.size()-1; ++it)
                {
                    if(sqrt(pow(position.x() - (points2[it].x()+510), 2) + pow(y0 - points2[it].y(), 2)) <= 2)
                    {
                        iterat = it;
                        copy = points2;
                    }
                }
            }
        }
    } else if(e->button() == Qt::LeftButton)
    {
        addPoint(position);
    }
    draw();
    update();
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    QPoint position = e -> pos();
//    if(position.x() >= 0 && position.x() < 500 && position.y() >= 0 && position.x() < 500)
//    {
//        img = img_tmp;
//    } else if(position.x() >= 510 && position.x() < 1010 && position.y() >= 0 && position.x() < 500)
//    {
//        img2 = img2_tmp;
//    }
    img = img_tmp.copy();
    if(e->button() == Qt::MiddleButton)
    {
        if(position.x() >= 0 && position.x() < 500 && position.y() >= 0 && position.x() < 500)
        {
            if(!points.empty() && iterat != -1)
            {
                //img.load("C://Users/Konrad/Pictures/earth.jpg");
                int it = iterat;
                QPoint p;
                //drawPoint(copy[it], getCol(copy[it]));
                p.setX(position.x());
                p.setY(position.y());
                points[iterat].setX(p.x());
                points[iterat].setY(p.y());
                //drawPoint(p, Qt::red);
            }
        } else if(position.x() >= 510 && position.x() < 1010 && position.y() >= 0 && position.y() < 500)
        {
            if(!points2.empty() && iterat != -1)
            {
                int it = iterat;
                QPoint p;
                //drawPoint(copy[it], Qt::black);
                p.setX(position.x());
                p.setY(position.y());
                points2[iterat].setX(p.x());
                points2[iterat].setY(p.y());
                //drawPoint(p, Qt::red);
            }
        }
    }
    draw();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    QPoint position = e -> pos();
//    if(position.x() >= 0 && position.x() < 500 && position.y() >= 0 && position.x() < 500)
//    {
//        img_tmp = img;
//    } else if(position.x() >= 510 && position.x() < 1010 && position.y() >= 0 && position.x() < 500)
//    {
//        img2_tmp = img2;
//    }
    if(e->button() == Qt::MiddleButton)
    {
        img.load("C://Users/Konrad/Pictures/earth.jpg");
        if(position.x() >= 0 && position.x() < 500 && position.y() >= 0 && position.x() < 500)
        {
            if(!points.empty() && iterat != -1)
            {
                int it = iterat;
                QPoint p;
                //drawPoint(copy[it], getCol(copy[it]));
                p.setX(position.x());
                p.setY(position.y());
                points[iterat].setX(p.x());
                points[iterat].setY(p.y());
                //drawPoint(p, Qt::red);
            }
        } else if(position.x() >= 510 && position.x() < 1010 && position.y() >= 0 && position.y() < 500)
        {
            if(!points2.empty() && iterat != -1)
            {
                int it = iterat;
                QPoint p;
                //drawPoint(copy[it], Qt::black);
                p.setX(position.x());
                p.setY(position.y());
                points2[iterat].setX(p.x()-510);
                points2[iterat].setY(p.y());
                //drawPoint(p, Qt::red);
            }
        }
    }
    //iterat = -1;
    draw();
}

void MainWindow::drawPoint(QPoint p, QColor col)
{
    if(p.x() >= 0 && p.x() < 500 && p.y() >= 0 && p.y() < 500)
    {
        int x = p.x();
        int y = p.y();

        putPixel(x-1, y+1, col);
        putPixel(x-1, y+1, col);
        putPixel(x, y+1, col);
        putPixel(x+1, y+1, col);
        putPixel(x, y, col);
        putPixel(x+1, y, col);
        putPixel(x-1, y, col);
        putPixel(x-1, y-1, col);
        putPixel(x, y-1, col);
        putPixel(x+1, y-1, col);
    }

    update();
}

void MainWindow::drawPointOnImg2(QPoint p, QColor col)
{
    int x = p.x();
    int y = p.y();

    putPixelOnImg2(x-1, y+1, col);
    putPixelOnImg2(x-1, y+1, col);
    putPixelOnImg2(x, y+1, col);
    putPixelOnImg2(x+1, y+1, col);
    putPixelOnImg2(x, y, col);
    putPixelOnImg2(x+1, y, col);
    putPixelOnImg2(x-1, y, col);
    putPixelOnImg2(x-1, y-1, col);
    putPixelOnImg2(x, y-1, col);
    putPixelOnImg2(x+1, y-1, col);

    update();
}


void MainWindow::addPoint(QPoint p)
{
    if(p.x() >= 0 && p.x() < 500 && p.y() >= 0 && p.y() < 500)
    {
        if(points.size() < 3)
        {
            points.push_back(p);
            //drawPoint(p, Qt::red);
        }
    } else if( p.x() >= 510 && p.x() < 1010 && p.y() >= 0 && p.y() < 500)
    {
        if(points2.size() < 3)
        {
            QPoint pt = QPoint(p.x()-510, p.y());
            points2.push_back(pt);
            //drawPoint(p, Qt::red);
        }
    }
}

void MainWindow::deletePoint(QPoint p)
{
    if(p.x() >= 0 && p.x() < 500 && p.y() >= 0 && p.y() < 500)
    {
        if(!points.empty())
        {
            for(QVector<QPoint>::iterator it = points.begin(); it <= points.end(); ++it)
            {
                if(sqrt(pow(p.x() - it->x(), 2) + pow(p.y() - it->y(), 2)) <= 2)
                {
                    //drawPoint(*it, getCol(*it));
                    points.erase(it);
                }
            }
        }
    } else if( p.x() >= 510 && p.x() < 1010 && p.y() >= 0 && p.y() < 500)
    {
        if(!points2.empty())
        {
            for(QVector<QPoint>::iterator it = points2.begin(); it <= points2.end(); ++it)
            {
                if(sqrt(pow(p.x() - (it->x()+510), 2) + pow(p.y() - it->y(), 2)) <= 2)
                {
                    //drawPoint(*it, getCol(*it));
                    points2.erase(it);
                }
            }
        }
    }
}

QColor MainWindow::interpolate(float x, float y)
{
    QColor a,b,c,d,e,f,out;
    int x_int,y_int;
    float x_float,y_float;
    x_int = floor(x);
    y_int = floor(y);
    x_float = x - x_int;
    y_float = y - y_int;

    a = img.pixelColor(x_int,y_int);
    b = img.pixelColor(x_int+1,y_int);
    c = img.pixelColor(x_int,y_int+1);
    d = img.pixelColor(x_int+1,y_int+1);

    e.setRed((1.-x_float)*a.red() + x_float*b.red());
    e.setGreen((1.-x_float)*a.green() + x_float*b.green());
    e.setBlue((1.-x_float)*a.blue() + x_float*b.blue());

    f.setRed(c.red()+x_float*(d.red()-c.red()));
    f.setGreen(c.green()+x_float*(d.green()-c.green()));
    f.setBlue(c.blue()+x_float*(d.blue()-c.blue()));

    out.setRed(e.red()+y_float*(f.red()-e.red()));
    out.setBlue(e.blue()+y_float*(f.blue()-e.blue()));
    out.setGreen(e.green()+y_float*(f.green()-e.green()));
    return out;
}

void MainWindow::texture()
{
    img2.fill(0);

    int x_a = points2[0].x();
    int x_b = points2[1].x();
    int x_c = points2[2].x();

    int y_a = points2[0].y();
    int y_b = points2[1].y();
    int y_c = points2[2].y();

    int W = (x_b-x_a)*(y_c-y_a) - (y_b-y_a)*(x_c-x_a);
    int Wv, Ww;
    float u,v,w;

    for(int y = 0; y < 500; ++y)
    {
        for(int x = 0; x < 500; ++x)
        {
            Wv = (x-x_a)*(y_c-y_a) - (y-y_a)*(x_c-x_a);
            Ww = (x_b-x_a)*(y-y_a) - (y_b-y_a)*(x-x_a);

            v = Wv/(float)W;
            w = Ww/(float)W;
            u = 1-v-w;

            float x_t;
            float y_t;

            QColor out = QColor(Qt::black);

            //if(u+v+w == 1.)
            if(u > 0 && u < 1 && v > 0 && v < 1 && w > 0 && w < 1)
            {
                x_t = u * (float)points[0].x() + v * (float)points[1].x() + w * (float)points[2].x();
                y_t = u * (float)points[0].y() + v * (float)points[1].y() + w * (float)points[2].y();

                out = interpolate(x_t,y_t);
                img2.setPixelColor(x,y,out);
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


