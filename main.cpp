#include <QApplication>
#include "autentification.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    autentification aut;
    aut.show();

    return a.exec();
}
