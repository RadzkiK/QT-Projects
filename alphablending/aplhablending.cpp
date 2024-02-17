#include "aplhablending.h"
#include "ui_aplhablending.h"

#include <qfiledialog.h>
#include <qpainter.h>

AplhaBlending::AplhaBlending(QWidget *parent)
    : QMainWindow(parent)
    ,out(500, 500, QImage::Format_RGB32)
    , ui(new Ui::AplhaBlending)
{
    out.fill(Qt::black);

    images[0] = QImage(500, 500, QImage::Format_RGB32);
    images[1] = QImage(500, 500, QImage::Format_RGB32);
    images[2] = QImage(500, 500, QImage::Format_RGB32);
    ui->setupUi(this);
    ui->comboBoxCat->addItem("Normal");
    ui->comboBoxCat->addItem("Multiply");
    ui->comboBoxCat->addItem("Darken");
    ui->comboBoxCat->addItem("Lighten");
    ui->comboBoxCat->addItem("Difference");
    ui->comboBoxCat->addItem("Glow");
    ui->comboBoxCat->addItem("Overlay");
    ui->comboBoxDog->addItem("Normal");
    ui->comboBoxDog->addItem("Multiply");
    ui->comboBoxDog->addItem("Darken");
    ui->comboBoxDog->addItem("Lighten");
    ui->comboBoxDog->addItem("Difference");
    ui->comboBoxDog->addItem("Glow");
    ui->comboBoxDog->addItem("Overlay");
    ui->comboBoxTree->addItem("Normal");
    ui->comboBoxTree->addItem("Multiply");
    ui->comboBoxTree->addItem("Darken");
    ui->comboBoxTree->addItem("Lighten");
    ui->comboBoxTree->addItem("Difference");
    ui->comboBoxTree->addItem("Glow");
    ui->comboBoxTree->addItem("Overlay");
}

void AplhaBlending::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(0,0,out);
    update();
}

AplhaBlending::~AplhaBlending()
{
    delete ui;
}

int AplhaBlending::lighten(int a, int b)
{
    if(a > b) return a;
    else return b;
}

int AplhaBlending::darken(int a, int b)
{
    if(a < b) return a;
    else return b;
}

int AplhaBlending::difference(int a, int b)
{
    return abs(a - b);
}

int AplhaBlending::multiply(int a, int b)
{
    return (a * b) >> 8;
}

int AplhaBlending::glow(int a, int b)
{
    if(a == 255) {
        return a;
    } else {
        int c = (b * b) / (255 - a);
        if(c > 255) return 255;
        else return c;
    }
}

int AplhaBlending::overlay(int a, int b)
{
    if(a < 128) return (a * b) >> 7;
    else return 255 - ((255 - a) * (255 - b) >> 7);
}

void AplhaBlending::blend()
{
    uchar *pixelO,*pixelI;
    int redB, greenB, blueB, redF, greenF, blueF;
    out.fill(Qt::black);
    for(int i = 0; i < 3; ++i)
    {
        for(int y = 0; y < 500; ++y)
        {
            pixelI = images[i].scanLine(y);
            pixelO = out.scanLine(y);
            for(int x = 0; x < 500; ++x)
            {
                redF = pixelI[4*x+2];
                greenF = pixelI[4*x+1];
                blueF = pixelI[4*x];
                redB = pixelO[4*x+2];
                greenB = pixelO[4*x+1];
                blueB = pixelO[4*x];
                switch(mode[i])
                {
                case 0:
                    break;
                case 1:
                    redF = multiply(redF, redB);
                    greenF = multiply(greenF, greenB);
                    blueF = multiply(blueF, blueB);
                    break;
                case 2:
                    redF = darken(redF, redB);
                    greenF = darken(greenF, greenB);
                    blueF = darken(blueF, blueB);
                    break;
                case 3:
                    redF = lighten(redF, redB);
                    greenF = lighten(greenF, greenB);
                    blueF = lighten(blueF, blueB);
                    break;
                case 4:
                    redF = difference(redF, redB);
                    greenF = difference(greenF, greenB);
                    blueF = difference(blueF, blueB);
                    break;
                case 5:
                    redF = glow(redF, redB);
                    greenF = glow(greenF, greenB);
                    blueF = glow(blueF, blueB);
                    break;
                case 6:
                    redF = overlay(redF, redB);
                    greenF = overlay(greenF, greenB);
                    blueF = overlay(blueF, blueB);
                    break;
                }
                pixelO[4*x] = alpha[i] * blueF + (1.0 - alpha[i]) * blueB;
                pixelO[4*x+1] = alpha[i] * greenF + (1.0 - alpha[i]) * greenB;
                pixelO[4*x+2] = alpha[i]  *redF + (1.0 - alpha[i]) * redB;
            }
        }
    }
    update();
}

void AplhaBlending::loadPicture(int i)
{
    QString path = QFileDialog::getOpenFileName(this,"Wczytaj obrazek","C://");
    qDebug().nospace() << qPrintable(path);
    if(images[i].load(path) == true) qDebug("Udalo sie wczytac obrazek");
    else exit(0);
}


void AplhaBlending::on_SliderCat_valueChanged(int value)
{
    alpha[0] = value/255.0;
    blend();
}


void AplhaBlending::on_SliderDog_valueChanged(int value)
{
    alpha[1] = value/255.0;
    blend();
}


void AplhaBlending::on_SliderTree_valueChanged(int value)
{
    alpha[2] = value/255.0;
    blend();
}


void AplhaBlending::on_comboBoxCat_currentIndexChanged(int index)
{
    mode[0] = index;
    blend();
}


void AplhaBlending::on_comboBoxDog_currentIndexChanged(int index)
{
     mode[1] = index;
     blend();
}


void AplhaBlending::on_comboBoxTree_currentIndexChanged(int index)
{
     mode[2] = index;
     blend();
}

void AplhaBlending::on_catButton_clicked()
{
    loadPicture(0);
}


void AplhaBlending::on_dogButton_clicked()
{
    loadPicture(1);
}


void AplhaBlending::on_treeButton_clicked()
{
    loadPicture(2);
}

