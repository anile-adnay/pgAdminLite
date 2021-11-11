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
