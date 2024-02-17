#include "ekran.h"

#include <QMouseEvent>
#include <QPainter>
#include <QStack>
#include <cmath>


Point::Point(int x0, int y0)
{
    this->x = x0;
    this->y = y0;
}

Point::Point()
{}

void Point::setX(int v)
{
    this->x = v;
}

void Point::setY(int v)
{
    this->y = v;
}

int Point::getX()
{
    return this->x;
}

int Point::getY()
{
    return this->y;
}

bool Point::operator ==(const Point &p)
{
    if((this->x == p.x ) && (this->y == p.y))
             return true;
        else
             return false;
}

Ekran::Ekran(QWidget *parent)
    : QWidget{parent},
      im(800, 600, QImage::Format_RGB32),
      imTmp(800, 600, QImage::Format_RGB32)
{
    im.fill(Qt::black);
    imTmp = im;
    numOfVertices = 3;
    red = 255;
    green = 255;
    blue = 255;
    iterat = -1;
    mode = 1;
    currentTool = 0;

}

void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(0, 0, width(), height(), Qt::green);
    p.drawImage(0, 0, im);
}

Point Ekran::countBezier(Point p1, Point p2, Point p3, Point p4, float t)
{
    Point p;
    float x, y;

    x = pow(1 - t, 3) * p1.getX() + 3 * pow(1 - t, 2) * t * p2.getX() + 3 * (1 - t) * pow(t, 2) * p3.getX() + pow(t, 3) * p4.getX();
    y = pow(1 - t, 3) * p1.getY() + 3 * pow(1 - t, 2) * t * p2.getY() + 3 * (1 - t) * pow(t, 2) * p3.getY() + pow(t, 3) * p4.getY();

    p.setX(floor(x + 0.5));
    p.setY(floor(y + 0.5));

    return p;
}

Point Ekran::countBSpline(Point p1, Point p2, Point p3, Point p4, float t)
{
    Point p;
    float x, y;

    x = 1.0 / 6.0 * ((-t*t*t + 3*t*t - 3*t + 1) * p1.getX() + (3*t*t*t - 6*t*t + 4) * p2.getX() + (-3*t*t*t + 3*t*t +3*t +1) * p3.getX() + t*t*t * p4.getX());
    y = 1.0 / 6.0 * ((-t*t*t + 3*t*t - 3*t + 1) * p1.getY() + (3*t*t*t - 6*t*t + 4) * p2.getY() + (-3*t*t*t + 3*t*t +3*t +1) * p3.getY() + t*t*t * p4.getY());

    p.setX(floor(x + 0.5));
    p.setY(floor(y + 0.5));

    return p;
}

void Ekran::drawPixel(int x, int y, int r, int g, int b)
{
    if (y < 0 || y > im.height() - 1 || x < 0 || x > im.width() - 1) return;
    uchar *pix = im.scanLine(y);
    pix[x*4 + 0] = b;
    pix[x*4 + 1] = g;
    pix[x*4 + 2] = r;
    update();
}

QColor Ekran::getCol(int x, int y)
{
    if (y < 0 || y > im.height() - 1 || x < 0 || x > im.width() - 1) return -1;
    uchar *pix = im.scanLine(y);
    QColor actual;
    actual.setRed(pix[x*4 + 2]);
    actual.setGreen(pix[x*4 + 1]);
    actual.setBlue(pix[x*4 + 0]);
    return actual;
}

void Ekran::drawPoint(Point p)
{
    int x = p.getX();
    int y = p.getY();

    drawPixel(x-1, y+1, red, green, blue);
    drawPixel(x, y+1, red, green, blue);
    drawPixel(x+1, y+1, red, green, blue);
    drawPixel(x, y, red, green, blue);
    drawPixel(x+1, y, red, green, blue);
    drawPixel(x-1, y, red, green, blue);
    drawPixel(x-1, y-1, red, green, blue);
    drawPixel(x, y-1, red, green, blue);
    drawPixel(x+1, y-1, red, green, blue);

    update();
}

