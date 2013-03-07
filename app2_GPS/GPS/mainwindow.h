#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "Node.hpp"

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

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    bool            isRoadDrawing;
    enum Mode
    {
        BEGIN,
        END,
        NONE
    };
    Mode    nodeMode;
    Node    *begin;
    Node    *end;

public slots:
    void chooseBegin();
    void chooseEnd();
    void parcourir();
    void selectNode();
    void doPathFinding();
};

#endif // MAINWINDOW_H
