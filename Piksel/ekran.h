#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>

class Point
{
private:
    int x;
    int y;

public:
    Point(int x, int y);
    Point();
    void setX(int v);
    void setY(int v);
    int getX();
    int getY();
    bool operator ==(const Point &p);
};

class Ekran : public QWidget
{
    Q_OBJECT
public:
    QImage im, imTmp;

    explicit Ekran(QWidget *parent = nullptr);
    void drawPixel(int x, int y, int r, int g, int b);
    void drawPoint(Point p);
    void drawPoint(Point p, QColor col);
    void drawLine(int x0, int y0, int x1, int y1);
    void drawLine(int x0, int y0, int x1, int y1, QColor col);
    void drawCircle(int x0, int y0, int x1, int y1);
    void drawEllipse(int x0, int y0, int x1, int y1, int v);
    void drawBezier(QVector<Point> vec, QColor col);
    void drawBSpline(QVector<Point> vec, QColor col);
    void drawPolygonScanLine(QVector<Point> vec);
    Point countBezier(Point p1, Point p2, Point p3, Point p4, float t);
    Point countBSpline(Point p1, Point p2, Point p3, Point p4, float t);
    void floodFill(int x, int y, QColor oldC, QColor newC);
    void addPoint(QVector<Point> *vec, Point p);
    void deletePoint(QVector<Point> *vec, Point p);
    void setNumOfVertices(int value);
    void setRed(int value);
    void setBlue(int value);
    void setGreen(int value);
    void setMode(int value);
    void setCurrentTool(int value);
    void clear();
    void clear2(int r, int g, int b);
    bool isInterception(Point p1, Point p2, int y);
    int pointOfInterception(Point p1, Point p2, int y);
    int getNumOfVertices();
    int getRed();
    int getBlue();
    int getGreen();
    int getMode();
    QColor getCol(int x, int y);
protected:
    int y0, x0, numOfVertices, red, green, blue, mode, iterat, currentTool;
    QVector<Point> points, copy, polygon;
    QVector<int> x;
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

public slots:



signals:

};

#endif // EKRAN_H
