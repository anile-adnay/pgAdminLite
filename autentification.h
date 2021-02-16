#ifndef AUTENTIFICATION_H
#define AUTENTIFICATION_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QTreeWidget>
#include <QtSql>
#include <QVector>
#include <QMessageBox>
#include <QProcess>
#include <QGridLayout>
#include <QGroupBox>
#include <QStandardItemModel>
#include <QTableView>
#include <mac.h>

struct ListMacLabelUser {
    QString name;
    QStringList MacLabel;
};
struct ListMacLabel {
    QString name;
    QString MacLabel;
};
struct ListDB {
    QString nameDB;
    QString MacLabel;
    QVector <ListMacLabel> TableList;
};


class admin;
class autentification: public QDialog
{
         Q_OBJECT
public:
    autentification(QWidget *parent=0);
    ~autentification();
    QLineEdit * ReturnUser() {return user; }
    QLineEdit *Returnpassword() {return password;}
    QPushButton *ReturnbtnConnect() {return btnConnect;}
private:
   QSqlDatabase db;
   QLineEdit *host;
   QLineEdit *dbname;
   QLineEdit *user;
   QLineEdit *password;
   QTextEdit *scr;
   QPushButton *btnConnect;
   admin *postgres;
private slots:
   bool connect_user();
   void Close_Main_Window();
};



class admin : public QDialog
{
    Q_OBJECT

QGroupBox *panel;
QLabel* Image;

    autentification *pMainWindow;
    QSqlDatabase add_db;
    QGridLayout *layout;
    QTreeWidget Trwgt;
    QTreeWidgetItem* DB;
    QTreeWidgetItem* Users;
    QVector <ListDB> DBMacLabel;
    QVector <ListMacLabelUser> UsersWithMac;

    QStandardItemModel *model_Table_OSMac;
    QTableView *Table_Level_Mac;

    QSqlTableModel *model_table_from_db;

    QString SaveList_Mac_User_to_OS[5];
    QTextEdit *Error;


    QPushButton *RedMacDB;
    QPushButton *RedMacTable;
    QPushButton *RedMacZapis;

    int parametrI = 0;

public:
    admin(QSqlDatabase &db, QWidget *parent=0, autentification *parent_window=0);
    ~admin();
    void ShowAllTreeList();
    void UpDateTreeList();
    void ClearTreeList();

    QGridLayout* Start_Panel();


    QVector <ListDB>  Spisok_BD();

public slots:
    void UserMac_open(QTreeWidgetItem*,int);
    void Visible_Main_Window();
    void Visible_User_add();

    void Visible_Mac_Level();
    void Visible_Mac_Kategorii();

    void Save_MacLabel();
    void Add_Row_Table();
    void Del_Row_Table();

    void Save_MacLabel_User1();

    void Index_for_table_DB_change(int index);

    void Slot_Admin1(QString str){SaveList_Mac_User_to_OS[0]=str.replace(QRegExp("^[ ]*[a-zA-Zа-яА-Я_-0-9]*"),"");}
    void Slot_Admin2(QString str){SaveList_Mac_User_to_OS[1]=str.replace(QRegExp("^[ ]*[a-zA-Zа-яА-Я_-0-9]*"),"");}
    void Slot_Admin3(QString str){SaveList_Mac_User_to_OS[2]=str.replace(QRegExp("^[ ]*[a-zA-Zа-яА-Я_-0-9]*"),"");}
    void Slot_Admin4(QString str){SaveList_Mac_User_to_OS[3]=str.replace(QRegExp("^[ ]*[a-zA-Zа-яА-Я_-0-9]*"),"");}
    void Slot_Admin5(QString str){SaveList_Mac_User_to_OS[4]=str.replace(QRegExp("^[ ]*[a-zA-Zа-яА-Я_-0-9]*"),"");}


    void RedMac_DB();
    void RedMac_Table();
    void UpDataList_MacDB();
    void UpDataList_MacTable();
    void RedMac_zapis();

    //    void Visible_DB_add();
    //    void Visible_DB_del();
    //    void Visible_DB_edit();
    //    void Visible_Table_add();
    //    void Visible_Table_del();
    //    void Visible_Table_edit();
    //    void Visible_User_del();
    //    void Visible_User_edit();
    //    void Visible_Sql_request();
    //    void Visible_Help();
};



#endif // AUTENTIFICATION_H
