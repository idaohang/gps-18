#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Converter.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    computeRoadEvents(NULL),
    voice(NULL),
    begin(NULL),
    end(NULL),
    nodeMode(NONE),
    currentScale(1),
    waitingForDecision(false)
{
    ui->setupUi(this);

    // Init scene
    this->scene = new QGraphicsScene();
    this->scene->setSceneRect(0.0, 0.0, 3793.0, 2704.0);
    this->myImg = new QImage("../common/voiture.png");
    this->myI = new QGraphicsPixmapItem();
    QPixmap pix;
    pix = pix.fromImage( *(this->myImg), Qt::AutoColor );
    this->myI->setPixmap(pix);
    this->myI->setOffset( -0.5 * QPointF( pix.size().width(), pix.size().height() ) );
    this->scene->addItem(this->myI);
    //this->ui->graphicsView->scale(0.2, 0.2);
    this->ui->graphicsView->setRenderHints( QPainter::Antialiasing );
    this->ui->graphicsView->setScene(this->scene);
    this->ui->graphicsView->show();

    // Search line and database init
    connect(this->ui->searchLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(searchLineEditTextChanged(const QString &)));
    Database::get().init("../common");
    connect(this->ui->launchSearch, SIGNAL(clicked()), this, SLOT(launchSearch()));

    // Car setup
    connect(&this->myCarIsAPlane, SIGNAL(moved()), this, SLOT(updateDisplayCarPos()));
    connect(&this->myCarIsAPlane, SIGNAL(distanceTraveled(double)), this, SLOT(carMoved(double)));
    connect(this->ui->speedSlider, SIGNAL(valueChanged(int)), this, SLOT(speedChanged(int)));

// tmp speed
    this->myCarIsAPlane.setSpeed(50);

    // gianni's buttons for pathfinding
    connect(this->ui->choosebegin, SIGNAL(released()), this, SLOT(chooseBegin()));
    connect(this->ui->chooseend, SIGNAL(released()), this, SLOT(chooseEnd()));
    connect(this->ui->toolButton, SIGNAL(released()), this, SLOT(parcourir()));
    connect(this->ui->graphicsView->scene(), SIGNAL(selectionChanged()), this, SLOT(selectNode()));

    // Move buttons
    this->ui->stopCarButton->setEnabled(false);
    connect(this->ui->stopCarButton, SIGNAL(clicked()), this, SLOT(stopCar()));
    connect(this->ui->startCarButton, SIGNAL(clicked()), this, SLOT(moveCar()));
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

    // Init voice
    this->voice = new GPSVoice();
    this->ui->languageList->addItem("English");
    this->ui->languageList->addItem("Francais");
    this->ui->languageList->addItem("汉语");
    connect(this->ui->languageList, SIGNAL(currentTextChanged(QString)), this, SLOT(changeVoice(QString)));

    // zoom
    this->ui->zoomSlider->setValue(10);
    connect(this->ui->zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(zoomChanged(int)));

    this->myI->setZValue(100);
    this->myI->setPos(this->scene->width() / 2, this->scene->height() / 2);

    // center camera
    connect(this->ui->centerCamera, SIGNAL(clicked()), this, SLOT(centerCamera()));
}

void MainWindow::centerCamera()
{
    this->ui->graphicsView->centerOn(QPointF(this->myCarIsAPlane.getX(), this->myCarIsAPlane.getY()));
}

void MainWindow::zoomChanged(int zoom)
{
    qreal newScale = zoom / 10.;
    this->ui->graphicsView->scale(newScale / this->currentScale, newScale / this->currentScale);
    this->currentScale = newScale;
}

void MainWindow::changeVoice(const QString & newLanguage)
{
    QString lang = newLanguage;
    if (lang == "Francais")
        this->voice->setLanguage(LanguageCountry::FR);
    else if (lang == "English")
        this->voice->setLanguage(LanguageCountry::EN);
    else if (lang == "汉语")
        this->voice->setLanguage(LanguageCountry::CN);
}

