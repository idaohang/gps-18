#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <deque>
#include <QFileDialog>
#include "../../common/Road.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void EnterRoadCreationMode();
    void LeaveRoadCreationMode();

private slots:
    void on_btAddRoad_clicked();
    void on_btFinishRoad_clicked();
    void on_btCancel_clicked();
    void on_btSave_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    bool            isRoadDrawing;
    std::deque<Road*>   roads;
    std::string     databasePath;
};

#endif // MAINWINDOW_H
