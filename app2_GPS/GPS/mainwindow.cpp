#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap             pix;
    QGraphicsPixmapItem *myI = new QGraphicsPixmapItem();
    QImage              *mapImg = new QImage("C:/Users/nole_p/Desktop/gps/app1_Editor/map/map-200m.png");

    this->isRoadDrawing = false;
    this->scene = new QGraphicsScene();
    this->scene->setSceneRect(0.0, 0.0, 3793.0, 2704.0);
    pix = pix.fromImage(*mapImg, Qt::AutoColor);
    myI->setPixmap(pix);
    this->scene->addItem(myI);
    this->ui->graphicsView->setScene(this->scene);
    this->ui->graphicsView->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
