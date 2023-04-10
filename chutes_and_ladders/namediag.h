#ifndef NAMEDIAG_H
#define NAMEDIAG_H

#include <QDialog>

namespace Ui {
class nameDiag;
}

class nameDiag : public QDialog
{
    Q_OBJECT

public:
    explicit nameDiag(QWidget *parent = nullptr, int numberNames = 0);
    ~nameDiag();
    std::vector<std::string> getNames();

private slots:
    void on_pushButton_clicked();

private:
    Ui::nameDiag *ui;
    int numNames;
    std::vector<std::string> names;
};

#endif // NAMEDIAG_H
