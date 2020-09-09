#include <QApplication>

#include "main_win/main_win.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWin mainWin;
    mainWin.show();

    return QApplication::exec();
}
