#ifndef CMPSYNCH_H
#define CMPSYNCH_H

#include <QMainWindow>
#include <QFileDialog>
#include <QSettings>

#include "transfermanager.h"

#define  SETTINGS_FILE      "cmpsynch.ini"
#define  SETTINGS_SAVE_DIR  "save_directory"

namespace Ui {
class CMPSynch;
}

class CMPSynch : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMPSynch(QWidget *parent = 0);
    ~CMPSynch();

private slots:
    void on_pbSearch_clicked();
    void on_connectionToMaster();

private:
    Ui::CMPSynch *ui;

    TransferManager dataReceiver;

    typedef enum {MT_NORMAL,MT_ERROR,MT_SUCCESS} LogMessageType;
    void addMessage(QString message, LogMessageType lmt =  MT_NORMAL);



};

#endif // CMPSYNCH_H
