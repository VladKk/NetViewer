#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QTimer>

#include <ifaddrs.h>

#include <linux/if_link.h>

#include "main_win.h"
#include "./ui_main_win.h"

MainWin::MainWin(QWidget *parent)
        : QMainWindow(parent),
          ui(new Ui::MainWin) {
    setWindowIcon(QIcon(":/icons/mainWinIcon.ico"));
    ui->setupUi(this);

    // Connect elements with appropriate slots
    connect(ui->actionAbout_developer, SIGNAL(triggered()), this, SLOT(actions()));
    connect(ui->actionAbout_program, SIGNAL(triggered()), this, SLOT(actions()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(actions()));
    connect(ui->actionRefresh, SIGNAL(triggered()), this, SLOT(actions()));

    connect(ui->buttonRefresh, SIGNAL(pressed()), this, SLOT(refresh()));

    connect(ui->buttonCheckConnection, SIGNAL(pressed()), this, SLOT(checkConnection()));

    // Check all address entries in network interface and print main information
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QNetworkInterface &eth : QNetworkInterface::allInterfaces()) {
        for (const QNetworkAddressEntry &entry : eth.addressEntries()) {
            // For IPv4, netmask and broadcast
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol && entry.ip() != localhost) {
                ui->labelIpv4->setText("IPv4: " + entry.ip().toString());
                ui->labelMask->setText("Netmask: " + entry.netmask().toString());
                ui->labelBroadcast->setText("Broadcast: " + entry.broadcast().toString());
                // For IPv6 and prefix length
            } else if (entry.ip().protocol() == QAbstractSocket::IPv6Protocol && entry.ip() != localhost) {
                ui->labelIpv6->setText("IPv6: " + entry.ip().toString());
                ui->labelPrefixlen->setText("Prefix length: " + QString::number(entry.prefixLength()));
                // For localhost
            } else {
                ui->labelIpv4->setText("IPv4: " + localhost.toString());
                ui->labelMask->setText("Netmask: " + entry.netmask().toString());
                ui->labelBroadcast->setText("Broadcast: " + entry.broadcast().toString());
            }
        }
        // For MAC address
        if (!(eth.flags() & QNetworkInterface::IsLoopBack))
            ui->labelMac->setText("MAC: " + eth.hardwareAddress());
    }
}

MainWin::~MainWin() { delete ui; }

void MainWin::actions() {
    // Get signal from any QAction
    auto *action = (QAction *) sender();

    // Trigger action if it is valid, otherwise exit with error
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
    } else if (action == ui->actionExit)
        QApplication::quit();
    else if (action == ui->actionRefresh)
        refresh();
    else {
        qDebug() << "Invalid action!";
        QApplication::exit(EXIT_FAILURE);
    }
}

void MainWin::checkConnection() {
    struct ifaddrs *ifaddr, *ifa; // Structs for interfaces
    int family, n; //
    qint32 download, upload; // Download and upload speed
    QString interface_name;

    if (getifaddrs(&ifaddr) == -1) {
        qDebug() << "Error getting ifaddrs";
        return;
    }

    if (ui->labelIpv6->text().contains('%'))
        interface_name = ui->labelIpv6->text().right(
                ui->labelIpv6->text().size() - ui->labelIpv6->text().indexOf('%') - 1);
    else
        interface_name = "lo";

    for (ifa = ifaddr, n = 0; ifa != nullptr; ifa = ifa->ifa_next, n++) {
        if (ifa->ifa_addr == nullptr) // If address is empty, continue
            continue;

        family = ifa->ifa_addr->sa_family; // Get interface family

        // If family is AF_PACKET and interface name is correct, get link statistics
        if ((family == AF_PACKET) && (ifa->ifa_data != nullptr) && (ifa->ifa_name == interface_name)) {
            auto *stats = static_cast<struct rtnl_link_stats *>(ifa->ifa_data);
            // Get downloaded (rx) and uploaded (tx) bytes
            download = stats->rx_bytes;
            upload = stats->tx_bytes;
        }
    }

    freeifaddrs(ifaddr); // Free pointer to interface address

    // Print connection speed in Bytes/second
    ui->labelDownload->setText("Download Speed: " + QString::number(download) + " B/s");
    ui->labelUpload->setText("Upload Speed: " + QString::number(upload) + " B/s");

    // Transform speed in MB/s or KB/s if needed
    if (download > 1000) {
        download = download / 1024;
        if (download > 1000) {
            download = download / 1024;
            ui->labelDownload->setText(ui->labelDownload->text() + " (" + QString::number(download) + " MB/s)");
        } else
            ui->labelDownload->setText(ui->labelDownload->text() + " (" + QString::number(download) + " KB/s)");
    }

    if (upload > 1000) {
        upload = upload / 1024;
        if (upload > 1000) {
            upload = upload / 1024;
            ui->labelUpload->setText(ui->labelUpload->text() + " (" + QString::number(upload) + " MB/s)");
        } else
            ui->labelUpload->setText(ui->labelUpload->text() + " (" + QString::number(upload) + " KB/s)");
    }
}

void MainWin::refresh() {
    // Recheck speed in 1 second
    ui->statusBar->showMessage("Checking connection...", 1000);
    QTimer::singleShot(1000, this, SLOT(checkConnection()));
}

