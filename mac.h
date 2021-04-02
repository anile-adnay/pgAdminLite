/*
Copyright 2021 Yandashevskaya Elina Andreevna

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

#ifndef MAC_H
#define MAC_H

#include <QWidget>

namespace Ui {
class Mac;
}

class Mac : public QWidget
{
    Q_OBJECT

    QString nameDB;
    QString nameTable;
public:
    explicit Mac(QWidget *parent = 0);
    ~Mac();
    void Red_mac_DB(QString DB, QString Tale, QString mac);
private:
    Ui::Mac *ui;

public slots:
    void change_mac_DB();
    void change_mac_Table();
signals:
    void UpDataList();
};
#endif
