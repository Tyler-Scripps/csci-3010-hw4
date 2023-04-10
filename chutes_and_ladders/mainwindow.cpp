#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    srand (time(NULL));
    ui->setupUi(this);

    pd = new playerDiag();
    pd->setModal(true);
    pd->exec();

    nd = new nameDiag(this, pd->getNumPlayers());
    nd->setModal(true);
    nd->exec();

    for (int i = 0; i < pd->getNumPlayers(); i++) {
        Player tempPlayer = {nd->getNames().at(i), 0};
        players.push_back(tempPlayer);
    }
    ui->playerLabel->setText(QString(("Active Player:\n" + players.at(0).name).c_str()));

    QGraphicsView* view = ui->graphicsView;
    scene = new QGraphicsScene();
    view->setScene(scene);
    int ySpacing = view->frameSize().height()/10;
    int xSpacing = view->frameSize().width()/10;

    //add lines to board to form graph
    for (int i = 0; i < 9; i++) {
        scene->addLine(0,i*ySpacing+ySpacing,view->frameSize().width(),i*ySpacing+ySpacing);
        scene->addLine(i*xSpacing+xSpacing,0,i*xSpacing+xSpacing,view->frameSize().height());
    }

    //add numbers to tiles
    for (int i = 0; i < 5; i++) {
        //left to right rows
        for (int j = 1; j < 11; j++) {
            QGraphicsTextItem *text = scene->addText(std::to_string(i*20+j).c_str());
            text->setPos((j-1)*xSpacing, view->frameSize().height() - (2*i+1)*ySpacing);
        }
        //right to left rows
        for (int j = 1; j < 11; j++) {
            QGraphicsTextItem *text = scene->addText(std::to_string(i*20+j+10).c_str());
            text->setPos(view->frameSize().width() - j*xSpacing, view->frameSize().height() - (2*i+2)*ySpacing);
        }
    }

    generateChutes();
    generateLadders();

    drawChutes();
    drawLadders();
}

//returns the numeric value of a cell from its xy coord (0-9)
int cellPosToNum(Position pos) {
    //input validation
    if(pos.x < 0 || pos.x > 9 || pos.y < 0 || pos.y > 9) {
        return -1;
    }

    if(pos.y % 2 == 0) {    //even row
        return pos.y*10 + pos.x + 1;
    } else {    //odd row
        return (pos.y+1)*10-pos.x;
    }
}

//returns xy pos of center of tile in px, expects 1 <= num <= 100
Position MainWindow::cellNumToPos(int num) {
    //input validation
    if (num < 1 || num > 100) {
        return Position() = {-1, -1};
    }
    QGraphicsView* view = ui->graphicsView;
    int ySpacing = view->frameSize().height()/10;
    int xSpacing = view->frameSize().width()/10;
    int x;
    int y = view->frameSize().height() - ySpacing * (int)(100/num);
    if(num%20 < 10) {   //left to right row
        x = num % 10 * xSpacing;
    } else {            //right ot left row
        x = view->frameSize().width() - num % 10 * xSpacing;
    }
    return Position() = {x, y};
}

