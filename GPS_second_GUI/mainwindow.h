#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Database.hpp"
#include "car.h"
#include "PathFinding.hpp"
#include "GPSVoice.hpp"
#include "RoadReading.hpp"
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
    Ui::MainWindow                          *ui;
    car                                     myCarIsAPlane;
    QMap<QString, PathFinding::PathMode>    displacementModes;
    std::deque<Link *>                      currentPath;
    std::deque<Link *>::const_iterator      currentNodeIndex;
    RoadReading                             *computeRoadEvents;
    GPSVoice                                *voice;
    QImage                                  *myImg;
    QGraphicsPixmapItem                     *myI;

    Node                                    *debugBeginPosition;
    Node                                    *destination;

    void                                    clearSearchComboBox();
    void                                    addDirectionMessage(QString);
    void                                    computeDirection(DirectionNext);
public slots:
    void            speedChanged(int);
    void            searchLineEditTextChanged(const QString & text);
    void            carMoved(double);
    void            moveCar();
    void            stopCar();
    void            updateCarPosN();
    void            updateCarPosS();
    void            updateCarPosE();
    void            updateCarPosW();
    void            updateCarPosNW();
    void            updateCarPosNE();
    void            updateCarPosSW();
    void            updateCarPosSE();
    void            updateDisplayCarPos();
    void            launchSearch();
};

#endif // MAINWINDOW_H
