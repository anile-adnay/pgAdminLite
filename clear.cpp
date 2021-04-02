/*
Copyright 2021 Yandashevskaya Alina Andreevna

====================================================================

This file is part of PgAdminLite.

PgAdminLite is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

PgAdminLite is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
