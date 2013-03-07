#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Database.hpp"
#include "car.h"
#include "PathFinding.hpp"
#include "GPSVoice.hpp"
#include "RoadReading.hpp"
#include "myqgraphicsellipseitem.h"
#include "mygraphicsview.h"
#include <QFileDialog>
#include <QMap>
#include <QGraphicsEllipseItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    QGraphicsScene                          *scene;
    Ui::MainWindow                          *ui;
    car                                     myCarIsAPlane;
    QMap<QString, PathFinding::PathMode>    displacementModes;
    std::deque<Link *>                      currentPath;
    std::deque<Link *>::const_iterator      currentNodeIndex;
    RoadReading                             *computeRoadEvents;
    GPSVoice                                *voice;
    QImage                                  *myImg;
    QGraphicsPixmapItem                     *myI;

    //faut tout casser
    Node                                    *debugBeginPosition;
    Node                                    *destination;

    enum Mode
    {
        BEGIN,
        END,
        NONE
    };
    Mode    nodeMode;
    Node    *begin;
    Node    *end;


    void                                    clearSearchComboBox();
    void                                    addDirectionMessage(QString);
    void                                    computeDirection(DirectionNext);
public slots:
    // gianni's slots for pathfinding
    void chooseBegin();
    void chooseEnd();
    void parcourir();
    void selectNode();
    void doPathFinding();

    void            speedChanged(int);
    void            searchLineEditTextChanged(const QString & text);
    void            carMoved(double);
    void            moveCar();
    void            stopCar();
//    void            updateCarPosN();
//    void            updateCarPosS();
//    void            updateCarPosE();
//    void            updateCarPosW();
//    void            updateCarPosNW();
//    void            updateCarPosNE();
//    void            updateCarPosSW();
//    void            updateCarPosSE();
    void            updateDisplayCarPos();
    void            launchSearch();
};

#endif // MAINWINDOW_H
