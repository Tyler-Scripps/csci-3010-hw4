#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qgraphicsscene.h>
#include <QMainWindow>

#include "playerdiag.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;

    playerDiag * pd;
    int chutes[7][2][2] = {{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}}};
    int ladders[7][2][2] = {{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}}};
    bool tileUsed(int x, int y);
    void generateChutes();
    void generateLadders();

    void drawChutes();
    void drawLadders();
};
#endif // MAINWINDOW_H
