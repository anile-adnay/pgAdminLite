#include "clear.h"
#include "ui_clear.h"

clear::clear(QWidget *parent) : QWidget(parent), ui(new Ui::clear)
{
    ui->setupUi(this);
}

clear::~clear()
{
    delete ui;
}
void clear::Spisok_DB(QVector <ListDB>  DB) {
    DB_AddUser = DB;
}
void clear::on_pushButton_clicked()
{
    QString strlist = "";
    ui->textEdit->setText(" ");
    if(ui->lineEdit->text() == "" || ui->lineEdit_2->text() == "") {
        QMessageBox* newMessage;
        newMessage = new QMessageBox(QMessageBox::Question ,"Ошибка" ,"Заполните все имеющиеся поля!",QMessageBox::Yes);
        newMessage->exec();
    } else {
    if(ui->checkBox->isChecked()) {

        QProcess * AddUser = new QProcess;
        AddUser->start("useradd " + ui->lineEdit->text() + " -p " + ui->lineEdit_2->text() +" ");

        AddUser->waitForReadyRead();

        strlist.append(AddUser->readAllStandardError());

        if(  strlist == "" ) {

            strlist +="Пользователь создан в системе";

        } else {
             ui->textEdit->setText(strlist);
        }

    }

        if(ui->checkBox_2->isChecked()) {

        QSqlDatabase db1 = QSqlDatabase::addDatabase("QPSQL" );
        db1.setHostName("localhost");
        db1.setDatabaseName("postgres");
        db1.setUserName("postgres");
        db1.setPassword("ast31ra1");

        if (db1.open()) {
            QSqlQuery query = QSqlQuery(db1);
            query.exec("CREATE ROLE \""+ ui->lineEdit->text()+"\"  LOGIN VALID UNTIL 'infinity';"  );

            strlist += "\nДобавлен в СУБД PostgreSQL";
        }

    }
         ui->textEdit->setText(strlist);
    }
}
