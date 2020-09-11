#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>
#include <QNetworkInterface>

#include "main_win.h"
#include "./ui_main_win.h"

MainWin::MainWin(QWidget *parent)
        : QMainWindow(parent),
          ui(new Ui::MainWin) {
    ui->setupUi(this);

    connect(ui->actionAbout_developer, SIGNAL(triggered()), this, SLOT(actions()));
    connect(ui->actionAbout_program, SIGNAL(triggered()), this, SLOT(actions()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(actions()));

    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QNetworkInterface &eth : QNetworkInterface::allInterfaces()) {
        for (const QNetworkAddressEntry &entry : eth.addressEntries()) {
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol && entry.ip() != localhost) {
                ui->labelIpv4->setText("IPv4: " + entry.ip().toString());
                ui->labelMask->setText("Netmask: " + entry.netmask().toString());
                ui->labelBroadcast->setText("Broadcast: " + entry.broadcast().toString());
            } else if (entry.ip().protocol() == QAbstractSocket::IPv6Protocol && entry.ip() != localhost) {
                ui->labelIpv6->setText("IPv6: " + entry.ip().toString());
                ui->labelPrefixlen->setText("Prefix length: " + QString::number(entry.prefixLength()));
            } else {
                ui->labelIpv4->setText("IPv4: " + localhost.toString());
                ui->labelMask->setText("Netmask: " + entry.netmask().toString());
                ui->labelBroadcast->setText("Broadcast: " + entry.broadcast().toString());
            }
        }

        if (!(eth.flags() & QNetworkInterface::IsLoopBack))
            ui->labelMac->setText("MAC: " + eth.hardwareAddress());
    }
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

