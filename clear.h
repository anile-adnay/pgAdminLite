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

#ifndef CLEAR_H
#define CLEAR_H

#include <QWidget>
#include <autentification.h>

namespace Ui {
class clear;
}

class clear : public QWidget
{
    Q_OBJECT

public:

    QVector <ListDB>  DB_AddUser;
    clear( QWidget *parent = 0);
    ~clear();
    void Spisok_DB(QVector <ListDB>  DB);

private slots:
    void on_pushButton_clicked();

private:
    Ui::clear *ui;
};

#endif // CLEAR_H
