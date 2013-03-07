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
    QImage              *mapImg = new QImage("../map/map-200m.png");

    this->isRoadDrawing = false;
    this->scene = new QGraphicsScene();
    this->scene->setSceneRect(0.0, 0.0, 1000.0, 1000.0);
    pix = pix.fromImage(*mapImg, Qt::AutoColor);
    myI->setPixmap(pix);
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
    if (this->ui->txtName->text().trimmed() != ""
            && this->ui->txtSpeed->text().trimmed() != "")
    {
        this->LeaveRoadCreationMode();
        this->roads.push_back(this->ui->mapDisplay->FinishRoadCreation(this->ui->txtName->text().trimmed().toStdString(), this->ui->txtSpeed->text().toInt()));

    }
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

void MainWindow::on_btCancel_clicked()
{
    this->LeaveRoadCreationMode();
    this->ui->mapDisplay->CancelRoadCreation();
}

void MainWindow::on_btSave_clicked()
{
    QStringList list;
    QFileDialog* fd = new QFileDialog( this, "Choose a name for your map!");

    fd->setFileMode(QFileDialog::AnyFile);
    fd->setAcceptMode(QFileDialog::AcceptSave);
    fd->setFilter(QDir::Writable);

    if ( fd->exec() == QDialog::Accepted )
    {
        list = fd->selectedFiles();
        this->databasePath = list.front().toStdString();
        if (Database::get().init(this->databasePath))
        {
            for (auto it = this->roads.begin(); it != this->roads.end(); ++it)
            {
                Database::get().addRoad(*(*it));
            }
            for (auto it = this->ui->mapDisplay->nodes.begin(); it != this->ui->mapDisplay->nodes.end(); ++it)
            {
                Database::get().addNode(*(*it), false);
            }
            for (auto it = this->ui->mapDisplay->nodes.begin(); it != this->ui->mapDisplay->nodes.end(); ++it)
            {
                Database::get().addNode(*(*it), true);
            }
        }
    }
}

void MainWindow::on_txtWidth_returnPressed()
{
    QString *tmp;

    if (this->ui->txtWidth->text().toInt() > 0)
    {
        this->scene->setSceneRect(0.0, 0.0, this->ui->txtWidth->text().toInt(), this->ui->txtHeight->text().toInt());
        this->mapWidth = this->ui->txtWidth->text().toInt();
        this->mapHeight = this->ui->txtHeight->text().toInt();
    }
    else
    {
        tmp = new QString(Converter::toString(this->mapWidth).c_str());
        this->ui->txtWidth->setText(*tmp);
    }
}

void MainWindow::on_txtHeight_returnPressed()
{
    QString *tmp;

    if (this->ui->txtHeight->text().toInt() > 0)
    {
        this->scene->setSceneRect(0.0, 0.0, this->ui->txtWidth->text().toInt(), this->ui->txtHeight->text().toInt());
        this->mapWidth = this->ui->txtWidth->text().toInt();
        this->mapHeight = this->ui->txtHeight->text().toInt();
    }
    else
    {
        tmp = new QString(Converter::toString(this->mapHeight).c_str());
        this->ui->txtHeight->setText(*tmp);
    }
}

void MainWindow::on_btBg_clicked()
{
    QStringList list;
    QFileDialog* fd = new QFileDialog( this, "Choose a name for your map!");
    QImage              *mapImg;
    QString *tmp;

    fd->setFileMode(QFileDialog::ExistingFile);
    fd->setAcceptMode(QFileDialog::AcceptOpen);
    fd->setFilter(QDir::Files & QDir::Readable);

    if ( fd->exec() == QDialog::Accepted )
    {
        list = fd->selectedFiles();
        mapImg = new QImage(list.front());
        this->scene->setBackgroundBrush(QBrush(*mapImg));
        this->mapWidth = mapImg->width();
        this->mapHeight = mapImg->height();
        this->scene->setSceneRect(0.0, 0.0, this->mapWidth, this->mapHeight);
        tmp = new QString(Converter::toString(this->mapWidth).c_str());
        this->ui->txtWidth->setText(*tmp);
        tmp = new QString(Converter::toString(this->mapHeight).c_str());
        this->ui->txtHeight->setText(*tmp);
    }
}