// function called each 1/10 second
void MainWindow::carMoved(double distance)
{
    if (this->myCarIsAPlane.getNextNode() == NULL)
        return;
    double distanceUntilNextNode = sqrt(pow((this->myCarIsAPlane.getX() - this->myCarIsAPlane.getNextNode()->getX()), 2) +
                                        pow((this->myCarIsAPlane.getY() - this->myCarIsAPlane.getNextNode()->getY()), 2));

    // computes new position
    double alpha = atan2(this->myCarIsAPlane.getNextNode()->getY() - this->myCarIsAPlane.getY(),
                         this->myCarIsAPlane.getNextNode()->getX() - this->myCarIsAPlane.getX());
    double newPosX = this->myCarIsAPlane.getX() + distance * cos(alpha);
    double newPosY = this->myCarIsAPlane.getY() + distance * sin(alpha);
    this->myCarIsAPlane.setPos(newPosX, newPosY);

    // directions

    DirectionNext myNextDirection = this->computeRoadEvents->update(this->myCarIsAPlane.getX(),
                                                                    this->myCarIsAPlane.getY(),
                                                                    1,
                                                                    distance);
    computeDirection(myNextDirection);
    this->voice->updateVoice(myNextDirection);


    this->myI->setPos(this->myCarIsAPlane.getX(),
                      this->myCarIsAPlane.getY());
    this->myI->setRotation((alpha + ::PI / 2) * (180 / ::PI));
    if (this->ui->followingCamera->isChecked())
        this->ui->graphicsView->centerOn(QPointF(this->myCarIsAPlane.getX(), this->myCarIsAPlane.getY()));

    // reached the next node
    if (distance - distanceUntilNextNode >= 0.)
    {
        double nextX;
        double nextY;
        nextX = this->currentPath[this->indexNode]->node->getX();
        nextY = this->currentPath[this->indexNode]->node->getY();

        Node *node = this->currentPath[this->indexNode]->node;

        this->begin = node;
        ++(this->indexNode);

        // reached goal
        if (this->indexNode >= this->currentPath.size())
        {
            this->myCarIsAPlane.setNodes(NULL, NULL);
            this->myCarIsAPlane.stopMoving();
            this->myCarIsAPlane.setPos(nextX, nextY);
            this->stopCar();
            this->begin = node;
            this->end = NULL;
            this->ui->startCarButton->setEnabled(false);
            return ;
        }
        this->addDirectionMessage("Checkpoint reached.");
        this->myCarIsAPlane.setNodes(this->myCarIsAPlane.getNextNode(), this->currentPath[this->indexNode]->node);

        if (this->ui->autoAdjustSpeed->isChecked())
        {
            this->myCarIsAPlane.setSpeed(this->currentPath[this->indexNode]->road->getSpeed());
            this->addDirectionMessage("Automatically adjusting speed to " + QString::number(this->currentPath[this->indexNode]->road->getSpeed()) + ".");
        }
        this->addDirectionMessage("You are now on the road \"" + QString(this->currentPath[this->indexNode]->road->getName().c_str()) + "\".");
        if ((this->ui->followPath->isChecked() == false) && (this->currentPath[this->indexNode]->node->getLinks().size() > 2))
        {
            this->myCarIsAPlane.setSpeed(0);
            this->waitingForDecision = true;
            this->nodeMode = Mode::NONE;
        }
        else
            carMoved(distance - distanceUntilNextNode);
    }
}

// Gianni's functions for pathfinding
void MainWindow::chooseBegin()
{
    this->waitingForDecision = false;
    this->nodeMode = MainWindow::BEGIN;
}

void MainWindow::chooseEnd()
{
    this->waitingForDecision = false;
    this->nodeMode = MainWindow::END;
}

