#include "cmpsynch.h"
#include "ui_cmpsynch.h"

CMPSynch::CMPSynch(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMPSynch)
{
    ui->setupUi(this);

    QSettings settings(SETTINGS_FILE);
    QString saveDir = settings.value(SETTINGS_SAVE_DIR,"").toString();
    if (saveDir.isEmpty()){
        addMessage("No save directory found",MT_ERROR);
    }
    else{
        addMessage("Save directory is " + saveDir);
        dataReceiver.setSaveDirectory(saveDir);
    }

    connect(&dataReceiver,&TransferManager::connectionEstablished,this,&CMPSynch::on_connectionToMaster);
    dataReceiver.listenForUDPAnnouncement("Colo Phone");
}

CMPSynch::~CMPSynch()
{
    delete ui;
}

void CMPSynch::on_connectionToMaster(){
    addMessage("Master found: " + dataReceiver.getMasterID(),MT_SUCCESS);
}

void CMPSynch::on_pbSearch_clicked()
{
    addMessage("Search pressed");
    QFileDialog diag(this);
    diag.setWindowTitle("Select destination");
    diag.setFileMode(QFileDialog::DirectoryOnly);
    diag.showMaximized();
    if (diag.exec()){
        QString chosen = diag.selectedFiles().first();
        addMessage("Selected " + chosen,MT_SUCCESS);
        dataReceiver.setSaveDirectory(chosen);
        QSettings settings(SETTINGS_FILE);
        settings.setValue(SETTINGS_SAVE_DIR,chosen);

    }
    else{
        addMessage("Cancel button was pressed ",MT_ERROR);
    }
}


void CMPSynch::addMessage(QString message, LogMessageType lmt){
    QColor color;

    switch (lmt) {
    case MT_ERROR:
        color = QColor(255,0,0);
        break;
    case MT_NORMAL:
        color = QColor(255,255,255);
        break;
    case MT_SUCCESS:
        color = QColor(0,255,0);
        break;
    }

    ui->teLog->setTextColor(color);
    ui->teLog->append(message);
}