//converts values from 0-9 to px values of center of tile
Position MainWindow::cellPosToPx(Position pos) {
    //input validation
    if (pos.x < 0 || pos.x > 9 || pos.y < 0 || pos.y > 9) {
        return Position() = {-1, -1};
    }
    QGraphicsView* view = ui->graphicsView;
    int ySpacing = view->frameSize().height()/10;
    int xSpacing = view->frameSize().width()/10;
    int x = pos.x * xSpacing + xSpacing/2;
    int y = view->frameSize().height() - (pos.y * ySpacing + ySpacing/2);
    return Position() = {x, y};
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::tileUsed(int x, int y) {
//    qDebug() << "testing: " << x << "," << y;
    bool used = false;
    //iterate across chutes and ladders arrays
    for (int i = 0; i < 7; i++) {
        //check if tile is in either chutes or ladders at i
        //chute start
        if(chutes[i][0][0] == x && chutes[i][0][1] == y) {
            used = true;
            break;
        }

        //chute end
        if(chutes[i][1][0] == x && chutes[i][1][1] == y) {
            used = true;
            break;
        }

        //ladder start
        if(ladders[i][0][0] == x && ladders[i][0][1] == y) {
            used = true;
            break;
        }

        //ladder end
        if(ladders[i][1][0] == x && ladders[i][1][1] == y) {
            used = true;
            break;
        }
    }
    if (used) {
//        qDebug() << "tile" << x << "," << y << "used";
        return true;
    } else {
//        qDebug() << "tile unused";
        return false;
    }

}


void MainWindow::generateChutes() {
    qDebug() << "generating chutes";
    //generate 7 chutes
    for (int i = 0; i < 7; i++) {
        //start point
        int startX = rand()%10; //0-9
        int startY = rand()%9 + 1;  //1-9
        while(tileUsed(startX, startY)) {
            startX = rand()%10; //0-9
            startY = rand()%9 + 1;  //1-9
        }
        chutes[i][0][0] = startX;
        chutes[i][0][1] = startY;

        //end point
        int endX = rand()%10;   //0-9
        int endY = rand()%9;    //0-8
        while(tileUsed(endX, endY) || endY >= startY) {
            endX = rand()%10;   //0-9
            endY = rand()%9;    //0-8
        }
        chutes[i][1][0] = endX;
        chutes[i][1][1] = endY;
//        qDebug() << "chute from: " << startX << "," << startY << " to: " << endX << "," << endY;
        qDebug() << "chute from: " << cellPosToNum({startX,startY}) << " to: " << cellPosToNum({endX,endY});
    }
}

void MainWindow::generateLadders() {
    qDebug() << "generating ladders";
    //generate 7 ladders
    for (int i = 0; i < 7; i++) {
        //start point
        int startX = rand()%10; //0-9
        int startY = rand()%9;  //0-8

        //end point
        int endX = rand()%10;   //0-9
        int endY = rand()%9+1;    //1-9

        while(tileUsed(startX, startY) || tileUsed(endX, endY) || endY >= startY || (startX == endX && startY == endY)) {
            startX = rand()%10; //0-9
            startY = rand()%9;  //0-8

            endX = rand()%10;   //0-9
            endY = rand()%9+1;  //1-9
        }
        ladders[i][0][0] = startX;
        ladders[i][0][1] = startY;

        ladders[i][1][0] = endX;
        ladders[i][1][1] = endY;
//        qDebug() << "ladder from: " << startX << "," << startY << " to: " << endX << "," << endY;
        qDebug() << "ladder from: " << cellPosToNum({startX,startY}) << " to: " << cellPosToNum({endX,endY});
    }
}

void MainWindow::drawChutes() {
//    qDebug() << "drawing chutes";
    QGraphicsView* view = ui->graphicsView;

    int ySpacing = view->frameSize().height()/10;
    int xSpacing = view->frameSize().width()/10;

    for (int i = 0; i < 7; i++) {
        int startX = chutes[i][0][0] * xSpacing + xSpacing/2;
        int startY = view->frameSize().height() - (chutes[i][0][1] * ySpacing + ySpacing/2);

        int endX = chutes[i][1][0] * xSpacing + xSpacing/2;
        int endY = view->frameSize().height() - (chutes[i][1][1] * ySpacing + ySpacing/2);

        scene->addLine(startX, startY, endX, endY, QPen(QColor(243,23,143)));
    }
}

void MainWindow::drawLadders() {
//    qDebug() << "drawing ladders";
    QGraphicsView* view = ui->graphicsView;

    int ySpacing = view->frameSize().height()/10;
    int xSpacing = view->frameSize().width()/10;

    for (int i = 0; i < 7; i++) {
        int startX = ladders[i][0][0] * xSpacing + xSpacing/2;
        int startY = view->frameSize().height() - (ladders[i][0][1] * ySpacing + ySpacing/2);

        int endX = ladders[i][1][0] * xSpacing + xSpacing/2;
        int endY = view->frameSize().height() - (ladders[i][1][1] * ySpacing + ySpacing/2);

        scene->addLine(startX, startY, endX, endY, QPen(QColor(34,162,15)));
    }
}

int MainWindow::rollDie() {
    int val1 = rand()%6+1;
    int val2 = rand()%6+1;
    ui->die1Label->setText(std::to_string(val1).c_str());
    ui->die2Label->setText(std::to_string(val2).c_str());
    players.at(activePlayer_).currentRoll = val1+val2;
    return val1+val2;
}

void MainWindow::on_rollBut_clicked()
{
    if(!players.at(activePlayer_).rolledForTurn) {
        rollDie();
        players.at(activePlayer_).rolledForTurn = true;
    }
}


void MainWindow::on_moveBut_clicked()
{
    if(!gameRunning){
        return;
    }

    //tile not on board
    if(players.at(activePlayer_).currentTile == -1 && players.at(activePlayer_).currentRoll == 6) {

    }
}


void MainWindow::on_startBut_clicked()
{
    gameRunning = true;
    activePlayer_ = 0;
}

