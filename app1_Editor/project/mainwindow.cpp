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

    this->databasePath = "";
    this->isRoadDrawing = false;
    this->scene = new QGraphicsScene();
    this->scene->setSceneRect(0.0, 0.0, 1000.0, 1000.0);
    pix = pix.fromImage(*mapImg, Qt::AutoColor);
    myI->setPixmap(pix);
    this->ui->mapDisplay->setScene(this->scene);
    this->ui->mapDisplay->show();

    connect(this->ui->mapDisplay->scene(), SIGNAL(selectionChanged()), this, SLOT(SelectNode()));
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
        this->ui->lstRoads->addItem(QString(this->ui->txtName->text().trimmed()));
        this->ui->txtName->clear();
        this->ui->txtSpeed->setText("90");

        for (auto it = this->roads.begin(); it != this->roads.end(); ++it)
        {
            Database::get().addRoad(*(*it));
        }
        this->roads.clear();
        for (auto it = this->ui->mapDisplay->nodes.begin(); it != this->ui->mapDisplay->nodes.end(); ++it)
        {
            Database::get().addNode(*(*it), false);
        }
        for (auto it = this->ui->mapDisplay->nodes.begin(); it != this->ui->mapDisplay->nodes.end(); ++it)
        {
            Database::get().addNode(*(*it), true);
        }
        this->ui->mapDisplay->nodes.clear();
    }
}

void MainWindow::EnterRoadCreationMode()
{
    this->isRoadDrawing = true;
    this->ui->gbRoadInfo->setEnabled(true);
    this->ui->lstRoads->setEnabled(false);
    this->ui->mapDisplay->setDragMode(QGraphicsView::NoDrag);
    this->ui->mapDisplay->isRoadDrawing = true;
    this->ui->btAddRoad->setEnabled(false);
}

void MainWindow::LeaveRoadCreationMode()
{
    this->isRoadDrawing = false;
    this->ui->gbRoadInfo->setEnabled(false);
    this->ui->lstRoads->setEnabled(true);
    this->ui->mapDisplay->setDragMode(QGraphicsView::ScrollHandDrag);
    this->ui->mapDisplay->isRoadDrawing = false;
    this->ui->btAddRoad->setEnabled(true);
}

void MainWindow::SelectNode()
{
    QList<QGraphicsItem *> &list = this->ui->mapDisplay->scene()->selectedItems();

    if (this->isRoadDrawing)
    {
        if (list.empty())
            return ;
        MyQGraphicsEllipseItem *item = static_cast<MyQGraphicsEllipseItem *>(list.front());
        this->ui->mapDisplay->selected = item;
    }
}

void    MainWindow::LoadDatabase()
{
    QString sFileName;
    QStringList list;
    QFileDialog* fd = new QFileDialog( this, "Choose your map!");
    QImage              *mapImg;
    QString *tmp;
    std::string img;

    fd->setFileMode(QFileDialog::AnyFile);

    this->ui->mapDisplay->scene()->clear();
    this->ui->lstRoads->clear();
    this->roads.clear();
    if ( fd->exec() == QDialog::Accepted )
    {
        list = fd->selectedFiles();
        sFileName = list.front();
        if (Database::get().init(sFileName.toStdString()))
        {
            Database::get().load();
            this->databasePath = sFileName.toStdString();

            if (Database::get().images.size() > 0)
            {
                img = "../map/";
                img += (*Database::get().images.rbegin()).second->path;
                std::cerr << img << std::endl;
                mapImg = new QImage(img.c_str());
                this->scene->setBackgroundBrush(QBrush(*mapImg));
                this->mapWidth = mapImg->width();
                this->mapHeight = mapImg->height();
                this->scene->setSceneRect(0.0, 0.0, this->mapWidth, this->mapHeight);
                tmp = new QString(Converter::toString(this->mapWidth).c_str());
                this->ui->txtWidth->setText(*tmp);
                tmp = new QString(Converter::toString(this->mapHeight).c_str());
                this->ui->txtHeight->setText(*tmp);
            }
            for (auto it = Database::get().roads.begin(); it != Database::get().roads.end(); ++it)
                this->ui->lstRoads->addItem(QString((*it).second->getName().c_str()));
            for (auto it = Database::get().nodes.begin(); it != Database::get().nodes.end(); ++it)
            {
                Node *tmpnode = it->second;

                MyQGraphicsEllipseItem *item = new MyQGraphicsEllipseItem(-4, -4, 8, 8, tmpnode);
                item->setPen(this->ui->mapDisplay->nodePen);
                item->setBrush(this->ui->mapDisplay->node);
                QPointF pos;
                pos.setX(tmpnode->getX());
                pos.setY(tmpnode->getY());
                item->setPos(pos);
                item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
                this->ui->mapDisplay->scene()->addItem(item);
                this->ui->mapDisplay->points.push_back(item);

                for (auto it2 = tmpnode->getLinks().begin(); it2 != tmpnode->getLinks().end(); ++it2)
                {
                    Node *node2 = (*it2).node;
                    QPointF pos2;
                    pos2.setX(node2->getX());
                    pos2.setY(node2->getY());
                    this->ui->mapDisplay->lines.push_back(this->ui->mapDisplay->scene()->addLine(tmpnode->getX(), tmpnode->getY(), node2->getX(), node2->getY(), this->ui->mapDisplay->linePen));
                }
            }
            this->ui->mapDisplay->points.clear();
            this->ui->mapDisplay->lines.clear();
        }
    }
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
    QString         *tmp;
    std::string img;

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
        img = list.front().toStdString();
        img = img.substr(img.rfind("/") + 1);
        std::cerr << img << std::endl;
        Database::get().addImage(*(new Image(img)));
    }
}

void MainWindow::on_rbBothWay_toggled(bool checked)
{
    this->ui->mapDisplay->isBothWay = checked;
}

void MainWindow::on_lstRoads_itemSelectionChanged()
{
    QListWidgetItem *items = this->ui->lstRoads->selectedItems().front();

    for (auto it = this->roads.begin(); it != this->roads.end(); ++it)
    {

    }
}

void MainWindow::on_pushButton_clicked()
{
    this->LoadDatabase();
    if (QFile(QString(this->databasePath.c_str())).exists())
    {
        this->ui->btAddRoad->setEnabled(true);
    }
}
