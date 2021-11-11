#include "autentification.h"
#include "clear.h"

#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QImage>
#include <clear.h>
#include <QStandardItem>

clear *Clear;
QMessageBox* newMessage;
QString regstr ="^[ ]*[a-zA-Zа-яА-Я_-0-9]*";

//================================================
//============ Function Help =====================
//================================================
QStringList List_MacLabel_In_OS_F1(QStringList listDB) {
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
void List_MacLabel_In_OS_ReLoad_To_Table(QStandardItemModel *model) {

    QStandardItem *item;

    QProcess * Mac_List = new QProcess;
    Mac_List->start("userlev ");
    Mac_List->waitForReadyRead();
    QStringList strlist ;
    strlist.append(Mac_List->readAllStandardOutput());

    QStringList strlist_reg = List_MacLabel_In_OS_F1(strlist);
    int N = 0;
    for( int i = 0 ; i < strlist_reg.size()  ; i+=2,N++) {
        item = new QStandardItem(QString(strlist_reg[i]));
        model->setItem(N,0,item);
        item = new QStandardItem(QString(strlist_reg[i+1]));
        model->setItem(N,1,item);
    }
}
void List_KatLabel_In_OS_ReLoad_To_Table(QStandardItemModel *model) {

    QStandardItem *item;

    QProcess * Mac_List = new QProcess;
    Mac_List->start("usercat ");
    Mac_List->waitForReadyRead();
    QStringList strlist ;
    strlist.append(Mac_List->readAllStandardOutput());

    QStringList strlist_reg = List_MacLabel_In_OS_F1(strlist);
    int N = 0;
    for( int i = 0 ; i < strlist_reg.size()  ; i+=2,N++) {
        item = new QStandardItem(QString(strlist_reg[i]));
        model->setItem(N,0,item);
        item = new QStandardItem(QString(strlist_reg[i+1]));
        model->setItem(N,1,item);
    }
}

//================================================
//============ autentification ===================
//================================================
autentification::~autentification() {}
autentification::autentification(QWidget *parent) :QDialog(parent)
{
       this->setWindowTitle("Мандатное разграничение доступа в СУБД");


       host = new QLineEdit(tr("localhost"), this);
       dbname = new QLineEdit(tr("postgres"),this);
       user = new QLineEdit(this);
       password = new QLineEdit(this);
       password->setEchoMode(QLineEdit::Password);
       btnConnect = new QPushButton(("Подключиться"),this);
       scr = new QTextEdit(this);
       scr->setReadOnly(true);

       host->setEnabled(false);
       dbname->setEnabled(false);

        QGridLayout *layout = new QGridLayout(this);

             layout->addWidget(new QLabel(tr("Хост:"), this),
                               0, 0 ,Qt::AlignRight);

             layout->addWidget(host, 0, 1);

             layout->addWidget(new QLabel(tr("База данных:"), this),
                               0, 2, Qt::AlignRight);
             layout->addWidget(dbname, 0, 3);

             layout->addWidget(new QLabel(tr("Пользователь:"), this),
                               1, 0, Qt::AlignRight);
             layout->addWidget(user, 1, 1);

             layout->addWidget(new QLabel(tr("Пароль:"), this),
                               1, 2, Qt::AlignRight);
             layout->addWidget(password, 1, 3);

             layout->addWidget(btnConnect, 2, 1, 1, 2);
             layout->addWidget(scr, 3, 0,1,4);

             layout->setMargin(6);
             layout->setSpacing(5);

             setLayout(layout);

        connect(btnConnect,SIGNAL(clicked(bool)),this,SLOT(connect_user()));
        connect(this,SIGNAL(finished(int)),this,SLOT(Close_Main_Window()));
}

//----------------------------------
// autentification SLOTS
//----------------------------------

void autentification::Close_Main_Window() {
    QSqlDatabase db =QSqlDatabase::database();
    if(db.open()) {
        db.close();
        scr->append("=======================");
        scr->append(tr("Соединение с БД закрыто!"));
    }
}
bool autentification::connect_user() {

    if(user->text() == "" || password->text() == "")
    {
        scr->append(tr("Проверьте правильность введенных данных"));
        return false;
    }

    scr->append(tr("Соединяюсь с базой данных..."));

    db = QSqlDatabase::addDatabase("QPSQL" );

    db.setHostName(host->text());
    db.setDatabaseName(dbname->text());
    db.setUserName(user->text());
    db.setPassword(password->text());


    if (db.open()) {
                host->setEnabled(false);
                dbname->setEnabled(false);
                user->setEnabled(false);
                password->setEnabled(false);
                btnConnect->setEnabled(false);
                scr->append(tr("Соединение установлено!"));
                //qDebug() << "11111";
                if(user->text() == tr("postgres")) {
                        postgres = new admin(db,0,this);
                        postgres->show();
                }


        }else{
                scr->append(tr("Не могу соединиться: "));
                scr->append(db.lastError().text());
                return false;
     }
    //if(user->text() == tr("postgres")) {
            //postgres = new admin(db,0,this);
            //postgres->show();
    //}


return true;
}


//================================================
//================= admin ========================
//================================================

admin::~admin(){}
admin::admin(QSqlDatabase &db, QWidget *parent, autentification *parent_window):QDialog(parent) {

    pMainWindow = parent_window;
    add_db = db;
    pMainWindow->setVisible(false);
    QObject::connect(this,SIGNAL(finished(int)),this,SLOT(Visible_Main_Window()));

    this->setWindowTitle("МРД в СУБД. Администратор");

      // MENU
      QMenuBar* pMainmenu = new QMenuBar(this);
      QMenu* SubUsers = new QMenu("Пользователи");
      QMenu* SubMac = new QMenu("Мандатные метки");
      QMenu* SubHelp = new QMenu("Справка");

      //QMenu* SubSql = new QMenu("Sql запрос");
      //QMenu* SubDatabase = new QMenu("БД");
      //QMenu* SubTables = new QMenu("Таблицы");

      //SubDatabase->addAction("Добавить",this,SLOT(Visible_DB_add()));
      //SubDatabase->addAction("Удалить",this,SLOT(Visible_DB_del()));
      //SubDatabase->addAction("Изменить",this,SLOT(Visible_DB_edit()));
      //SubTables->addAction("Добавить",this,SLOT(Visible_Table_add()()));
      //SubTables->addAction("Удалить",this,SLOT(Visible_Table_del()));
      //SubTables->addAction("Изменить",this,SLOT(Visible_Table_edit()));
      SubUsers->addAction("Добавить",this,SLOT(Visible_User_add()));
      SubMac->addAction("Уровни",this,SLOT(Visible_Mac_Level()));

      pMainmenu->addMenu(SubUsers);
      pMainmenu->addMenu(SubMac);
      pMainmenu->addMenu(SubHelp);
      pMainmenu->show();

    ShowAllTreeList();

    Trwgt.resize(300,220);

    connect(&Trwgt,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(UserMac_open(QTreeWidgetItem*,int)));
    layout = new QGridLayout(this);

    Image = new QLabel;
    QPixmap pMap("/QT/Project02/Main1.jpg");
    Image->setPixmap(pMap);

    panel = new QGroupBox;
    QGridLayout *layout_new = new QGridLayout(this);
    layout_new->addWidget(Image, 0, 0 );
    panel->setLayout(layout_new);
    panel->setMinimumSize(Image->width(),Image->height());
    panel->show();


           layout->addWidget(pMainmenu, 0,0,1,0);
           layout->addWidget(&Trwgt, 1, 0 );

           layout->addWidget(panel, 1, 1);
           layout->setColumnMinimumWidth(0,230);
           setLayout(layout);

}
//----------------------------------
// admin FUNCTION
//----------------------------------
QGridLayout* admin::Start_Panel() {
    Image->setVisible(0);

    QGroupBox*newWidget = new QGroupBox;
    newWidget->setMinimumSize(Image->width(),Image->height());

    QGridLayout *layout_new123 = new QGridLayout(this);
    layout_new123->setMargin(6);
    layout_new123->setSpacing(5);
    newWidget->setLayout(layout_new123);
    newWidget->show();

    QLabel *Image1 = new QLabel;
    QPixmap pMap("/QT/Project02/1122.jpg");
    Image1->setPixmap(pMap);
    Image1->setMaximumSize(Image->size());
    Image1->show();
    layout->addWidget(Image1, 1, 1);
    layout->addWidget(newWidget, 1, 1);


    return layout_new123;
}

void admin::ClearTreeList() {

    UsersWithMac.clear();
    DBMacLabel.clear();

 int index = DB->parent()->indexOfChild(Trwgt.currentItem());
 delete DB->parent()->takeChild(index);

 index = Users->parent()->indexOfChild(Trwgt.currentItem());
 delete Users->parent()->takeChild(index);

}

void admin::UpDateTreeList() {

    DBMacLabel.clear();
    UsersWithMac.clear();

        // Cписок БД
        QSqlQuery query = QSqlQuery(add_db);
        if ( !query.exec("SELECT maclabel,datname FROM pg_database"))
           {
            newMessage = new QMessageBox(QMessageBox::Question , "Ошибка!",query.lastError().text(),QMessageBox::Yes);
            newMessage->exec();
           }
        else
        {
           // while (query.next() )
           {query.next();query.next();query.next();query.next();}
           //{
                    if(!(query.value(1).toString() == "template0" ||
                    query.value(1).toString() == "template1" ||
                    query.value(1).toString() == "postgres"))
                    {
                        QString str = query.value(1).toString();
                        QString mac = query.value(0).toString();
                        ListDB db1;
                        db1.nameDB = str;
                        db1.MacLabel = mac;

                        ListMacLabel LM1;

                        QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL" );
                        db.setHostName("localhost");
                        db.setDatabaseName(str);
                        db.setUserName("postgres");
                        db.setPassword("toortoor");

                        if (db.open()) {
                        QSqlQuery query_List_Table = QSqlQuery(db);

                        if (query_List_Table.exec("select * from pg_tables where tablename !~'^pg' and tablename !~'^sql'"))
                        {
                        while (query_List_Table.next())
                        {
                        QString TableName = query_List_Table.value(1).toString();
                        LM1.name = TableName;
                        db1.TableList.push_back(LM1);
                                                }
                        }
                        }
                        DBMacLabel.push_back(db1);

                    }
               //}
        }

        // Добавляем метки к таблицам
        QStringList NameTabel;
        QStringList MacTabel;

       for(int i = 0 ; i < DBMacLabel.size(); i++ ) {

           QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL" );
           db.setHostName("localhost");
           db.setDatabaseName(DBMacLabel[i].nameDB);
           db.setUserName("postgres");
           db.setPassword("toortoor");

           if (db.open()) {
           QSqlQuery query_List_Table = QSqlQuery(db);

           if ( query_List_Table.exec("select maclabel,relname from pg_class where relname !~'^pg' and relname !~'^sql'"))
           {
           while (query_List_Table.next() )
           {
           QString TableName = query_List_Table.value(1).toString();
           QString TableMac = query_List_Table.value(0).toString();

           NameTabel << TableName;
           MacTabel << TableMac;

           }
           }
           }

           for(int j = 0; j < DBMacLabel[i].TableList.size(); j++) {
               for(int k = 0 ;k < NameTabel.size(); k++) {
               if(DBMacLabel[i].TableList[j].name == NameTabel[k]) {
                   DBMacLabel[i].TableList[j].MacLabel = MacTabel[k];
               }
               }
           }
       }

        // Список Пользователей
        QSqlQuery query_users = QSqlQuery(add_db);
        if ( !query_users.exec("SELECT rolname FROM pg_roles"))
           {
            newMessage = new QMessageBox(QMessageBox::Question , "Ошибка! User",query_users.lastError().text(),QMessageBox::Yes);
            newMessage->exec();
           }
        else {
            while (query_users.next() ) {
                QString str = query_users.value(0).toString();
                ListMacLabelUser user1;
                user1.name = str;


                QProcess * MacUser = new QProcess;
                MacUser->start("pdp-ulbls " + str);
                MacUser->waitForReadyRead();
                QStringList strlist ;
                strlist.append(MacUser->readAllStandardOutput());

                user1.MacLabel = strlist;
                UsersWithMac.push_back(user1);
                }
            }
}

void admin::ShowAllTreeList() {

    Trwgt.clear();

    QStringList header ;
    header<< "Меню" << "MacLable";
    Trwgt.setHeaderLabels(header);
    Trwgt.setColumnWidth(0,150);
    Trwgt.setColumnWidth(1,70);
    DB = new QTreeWidgetItem(&Trwgt);
    DB->setText(0,"БД");
    Users = new QTreeWidgetItem(&Trwgt);
    Users->setText(0,"Пользователи");

    Trwgt.setItemExpanded(DB,true);
    Trwgt.setItemExpanded(Users,true);

    Trwgt.sortColumn();

    UpDateTreeList();

    QTreeWidgetItem*_Item = 0;

    for(int i = 0 ; i < UsersWithMac.size(); ++i) {
            _Item = new QTreeWidgetItem(Users);
            _Item->setText(0,UsersWithMac[i].name);
    }
    for(int i = 0 ; i < DBMacLabel.size(); ++i) {
            _Item = new QTreeWidgetItem(DB);
            _Item->setText(0,DBMacLabel[i].nameDB);
            _Item->setText(1,DBMacLabel[i].MacLabel);
    }
}

//----------------------------------
// admin SLOT
//----------------------------------
void admin::UserMac_open(QTreeWidgetItem *TrwgtItem, int)
{

    if(TrwgtItem->parent() == Users) {
        QPushButton *Yes = new QPushButton("Применить");
        int index = Users->indexOfChild(Trwgt.currentItem());
        QString text = UsersWithMac[index].MacLabel.at(0);

        QGridLayout* layoutUser = Start_Panel();

        layoutUser->addWidget(new QLabel("\nМандатные атрибуты : \n"),0, 0);
        layoutUser->addWidget(new QLabel("\tМакс. уровень целостности : "),1, 0);
        layoutUser->addWidget(new QLabel("\tМин. уровень: "),2, 0);
        layoutUser->addWidget(new QLabel("\tМакс. уровень: "),3, 0);
        layoutUser->addWidget(new QLabel("\tМин. набор категорий : "),4, 0);
        layoutUser->addWidget(new QLabel("\tМакс. набор категорий : "),5, 0);

        QComboBox *MacAtr = new QComboBox;
        QComboBox *MaxLevMac = new QComboBox;
        QComboBox *MinLevMac = new QComboBox;
        QComboBox *MaxKat = new QComboBox;
        QComboBox *MinKat = new QComboBox;

        MacAtr->addItem("        0");
        MacAtr->addItem("        1");

        QStandardItemModel *modelCombo_Mac = new QStandardItemModel;
        List_MacLabel_In_OS_ReLoad_To_Table(modelCombo_Mac);
        for(int i = 0 ; i < modelCombo_Mac->rowCount(); i++) {
            MaxLevMac->addItem(modelCombo_Mac->item(i,0)->text()+" "+
                               modelCombo_Mac->item(i,1)->text()+" ");
            MinLevMac->addItem(modelCombo_Mac->item(i,0)->text()+" "+
                               modelCombo_Mac->item(i,1)->text()+" ");
        }

        QStandardItemModel *modelCombo_Kat = new QStandardItemModel;
        List_KatLabel_In_OS_ReLoad_To_Table(modelCombo_Kat);
        for(int i = 0 ; i < modelCombo_Kat->rowCount(); i++) {
            MaxKat->addItem(modelCombo_Kat->item(i,0)->text()   +" "+
                               modelCombo_Kat->item(i,1)->text()+" ");
            MinKat->addItem(modelCombo_Kat->item(i,0)->text()   +" "+
                               modelCombo_Kat->item(i,1)->text()+" ");
        }

        SaveList_Mac_User_to_OS[0] = MacAtr->itemText(0);
        SaveList_Mac_User_to_OS[1] = MaxLevMac->itemText(0).replace(QRegExp("^[ ]*[a-zA-Zа-яА-Я_-0-9]*"),"");
        SaveList_Mac_User_to_OS[2] = MinLevMac->itemText(0).replace(QRegExp("^[ ]*[a-zA-Zа-яА-Я_-0-9]*"),"");
        SaveList_Mac_User_to_OS[3] = MaxKat->itemText(0).replace(QRegExp("^[ ]*[a-zA-Zа-яА-Я_-0-9]*"),"");
        SaveList_Mac_User_to_OS[4] = MinKat->itemText(0).replace(QRegExp("^[ ]*[a-zA-Zа-яА-Я_-0-9]*"),"");

        layoutUser->addWidget(MacAtr,1, 1,Qt::AlignLeft);
        layoutUser->addWidget(MinLevMac,2, 1,Qt::AlignLeft);
        layoutUser->addWidget(MaxLevMac,3, 1,Qt::AlignLeft);
        layoutUser->addWidget(MinKat,4, 1,Qt::AlignLeft);
        layoutUser->addWidget(MaxKat,5, 1,Qt::AlignLeft);

        QObject::connect(MacAtr,SIGNAL(currentIndexChanged(QString)),this,SLOT(Slot_Admin1(QString)));
        QObject::connect(MinLevMac,SIGNAL(currentIndexChanged(QString)),this,SLOT(Slot_Admin2(QString)));
        QObject::connect(MaxLevMac,SIGNAL(currentIndexChanged(QString)),this,SLOT(Slot_Admin3(QString)));
        QObject::connect(MinKat,SIGNAL(currentIndexChanged(QString)),this,SLOT(Slot_Admin4(QString)));
        QObject::connect(MaxKat,SIGNAL(currentIndexChanged(QString)),this,SLOT(Slot_Admin5(QString)));

        layoutUser->addWidget(new QLabel("\nТекущее значение :\n"),11, 0);
        layoutUser->addWidget(new QLabel(text),12, 0);

        Yes->setMaximumWidth(100);
        layoutUser->addWidget(Yes,13, 1,Qt::AlignCenter);
        Error = new QTextEdit("");
        Error->setMaximumWidth(900);
        Error->setMinimumWidth(500);
        Error->setMaximumHeight(200);
        layoutUser->addWidget(Error,13,0,Qt::AlignLeft);
        QObject::connect(Yes,SIGNAL(clicked()),this,SLOT(Save_MacLabel_User1()));
    }
    if(TrwgtItem->parent() == DB) {
        QGridLayout* layoutDB = Start_Panel();

        int Index = DB->indexOfChild(Trwgt.currentItem());
        QString NameDB = DBMacLabel[Index].nameDB;

        layoutDB->addWidget(new QLabel("\nМандатная метка БД(" + NameDB +") = " + DBMacLabel[Index].MacLabel),0, 0);

        RedMacDB = new QPushButton("Изменить МРД");
        layoutDB->addWidget(RedMacDB,0, 1);
        connect(RedMacDB,SIGNAL(clicked()),this,SLOT(RedMac_DB()));

        RedMacTable = new QPushButton("Изменить МРД");
        layoutDB->addWidget(RedMacTable,2, 1);
        connect(RedMacTable,SIGNAL(clicked()),this,SLOT(RedMac_Table()));
        parametrI = 0;

        RedMacZapis = new QPushButton("Изменить МРД");
        layoutDB->addWidget(RedMacZapis,5, 1);
        connect(RedMacZapis,SIGNAL(clicked()),this,SLOT(RedMac_zapis()));


        if(DBMacLabel[Index].TableList.size() != 0){
            QComboBox * CMB1 = new QComboBox;
            QObject::connect(CMB1,SIGNAL(activated(int)),this,SLOT(Index_for_table_DB_change(int)));
            for(int i = 0 ; i < DBMacLabel[Index].TableList.size(); i++) {
            CMB1->addItem(DBMacLabel[Index].TableList[i].name  + "   "+
                          DBMacLabel[Index].TableList[i].MacLabel );}
            layoutDB->addWidget(new QLabel("Выбранная таблица:   " + CMB1->itemText(0)),2, 0);
            layoutDB->addWidget(CMB1,3, 0);

            QSqlDatabase db1 = QSqlDatabase::addDatabase("QPSQL" );
            db1.setHostName("localhost");
            db1.setDatabaseName(NameDB);
            db1.setUserName("postgres");
            db1.setPassword("toortoor");

            model_table_from_db = new QSqlTableModel;
            model_table_from_db->setEditStrategy(QSqlTableModel::OnManualSubmit);
            if (db1.open()) {
            QString ZaprosTable = "";
            QSqlQuery query_maclabel_estOrNet = QSqlQuery(db1);
            query_maclabel_estOrNet.exec("select maclabel from  \"" + DBMacLabel[Index].TableList[0].name  + "\""  );

            if("" == query_maclabel_estOrNet.record().value(0)) {
               ZaprosTable =  "select maclabel, * from  \"" + DBMacLabel[Index].TableList[0].name  + "\"" ;
            } else {
                ZaprosTable =  "select * from  \"" + DBMacLabel[Index].TableList[0].name  + "\"" ;
            }

            QTableView *newView = new QTableView;

            QSqlQuery query_table = QSqlQuery(db1);
            QSqlQueryModel *model = new QSqlTableModel(newView);

            query_table.exec(ZaprosTable);
            model->setQuery(query_table);

            newView->setModel(model);
            layoutDB->addWidget(newView,4, 0);
            }
        }
    }
}

void admin::Index_for_table_DB_change(int index) {
    int Index = DB->indexOfChild(Trwgt.currentItem());

    QGridLayout* layoutDB = Start_Panel();

    layoutDB->addWidget(new QLabel("\nМандатная метка БД(" + DBMacLabel[Index].nameDB +") = " + DBMacLabel[Index].MacLabel),0, 0);

        QComboBox * CMB1 = new QComboBox;
        QObject::connect(CMB1,SIGNAL(activated(int)),this,SLOT(Index_for_table_DB_change(int)));
        for(int i = 0 ; i < DBMacLabel[Index].TableList.size(); i++) {
        CMB1->addItem(DBMacLabel[Index].TableList[i].name  + "   "+
                      DBMacLabel[Index].TableList[i].MacLabel );}

        layoutDB->addWidget(new QLabel("Выбранная таблица:   " + CMB1->itemText(index)),2, 0);

        layoutDB->addWidget(CMB1,3, 0);

        RedMacDB = new QPushButton("Изменить МРД");
        layoutDB->addWidget(RedMacDB,0, 1);
        connect(RedMacDB,SIGNAL(clicked()),this,SLOT(RedMac_DB()));

        RedMacTable = new QPushButton("Изменить МРД");
        layoutDB->addWidget(RedMacTable,2, 1);
        connect(RedMacTable,SIGNAL(clicked()),this,SLOT(RedMac_Table()));
        parametrI = index;

        RedMacZapis = new QPushButton("Изменить МРД");
        layoutDB->addWidget(RedMacZapis,5, 1);
        connect(RedMacZapis,SIGNAL(clicked()),this,SLOT(RedMac_zapis()));


    QSqlDatabase db1 = QSqlDatabase::addDatabase("QPSQL" );
    db1.setHostName("localhost");
    db1.setDatabaseName(DBMacLabel[Index].nameDB);
    db1.setUserName("postgres");
    db1.setPassword("toortoor");

    model_table_from_db = new QSqlTableModel;
    model_table_from_db->setEditStrategy(QSqlTableModel::OnManualSubmit);
    if (db1.open()) {
    QString ZaprosTable = "";
    QSqlQuery query_maclabel_estOrNet = QSqlQuery(db1);
    query_maclabel_estOrNet.exec("select maclabel from  \"" + DBMacLabel[Index].TableList[index].name  + "\""  );

    if("" == query_maclabel_estOrNet.record().value(0)) {
       ZaprosTable =  "select maclabel, * from  \"" + DBMacLabel[Index].TableList[index].name  + "\"" ;
    } else {
        ZaprosTable =  "select * from  \"" + DBMacLabel[Index].TableList[index].name  + "\"" ;
    }

    QSqlQuery query_table = QSqlQuery(db1);
    query_table.exec(ZaprosTable);

    QTableView *newView = new QTableView;
    QSqlQueryModel *model = new QSqlTableModel(newView);
    model->setQuery(query_table);
    newView->setModel(model);

    newView->setModel(model_table_from_db);
    layoutDB->addWidget(newView,4, 0);
    }
}

void admin::Save_MacLabel_User1() {

    QStringList strlist;
    QString arg0 = SaveList_Mac_User_to_OS[0].replace(QRegExp("[ ]*"),""); // целостность
    QString arg1 = SaveList_Mac_User_to_OS[1].replace(QRegExp("[ ]*"),""); // макс. мак
    QString arg2 = SaveList_Mac_User_to_OS[2].replace(QRegExp("[ ]*"),""); // мин. мак.
    QString arg3 = SaveList_Mac_User_to_OS[3].replace(QRegExp("[ ]*"),""); // макс. кат.
    QString arg4 = SaveList_Mac_User_to_OS[4].replace(QRegExp("[ ]*"),""); // мин. кат.

    int Index = Users->indexOfChild(Trwgt.currentItem());
    QString user_name = Users->child(Index)->text(0);

    if((arg2 >= arg1) && (arg4 >= arg3 )) {
    QProcess * Mac_Add = new QProcess;
    QString Zap = "pdp-ulbls -m " + arg1 + ":" + arg2 +
            " -i " + arg0 + " -c " + arg3 + ":" + arg4 +
            " " + user_name;
    Mac_Add->start(Zap);
    Mac_Add->waitForReadyRead();
    strlist.append(Mac_Add->readAllStandardOutput());

    QString val = strlist.value(0);
    if(val != "") {
    Error->setText(val);}
    else {
        Error->setText("Error : Сравнение категорий происходит не по абсолютному значению!");
    }
    } else {
        Error->setText("Error : Макс. уровень или макс. набор категорий не может превышать минимальный!");
    }
}

void admin::Visible_Main_Window() {
    pMainWindow->setVisible(true);

    pMainWindow->ReturnUser()->setEnabled(true);
    pMainWindow->Returnpassword()->setEnabled(true);
    pMainWindow->ReturnbtnConnect()->setEnabled(true);

    pMainWindow->ReturnUser()->setText("");
    pMainWindow->Returnpassword()->setText("");
}

// Мандатные метки/Уровни. MAIN
void admin::Visible_Mac_Level(){

    QGridLayout * layout_new123 = Start_Panel();

    QLabel *l1 = new QLabel(tr("\nУровни : "), this);
    QLabel *l2 = new QLabel(tr("Создать/Удалить"), this);

    QStandardItemModel *model = new QStandardItemModel;

    QStringList horizontalHeader;
    horizontalHeader.append("Наименование");
    horizontalHeader.append("Значение");

    model->setHorizontalHeaderLabels(horizontalHeader);

    List_MacLabel_In_OS_ReLoad_To_Table(model);
    model_Table_OSMac = model;

    Table_Level_Mac = new QTableView;
    Table_Level_Mac->setModel(model);
    Table_Level_Mac->resizeRowsToContents();
    Table_Level_Mac->resizeColumnsToContents();

    layout_new123->addWidget(l1,0, 0 );
    layout_new123->addWidget(Table_Level_Mac,1, 0 );
    layout_new123->addWidget(l2,2, 0 );
    QPushButton *Change_Mac_add = new QPushButton("+");
    QPushButton *Change_Mac_del = new QPushButton("-");

    QGridLayout *AddDell = new QGridLayout;
    AddDell->addWidget(Change_Mac_add,1, 0 , Qt::AlignRight);
    AddDell->addWidget(Change_Mac_del,1, 1 , Qt::AlignLeft);
    QWidget * Wd = new QWidget;
    Wd->setLayout(AddDell);
    Wd->show();
    layout_new123->addWidget(Wd, 3, 0 );


    QPushButton *Change_Mac = new QPushButton("Применить");
    layout_new123->addWidget(Change_Mac,4, 0 );
    QObject::connect(Change_Mac,SIGNAL(clicked()),this,SLOT(Save_MacLabel()));
    QObject::connect(Change_Mac_add,SIGNAL(clicked()),this,SLOT(Add_Row_Table()));
    QObject::connect(Change_Mac_del,SIGNAL(clicked()),this,SLOT(Del_Row_Table()));
}

//// Сохранить в ОС Linux
void admin::Add_Row_Table() {
   model_Table_OSMac->insertRow(model_Table_OSMac->rowCount());
}
void admin::Del_Row_Table() {

   int count = Table_Level_Mac->selectionModel()->selectedRows().count();
   for(int i = 0 ; i < count ; i++) {
       Table_Level_Mac->model()->removeRow(Table_Level_Mac->selectionModel()->selectedRows().at(i).row(),QModelIndex());
   }
}
bool OS_Del_MacLabel_All(QString N) {

    QProcess * Mac_Del = new QProcess;
    Mac_Del->start("userlev -d " + N);
    Mac_Del->waitForReadyRead();
    QStringList strlist ;
    strlist.append(Mac_Del->readAllStandardOutput());

    if(strlist.value(0) != "" ) {
        newMessage = new QMessageBox(QMessageBox::Question , "Ошибка!",strlist.value(0),QMessageBox::Yes);
        newMessage->exec();
        return 0;
    }
    return 1;
}
bool OS_Add_MacLabel(QString arg0,QString arg1) {

    QStringList strlist ;

    QProcess * Mac_Add = new QProcess;
    Mac_Add->start("userlev " + arg0 + " -a " + arg1);
    Mac_Add->waitForReadyRead();
    strlist.append(Mac_Add->readAllStandardOutput());

    if(strlist.value(0) != "" ) {
        newMessage = new QMessageBox(QMessageBox::Question , "Ошибка!",strlist.value(0),QMessageBox::Yes);
        newMessage->exec();
        return 0;
    }
    return 1;
}
void admin::Save_MacLabel() {

    QStandardItemModel *model = new QStandardItemModel;
    List_MacLabel_In_OS_ReLoad_To_Table(model);

    for(int i = 0 ; i < model->rowCount(); ++i) {
         if(!OS_Del_MacLabel_All(model->item(i,1)->text())) {
         }
    }

    int count = 0;
    for(int i = 0 ; i < model_Table_OSMac->rowCount(); ++i) {

        if(OS_Add_MacLabel(model_Table_OSMac->item(i,0)->text(),model_Table_OSMac->item(i,1)->text()) ) {
              count ++;
        }

    }

if(count == model_Table_OSMac->rowCount()) {
    newMessage = new QMessageBox(QMessageBox::Question , "Успех" ,"Настройки успешно применены",QMessageBox::Yes);
    newMessage->exec();
} else {

    // Перезапись старых меток
    // Удаление
    QStandardItemModel *model_new = new QStandardItemModel;
    List_MacLabel_In_OS_ReLoad_To_Table(model_new);

    for(int i = 0 ; i < model_new->rowCount(); ++i) {
         if(!OS_Del_MacLabel_All(model_new->item(i,1)->text())) {
             return;
         }
    }
    // Запись
    for(int i = 0 ; i < model->rowCount(); ++i) {

        if(!OS_Add_MacLabel(model->item(i,0)->text(),model->item(i,1)->text()) ) {
             return;
         }
    }
    newMessage = new QMessageBox(QMessageBox::Question , "Ошибка!!!" ,"Проверьте правильность введенных данных!!!",QMessageBox::Yes);
    newMessage->exec();
}
}

void admin::Visible_User_add() {

    clear *c1 = new clear;
    c1->show();
}

void admin::Visible_Mac_Kategorii(){};

QVector <ListDB>  admin::Spisok_BD() {
     return DBMacLabel;
}
void admin::RedMac_DB() {
    int Index = DB->indexOfChild(Trwgt.currentItem());
    QString name = DBMacLabel[Index].nameDB;
    QString mac  = DBMacLabel[Index].MacLabel;

    Mac *m1 = new Mac;
    m1->show();

    m1->Red_mac_DB(name,"",mac);
    connect(m1,SIGNAL(UpDataList()),this,SLOT(UpDataList_MacDB()));
}
void admin::UpDataList_MacDB() {
   int Index = DB->indexOfChild(Trwgt.currentItem());
   ShowAllTreeList();
   Trwgt.setCurrentItem(DB->child(Index));
      Index_for_table_DB_change(parametrI);
}
void admin::RedMac_Table() {
    int Index = DB->indexOfChild(Trwgt.currentItem());
    QString name = DBMacLabel[Index].nameDB;

    int index = parametrI;
    QString Table_name=  DBMacLabel[Index].TableList[index].name;
    QString mac = DBMacLabel[Index].TableList[index].MacLabel ;

    Mac *m1 = new Mac;
    m1->show();

    QString mac_new;
    m1->Red_mac_DB(name,Table_name,mac);
    connect(m1,SIGNAL(UpDataList()),this,SLOT(UpDataList_MacTable()));
}
void admin::UpDataList_MacTable() {
   int Index = DB->indexOfChild(Trwgt.currentItem());
   ShowAllTreeList();
   Trwgt.setCurrentItem(DB->child(Index));
   Index_for_table_DB_change(parametrI);
}
void admin::RedMac_zapis() {

    int Index = DB->indexOfChild(Trwgt.currentItem());
    QString NameDB = DBMacLabel[Index].nameDB;
    QString Table_name =  DBMacLabel[Index].TableList[parametrI].name;

    QSqlDatabase db1 = QSqlDatabase::addDatabase("QPSQL" );
    db1.setHostName("localhost");
    db1.setDatabaseName(NameDB);
    db1.setUserName("postgres");
    db1.setPassword("toortoor");

    if (db1.open()) {

     QSqlQuery query = QSqlQuery(db1);

    QString ERROR = "";
    for(int i = 0 ; i < model_table_from_db->rowCount(); i++ ) {
        QSqlRecord rec = model_table_from_db->record(i);
        QString mac = rec.value("maclabel").toString();
        QVariant Iint = i+1;
        QString Sint = Iint.toString();
        QString zapros = "chmac " + Table_name + " set maclabel = ' " + mac + " ' where \"N\" = "+ Sint +";";
        query.exec(zapros);
        QString error_query = query.lastError().text();
        if(error_query != " ") {ERROR +="\nСТРОКА №" + Sint + " \n " + error_query + "\n";}
    }

    if(ERROR == "") {
        newMessage = new QMessageBox(QMessageBox::Question , "Отчет!","МРД применены",QMessageBox::Yes);
        newMessage->exec();
    } else {
        newMessage = new QMessageBox(QMessageBox::Question , "Ошибка!",ERROR,QMessageBox::Yes);
        newMessage->exec();
    }
    }
}