void Ekran::drawPoint(Point p, QColor col)
{
    int x = p.getX();
    int y = p.getY();

    drawPixel(x-1, y+1, col.red(), col.green(), col.blue());
    drawPixel(x, y+1, col.red(), col.green(), col.blue());
    drawPixel(x+1, y+1, col.red(), col.green(), col.blue());
    drawPixel(x, y, col.red(), col.green(), col.blue());
    drawPixel(x+1, y, col.red(), col.green(), col.blue());
    drawPixel(x-1, y, col.red(), col.green(), col.blue());
    drawPixel(x-1, y-1, col.red(), col.green(), col.blue());
    drawPixel(x, y-1, col.red(), col.green(), col.blue());
    drawPixel(x+1, y-1, col.red(), col.green(), col.blue());

    update();
}

void Ekran::drawLine(int x0, int y0, int x1, int y1)
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
            drawPixel(x, y, red, green, blue);
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
            drawPixel(x, y, red, green, blue);
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

void Ekran::drawLine(int x0, int y0, int x1, int y1, QColor col)
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
            drawPixel(x, y, col.red(), col.green(), col.blue());
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
            drawPixel(x, y, col.red(), col.green(), col.blue());
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

void Ekran::drawCircle(int x0, int y0, int x1, int y1)
{
    double radius = sqrt(((x1 - x0)*(x1 - x0)) + ((y1 - y0)*(y1 - y0)));

    int x, y, d;
    x = 0;
    y = radius;
    d = 1 - radius;

    drawPixel(x + x0, y + y0, red, green, blue);
    drawPixel(x + x0, -y + y0, red, green, blue);
    drawPixel(-x + x0, y + y0, red, green, blue);
    drawPixel(-x + x0, -y + y0, red, green, blue);
    drawPixel(y + x0, x + y0, red, green, blue);
    drawPixel(y + x0, -x + y0, red, green, blue);
    drawPixel(-y + x0, x + y0, red, green, blue);
    drawPixel(-y + x0, -x + y0, red, green, blue);

    while(y > x)
    {
        if(d < 0)       // wybieramy punkt E
        {
            d += 2 * x + 3;
            x++;
        }
        else     // wybieramy punkt SE
        {
            d += 2 * (x - y) + 5;
            x++;
            y--;
        }
        drawPixel(x + x0, y + y0, red, green, blue);
        drawPixel(x + x0, -y + y0, red, green, blue);
        drawPixel(-x + x0, y + y0, red, green, blue);
        drawPixel(-x + x0, -y + y0, red, green, blue);
        drawPixel(y + x0, x + y0, red, green, blue);
        drawPixel(y + x0, -x + y0, red, green, blue);
        drawPixel(-y + x0, x + y0, red, green, blue);
        drawPixel(-y + x0, -x + y0, red, green, blue);
    }
    update();
}

void Ekran::drawEllipse(int x0, int y0, int x1, int y1, int v)
{
   int x, y, xOld = 0, yOld = 0;
   int a = abs(x1 - x0);
   int b2 = abs(y1 - y0);
   double alpha = 0;

   for(int i = 0; i <= v; i++)
   {
       alpha = (i * 2 * M_PI) / v;
       x = a * cos(alpha) + 0.5;
       y = b2 * sin(alpha) + 0.5;

       if(i != 0) drawLine(x0 + x, y0 + y, x0 + xOld, y0 + yOld);

       xOld = x;
       yOld = y;
   }
   update();
}

void Ekran::drawBezier(QVector<Point> vec, QColor col)
{
    float t = 0;
    Point P, Q;
    if(vec.size() >= 4)
    {
        for(int i = 0; i <= vec.size()-1; i += 3)
        {
            if(i+3 <= vec.size()-1)
            {
                for(int j = 0; j < 50; j++)
                {
                    t = (float) j / 50.0;
                    P = countBezier(vec[i], vec[i+1], vec[i+2], vec[i+3], t);
                    Q = countBezier(vec[i], vec[i+1], vec[i+2], vec[i+3], t + 1.0 / 50.0);

                    drawLine(P.getX(), P.getY(), Q.getX(), Q.getY(), col);
                }
            }
        }
    }
    update();
}

