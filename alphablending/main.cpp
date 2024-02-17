#include "aplhablending.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AplhaBlending w;
    w.show();
    return a.exec();
}
