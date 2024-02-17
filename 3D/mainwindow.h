
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <armadillo>

typedef std::tuple<int,int> Point2D;
typedef std::tuple<int,int,int> Point3D;
typedef std::tuple<int,int,int> Triangle;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QImage img, triangle_img;

    void paintEvent(QPaintEvent *);
    void drawPixel(int x, int y);
    void drawLine(int x0, int y0, int x1, int y1);
    void draw();
    void transform();
    QColor interpolate(float x, float y);
    void texture_triangles(Triangle);

    QVector<Point3D> points
    {
        Point3D(500/2-75,500/2-75,500/2-75),//0
        Point3D(500/2-75,500/2-75,500/2+75),//1
        Point3D(500/2-75,500/2+75,500/2-75),//2
        Point3D(500/2+75,500/2-75,500/2-75),//3
        Point3D(500/2+75,500/2+75,500/2-75),//4
        Point3D(500/2-75,500/2+75,500/2+75),//5
        Point3D(500/2+75,500/2-75,500/2+75),//6
        Point3D(500/2+75,500/2+75,500/2+75) //7
    };

    QVector<Point3D> transformedPoints;

    QVector<Triangle> triangles
    {
        Triangle(0,2,1),//left
        Triangle(5,1,2),
        Triangle(0,1,3),//upper
        Triangle(6,3,1),
        Triangle(0,3,2),//rear
        Triangle(4,2,3),
        Triangle(1,5,6),//front
        Triangle(7,6,5),
        Triangle(2,4,5),//lower
        Triangle(7,5,4),
        Triangle(6,7,3),//right
        Triangle(4,3,7)
    };

    QVector<QPoint> triangle_points
    {
        QPoint(1,1),
        QPoint(1, 149),
        QPoint(149,149)
    };

private slots:
    void on_translationX_slider_valueChanged(int value);

    void on_translationY_slider_valueChanged(int value);

    void on_translationZ_slider_valueChanged(int value);

    void on_rotationX_slider_valueChanged(int value);

    void on_rotationY_slider_valueChanged(int value);

    void on_rotationZ_slider_valueChanged(int value);

    void on_scalingX_slider_valueChanged(int value);

    void on_scalingY_slider_valueChanged(int value);

    void on_scalingZ_slider_valueChanged(int value);

    void on_checkBox_toggled(bool checked);

    void on_texture_toggled(bool checked);

private:

    float TX,TY,TZ,RX,RY,RZ,SX,SY,SZ;
    bool showOnlyVisible = false;
    bool texture = false;
    int d = 1000;
    arma::mat translationM = arma::eye(4,4);
    arma::mat scalingM = arma::eye(4,4);
    arma::mat rotationXM = arma::eye(4,4);
    arma::mat rotationYM = arma::eye(4,4);
    arma::mat rotationZM = arma::eye(4,4);
    arma::mat transformationM = arma::eye(4,4);

    //macierze pomocnicze
    arma::mat Tminus = arma::eye(4,4);
    arma::mat Tplus = arma::eye(4,4);
    QColor col = Qt::green;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
