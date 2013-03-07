#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsEllipseItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Search line and database init
    connect(this->ui->searchLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(searchLineEditTextChanged(const QString &)));
    Database::get().init("../common");
    connect(this->ui->launchSearch, SIGNAL(clicked()), this, SLOT(launchSearch()));

    // Car setup
    connect(&this->myCarIsAPlane, SIGNAL(moved()), this, SLOT(updateDisplayCarPos()));

    // Move buttons
    this->ui->stopCarButton->setEnabled(false);
    connect(this->ui->stopCarButton, SIGNAL(clicked()), this, SLOT(stopCar()));
    connect(this->ui->startCarButton, SIGNAL(clicked()), this, SLOT(moveCar()));
    connect(this->ui->pushButtonUp, SIGNAL(pressed()), this, SLOT(updateCarPosN()));
    connect(this->ui->pushButtonDown, SIGNAL(pressed()), this, SLOT(updateCarPosS()));
    connect(this->ui->pushButtonRight, SIGNAL(pressed()), this, SLOT(updateCarPosE()));
    connect(this->ui->pushButtonLeft, SIGNAL(pressed()), this, SLOT(updateCarPosW()));
    connect(this->ui->pushButtonUpLeft, SIGNAL(pressed()), this, SLOT(updateCarPosNW()));
    connect(this->ui->pushButtonUpRight, SIGNAL(pressed()), this, SLOT(updateCarPosNE()));
    connect(this->ui->pushButtonDownLeft, SIGNAL(pressed()), this, SLOT(updateCarPosSW()));
    connect(this->ui->pushButtonDownRight, SIGNAL(pressed()), this, SLOT(updateCarPosSE()));
    this->updateDisplayCarPos();
    connect(&this->myCarIsAPlane, SIGNAL(moved()), this, SLOT(updateDisplayCarPos()));

    // Init values
    this->displacementModes["Shortest"] = PathFinding::SHORTEST;
    this->displacementModes["Fastest"] = PathFinding::FASTEST;

    for (QMap<QString, PathFinding::PathMode>::Iterator it = this->displacementModes.begin();
         it != this->displacementModes.end();
         it++)
    {
        this->ui->displacementMode->addItem(it.key());
    }

    QGraphicsScene *scene = new QGraphicsScene();
    QImage *myImg = new QImage("C:\\Users\\hameli_p\\Pictures\\WOOT.png");
    QGraphicsPixmapItem *myI = new QGraphicsPixmapItem();
    QPixmap pix;
    pix = pix.fromImage( *myImg, Qt::AutoColor );
    myI->setPixmap(pix);
    scene->addItem(myI);
    this->ui->graphicsViewMap->scale(0.2, 0.2);
    this->ui->graphicsViewMap->setRenderHints( QPainter::Antialiasing );
    this->ui->graphicsViewMap->setScene(scene);
    this->ui->graphicsViewMap->show();

}

void MainWindow::carMoved(quint16 distance)
{

}

void MainWindow::launchSearch()
{
    if (this->ui->searchComboBox->currentText() == "")
        return ;
//    std::cout << this->ui->searchComboBox->
    Node *n1 = new Node(0 * 100, 0 * 100);
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

    PathFinding &path = PathFinding::get();

    path.setBegin(n4);
    path.setEnd(n1);
    path.resolve(this->displacementModes[this->ui->displacementMode->currentText()]);
    std::deque<Link *> const &result = path.getResult();

    this->currentPath = result;

    this->myCarIsAPlane.setPos(n4->getX(), n4->getY());

    for (std::deque<Link *>::const_iterator it = this->currentPath.begin(); it != this->currentPath.end(); it++)
    {
        std::cout << (*it)->node->getX() << " | " << (*it)->node->getY() << std::endl;
    }
}

void MainWindow::updateDisplayCarPos()
{
    QString text;
    text = "Car data\n";
    text += "[X axis] : ";
    text += QString::number(this->myCarIsAPlane.getX());
    text += "\n";
    text += "[Y axis] : ";
    text += QString::number(this->myCarIsAPlane.getY());
    text += "\n";
    this->ui->carData->setPlainText(text);
}

void MainWindow::moveCar()
{
    if (this->currentPath.size() == 0)
        return;
    this->ui->startCarButton->setEnabled(false);
    this->ui->stopCarButton->setEnabled(true);
    this->myCarIsAPlane.startMoving();
}

void MainWindow::stopCar()
{
    this->ui->startCarButton->setEnabled(true);
    this->ui->stopCarButton->setEnabled(false);
    this->myCarIsAPlane.stopMoving();
}

