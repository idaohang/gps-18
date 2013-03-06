#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Database.hpp"
#include "car.h"
#include "PathFinding.hpp"
#include <QMap>

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
    void                                    clearSearchComboBox();
    car                                     myCarIsAPlane;
    QMap<QString, PathFinding::PathMode>    displacementModes;
    std::deque<Link *>                      currentPath;
public slots:
    void            searchLineEditTextChanged(const QString & text);
    void            carMoved(quint16);
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