void MainWindow::selectNode()
{
    QList<QGraphicsItem *> &list = this->ui->graphicsView->scene()->selectedItems();
    if (list.empty())
        return ;
    MyQGraphicsEllipseItem *item = static_cast<MyQGraphicsEllipseItem *>(list.front());
    item->setSelected(false);

    if (this->waitingForDecision == true)
    {
        Node * node = item->node;
        Node * currentNode = this->currentPath[this->indexNode - 1]->node;
        for (auto it = currentNode->getLinks().begin(); it != currentNode->getLinks().end(); ++it)
        {
            if (it->node == node)
            {
                if ((this->indexNode < this->currentPath.size()) && (this->currentPath[this->indexNode]->node == node))
                {
                    this->waitingForDecision = false;
                    this->myCarIsAPlane.setSpeed(this->currentPath[this->indexNode]->road->getSpeed());
                }
                else
                {
                    std::cout << "Next node" << std::endl;
                    this->begin = currentNode;
                    // get path
                    PathFinding &path = PathFinding::get();
                    path.setBegin(node);

                    path.resolve(this->displacementModes[this->ui->displacementMode->currentText()]);
                    std::deque<Link *> &result = path.getResult();
                    result.push_front(&(*it));

                    // reset path
                    for (auto it = this->ui->graphicsView->secondLines.begin(); it != this->ui->graphicsView->secondLines.end(); ++it)
                    {
                        this->scene->removeItem(*it);
                    }
                    drawPath(result);
                    this->currentPath = result;
                    this->indexNode = 0;
                    this->myCarIsAPlane.setNodes(currentNode, node);
                    this->computeRoadEvents->setNewRoad(this->currentPath);
                }
            }
        }
        return ;
    }
    if (this->nodeMode == MainWindow::BEGIN)
    {
        QGraphicsEllipseItem *item2 = new QGraphicsEllipseItem(-4, -4, 8, 8);
        if (this->ui->graphicsView->beginPoint)
            this->ui->graphicsView->scene()->removeItem(this->ui->graphicsView->beginPoint);
        this->ui->graphicsView->beginPoint = item2;
        item2->setPen(this->ui->graphicsView->beginPen);
        item2->setBrush(this->ui->graphicsView->beginBrush);
        QPointF pos;
        pos.setX(item->node->getX());
        pos.setY(item->node->getY());
        item2->setPos(pos);
        this->ui->graphicsView->scene()->addItem(item2);

        this->begin = item->node;
    }
    else if (this->nodeMode == MainWindow::END)
    {
        QGraphicsEllipseItem *item2 = new QGraphicsEllipseItem(-4, -4, 8, 8);
        if (this->ui->graphicsView->endPoint)
            this->ui->graphicsView->scene()->removeItem(this->ui->graphicsView->endPoint);
        this->ui->graphicsView->endPoint = item2;
        item2->setPen(this->ui->graphicsView->endPen);
        item2->setBrush(this->ui->graphicsView->endBrush);
        QPointF pos;
        pos.setX(item->node->getX());
        pos.setY(item->node->getY());
        item2->setPos(pos);
        this->ui->graphicsView->scene()->addItem(item2);
        this->end = item->node;
    }
}

