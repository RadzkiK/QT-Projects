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
    QImage img;

    void paintEvent(QPaintEvent *);
    void drawPixel(int x, int y, int r, int g, int b);
    void drawPixel(QImage im, int x, int y, int r, int g, int b);
    bool inImage(int x, int y);
    QColor getCol(int x, int y);
    void toMonochrome();
    void toBinary();
    void negative();
    void dilation();
    void erode();
    void close();
    void open();
    void loadPicture();
    void setThreshold(int i);
    int getThreshold();

private slots:
    void on_loadButton_clicked();

    void on_dilationButton_clicked();

    void on_erodeButton_clicked();

    void on_closeButton_clicked();

    void on_openButton_clicked();

    void on_thresholdSlider_valueChanged(int value);

    void on_monButton_clicked();

    void on_biButton_clicked();

private:
    int threshold;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