void Ekran::drawBSpline(QVector<Point> vec, QColor col)
{
    float t = 0;
    Point P, Q;
    if(vec.size() >= 4)
    {
        for(int i = 0; i <= vec.size()-1; i++)
        {
            if(i+3 <= vec.size()-1)
            {
                for(int j = 0; j < 50; j++)
                {
                    t = (float) j / 50.0;
                    P = countBSpline(vec[i], vec[i+1], vec[i+2], vec[i+3], t);
                    Q = countBSpline(vec[i], vec[i+1], vec[i+2], vec[i+3], t + 1.0 / 50.0);

                    drawLine(P.getX(), P.getY(), Q.getX(), Q.getY(), col);
                }
            }
        }
    }
    update();
}

void Ekran::drawPolygonScanLine(QVector<Point> vec)
{
    int y, ymin = 0 , ymax = im.height()-1;
    Point p1, p2;
    QVector<int> x;

    for(int i = 0; i <= vec.size()-1; ++i)
    {
        if(vec[i].getY() < ymin) ymin = vec[i].getY();
        if(vec[i].getY() > ymax) ymax = vec[i].getY();
    }

    for(y = ymin; y < ymax; ++y)
    {
        x.clear();
        for(int i = 0; i < vec.size(); ++i)
        {
            p1 = vec[i];
            if(i+1 < vec.size()) p2 = vec[i+1];
            else p2 = vec[0];
            if(isInterception(p1, p2, y)) x.push_back(pointOfInterception(p1,p2,y));
        }
        std::sort(x.begin(), x.end());
        for(int j = 0; j <= x.size()-1; ++j)
        {
            if(j%2 == 0 && j+1 < x.size()) drawLine(x[j],y,x[j+1],y);
        }
    }

}

void Ekran::floodFill(int x, int y, QColor oldC, QColor newC)
{
    if(oldC != newC)
    {
        QStack<Point> stack;
        stack.push(Point(x, y));

        while(!stack.empty())
        {
            Point p = stack.pop();
            if(getCol(p.getX(), p.getY()) == oldC)
            {
                int w = p.getX();
                int e = p.getX();
                while(getCol(w, p.getY()) == oldC && w >= 0) w--;
                while(getCol(e, p.getY()) == oldC && e+1 < im.width()) e++;
                for(int i = w+1; i < e; i++) drawPixel(i, p.getY(), newC.red(), newC.green(), newC.blue());
                for(int i = w+1; i < e; i++)
                {
                    if(getCol(i, p.getY()+1) == oldC && p.getY()+1 < im.height()) stack.push(Point(i, p.getY()+1));
                    if(getCol(i, p.getY()-1) == oldC && p.getY()-1 >= 0) stack.push(Point(i, p.getY()-1));
                }
            }
        }
    }
    else return;

    update();
}

void Ekran::addPoint(QVector<Point> *vec, Point p)
{
    drawPoint(p);
    vec->push_back(p);
    update();
}

void Ekran::deletePoint(QVector<Point> *vec, Point p)
{
    if(!vec->empty())
    {
        for(QVector<Point>::iterator it = vec->begin(); it <= vec->end(); ++it)
        {
            if(sqrt(pow(p.getX() - it->getX(), 2) + pow(p.getY() - it->getY(), 2)) <= 2)
            {
                drawPoint(*it, getCol(it->getX(), it->getY()+2));
                vec->erase(it);
            }
        }
    }
    update();
}