void MainWindow::parcourir()
{
    QString sFileName;
    QStringList list;
    QFileDialog* fd = new QFileDialog( this, "Choose an image file to open");

    fd->setFileMode(QFileDialog::ExistingFiles);

    if ( fd->exec() == QDialog::Accepted )
    {
        list = fd->selectedFiles();
        sFileName = list.front();
        if (Database::get().init(sFileName.toStdString()))
        {
            Database::get().load();
            // load background image

            if (Database::get().images.size() > 0)
            {
                std::string img;
                QImage              *mapImg;
                img = "../app1_Editor/map/";
                img += (*Database::get().images.rbegin()).second->path;
                mapImg = new QImage(img.c_str());
                this->scene->setBackgroundBrush(QBrush(*mapImg));
                int mapWidth = mapImg->width();
                int mapHeight = mapImg->height();
                this->scene->setSceneRect(0.0, 0.0, mapWidth, mapHeight);
                QString *tmp = new QString(Converter::toString(mapWidth).c_str());
                tmp = new QString(Converter::toString(mapHeight).c_str());
            }
            //////////////////////////

            for (auto it = Database::get().nodes.begin(); it != Database::get().nodes.end(); ++it)
            {
                Node *tmpnode = it->second;

                QGraphicsEllipseItem *item = new MyQGraphicsEllipseItem(-4, -4, 8, 8, tmpnode);
                item->setPen(this->ui->graphicsView->nodePen);
                item->setBrush(this->ui->graphicsView->node);
                QPointF pos;
                pos.setX(tmpnode->getX());
                pos.setY(tmpnode->getY());
                item->setPos(pos);
                item->setFlags(QGraphicsItem::ItemIsSelectable);
                this->ui->graphicsView->scene()->addItem(item);
                this->ui->graphicsView->points.push_back(item);

                for (auto it2 = tmpnode->getLinks().begin(); it2 != tmpnode->getLinks().end(); ++it2)
                {
                    Node *node2 = (*it2).node;
                    QPointF pos2;
                    pos2.setX(node2->getX());
                    pos2.setY(node2->getY());
                    this->ui->graphicsView->linePen.setWidth((*it2).road->getSpeed() / 20.0 + 2);
                    this->ui->graphicsView->lines.push_back(this->ui->graphicsView->scene()->addLine(tmpnode->getX(), tmpnode->getY(), node2->getX(), node2->getY(), this->ui->graphicsView->linePen));
                }
            }
        }
    }
}


void MainWindow::computeDirection(DirectionNext direction)
{
    switch (direction)
    {
    case STRAIGHT_150 :
        this->addDirectionMessage("Go straight in 150 meters.");
        break;
    case STRAIGHT_100 :
        this->addDirectionMessage("Go straight in 100 meters.");
        break;
    case STRAIGHT_50 :
        this->addDirectionMessage("Go straight in 50 meters.");
        break;
    case STRAIGHT :
        this->addDirectionMessage("Go straight.");
        break;
    case TURN_LEFT_150 :
        this->addDirectionMessage("Turn left in 150 meters.");
        break;
    case TURN_LEFT_100 :
        this->addDirectionMessage("Turn left in 100 meters.");
        break;
    case TURN_LEFT_50 :
        this->addDirectionMessage("Turn left in 50 meters.");
        break;
    case TURN_LEFT :
        this->addDirectionMessage("Turn left.");
        break;
    case TURN_RIGHT_150 :
        this->addDirectionMessage("Turn right in 150 meters.");
        break;
    case TURN_RIGHT_100 :
        this->addDirectionMessage("Turn right in 100 meters.");
        break;
    case TURN_RIGHT_50 :
        this->addDirectionMessage("Turn right in 50 meters.");
        break;
    case TURN_RIGHT :
        this->addDirectionMessage("Turn right.");
        break;
    case TURN_BACK :
        this->addDirectionMessage("Turn back asap.");
        break;
    case ARRIVE :
        this->addDirectionMessage("You have reached your destination.");
        break;
    default :
        break;
    }
}

void MainWindow::addDirectionMessage(QString message)
{
    this->ui->directionsOutput->setPlainText(message + "\n" + this->ui->directionsOutput->toPlainText());
}

