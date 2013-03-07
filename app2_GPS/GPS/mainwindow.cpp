#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "Database.hpp"
#include "myqgraphicsellipseitem.h"
#include "PathFinding.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), nodeMode(NONE), begin(0), end(0)
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

    connect(this->ui->choosebegin, SIGNAL(released()), this, SLOT(chooseBegin()));
    connect(this->ui->chooseend, SIGNAL(released()), this, SLOT(chooseEnd()));
    connect(this->ui->searchbutton, SIGNAL(released()), this, SLOT(doPathFinding()));
    connect(this->ui->toolButton, SIGNAL(released()), this, SLOT(parcourir()));
    connect(this->ui->graphicsView->scene(), SIGNAL(selectionChanged()), this, SLOT(selectNode()));
}

void MainWindow::chooseBegin()
{
    this->nodeMode = MainWindow::BEGIN;
}

void MainWindow::chooseEnd()
{
    this->nodeMode = MainWindow::END;
}

void MainWindow::doPathFinding()
{
    if (!this->begin || !this->end)
        return ;
    PathFinding::get().setBegin(this->begin);
    PathFinding::get().setEnd(this->end);
    PathFinding::get().resolve(PathFinding::FASTEST);
    std::deque<Link *> const &list = PathFinding::get().getResult();
    Node *prev = this->begin;
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        Node *cur = (*it)->node;
    }
}

void MainWindow::selectNode()
{
    std::cout << "select" << std::endl;
    QList<QGraphicsItem *> &list = this->ui->graphicsView->scene()->selectedItems();
    if (list.empty())
        return ;
    MyQGraphicsEllipseItem *item = static_cast<MyQGraphicsEllipseItem *>(list.front());
    if (this->nodeMode == MainWindow::BEGIN)
        this->begin = item->node;
    else if (this->nodeMode == MainWindow::END)
        this->end = item->node;
}

void MainWindow::parcourir()
{
    QString sFileName;
    QStringList list;
    QFileDialog* fd = new QFileDialog( this, "Choose an image file to open");

    fd->setFileMode(QFileDialog::ExistingFiles);
//    fd->setFilter( "Images (*.png *.gif *.jpg)" );

    if ( fd->exec() == QDialog::Accepted )
    {
        list = fd->selectedFiles();
        sFileName = list.front();
        if (Database::get().init(sFileName.toStdString()/*"test"*/))
        {
            Database::get().load();
            std::cout << "database found" << std::endl;
            // ajout d'un graph

   /*         Node *n1 = new Node(0 * 100, 0 * 100);
            Node *n2 = new Node(5 * 100, 0 * 100);
            Node *n3 = new Node(2 * 100, 1 * 100);
            Node *n4 = new Node(2 * 100, 2 * 100);
            Node *n5 = new Node(4 * 100, 2 * 100);
            Node *n6 = new Node(1 * 100, 4 * 100);
            Node *n7 = new Node(3 * 100, 4 * 100);
            Node *n8 = new Node(5 * 100, 4 * 100);
            Node *n9 = new Node(2 * 100, 5 * 100);
            Node *n10 = new Node(4 * 100, 5 * 100);

            Road *road = new Road("youpi", 30);
            Road *road2 = new Road("youpa", 120);

            n1->addLink(*n3, 1, road);
            n3->addLink(*n1, 1, road);

            n2->addLink(*n3, 1, road);
            n3->addLink(*n2, 1, road);

            n2->addLink(*n4, 1, road);
            n4->addLink(*n2, 1, road);

            n2->addLink(*n5, 1, road);
            n5->addLink(*n2, 1, road);

            n4->addLink(*n5, 1, road);
            n5->addLink(*n4, 1, road);

            n4->addLink(*n7, 1, road);
            n7->addLink(*n4, 1, road);

            n5->addLink(*n8, 1, road);
            n8->addLink(*n5, 1, road);

            n10->addLink(*n8, 1, road);
            n8->addLink(*n10, 1, road);

            n10->addLink(*n7, 1, road);
            n7->addLink(*n10, 1, road);

            n9->addLink(*n7, 1, road2);
            n7->addLink(*n9, 1, road2);

            n9->addLink(*n6, 1, road2);
            n6->addLink(*n9, 1, road2);

            n1->addLink(*n6, 1, road2);
            n6->addLink(*n1, 1, road2);

            Database::get().addRoad(*road);
            Database::get().addRoad(*road2);
            Database::get().addNode(*n1, false);
            Database::get().addNode(*n2, false);
            Database::get().addNode(*n3, false);
            Database::get().addNode(*n4, false);
            Database::get().addNode(*n5, false);
            Database::get().addNode(*n6, false);
            Database::get().addNode(*n7, false);
            Database::get().addNode(*n8, false);
            Database::get().addNode(*n9, false);
            Database::get().addNode(*n10, false);

            Database::get().addNode(*n1, true);
            Database::get().addNode(*n2, true);
            Database::get().addNode(*n3, true);
            Database::get().addNode(*n4, true);
            Database::get().addNode(*n5, true);
            Database::get().addNode(*n6, true);
            Database::get().addNode(*n7, true);
            Database::get().addNode(*n8, true);
            Database::get().addNode(*n9, true);
            Database::get().addNode(*n10, true);*/
            ////////////////////


            for (auto it = Database::get().nodes.begin(); it != Database::get().nodes.end(); ++it)
            {
                std::cout << "database found" << std::endl;
                Node *tmpnode = it->second;

//                this->ui->graphicsView->points.push_back(this->ui->graphicsView->scene()->addEllipse(-4, -4, 8, 8, this->ui->graphicsView->nodePen, this->ui->graphicsView->node));
                QGraphicsEllipseItem *item = new MyQGraphicsEllipseItem(-4, -4, 8, 8, tmpnode);
                item->setPen(this->ui->graphicsView->nodePen);
                item->setBrush(this->ui->graphicsView->node);
                QPointF pos;
                pos.setX(tmpnode->getX());
                pos.setY(tmpnode->getY());
                item->setPos(pos);
                item->setFlags(/*QGraphicsItem::ItemIsMovable | */QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
                this->ui->graphicsView->scene()->addItem(item);
                this->ui->graphicsView->points.push_back(item);

                for (auto it2 = tmpnode->getLinks().begin(); it2 != tmpnode->getLinks().end(); ++it2)
                {
                    Node *node2 = (*it2).node;
                    QPointF pos2;
                    pos2.setX(node2->getX());
                    pos2.setY(node2->getY());
                    this->ui->graphicsView->lines.push_back(this->ui->graphicsView->scene()->addLine(tmpnode->getX(), tmpnode->getY(), node2->getX(), node2->getY(), this->ui->graphicsView->linePen));
                }
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