void Ekran::mousePressEvent(QMouseEvent *e)
{
    imTmp = im;
    QPoint position = e -> pos();
    x0 = position.x();
    y0 = position.y();
    switch (currentTool)
    {
    case 0:
        if(!(position.y() < 0 || position.y() > im.height() - 1 || position.x() < 0 || position.x() > im.width() - 1))
        {
            drawLine(x0, y0, position.x(), position.y());
        }
        break;
    case 1:
        if(!(position.y() < 0 || position.y() > im.height() - 1 || position.x() < 0 || position.x() > im.width() - 1))
        {
            drawCircle(x0, y0, position.x(), position.y());
        }
        break;
    case 2:
        if(!(position.y() < 0 || position.y() > im.height() - 1 || position.x() < 0 || position.x() > im.width() - 1))
        {
            drawEllipse(x0, y0, position.x(), position.y(), numOfVertices);
        }
        break;
    case 3:
        if(!(position.y() < 0 || position.y() > im.height() - 1 || position.x() < 0 || position.x() > im.width() - 1))
        {
            if(mode == 1)
            {
                if(e -> buttons() & Qt::LeftButton)
                {
                    Point p;
                    p.setX(position.x());
                    p.setY(position.y());
                    drawPoint(p);
                }
            }
            if(mode == 3)
            {
                if(e -> buttons() & Qt::LeftButton)
                {
                    Point p;
                    p.setX(position.x());
                    p.setY(position.y());
                    drawBezier(points, getCol(p.getX(), p.getY() + 3));
                    deletePoint(&points, p);
                    drawBezier(points, QColor(red, green, blue, 255));
                }
            }
            if(mode == 2)
            {
                if(!points.empty())
                {
                    for(int it = 0; it <= points.size()-1; ++it)
                    {
                        if(sqrt(pow(x0 - points[it].getX(), 2) + pow(y0 - points[it].getY(), 2)) <= 2)
                        {
                            iterat = it;
                            copy = points;
                        }
                    }
                }
            }
        }
        break;
    case 4:
        if(!(position.y() < 0 || position.y() > im.height() - 1 || position.x() < 0 || position.x() > im.width() - 1))
        {
            if(mode == 1)
            {
                if(e -> buttons() & Qt::LeftButton)
                {
                    Point p;
                    p.setX(position.x());
                    p.setY(position.y());
                    drawPoint(p);
                }
            }
            if(mode == 3)
            {
                if(e -> buttons() & Qt::LeftButton)
                {
                    Point p;
                    p.setX(position.x());
                    p.setY(position.y());
                    drawBSpline(points, getCol(p.getX(), p.getY() + 3));
                    deletePoint(&points, p);
                    drawBSpline(points, QColor(red, green, blue, 255));
                }
            }
            if(mode == 2)
            {
                if(!points.empty())
                {
                    for(int it = 0; it <= points.size()-1; ++it)
                    {
                        if(sqrt(pow(x0 - points[it].getX(), 2) + pow(y0 - points[it].getY(), 2)) <= 2)
                        {
                            iterat = it;
                            copy = points;
                        }
                    }
                }
            }
        }
        break;
    case 5:
        break;
    case 6:
        if(!(position.y() < 0 || position.y() > im.height() - 1 || position.x() < 0 || position.x() > im.width() - 1) && (e->button() == Qt::LeftButton))
        {
            if(polygon.empty())
            {
                addPoint(&polygon, Point(position.x(), position.y()));
            }
            else
            {
                drawLine(polygon[polygon.size()-1].getX(), polygon[polygon.size()-1].getY(), position.x(), position.y());
                addPoint(&polygon, Point(position.x(), position.y()));
            }
        }
        if(!(position.y() < 0 || position.y() > im.height() - 1 || position.x() < 0 || position.x() > im.width() - 1) && (e->button() == Qt::RightButton))
        {
            if(polygon.size() > 2)
            {
                drawLine(polygon[0].getX(), polygon[0].getY(), polygon[polygon.size()-1].getX(), polygon[polygon.size()-1].getY());
                drawPolygonScanLine(polygon);
            }
        }
        break;
    default:
        break;
    }

    update();
}

