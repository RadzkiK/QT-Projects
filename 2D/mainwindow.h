
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <armadillo>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QImage img, img2;
    arma::mat transformationM = arma::eye(3,3);
    arma::mat translationM = arma::eye(3,3);
    arma::mat scalingM = arma::eye(3,3);
    arma::mat rotationM = arma::eye(3,3);
    arma::mat bendingM = arma::eye(3,3);
    void paintEvent(QPaintEvent *);
    void transformation();



private slots:
    void on_bendX_valueChanged(int value);

    void on_bendY_valueChanged(int value);

    void on_rotationSlider_valueChanged(int value);

    void on_scalingX_valueChanged(int value);

    void on_skalingY_valueChanged(int value);

    void on_translationX_valueChanged(int value);

    void on_translationY_valueChanged(int value);

private:
    float TX, TY, R, SX, SY, BX, BY;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
