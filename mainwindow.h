#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>
#include <QComboBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    virtual void paintEvent(QPaintEvent *event);

private slots:
    void on_actionNovy_triggered();

    void on_actionOtevrit_triggered();
    void nastavSirku(int sirka);
    void on_actionCara_triggered();

    void on_actionObdelnik_triggered();

    void on_actionElipsa_triggered();

    void on_actionUlo_it_triggered();

    void on_actionO_Programu_triggered();

    void on_actionVypln_triggered();

    void setFillColor();

private:
    Ui::MainWindow *ui;
    QImage obrazek;
    QPoint startP, endP;
    QPen pero;
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent*event);
    QPushButton *barvaNastroje, *barvaPozadi;
    void setPenColor();
    QBrush fillBrush;
    QComboBox *combo;


    bool stisknuto = false, kreslim = false;

    enum nastroje{PERO, OBDELNIK, ELIPSA, VYPLN, VLAJKA};
    int nastroj = PERO;
};
#endif // MAINWINDOW_H