void Ekran::mouseReleaseEvent(QMouseEvent *e)
{
    imTmp = im;
    QPoint position = e -> pos();
    switch (currentTool)
    {
    case 0:
        if(!(position.y() < 0 || position.y() > im.height() - 1 || position.x() < 0 || position.x() > im.width() - 1))
        {
            drawLine(x0, y0, position.x(), position.y());
        }
        break;
    case 1:
        if(!(position.y() < 0 || position.y() > im.height() - 1 || position.x() < 0 || position.x() > im.width() - 1))
        {
            drawCircle(x0, y0, position.x(), position.y());
        }
        break;
    case 2:
        if(!(position.y() < 0 || position.y() > im.height() - 1 || position.x() < 0 || position.x() > im.width() - 1))
        {
            drawEllipse(x0, y0, position.x(), position.y(), numOfVertices);
        }
        break;
    case 3:
        if(!(position.y() < 0 || position.y() > im.height() - 1 || position.x() < 0 || position.x() > im.width() - 1))
        {
            if(mode == 1)
            {
                if(e -> button() & Qt::LeftButton)
                {
                    Point p;
                    p.setX(position.x());
                    p.setY(position.y());
                    addPoint(&points, p);
                    drawBezier(points, QColor(red, green, blue, 255));
                }
            }
            if(mode == 2)
            {
                if(!points.empty())
                {
                    for(QVector<Point>::iterator it = points.begin(); it <= points.end(); ++it)
                    {
                        if(sqrt(pow(x0 - it->getX(), 2) + pow(y0 - it->getY(), 2)) <= 2)
                        {
                            Point p;
                            p.setX(position.x());
                            p.setY(position.y());
                            it->setX(position.x());
                            it->setY(position.y());
                            drawPoint(p);
                            drawBezier(points, QColor(red, green, blue, 255));

                        }
                    }
                }
                iterat = -1;
            }
        }
        break;
    case 4:
        if(!(position.y() < 0 || position.y() > im.height() - 1 || position.x() < 0 || position.x() > im.width() - 1))
        {
            if(mode == 1)
            {
                if(e -> button() & Qt::LeftButton)
                {
                    Point p;
                    p.setX(position.x());
                    p.setY(position.y());
                    addPoint(&points, p);
                    drawBSpline(points, QColor(red, green, blue, 255));
                }
            }
            if(mode == 2)
            {
                if(!points.empty())
                {
                    for(QVector<Point>::iterator it = points.begin(); it <= points.end(); ++it)
                    {
                        if(sqrt(pow(x0 - it->getX(), 2) + pow(y0 - it->getY(), 2)) <= 2)
                        {
                            Point p;
                            p.setX(position.x());
                            p.setY(position.y());
                            it->setX(position.x());
                            it->setY(position.y());
                            drawPoint(p);
                            drawBSpline(points, QColor(red, green, blue, 255));

                        }
                    }
                }
                iterat = -1;
            }
        }
        break;
    case 5:
        if(!(position.y() < 0 || position.y() > im.height() - 1 || position.x() < 0 || position.x() > im.width() - 1))
        {
            floodFill(position.x(), position.y(), getCol(position.x(), position.y()), QColor(red, green, blue, 255));
        }
        break;
    case 6:

        break;
    default:
        break;
    }

    update();
}

