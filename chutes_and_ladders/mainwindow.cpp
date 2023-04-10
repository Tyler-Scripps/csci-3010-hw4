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

    QGraphicsView* view = ui->graphicsView;
    scene = new QGraphicsScene();
    view->setScene(scene);
    int ySpacing = view->frameSize().height()/10;
    int xSpacing = view->frameSize().width()/10;

    for (int i = 0; i < 9; i++) {
        scene->addLine(0,i*ySpacing+ySpacing,view->frameSize().width(),i*ySpacing+ySpacing);
        scene->addLine(i*xSpacing+xSpacing,0,i*xSpacing+xSpacing,view->frameSize().height());
    }

    generateChutes();
    generateLadders();

    drawChutes();

}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::tileUsed(int x, int y) {
    //iterate across chutes and ladders arrays
    for (int i = 0; i < 7; i++) {
        //check if tile is in either chutes or ladders at i
        //chute start
        if(chutes[i][0][0] == x && chutes[i][0][1] == y) {
            return true;
        }

        //chute end
        if(chutes[i][1][0] == x && chutes[i][1][1] == y) {
            return true;
        }

        //ladder start
        if(ladders[i][0][0] == x && ladders[i][0][1] == y) {
            return true;
        }

        //ladder end
        if(ladders[i][1][0] == x && ladders[i][1][1] == y) {
            return true;
        }
    }
    return false;
}


void MainWindow::generateChutes() {
    //generate 7 chutes
    for (int i = 0; i < 7; i++) {
        //start point
        int startX = rand()%10;
        int startY = rand()%9 + 1;
        while(tileUsed(startX, startY)) {
            startX = rand()%10;
            startY = rand()%9 + 1;
        }
        chutes[i][0][0] = startX;
        chutes[i][0][1] = startY;

        //end point
        int endX = rand()%10;
        int endY = rand()%9;
        while(tileUsed(endX, endY) || endY >= startY) {
            endX = rand()%10;
            endY = rand()%9;
        }
        chutes[i][1][0] = endX;
        chutes[i][1][1] = endX;
        qDebug() << "chute from: " << startX << "," << startY << " to: " << endX << "," << endY;
    }
}

void MainWindow::generateLadders() {
    //generate 7 ladders
    for (int i = 0; i < 7; i++) {
        //start point
        int startX = rand()%10;
        int startY = rand()%9 + 1;
        while(tileUsed(startX, startY)) {
            startX = rand()%10;
            startY = rand()%9 + 1;
        }
        ladders[i][0][0] = startX;
        ladders[i][0][1] = startY;

        //end point
        int endX = rand()%10;
        int endY = rand()%9;
        while(tileUsed(endX, endY) || endY >= startY) {
            endX = rand()%10;
            endY = rand()%9;
        }
        ladders[i][1][0] = endX;
        ladders[i][1][1] = endX;
    }
}

void MainWindow::drawChutes() {
    QGraphicsView* view = ui->graphicsView;
//    scene = new QGraphicsScene();
//    view->setScene(scene);

    int ySpacing = view->frameSize().height()/10;
    int xSpacing = view->frameSize().width()/10;

    for (int i = 0; i < 7; i++) {
//        QLine * chute = new QLine()
        int startX = chutes[i][0][0] * xSpacing + xSpacing/2;
        int startY = chutes[i][0][1] * ySpacing + ySpacing/2;

        int endX = chutes[i][1][0] * xSpacing + xSpacing/2;
        int endY = chutes[i][1][1] * ySpacing + ySpacing/2;

        scene->addLine(startX, startY, endX, endY, QPen(QColor(243,23,143)));
    }
}
