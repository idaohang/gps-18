#ifndef MYGPSVIEW_H
#define MYGPSVIEW_H

#include <QWidget>

namespace Ui {
class MyGPSView;
}

class MyGPSView : public QWidget
{
    Q_OBJECT
    
public:
    explicit MyGPSView(QWidget *parent = 0);
    ~MyGPSView();
    
private slots:
    void on_graphicsView_destroyed();

private:
    Ui::MyGPSView *ui;
};

#endif // MYGPSVIEW_H
