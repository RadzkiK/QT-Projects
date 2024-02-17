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



private slots:
    void on_horizontalSlider_NOV_valueChanged(int value);

    void on_horizontalSlider_red_valueChanged(int value);

    void on_horizontalSlider_green_valueChanged(int value);

    void on_horizontalSlider_blue_valueChanged(int value);

    void on_pushButton_add_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_move_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_clear_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
