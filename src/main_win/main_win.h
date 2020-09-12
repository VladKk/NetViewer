#ifndef NETVIEWER_MAIN_WIN_H
#define NETVIEWER_MAIN_WIN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWin; }
QT_END_NAMESPACE

class MainWin final : public QMainWindow {
Q_OBJECT

public:
    explicit MainWin(QWidget *parent = nullptr);

    ~MainWin() final;

private slots:

    void actions();

    void refresh();

    void checkConnection();

private:
    Ui::MainWin *ui;
};

#endif //NETVIEWER_MAIN_WIN_H
