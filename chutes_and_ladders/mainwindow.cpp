#include "mainwindow.h"
#include "qdatetime.h"
#include "ui_mainwindow.h"

//retrieved from: https://stackoverflow.com/questions/3752742/how-do-i-create-a-pause-wait-function-using-qt
void delay(unsigned int millis)
{
    QTime dieTime= QTime::currentTime().addMSecs(millis);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

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
        Player tempPlayer;
        tempPlayer.name = nd->getNames().at(i);
        tempPlayer.currentTile = 0;
        tempPlayer.points = 0;
//        tempPlayer.color = QColor(rand()%256,rand()%256,rand()%256);
        tempPlayer.numUndo = 0;
        players.push_back(tempPlayer);

        Position tempPos = cellNumToPos(1);
        pawn * tempPawn = new pawn(QColor(rand()%256,rand()%256,rand()%256), tempPos.x - 20 + 10*i, tempPos.y);

        pawns.push_back(tempPawn);
//        qDebug() << pawns.at(i)->getX() << "," << pawns.at(i)->getY();
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
    int y = view->frameSize().height() - ySpacing * ((num-1) / 10) - ySpacing/2;
    if(num%20 < 11) {   //left to right row
        x = num % 11 * xSpacing - xSpacing/2;
    } else {            //right to left row
        x = view->frameSize().width() - (num-1) % 10 * xSpacing - xSpacing/2;
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
        while(tileUsed(startX, startY) || (startX == 0 && startY == 9)) {
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

        // purple
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

        // green
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
    if(!gameRunning){
        return;
    }

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
    qDebug() << "roll value:" << players.at(activePlayer_).currentRoll;

    //tile not on board
    if(players.at(activePlayer_).currentTile == -1 && players.at(activePlayer_).currentRoll == 6) {
//        qDebug() << pawns.at(activePlayer_)->getX();
        qDebug() << "tile not on board";
        players.at(activePlayer_).currentTile = 1;
        scene->addItem(pawns.at(activePlayer_));
        ui->die1Label->setText("Die 1 Value");
        ui->die2Label->setText("Die 2 Value");
    } else if (players.at(activePlayer_).currentTile > 0 && players.at(activePlayer_).currentRoll > 0) { //tile on board
        //move pawn
//        qDebug() << "tile on board";
        qDebug() << "moving to" << players.at(activePlayer_).currentRoll + players.at(activePlayer_).currentTile;
        for (int i = 0; i < players.at(activePlayer_).currentRoll; i++) {
            players.at(activePlayer_).currentTile++;
            //check if landed on ladder start
            scene->removeItem(pawns.at(activePlayer_));
            Position tempPos = cellNumToPos(players.at(activePlayer_).currentTile);
            qDebug() << "new tile:" << players.at(activePlayer_).currentTile;
            qDebug() << "new pos:" << tempPos.x << "," << tempPos.y;
            pawns.at(activePlayer_)->setXY(tempPos.x, tempPos.y);
            scene->addItem(pawns.at(activePlayer_));
            delay(500);
        }
        int ladder = -1;
        for(int j = 0; j < 7; j++) {
            if(cellPosToNum(Position() = {ladders[j][0][0], ladders[j][0][1]}) == players.at(activePlayer_).currentTile) {
                ladder = j;
                break;
            }
        }
        //check if landed on chute start
        int chute = -1;
        for(int j = 0; j < 7; j++) {
            if(cellPosToNum(Position() = {chutes[j][0][0], chutes[j][0][1]}) == players.at(activePlayer_).currentTile) {
                chute = j;
                break;
            }
        }

        if(ladder > -1) {   //landed at base of ladder
            qDebug() << "moving to:" << cellPosToNum(Position() = {ladders[ladder][1][0], ladders[ladder][1][1]});
            players.at(activePlayer_).currentTile = cellPosToNum(Position() = {ladders[ladder][1][0], ladders[ladder][1][1]});
        } else if (chute > -1) {    //landed at top of chute
            qDebug() << "moving to:" << cellPosToNum(Position() = {chutes[chute][1][0], chutes[chute][1][1]});
            players.at(activePlayer_).currentTile = cellPosToNum(Position() = {chutes[ladder][1][0], chutes[ladder][1][1]});
        }

        scene->removeItem(pawns.at(activePlayer_));
        Position tempPos = cellNumToPos(players.at(activePlayer_).currentTile);
        pawns.at(activePlayer_)->setXY(tempPos.x, tempPos.y);
        scene->addItem(pawns.at(activePlayer_));

        //reset current roll
        players.at(activePlayer_).rolledForTurn = false;
        players.at(activePlayer_).currentRoll = -1;
    }
    players.at(activePlayer_).currentRoll = -1;
    players.at(activePlayer_).rolledForTurn = false;

    //finally increment activeplayer_
    activePlayer_++;
    if(activePlayer_ >= players.size()) {
        activePlayer_ = 0;
    }

    //reset dice values
    ui->die1Label->setText("Die 1 Value");
    ui->die2Label->setText("Die 2 Value");

    //show active player
    ui->playerLabel->setText(QString(("Active Player:\n" + players.at(activePlayer_).name).c_str()));
}


void MainWindow::on_startBut_clicked()
{
    if(!gameRunning){
        gameRunning = true;
        activePlayer_ = 0;
        for (unsigned int i = 0; i < players.size(); i++) {
            players.at(i).currentTile = -1;
            players.at(i).rolledForTurn = false;
        }
    }
}

void MainWindow::on_rerollBut_clicked()
{
    if(!gameRunning || !players.at(activePlayer_).rolledForTurn){
        qDebug() << "not rerolling";
        return;
    }

    if(players.at(activePlayer_).rerolls < 5) {
        rollDie();
        players.at(activePlayer_).rerolls++;
    }
}


void MainWindow::on_newBut_clicked()
{
    activePlayer_ = 0;
    for (unsigned int i = 0; i < players.size(); i++) {
        players.at(i).currentRoll = -1;
        players.at(i).currentTile = -1;
        players.at(i).rerolls = 0;
        players.at(i).prevTile = -1;
        players.at(i).numUndo = 0;

        Position tempPos = cellNumToPos(1);
//        pawn * tempPawn = new pawn(QColor(rand()%256,rand()%256,rand()%256), tempPos.x - 20 + 10*i, tempPos.y);

        pawns.at(i)->setXY(tempPos.x-20+10*i, tempPos.y);
        scene->removeItem(pawns.at(i));
    }

    gameRunning = false;

    //reset dice values
    ui->die1Label->setText("Die 1 Value");
    ui->die2Label->setText("Die 2 Value");

    //show active player
    ui->playerLabel->setText(QString(("Active Player:\n" + players.at(activePlayer_).name).c_str()));
}


void MainWindow::on_quitBut_clicked()
{
    this->close();
}