void MainWindow::launchSearch()
{
    if (!this->begin || !this->end || (this->begin == this->end))
        return ;

    // reset scene
    for (auto it = this->ui->graphicsView->secondLines.begin(); it != this->ui->graphicsView->secondLines.end(); ++it)
    {
        this->scene->removeItem(*it);
    }

    // get path
    PathFinding &path = PathFinding::get();
    path.setBegin(this->begin);
    path.setEnd(this->end);

    path.resolve(this->displacementModes[this->ui->displacementMode->currentText()]);
    std::deque<Link *> &result = path.getResult();

    this->ui->startCarButton->setEnabled(true);

    this->nodeMode = Mode::NONE;

    // draw path
    drawPath(result);

    // directions stuff
    if (this->computeRoadEvents == NULL)
        this->computeRoadEvents = new RoadReading(result);
    else
        this->computeRoadEvents->setNewRoad(result);

    this->currentPath = result;
    this->indexNode = 0;

    if (this->ui->autoAdjustSpeed->isChecked())
    {
        this->myCarIsAPlane.setSpeed(this->currentPath[this->indexNode]->road->getSpeed());
        this->addDirectionMessage("Automatically adjusting speed to " + QString::number(this->currentPath[this->indexNode]->road->getSpeed()) + ".");
    }
    this->addDirectionMessage("You are now on the road \"" + QString(this->currentPath[this->indexNode]->road->getName().c_str()) + "\".");

    this->myCarIsAPlane.setNodes(this->begin, (result.size() > 1) ? ((*(result.begin()))->node) : (this->begin));
    this->myCarIsAPlane.setPos(this->begin->getX(), this->begin->getY());

    this->myI->setPos(this->myCarIsAPlane.getX(),
                      this->myCarIsAPlane.getY());
}

void MainWindow::drawPath(std::deque<Link *> &result)
{
    // draw path
    Node *prev = this->begin;
    for (auto it = result.begin(); it != result.end(); ++it)
    {
        Node *cur = (*it)->node;
        this->ui->graphicsView->secondLines.push_back(this->ui->graphicsView->scene()->addLine(prev->getX(),
                                                                                               prev->getY(),
                                                                                               cur->getX(),
                                                                                               cur->getY(),
                                                                                               this->ui->graphicsView->secondLinePen));

        prev = cur;
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
    text += "[Car speed] : ";
    text += QString::number(this->myCarIsAPlane.getSpeed());
    text += "km/h\n";
    this->ui->carData->setPlainText(text);

    QString text2;
    QString distanceUntilNextNode;

    if (this->myCarIsAPlane.getNextNode() != NULL)
        distanceUntilNextNode = QString::number(sqrt(pow((this->myCarIsAPlane.getX() - this->myCarIsAPlane.getNextNode()->getX()), 2) +
                                                     pow((this->myCarIsAPlane.getY() - this->myCarIsAPlane.getNextNode()->getY()), 2)));
    else
        distanceUntilNextNode = "(NA)";
    text2 = "Destination data\n";
    text2 += "[Next node X axis] : ";
    if (this->myCarIsAPlane.getNextNode() != NULL)
        text2 += QString::number(this->myCarIsAPlane.getNextNode()->getX());
    else
        text2 += "(NA)";
    text2 += "\n";
    text2 += "[Next node Y axis] : ";
    if (this->myCarIsAPlane.getNextNode() != NULL)
        text2 += QString::number(this->myCarIsAPlane.getNextNode()->getY());
    else
        text2 += "(NA)";
    text2 += "\n";
    text2 += "[Distance until next node] : ";
    text2 += distanceUntilNextNode;
    this->ui->carData_2->setPlainText(text2);
    this->ui->speedSlider->setValue(this->myCarIsAPlane.getSpeed());
}

void MainWindow::moveCar()
{
    if (this->currentPath.size() == 0)
        return;
    this->ui->startCarButton->setEnabled(false);
    this->ui->stopCarButton->setEnabled(true);
    this->myCarIsAPlane.startMoving();
    if (this->myCarIsAPlane.getSpeed() == 0)
        this->myCarIsAPlane.setSpeed(50);
    this->ui->speedSlider->setSliderPosition(this->myCarIsAPlane.getSpeed());
}

void MainWindow::stopCar()
{
    this->ui->startCarButton->setEnabled(true);
    this->ui->stopCarButton->setEnabled(false);
    this->myCarIsAPlane.stopMoving();
    this->ui->speedSlider->setSliderPosition(this->myCarIsAPlane.getSpeed());
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

void            MainWindow::speedChanged(int newSpeed)
{
    if (newSpeed < 0)
        return;
    if (newSpeed != 0)
        this->waitingForDecision = false;
    this->myCarIsAPlane.setSpeed(newSpeed);
}

MainWindow::~MainWindow()
{
    delete ui;
}
