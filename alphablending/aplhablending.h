#ifndef APLHABLENDING_H
#define APLHABLENDING_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class AplhaBlending; }
QT_END_NAMESPACE


class AplhaBlending : public QMainWindow
{
    Q_OBJECT

public:
    AplhaBlending(QWidget *parent = nullptr);
    ~AplhaBlending();
    QImage out;
    QImage images[3];
    double alpha[3];
    int mode[3] = {0, 0, 0};
    void paintEvent(QPaintEvent *);
    int lighten(int a, int b);
    int darken(int a, int b);
    int difference(int a, int b);
    int multiply(int a, int b);
    int glow(int a, int b);
    int overlay(int a, int b);
    void blend();
    void loadPicture(int i);


private slots:
    void on_SliderCat_valueChanged(int value);

    void on_SliderDog_valueChanged(int value);

    void on_SliderTree_valueChanged(int value);

    void on_comboBoxCat_currentIndexChanged(int index);

    void on_comboBoxDog_currentIndexChanged(int index);

    void on_comboBoxTree_currentIndexChanged(int index);

    void on_catButton_clicked();

    void on_dogButton_clicked();

    void on_treeButton_clicked();

private:
    Ui::AplhaBlending *ui;
};
#endif // APLHABLENDING_H
