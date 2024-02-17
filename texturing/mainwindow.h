
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QImage img, img2, img_tmp, img2_tmp;
    QVector<QPoint> points, points2, copy;
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void drawPoint(QPoint p, QColor col);
    void drawPointOnImg2(QPoint p, QColor col);
    void addPoint(QPoint);
    void deletePoint(QPoint);
    void deletePointFromImg2(QPoint);
    void putPixel(int x, int y);
    void putPixel(int x, int y, QColor col);
    void putPixelOnImg2(int x, int y);
    void putPixelOnImg2(int x, int y, QColor col);
    void drawLine(int x1, int y1, int x2, int y2);
    void drawLineOnImg2(int x1, int y1, int x2, int y2);
    void draw();
    QColor interpolate(float x, float y);
    QColor getCol(QPoint p);
    void texture();


private:
    Ui::MainWindow *ui;
    QColor c = Qt::white;
    int x0, y0, iterat;
};

#endif // MAINWINDOW_H
