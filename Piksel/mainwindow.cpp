#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->comboBox->addItem("Line");
    ui->comboBox->addItem("Circle");
    ui->comboBox->addItem("Ellipse");
    ui->comboBox->addItem("Bezier");
    ui->comboBox->addItem("B-Spline");
    ui->comboBox->addItem("FloodFill");
    ui->comboBox->addItem("ScanLine");
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_horizontalSlider_NOV_valueChanged(int value)
{
    QString val = "Ilosc wierzcholkow: " + QString::number(value);
    ui->widget->setNumOfVertices(value);
    ui->label->setText(val);
}


void MainWindow::on_horizontalSlider_red_valueChanged(int value)
{
    QString val = "Red: " + QString::number(value);
    ui->widget->setRed(value);
    ui->label_red->setText(val);
}


void MainWindow::on_horizontalSlider_green_valueChanged(int value)
{
    QString val = "Green: " + QString::number(value);
    ui->widget->setGreen(value);
    ui->label_green->setText(val);
}


void MainWindow::on_horizontalSlider_blue_valueChanged(int value)
{
    QString val = "Blue: " + QString::number(value);
    ui->widget->setBlue(value);
    ui->label_blue->setText(val);
}


void MainWindow::on_pushButton_add_clicked()
{
    ui->widget->setMode(1);
}


void MainWindow::on_pushButton_delete_clicked()
{
    ui->widget->setMode(3);
}


void MainWindow::on_pushButton_move_clicked()
{
    ui->widget->setMode(2);
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    ui->widget->setCurrentTool(index);
}


void MainWindow::on_pushButton_clear_clicked()
{
    ui->widget->clear();
}

