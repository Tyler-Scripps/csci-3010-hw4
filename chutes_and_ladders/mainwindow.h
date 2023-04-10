#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qgraphicsscene.h>
#include <QMainWindow>
#include <QGraphicsTextItem>

#include "playerdiag.h"
#include "namediag.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Player {
    std::string name = "";
    int points = 0;
    int numUndo = 0;
//    int currentTile[2] = {-1,-1};
//    int prevTile[2] = {-1,-1};
    int currentTile = -1;
    int prevTile = -1;
    int currentRoll;
    bool rolledForTurn = false;
};

struct Position {
    int x;
    int y;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_rollBut_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;

    playerDiag * pd;
    nameDiag * nd;
    int chutes[7][2][2] = {{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}}};
    int ladders[7][2][2] = {{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}},{{-1,-1},{-1,-1}}};
    bool tileUsed(int x, int y);
    void generateChutes();
    void generateLadders();

    Position cellNumToPos(int num);
    Position cellPosToPx(Position pos);

    void drawChutes();
    void drawLadders();

    std::vector<Player> players;
    int activePlayer_ = 0;

    int rollDie();

    bool gameRunning = false;
};
#endif // MAINWINDOW_H
