
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    img(500,500,QImage::Format_RGB32),
    triangle_img(500,500,QImage::Format_RGB32),
    ui(new Ui::MainWindow)
{
    triangle_img.load("C:/Users/Konrad/Pictures/domek.png");
    img.fill(0);
    for(int i = 0; i < points.size(); ++i)
    {
        transformedPoints.push_back(points[i]);
    }


    Tminus(0,3) = -250;
    Tminus(1,3) = -250;
    Tminus(2,3) = -250;
    Tplus(0,3) = 250;
    Tplus(1,3) = 250;
    Tplus(2,3) = 250;

    draw();

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(0,0,img);
    update();
}

void MainWindow::drawPixel(int x, int y)
{
    if (y < 0 || y > img.height() - 1 || x < 0 || x > img.width() - 1) return;
    uchar *pix = img.scanLine(y);
    pix[x*4 + 0] = col.blue();
    pix[x*4 + 1] = col.green();
    pix[x*4 + 2] = col.red();
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
            drawPixel(x, y);
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
            drawPixel(x, y);
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

void MainWindow::transform()
{
    transformationM =  translationM * rotationZM * rotationYM * rotationXM * scalingM;
    auto transformedCoordinates = [&](Point3D pt)
    {
        int a,b,c;
        a = std::get<0>(pt);
        b = std::get<1>(pt);
        c = std::get<2>(pt);
        arma::vec v{(float)a,(float)b,(float)c,1};
        arma::vec res = transformationM * v;
        int x = (int)res[0];
        int y = (int)res[1];
        int z = (int)res[2];
        return std::make_tuple(x,y,z);
    };
    transformedPoints.clear();
    for(int i = 0; i < points.size(); ++i) transformedPoints.push_back(transformedCoordinates(points[i]));
    draw();
    update();
}

void MainWindow::texture_triangles(Triangle t)
{
    Point3D A1,B1,C1;
    A1 = transformedPoints[std::get<0>(t)];
    B1 = transformedPoints[std::get<1>(t)];
    C1 = transformedPoints[std::get<2>(t)];

    int x_a = std::get<0>(Point2D(((std::get<0>(A1) - 250) * d / (std::get<2>(A1) + d)) + 250, ((std::get<1>(A1) - 250) * d / (std::get<2>(A1) + d)) + 250));
    int x_b = std::get<0>(Point2D(((std::get<0>(B1) - 250) * d / (std::get<2>(B1) + d)) + 250, ((std::get<1>(B1) - 250) * d / (std::get<2>(B1) + d)) + 250));
    int x_c = std::get<0>(Point2D(((std::get<0>(C1) - 250) * d / (std::get<2>(C1) + d)) + 250, ((std::get<1>(C1) - 250) * d / (std::get<2>(C1) + d)) + 250));

    int y_a = std::get<1>(Point2D(((std::get<0>(A1) - 250) * d / (std::get<2>(A1) + d)) + 250, ((std::get<1>(A1) - 250) * d / (std::get<2>(A1) + d)) + 250));
    int y_b = std::get<1>(Point2D(((std::get<0>(B1) - 250) * d / (std::get<2>(B1) + d)) + 250, ((std::get<1>(B1) - 250) * d / (std::get<2>(B1) + d)) + 250));
    int y_c = std::get<1>(Point2D(((std::get<0>(C1) - 250) * d / (std::get<2>(C1) + d)) + 250, ((std::get<1>(C1) - 250) * d / (std::get<2>(C1) + d)) + 250));

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

            if(u > 0 && u < 1 && v > 0 && v < 1 && w > 0 && w < 1)
            {
                x_t = u * (float)triangle_points[0].x() + v * (float)triangle_points[1].x() + w * (float)triangle_points[2].x();
                y_t = u * (float)triangle_points[0].y() + v * (float)triangle_points[1].y() + w * (float)triangle_points[2].y();

                out = interpolate(x_t,y_t);

                img.setPixelColor(x,y,out);
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

    a = triangle_img.pixelColor(x_int,y_int);
    b = triangle_img.pixelColor(x_int+1,y_int);
    c = triangle_img.pixelColor(x_int,y_int+1);
    d = triangle_img.pixelColor(x_int+1,y_int+1);

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

void MainWindow::draw()
{
    img.fill(Qt::black);
    Triangle t;
    Point2D A,B,C;
    Point3D A1,B1,C1;
    if(showOnlyVisible)
    {
        for(int i = 0; i < triangles.size(); ++i)
        {
            t = triangles[i];
            A1 = transformedPoints[std::get<0>(t)];
            B1 = transformedPoints[std::get<1>(t)];
            C1 = transformedPoints[std::get<2>(t)];

            if(std::get<2>(A1) + d > 0)
            {
                A = Point2D(((std::get<0>(A1) - 250) * d / (std::get<2>(A1) + d)) + 250, ((std::get<1>(A1) - 250) * d / (std::get<2>(A1) + d)) + 250);
            }

            if(std::get<2>(B1) + d> 0)
            {
                B = Point2D(((std::get<0>(B1) - 250) * d / (std::get<2>(B1) + d)) + 250, ((std::get<1>(B1) - 250) * d / (std::get<2>(B1) + d)) + 250);
            }

            if(std::get<2>(C1) + d > 0)
            {
                C = Point2D(((std::get<0>(C1) - 250) * d / (std::get<2>(C1) + d)) + 250, ((std::get<1>(C1) - 250) * d / (std::get<2>(C1) + d)) + 250);
            }

//            A = Point2D(std::get<0>(A1) * d / (std::get<2>(A1) + d), std::get<1>(A1) * d / (std::get<2>(A1) + d));
//            B = Point2D(std::get<0>(B1) * d / (std::get<2>(B1) + d), std::get<1>(B1) * d / (std::get<2>(B1) + d));
//            C = Point2D(std::get<0>(C1) * d / (std::get<2>(C1) + d), std::get<1>(C1) * d / (std::get<2>(C1) + d));
            auto visible = [&](Point2D A, Point2D B, Point2D C){
                int Ax,Ay,Bx,By,Cx,Cy;
                Ax = (int)std::get<0>(A);
                Bx = (int)std::get<0>(B);
                Cx = (int)std::get<0>(C);
                Ay = (int)std::get<1>(A);
                By = (int)std::get<1>(B);
                Cy = (int)std::get<1>(C);
                if((Bx-Ax)*(Cy-Ay)-(By-Ay)*(Cx-Ax) < 0) return false;
                else return true;
            };
            if(visible(A,B,C))
            {
                if(texture) texture_triangles(triangles[i]);
                drawLine(std::get<0>(A), std::get<1>(A), std::get<0>(B), std::get<1>(B));
                drawLine(std::get<0>(A), std::get<1>(A), std::get<0>(C), std::get<1>(C));
            }
        }
    }
    else
    {
        for(int i = 0; i < triangles.size(); ++i)
        {
            t = triangles[i];
            A1 = transformedPoints[std::get<0>(t)];
            B1 = transformedPoints[std::get<1>(t)];
            C1 = transformedPoints[std::get<2>(t)];

            if(std::get<2>(A1) + d > 0)
            {
                A = Point2D(((std::get<0>(A1) - 250) * d / (std::get<2>(A1) + d)) + 250, ((std::get<1>(A1) - 250) * d / (std::get<2>(A1) + d)) + 250);
            }

            if(std::get<2>(B1) + d> 0)
            {
                B = Point2D(((std::get<0>(B1) - 250) * d / (std::get<2>(B1) + d)) + 250, ((std::get<1>(B1) - 250) * d / (std::get<2>(B1) + d)) + 250);
            }

            if(std::get<2>(C1) + d > 0)
            {
                C = Point2D(((std::get<0>(C1) - 250) * d / (std::get<2>(C1) + d)) + 250, ((std::get<1>(C1) - 250) * d / (std::get<2>(C1) + d)) + 250);
            }

//            A = Point2D(std::get<0>(A1) * d / (std::get<2>(A1) + d), std::get<1>(A1) * d / (std::get<2>(A1) + d));
//            B = Point2D(std::get<0>(B1) * d / (std::get<2>(B1) + d), std::get<1>(B1) * d / (std::get<2>(B1) + d));
//            C = Point2D(std::get<0>(C1) * d / (std::get<2>(C1) + d), std::get<1>(C1) * d / (std::get<2>(C1) + d));
            if(texture) texture_triangles(triangles[i]);
            drawLine(std::get<0>(A), std::get<1>(A), std::get<0>(B), std::get<1>(B));
            drawLine(std::get<0>(A), std::get<1>(A), std::get<0>(C), std::get<1>(C));

        }
    }
    update();
}

void MainWindow::on_translationX_slider_valueChanged(int value)
{
    TX = value;
    translationM(0,3) = TX;
    transform();
}


void MainWindow::on_translationY_slider_valueChanged(int value)
{
    TY = value;
    translationM(1,3) = TY;
    transform();
}


void MainWindow::on_translationZ_slider_valueChanged(int value)
{
    TZ = value;
    translationM(2,3) = TZ;
    transform();
}


void MainWindow::on_rotationX_slider_valueChanged(int value)
{
    rotationXM = arma::eye(4,4);

    RX = value*M_PI / 180;
    float c = cos(RX);
    float s = sin(RX);
    rotationXM(1,1) = c;
    rotationXM(1,2) = -s;
    rotationXM(2,1) = s;
    rotationXM(2,2) = c;

    rotationXM = Tplus * rotationXM * Tminus;
    transform();
}


void MainWindow::on_rotationY_slider_valueChanged(int value)
{
    rotationYM = arma::eye(4,4);

    RY = value*M_PI / 180;
    float c = cos(RY);
    float s = sin(RY);
    rotationYM(0,0) = c;
    rotationYM(0,2) = s;
    rotationYM(2,0) = -s;
    rotationYM(2,2) = c;

    rotationYM = Tplus * rotationYM * Tminus;
    transform();
}


void MainWindow::on_rotationZ_slider_valueChanged(int value)
{
    rotationZM = arma::eye(4,4);

    RZ = value*M_PI / 180;
    float c = cos(RZ);
    float s = sin(RZ);
    rotationZM(0,0) = c;
    rotationZM(0,1) = -s;
    rotationZM(1,0) = s;
    rotationZM(1,1) = c;

    rotationZM = Tplus * rotationZM * Tminus;
    transform();
}


void MainWindow::on_scalingX_slider_valueChanged(int value)
{
    scalingM = arma::eye(4,4);

    SX = value/50.0f;
    scalingM(0,0) = SX;
    scalingM = Tplus * scalingM * Tminus;
    transform();
}


void MainWindow::on_scalingY_slider_valueChanged(int value)
{
    scalingM = arma::eye(4,4);

    SY = value/50.0f;
    scalingM(1,1) = SY;
    scalingM = Tplus * scalingM * Tminus;
    transform();
}


void MainWindow::on_scalingZ_slider_valueChanged(int value)
{
    scalingM = arma::eye(4,4);

    SZ = value/50.0f;
    scalingM(2,2) = SZ;
    scalingM = Tplus * scalingM * Tminus;
    transform();
}


void MainWindow::on_checkBox_toggled(bool checked)
{
    showOnlyVisible = checked;
    transform();
}


void MainWindow::on_texture_toggled(bool checked)
{
    texture = checked;
    transform();
}

