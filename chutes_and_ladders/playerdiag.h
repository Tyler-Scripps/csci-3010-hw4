#ifndef PLAYERDIAG_H
#define PLAYERDIAG_H

#include <QDialog>

namespace Ui {
class playerDiag;
}

class playerDiag : public QDialog
{
    Q_OBJECT

public:
    explicit playerDiag(QWidget *parent = nullptr);
    ~playerDiag();
    int getNumPlayers();

private slots:
    void on_submitBut_clicked();

private:
    Ui::playerDiag *ui;
    int number_players_;
};

#endif // PLAYERDIAG_H
