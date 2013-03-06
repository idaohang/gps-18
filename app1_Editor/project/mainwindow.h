#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

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
    void SaveRoad();

private slots:
    void on_btAddRoad_clicked();
    void on_btFinishRoad_clicked();

    void on_btCancel_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    bool            isRoadDrawing;
};

#endif // MAINWINDOW_H
