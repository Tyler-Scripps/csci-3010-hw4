#include "namediag.h"
#include "ui_namediag.h"

#include <qlabel.h>
#include <qtextedit.h>
#include <QHBoxLayout>

nameDiag::nameDiag(QWidget *parent, int numberNames) :
    QDialog(parent),
    ui(new Ui::nameDiag)
{
    numNames = numberNames;
    ui->setupUi(this);

    for (int i = 0; i < numNames; i++) {
        QHBoxLayout *QHB = new QHBoxLayout();
        std::string name_str = "Player " + std::to_string(i+1) + " name: ";
        QLabel *nameLabel = new QLabel(name_str.c_str());
        QHB->addWidget(nameLabel);

        QTextEdit *nameInput = new QTextEdit();
        nameInput->setObjectName(("name" + std::to_string(i)).c_str());
        QHB->addWidget(nameInput);

        ui->verticalLayout->addLayout(QHB);
    }
}

nameDiag::~nameDiag()
{
    delete ui;
}

void nameDiag::on_pushButton_clicked()
{
    for (int i = 0; i < numNames; i++) {
        std::string inputName = "name"+std::to_string(i);
        std::string playerName = this->findChild<QTextEdit*>(inputName.c_str())->toPlainText().toStdString();
//        qDebug() << playerName.c_str();
        names.push_back(playerName);
    }
    this->close();
}

std::vector<std::string> nameDiag::getNames() {
    return names;
}
