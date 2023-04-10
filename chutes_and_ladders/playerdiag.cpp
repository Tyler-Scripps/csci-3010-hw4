#include "playerdiag.h"
#include "ui_playerdiag.h"

playerDiag::playerDiag(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::playerDiag)
{
    ui->setupUi(this);
}

playerDiag::~playerDiag()
{
    delete ui;
}

void playerDiag::on_submitBut_clicked()
{
    number_players_ = ui->playerSlider->value();
    this->close();
}

int playerDiag::getNumPlayers() {
    return number_players_;
}
