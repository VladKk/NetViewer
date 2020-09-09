#include <QDebug>
#include <QMessageBox>

#include "main_win.h"
#include "./ui_main_win.h"

MainWin::MainWin(QWidget *parent)
        : QMainWindow(parent),
          ui(new Ui::MainWin) {
    ui->setupUi(this);

    connect(ui->actionAbout_developer, SIGNAL(triggered()), this, SLOT(actions()));
    connect(ui->actionAbout_program, SIGNAL(triggered()), this, SLOT(actions()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(actions()));
}

MainWin::~MainWin() { delete ui; }

void MainWin::actions() {
    auto *action = (QAction *) sender();

    if (action == ui->actionAbout_developer) {
        QMessageBox::information(this, "NetViewer",
                                 "Find me in the Web!\n"
                                 "Telegram: @vlad_is_real\n"
                                 "GMail: vladislav.kolyadenko@gmail.com\n"
                                 "Instagram: @ncks_gwc");
    } else if (action == ui->actionAbout_program) {
        QMessageBox::information(this, "NetViewer",
                                 "Net status tracker (with additional information)\n"
                                 "May have a lot of bugs and mistakes\n"
                                 "Used technologies:\n"
                                 "C++, Qt, CMake");
    } else if (action == ui->actionExit) {
        QApplication::quit();
    } else {
        qDebug() << "Invalid action!";
        QApplication::exit(EXIT_FAILURE);
    }
}

