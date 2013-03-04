#include "mainwindow.h"
#include "ui_mainwindow.h"
 #include <QGraphicsPixmapItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsScene      *scene;
    QPixmap             pix;
    QGraphicsPixmapItem *myI = new QGraphicsPixmapItem();
    QImage              *mapImg = new QImage("C:\\Users\\nole_p\\Pictures\\gps-200m.png");

    scene = new QGraphicsScene();
    scene->setSceneRect(0.0, 0.0, 3793.0, 2704.0);
    pix = pix.fromImage(*mapImg, Qt::AutoColor);
    myI->setPixmap(pix);
    scene->addItem(myI);

    //ui->mapDisplay->scale(1, 1);
    //ui->mapDisplay->setRenderHints( QPainter::Antialiasing );
    //brush = new QBrush(QImage("C:\\Users\\nole_p\\Pictures\\gps-200m.png"));

    //scene->setForegroundBrush(*brush);
    ui->mapDisplay->setScene(scene);
    ui->mapDisplay->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