void Ekran::mouseMoveEvent(QMouseEvent *e)
{
    QPoint position = e -> pos();
    im = imTmp;
    switch (currentTool) {
    case 0:
        if(!(position.y() < 0 || position.y() > im.height() - 1 || position.x() < 0 || position.x() > im.width() - 1))
        {
            drawLine(x0, y0, position.x(), position.y());
        }
        break;
    case 1:
        if(!(position.y() < 0 || position.y() > im.height() - 1 || position.x() < 0 || position.x() > im.width() - 1))
        {
            drawCircle(x0, y0, position.x(), position.y());
        }
        break;
    case 2:
        if(!(position.y() < 0 || position.y() > im.height() - 1 || position.x() < 0 || position.x() > im.width() - 1))
        {
            drawEllipse(x0, y0, position.x(), position.y(), numOfVertices);
        }
        break;
    case 3:
        if(!(position.y() < 0 || position.y() > im.height() - 1 || position.x() < 0 || position.x() > im.width() - 1))
        {

            if(mode == 1)
            {
                if(e -> buttons() & Qt::LeftButton)
                {
                    Point p;
                    p.setX(position.x());
                    p.setY(position.y());
                    drawPoint(p);
                }
            }
            if(mode == 2)
            {
                if(!points.empty() && iterat != -1)
                {
                    int it = iterat;
                    Point p;
                    drawBezier(copy, Qt::black);
                    drawPoint(copy[it], Qt::black);
                    p.setX(position.x());
                    p.setY(position.y());
                    points[iterat].setX(p.getX());
                    points[iterat].setY(p.getY());
                    drawPoint(p);
                    drawBezier(points, QColor(red, green, blue, 255));
                }
            }
        }
        break;
    case 4:
        if(!(position.y() < 0 || position.y() > im.height() - 1 || position.x() < 0 || position.x() > im.width() - 1))
        {

            if(mode == 1)
            {
                if(e -> buttons() & Qt::LeftButton)
                {
                    Point p;
                    p.setX(position.x());
                    p.setY(position.y());
                    drawPoint(p);
                }
            }
            if(mode == 2)
            {
                if(!points.empty() && iterat != -1)
                {
                    int it = iterat;
                    Point p;
                    drawBSpline(copy, Qt::black);
                    drawPoint(copy[it], Qt::black);
                    p.setX(position.x());
                    p.setY(position.y());
                    points[iterat].setX(p.getX());
                    points[iterat].setY(p.getY());
                    drawPoint(p);
                    drawBSpline(points, QColor(red, green, blue, 255));
                }
            }
        }
        break;
    case 5:
        break;
    case 6:
        break;
    default:
        break;
    }

    update();
}

void Ekran::setNumOfVertices(int value)
{
    if(value >= 3) numOfVertices = value;
}

void Ekran::setRed(int value)
{
    if(value >= 0 && value <= 255) red = value;
}

void Ekran::setBlue(int value)
{
    if(value >= 0 && value <= 255) blue = value;
}

void Ekran::setGreen(int value)
{
    if(value >= 0 && value <= 255) green = value;
}

void Ekran::setMode(int value)
{
    if(value > 0 && value < 4) this->mode = value;
}

void Ekran::setCurrentTool(int value)
{
    currentTool = value;
}

bool Ekran::isInterception(Point p1, Point p2, int y)
{
    if((p1.getY() > y && p2.getY() < y) || (p1.getY() < y && p2.getY() > y) ||
            (p1.getY() < p2.getY() && p1.getY() == y) || (p2.getY() < p1.getY() && p2.getY() == y)) return true;
    else return false;
}

int Ekran::pointOfInterception(Point p1, Point p2, int y)
{
    int x;

    if(p1.getY() == p2.getY()) return 0; // prosta rownajaca sie z prosta y
    else
    {
       x = p1.getX() + (y-p1.getY())*(p2.getX()-p1.getX())/(p2.getY()-p1.getY());
       return x;
    }
}

int Ekran::getNumOfVertices()
{
    return this->numOfVertices;
}

int Ekran::getRed()
{
    return this->red;
}

int Ekran::getBlue()
{
    return this->blue;
}

int Ekran::getGreen()
{
    return this->green;
}

int Ekran::getMode()
{
    return this->mode;
}

void Ekran::clear()
{
    points.clear();
    points.squeeze();
    copy.clear();
    copy.squeeze();
    polygon.clear();
    polygon.squeeze();

    im.fill(Qt::black);
    imTmp = im;
    update();
}

void Ekran::clear2(int r, int g, int b) {
    points.clear();
    points.squeeze();
    copy.clear();
    copy.squeeze();
    polygon.clear();
    polygon.squeeze();

    im.fill(QColor(r,g,b,255));
    imTmp = im;
    update();
}

