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
    RoadReading                             *computeRoadEvents;
    GPSVoice                                *voice;
    QImage                                  *myImg;
    QGraphicsPixmapItem                     *myI;

    quint16                                 indexNode;

    enum Mode
    {
        BEGIN,
        END,
        NONE
    };
    Mode    nodeMode;
    Node    *begin;
    Node    *end;
    qreal   currentScale;
    bool    waitingForDecision;


    void                                    drawPath(std::deque<Link *> &result);
    void                                    clearSearchComboBox();
    void                                    addDirectionMessage(QString);
    void                                    computeDirection(DirectionNext);
public slots:
    // gianni's slots for pathfinding
    void chooseBegin();
    void chooseEnd();
    void parcourir();
    void selectNode();

    //for voice's language change
    void            changeVoice(const QString &);

    // zoom
    void zoomChanged(int);
    void            speedChanged(int);
    void            searchLineEditTextChanged(const QString & text);
    void            carMoved(double);
    void            moveCar();
    void            stopCar();
    void            updateDisplayCarPos();
    void            launchSearch();
    void            centerCamera();
};

#endif // MAINWINDOW_H
