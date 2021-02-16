#include "mac.h"
#include "ui_mac.h"
#include <QtSql>
#include <autentification.h>

QStringList List_MacLabel_In_OS_F11(QStringList listDB) {

    QStringList mas;
    QRegExp re( "([0-9]*)([A-Za-zа-яА-Я0-9_ -]*)\n" );
    int i = 0;
    while ( ( i = re.indexIn(listDB.value(0),i)) != -1 ) {
        i +=re.matchedLength();
        mas << re.cap( 2 );
        mas << re.cap( 1 );
    }
    return mas;
}

int proverka () {

    QProcess * Mac_List = new QProcess;
    Mac_List->start("userlev ");
    Mac_List->waitForReadyRead();
    QStringList strlist ;
    strlist.append(Mac_List->readAllStandardOutput());

    QStringList strlist_reg = List_MacLabel_In_OS_F11(strlist);
    return strlist_reg.size() /2 ;
}
Mac::Mac(QWidget *parent) : QWidget(parent), ui(new Ui::Mac)
{
    ui->setupUi(this);
}
Mac::~Mac()
{
    delete ui;
}
void Mac::Red_mac_DB(QString DB,QString Tale,QString mac) {

    QString level = mac.at(1);
    QString kategoria = mac.at(3);
    ui->lineEdit->setText(level);
    ui->lineEdit_2->setText(kategoria);

    nameDB = DB;
    nameTable = Tale;

    if(Tale == "") {
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(change_mac_DB()));
    }
    else {
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(change_mac_Table()));
    }
}
void Mac::change_mac_DB() {

    proverka();

    if( proverka() >= ui->lineEdit->text().toInt() ) {
    QString str1 = "{" + ui->lineEdit->text() + "," + ui->lineEdit_2->text() + "}";

    QSqlDatabase db1 = QSqlDatabase::addDatabase("QPSQL" );
    db1.setHostName("localhost");
    db1.setDatabaseName(nameDB);
    db1.setUserName("postgres");
    db1.setPassword("ast31ra1");

    if (db1.open()) {
    // Назначение метки
    QSqlQuery query = QSqlQuery(db1);
    query.exec("MAC LABEL ON DATABASE "+nameDB+" IS '" + str1 + "';");
    QString ERROR = query.lastError().text();
    if(ERROR != " ") {
        QMessageBox* newMessage;
        newMessage = new QMessageBox(QMessageBox::Question , "Ошибка!",ERROR,QMessageBox::Yes);
        newMessage->exec();
    }
    // Назначение CRR
    QString zapros;
    if(ui->radioButton->isChecked()) {
            zapros = "MAC CCR ON DATABASE "+nameDB+" IS ON;";
    } else {
            zapros = "MAC CCR ON DATABASE "+nameDB+" IS OFF;";
    }
    QSqlQuery query1 = QSqlQuery(db1);
    query1.exec(zapros);
    QString ERROR1 = query1.lastError().text();
    if(ERROR1 != " ") {
        QMessageBox* newMessage;
        newMessage = new QMessageBox(QMessageBox::Question , "Ошибка!",ERROR1,QMessageBox::Yes);
        newMessage->exec();
    }
    }
    UpDataList();
    close();
    } else {
        QMessageBox* newMessage;
        newMessage = new QMessageBox(QMessageBox::Question , "Ошибка!","Уровня МРД с таким значением в системе нет!",QMessageBox::Yes);
        newMessage->exec();
    }
}
void Mac::change_mac_Table() {

    QString str1 = "{" + ui->lineEdit->text() + "," + ui->lineEdit_2->text() + "}";

    QSqlDatabase db1 = QSqlDatabase::addDatabase("QPSQL" );
    db1.setHostName("localhost");
    db1.setDatabaseName(nameDB);
    db1.setUserName("postgres");
    db1.setPassword("ast31ra1");

    if (db1.open()) {
    QSqlQuery query = QSqlQuery(db1);

    QString zapros ="MAC LABEL ON SCHEMA public IS '" + str1 + "';";
    zapros += "MAC LABEL ON TABLE \"" + nameTable + "\" IS '" + str1 + "';";

    query.exec(zapros);

    QString error = query.lastError().text();
    QMessageBox* newMessage;
        if( error != " ") {
            newMessage = new QMessageBox(QMessageBox::Question , "Ошибка!",error,QMessageBox::Yes);
            } else {
            newMessage = new QMessageBox(QMessageBox::Question , "Изменения применены!","Мандатные атрибуты установлены!",QMessageBox::Yes);
            }
            newMessage->exec();

        //// Назначение CRR
        QString zapros1;
        if(ui->radioButton->isChecked()) {
                zapros1 = "MAC CCR ON TABLE "+nameTable+" IS ON;";
        } else {
                zapros1 = "MAC CCR ON TABLE "+nameTable+" IS OFF;";
        }
        QSqlQuery query1 = QSqlQuery(db1);
        query1.exec(zapros1);
        QString ERROR1 = query1.lastError().text();
        if(ERROR1 != " ") {
            QMessageBox* newMessage;
            newMessage = new QMessageBox(QMessageBox::Question , "Ошибка!",ERROR1,QMessageBox::Yes);
            newMessage->exec();
        }
    }
    UpDataList();
    close();
}