void MainWindow::searchLineEditTextChanged(const QString & text)
{
    QList<QString> possibilities;
    possibilities.append("Hopital Saint-Amand");
    possibilities.append("Hopital De la Charetiere");
    possibilities.append("Hopital Saint-Patrick");
    possibilities.append("Hotel de l'amitie");
    possibilities.append("Hotel des fous");
    possibilities.append("College Hilton");
    possibilities.append("College POUET CONNARD");

    auto request = Database::get().request("SELECT name FROM roads WHERE name LIKE ?1%");
    request->bind(text.toStdString());

    // column 1 == name
    while (request->next())
    {
        std::string tmpName;
        tmpName = request->get<std::string>(1);
        possibilities.append(QString(tmpName.c_str()));
    }

    this->clearSearchComboBox();
    for (QListIterator<QString> it(possibilities); it.hasNext();)
    {
        QString currentValue = it.next();

        if (text.size() != 0 && text.compare(currentValue.left(text.size()), Qt::CaseInsensitive) == 0)
        {
            this->ui->searchComboBox->addItem(currentValue);
        }
    }
}

void MainWindow::clearSearchComboBox()
{
    while (this->ui->searchComboBox->count() > 0)
    {
        this->ui->searchComboBox->removeItem(0);
    }
}

void            MainWindow::updateCarPosN()
{
    if (!this->ui->pushButtonUp->isDown())
        return;
    QTimer::singleShot(500, this, SLOT(updateCarPosN()));
    this->myCarIsAPlane.setY(this->myCarIsAPlane.getY() - this->myCarIsAPlane.getMoveDistance());
    this->updateDisplayCarPos();
}

void            MainWindow::updateCarPosS()
{
    if (!this->ui->pushButtonDown->isDown())
        return;
    QTimer::singleShot(500, this, SLOT(updateCarPosS()));
    this->myCarIsAPlane.setY(this->myCarIsAPlane.getY() + this->myCarIsAPlane.getMoveDistance());
    this->updateDisplayCarPos();
}

void            MainWindow::updateCarPosE()
{
    if (!this->ui->pushButtonRight->isDown())
        return;
    QTimer::singleShot(500, this, SLOT(updateCarPosE()));
    this->myCarIsAPlane.setX(this->myCarIsAPlane.getX() + this->myCarIsAPlane.getMoveDistance());
    this->updateDisplayCarPos();
}

void            MainWindow::updateCarPosW()
{
    if (!this->ui->pushButtonLeft->isDown())
        return;
    QTimer::singleShot(500, this, SLOT(updateCarPosW()));
    this->myCarIsAPlane.setX(this->myCarIsAPlane.getX() - this->myCarIsAPlane.getMoveDistance());
    this->updateDisplayCarPos();
}

void            MainWindow::updateCarPosNW()
{
    if (!this->ui->pushButtonUpLeft->isDown())
        return;
    QTimer::singleShot(500, this, SLOT(updateCarPosNW()));
    this->myCarIsAPlane.setPos(this->myCarIsAPlane.getX() - this->myCarIsAPlane.getMoveDistance(),
                               this->myCarIsAPlane.getY() - this->myCarIsAPlane.getMoveDistance());
    this->updateDisplayCarPos();
}

void            MainWindow::updateCarPosNE()
{
    if (!this->ui->pushButtonUpRight->isDown())
        return;
    QTimer::singleShot(500, this, SLOT(updateCarPosNE()));
    this->myCarIsAPlane.setPos(this->myCarIsAPlane.getX() + this->myCarIsAPlane.getMoveDistance(),
                               this->myCarIsAPlane.getY() - this->myCarIsAPlane.getMoveDistance());
    this->updateDisplayCarPos();
}

void            MainWindow::updateCarPosSW()
{
    if (!this->ui->pushButtonDownLeft->isDown())
        return;
    QTimer::singleShot(500, this, SLOT(updateCarPosSW()));
    this->myCarIsAPlane.setPos(this->myCarIsAPlane.getX() - this->myCarIsAPlane.getMoveDistance(),
                               this->myCarIsAPlane.getY() + this->myCarIsAPlane.getMoveDistance());
    this->updateDisplayCarPos();
}

void            MainWindow::updateCarPosSE()
{
    if (!this->ui->pushButtonDownRight->isDown())
        return;
    QTimer::singleShot(500, this, SLOT(updateCarPosSE()));
    this->myCarIsAPlane.setPos(this->myCarIsAPlane.getX() + this->myCarIsAPlane.getMoveDistance(),
                               this->myCarIsAPlane.getY() + this->myCarIsAPlane.getMoveDistance());
    this->updateDisplayCarPos();
}

MainWindow::~MainWindow()
{
    delete ui;
}
