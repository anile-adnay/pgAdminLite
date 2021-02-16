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
