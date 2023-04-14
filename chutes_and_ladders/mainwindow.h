#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qgraphicsscene.h>
#include <QMainWindow>
#include <QGraphicsTextItem>

#include "playerdiag.h"
#include "namediag.h"
#include "pawn.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Player {
    std::string name = "";
    int points = 0;
    int numUndo = 0;
    int currentTile = -1;
    int prevTile = -1;
    int currentRoll;
    bool rolledForTurn = false;
    int rerolls = 0;
//    QColor color;
//    pawn * playerPawn;

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
    void on_moveBut_clicked();
    void on_startBut_clicked();
    void on_rerollBut_clicked();

    void on_newBut_clicked();

    void on_quitBut_clicked();

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
    std::vector<pawn *> pawns;

    int rollDie();

    bool gameRunning = false;
};
#endif // MAINWINDOW_H
