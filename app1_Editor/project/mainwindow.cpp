#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsSceneMouseEvent>

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
    this->ui->mapDisplay->setScene(this->scene);
    this->ui->mapDisplay->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btAddRoad_clicked()
{
    this->EnterRoadCreationMode();
}

void MainWindow::on_btFinishRoad_clicked()
{
    this->LeaveRoadCreationMode();
    this->ui->mapDisplay->finishRoadCreation();
}

void MainWindow::EnterRoadCreationMode()
{
    this->isRoadDrawing = true;
    this->ui->gbRoadInfo->setEnabled(true);
    this->ui->mapDisplay->setDragMode(QGraphicsView::NoDrag);
    this->ui->mapDisplay->isRoadDrawing = true;
    this->ui->btAddRoad->setEnabled(false);
}

void MainWindow::LeaveRoadCreationMode()
{
    this->isRoadDrawing = false;
    this->ui->gbRoadInfo->setEnabled(false);
    this->ui->mapDisplay->setDragMode(QGraphicsView::ScrollHandDrag);
    this->ui->mapDisplay->isRoadDrawing = false;
    this->ui->btAddRoad->setEnabled(true);
}

void MainWindow::SaveRoad()
{

}

void MainWindow::on_btCancel_clicked()
{
    this->LeaveRoadCreationMode();
    this->ui->mapDisplay->CancelRoadCreation();
}
