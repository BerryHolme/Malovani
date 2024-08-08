#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QColorDialog>
#include <QComboBox>
#include <QDebug>
#include <QWidget>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QPainterPath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->actionOtevrit->setIcon(this->style()->standardIcon(QStyle::SP_DialogButton));
    ui->actionOtevrit->setIcon(QIcon::fromTheme("dialog-open",QIcon(":/Icons/open.png")));

    QComboBox *combo = new QComboBox(ui->tbStyl);
    combo->setToolTip("Nastevení šířky čáry");
    combo->setEditable(false);
    combo->setIconSize(QSize(80,14));
    combo->setMinimumWidth(60);
    for(int i = 1; i <6; i++){
        QPixmap p(80,14);
        p.fill(Qt::white);
        QPen pen(QBrush(Qt::black),(double)i,Qt::SolidLine);
        QPainter painter(&p);
        painter.setPen(pen);
        painter.drawLine(2,7,78,7);
        combo->addItem(QIcon(p),"");
    }
    connect(combo,SIGNAL(currentIndexChange(int)),this,SLOT(nastavSirku(int)));
    connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [=] (int index) {pero.setWidth(index+1);});
    ui->tbStyl->addWidget(combo);

    QPixmap pix(40,40);
    pix.fill(Qt::black);
    QIcon icon = QIcon(pix);
    barvaNastroje = new QPushButton(icon,"");
    connect(barvaNastroje,&QPushButton::clicked,this,&MainWindow::setPenColor);
    ui->tbStyl->addWidget(barvaNastroje);
    on_actionNovy_triggered();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter platno(this);
    if(stisknuto == true){
        platno.drawImage(0, 0, obrazek);
        platno.setPen(pero);
        platno.setBrush(fillBrush);
        if(nastroj == PERO){
            platno.drawLine(startP, endP);
        }
        else if(nastroj == OBDELNIK){
            platno.drawRect(QRect(startP, endP));
        }
        else if(nastroj == ELIPSA){
            platno.drawEllipse(QRectF(startP, endP));
        }
        else if(nastroj == VYPLN){
            platno.drawRect(QRect(0, 0, this->width(), this->height()));
        }
        else if(nastroj == VLAJKA){
            pero.setColor(Qt::blue);
            platno.drawRect(QRect(startP, endP));
        }
        kreslim = true;
    }else if(kreslim == true){
        QPainter docasnePlatno(&obrazek);
        docasnePlatno.setPen(pero);
        docasnePlatno.setBrush(fillBrush);
        if(nastroj == PERO){
            docasnePlatno.drawLine(startP, endP);
        }
        else if(nastroj == OBDELNIK){
            docasnePlatno.drawRect(QRect(startP, endP));
        }
        else if(nastroj == ELIPSA){
            docasnePlatno.drawEllipse(QRectF(startP, endP));
        }
        else if(nastroj == VYPLN){
            QPainterPath cesta;
            cesta.addRect(QRect(startP, endP));
            platno.fillPath(cesta, fillBrush);

            platno.setPen(pero);
            platno.fillPath(cesta, fillBrush);
        }else if(nastroj == VLAJKA){
            pero.setColor(Qt::blue);
            docasnePlatno.drawRect(QRect(startP, endP));
            }
        platno.drawImage(0,0,obrazek);
        kreslim = false;
    }else{
        platno.drawImage(0,0,obrazek);
    }
}


void MainWindow::on_actionNovy_triggered()
{
    obrazek = QImage(this->width(),this->height(),QImage::Format_RGB32);
    obrazek.fill(Qt::white);
    pero.setColor(Qt::black);
    pero.setWidth(1);
    QPixmap pix(40,40);
    pix.fill(pero.color());
    barvaNastroje->setIcon(QIcon(pix));

    update();
}


void MainWindow::on_actionOtevrit_triggered()
{
    obrazek.load(QFileDialog::getOpenFileName(
                     this,"Otevřít soubor","",
                     "Pouze PNG,JPG,JPEG,BMP (*png *jpg *jpeg *bmp)"),
                     "PNG,JPG,JPEG,BMP"
                 );
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    startP = event->pos();
    stisknuto = true, kreslim=false;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    endP = event->pos();
    stisknuto = false, kreslim = false;
    QPainter painter(&obrazek);
    painter.setPen(pero);
    if(nastroj == PERO){
        painter.drawLine(startP, endP);
    }
    else if(nastroj == OBDELNIK){
        painter.drawRect(QRect(startP, endP));
    }
    else if(nastroj == ELIPSA){
        painter.drawEllipse(QRectF(startP, endP));
    }
    update();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    endP = event->pos();
    update();
}

void MainWindow::setPenColor()
{
    pero.setColor(QColorDialog::getColor());
    QPixmap pix(40,40);
    pix.fill(pero.color());
    barvaNastroje->setIcon(QIcon(pix));

}

void MainWindow::nastavSirku(int sirka)
{
    pero.setWidth(sirka);
}


void MainWindow::on_actionCara_triggered()
{
    nastroj = PERO;
}


void MainWindow::on_actionObdelnik_triggered()
{
    nastroj = OBDELNIK;
}
void MainWindow::on_actionElipsa_triggered()
{
    nastroj=ELIPSA;
}


void MainWindow::on_actionUlo_it_triggered()
{
    //QImage image("cesta/k/obrazku/image.png");

    QString filePath = QFileDialog::getSaveFileName(this, tr("Uložit obrázek"), "", tr("Soubory PNG (*.png);;Soubory JPEG (*.jpg *.jpeg)"));

    if (!filePath.isEmpty()) {
        if(!obrazek.save(filePath)) {
            qDebug() << "Nepodařilo se uložit obrázek.";
        }
    }

}


void MainWindow::on_actionO_Programu_triggered()
{
    QMessageBox::about(this, tr("O Programu"), tr("Program MALOVÁNÍ, Matouš Suchý, E3, 2023"));

}


void MainWindow::on_actionVypln_triggered()
{
    nastroj = VYPLN;
}

void MainWindow::setFillColor()
{
    QColor color = QColorDialog::getColor(Qt::white);
    fillBrush.setColor(color);
    QPixmap pix(40,40);
    pix.fill(fillBrush.color());
    ui->actionVypln->setIcon(QIcon(pix));
}
