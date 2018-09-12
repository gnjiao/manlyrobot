#include "projmanageDlg.h"
#include "msqldb.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 项目管理对话框
ProjManageDlg::ProjManageDlg(QWidget *parent) :
    QDialog(parent)
{
    dlgWidth=500;
    dlgHeight=500;
    nameEdit=NULL;
    program=new QProcess(this);
    keybaordOpenedFlag=0;
    changeMod=0;
}

ProjManageDlg::~ProjManageDlg()
{
    delete program;
}

void ProjManageDlg::initDialog()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT+4);
    ft.setItalic(false);

    QLabel *capLabel=new QLabel(tr("焊接项目"));
    capLabel->setFont(ft);

    createProjectBox();
    createButtonLayout();
    listWidget=new QListWidget;
    listWidget->setFont(ft);
    listWidget->setCurrentRow(0);

    QPushButton *cancelButton=new QPushButton(tr("关闭"));
    cancelButton->setFont(ft);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(OnCancelBtnClicked()));

    QVBoxLayout *projLayout=new QVBoxLayout;
    projLayout->addWidget(capLabel);
    projLayout->addWidget(listWidget);
    projLayout->addWidget(cancelButton);
    //projLayout->addLayout(btnLayout);

    QHBoxLayout *layout=new QHBoxLayout;
    layout->addLayout(projLayout);
    layout->addLayout(btnLayout);
    layout->addWidget(projBox);

    setLayout(layout);
    setWindowTitle(tr("项目管理"));
    resize(dlgWidth,dlgHeight);
    projBox->hide();

    QMessageBox msgBox;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }
    if(db.ReadProjNameFromDb(projectNameList))
    {
        //QListWidgetItem *item=new QListWidgetItem("0");
        //item->setFlags(Qt::ItemIsUserCheckable);
        //item->setText(projectName[i]);
        for(int i=0;i<projectNameList.size();i++)
            listWidget->addItem(projectNameList[i]);
    }
    db.closeDB();
    if(listWidget->count()>0)
        listWidget->setCurrentRow(0);
}

void ProjManageDlg::createProjectBox()
{   
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT+4);
    ft.setItalic(false);

    QLabel *projNameLabel=new QLabel(tr("项目名称:"));
    projNameLabel->setFont(ft);
    nameEdit=new QLineEdit;
    nameEdit->setFont(ft);
    nameEdit->setAlignment(Qt::AlignCenter);
    directionLabel=new QLabel(tr("镀锡面方向:"));
    directionLabel->setFont(ft);
    directComboBox=new QComboBox;
    directComboBox->setFont(ft);
    directComboBox->addItem(tr("前面"));
    directComboBox->addItem(tr("后面"));
    directComboBox->addItem(tr("左面"));
    directComboBox->addItem(tr("右面"));
    directComboBox->setCurrentIndex(0);
    QPushButton * okBtn=new QPushButton(tr("确定"));
    okBtn->setFont(ft);
    connect(okBtn, SIGNAL(clicked()), this, SLOT(OnOkBtnClicked()));

    QVBoxLayout *projLayout = new QVBoxLayout;
    projLayout->addWidget(projNameLabel);
    projLayout->addWidget(nameEdit);
    projLayout->addStretch();
    projLayout->addWidget(directionLabel);
    projLayout->addWidget(directComboBox);
    projLayout->addStretch();
    projLayout->addWidget(okBtn);
    projLayout->addStretch();

    projBox=new QGroupBox;
    projBox->setLayout(projLayout);
    projBox->setMaximumWidth(200);
}

void ProjManageDlg::createButtonLayout()
{
    //QPalette palette = QPalette();  //  得到窗口部件的调色板
    //palette.setColor(QPalette::WindowText, QColor(OPERATE_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT+4);
    ft.setItalic(false);

    delButton=new QPushButton(tr("删除"));
    delButton->setFont(ft);
    connect(delButton, SIGNAL(clicked()), this, SLOT(OnDelBtnClicked()));
    copyButton=new QPushButton(tr("复制"));
    copyButton->setFont(ft);
    connect(copyButton, SIGNAL(clicked()), this, SLOT(OnCopyBtnClicked()));
    changeButton=new QPushButton(tr("修改"));
    changeButton->setFont(ft);
    connect(changeButton, SIGNAL(clicked()), this, SLOT(OnChangeBtnClicked()));
    importButton=new QPushButton(tr("导入项目"));
    importButton->setFont(ft);
    connect(importButton, SIGNAL(clicked()), this, SLOT(OnImportBtnClicked()));
    emportCheck=new QCheckBox(tr("导出"));
    emportCheck->setFont(ft);
    connect(emportCheck, SIGNAL(clicked()), this, SLOT(OnExportChkClicked()));
    exportButton=new QPushButton(tr("导出项目"));
    exportButton->setFont(ft);
    exportButton->setEnabled(false);
    connect(exportButton, SIGNAL(clicked()), this, SLOT(OnExportBtnClicked()));

    QVBoxLayout *exLayout=new QVBoxLayout;
    exLayout->addWidget(emportCheck);
    exLayout->addWidget(exportButton);

    btnLayout=new QVBoxLayout;
    btnLayout->addWidget(delButton);
    btnLayout->addWidget(copyButton);
    btnLayout->addWidget(changeButton);
    btnLayout->addWidget(importButton);
    btnLayout->addLayout(exLayout);
}

bool ProjManageDlg::event(QEvent *e)
{
    if(nameEdit==NULL) return QWidget::event(e);
    if(nameEdit->hasFocus())
    {
        if(keybaordOpenedFlag==0)
        {
            program->start("C:/Progra~1/Common~1/Micros~1/ink/TabTip.exe");
            program->waitForStarted();
            program->waitForFinished();
            keybaordOpenedFlag=1;
        }
    }
    else
    {
        program->kill();
    }
    return QWidget::event(e);
}

void ProjManageDlg::loadProjectContext()
{
}

void ProjManageDlg::OnDelBtnClicked()
{
    QMessageBox msgBox,msgBox1;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    if(listWidget->currentRow()==-1)
    {
        msgBox.setText(tr("请选择要删除的项目名称."));
        msgBox.exec();
        return;
    }

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }
    QString strProj=listWidget->item(listWidget->currentRow())->text();

    msgBox1.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox1.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox1.setFont(ft);
    msgBox1.setIcon(QMessageBox::Question);
    msgBox1.setWindowTitle(tr(APPLICATION_NAME));
    msgBox1.addButton(tr("是"), QMessageBox::YesRole);
    msgBox1.addButton(tr("否"), QMessageBox::NoRole);
    msgBox1.setText(tr("您是否真的要删除项目 %1?").arg(strProj));
    if(msgBox1.exec()==QMessageBox::RejectRole)
    {
        db.closeDB();
        return;
    }
    else
    {
        if(!db.DeleteProjectFromDb(strProj))
        {
            msgBox.setText(tr("删除所选择的项目失败."));
            msgBox.exec();
            db.closeDB();
            return;
        }
        if(!db.DeleteProjLogFromDb(strProj))
        {
            msgBox.setText(tr("删除所选择的项目产量失败."));
            msgBox.exec();
            db.closeDB();
            return;
        }
        if(!db.DeleteClearSolderFromDb(strProj))
        {
            msgBox.setText(tr("删除项目中的清锡操作失败."));
            msgBox.exec();
            db.closeDB();
            return;
        }
        listWidget->takeItem(listWidget->currentRow());
    }
    emit changeProjFinished();
}

void ProjManageDlg::OnCopyBtnClicked()
{
    changeMod=1;
    QMessageBox msgBox,msgBox1;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    if(listWidget->currentRow()==-1)
    {
        msgBox.setText(tr("请选择要复制的项目名称."));
        msgBox.exec();
        return;
    }

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }
    oldProjectName=listWidget->currentItem()->text();

    msgBox1.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox1.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox1.setFont(ft);
    msgBox1.setIcon(QMessageBox::Question);
    msgBox1.setWindowTitle(tr(APPLICATION_NAME));
    msgBox1.addButton(tr("是"), QMessageBox::YesRole);
    msgBox1.addButton(tr("否"), QMessageBox::NoRole);
    msgBox1.setText(QString(tr("您是否确定要复制项目 %1?")).arg(oldProjectName));
    if(msgBox1.exec()==QMessageBox::RejectRole)
    {
        db.closeDB();
        return;
    }
    else
    {
        directComboBox->hide();
        directionLabel->setText(QString(tr("准备复制项目 %1")).arg(oldProjectName));
    }
    int cnt=listWidget->findItems(oldProjectName+tr("-副本"),Qt::MatchStartsWith).size();
    if(cnt==0)
        nameEdit->setText(oldProjectName+tr("-副本"));
    else
        nameEdit->setText(oldProjectName+tr("-副本%1").arg(cnt));

    resize(dlgWidth+150,dlgHeight);
    projBox->show();
}

void ProjManageDlg::OnChangeBtnClicked()
{
    changeMod=0;
    QMessageBox msgBox;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    if(listWidget->currentRow()==-1)
    {
        msgBox.setText(tr("请选择要修改的项目名称."));
        msgBox.exec();
        return;
    }

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }

    directionLabel->setText(tr("镀锡面方向:"));
    directComboBox->show();

    oldProjectName=listWidget->currentItem()->text();
    PROJ_ITEM item;
    if(!db.ReadProjItemFromDb(oldProjectName,0,item))
    {
        db.closeDB();
        msgBox.setText(tr("从数据库读取数据失败."));
        msgBox.exec();
        return;
    }
    db.closeDB();
    nameEdit->setText(oldProjectName);
    if(item.direction==QString("up"))
        directComboBox->setCurrentIndex(0);
    else if(item.direction==QString("down"))
        directComboBox->setCurrentIndex(1);
    else if(item.direction==QString("left"))
        directComboBox->setCurrentIndex(2);
    else if(item.direction==QString("right"))
        directComboBox->setCurrentIndex(3);
    resize(dlgWidth+150,dlgHeight);
    projBox->show();
}

void ProjManageDlg::OnCancelBtnClicked()
{
    done(QDialog::Rejected);
}

void ProjManageDlg::OnOkBtnClicked()
{
    QMessageBox msgBox,msgBox1;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }

    QString newProjectName=nameEdit->text().trimmed();
    if(changeMod==0)    //修改
    {
        QString direction;
        if(directComboBox->currentIndex()==0)
            direction=QString("up");
        else if(directComboBox->currentIndex()==1)
            direction=QString("down");
        else if(directComboBox->currentIndex()==2)
            direction=QString("left");
        else if(directComboBox->currentIndex()==3)
            direction=QString("right");
        if(!db.UpdateProjectFromDb(oldProjectName,newProjectName,direction))
        {
            db.closeDB();
            msgBox.setText(tr("在数据库中更新数据失败."));
            msgBox.exec();
            return;
        }
        QStringList projectName;
        if(db.ReadProjNameFromDb(projectName))
        {
            listWidget->clear();
            for(int i=0;i<projectName.size();i++)
                listWidget->addItem(projectName[i]);
        }
        msgBox.setText(tr("修改完毕."));
        msgBox.exec();
    }
    else    //复制
    {
        QList <CLEAR_SOLDER> AllClearItem;
        if(db.ReadAllClearItemFromDb(oldProjectName,AllClearItem))  //复制清锡操作
        {
            db.SaveAllClearItemToDb(newProjectName,AllClearItem);
        }
        QList <PROJ_ITEM> AllItem;
        if(db.ReadProjAllItemFromDb(oldProjectName,AllItem))
        {
            if(db.SaveProjAllItemToDb(newProjectName,AllItem))
            {
                listWidget->addItem(newProjectName);
                msgBox.setText(tr("复制完毕."));
                msgBox.exec();
            }
            else
            {
                db.closeDB();
                msgBox.setText(tr("在数据库中写入数据失败."));
                msgBox.exec();
                return;
            }
        }
        else
        {
            msgBox.setText(tr("在数据库中读取原项目数据失败."));
            msgBox.exec();
        }
    }
    db.closeDB();
    projBox->hide();
    emit changeProjFinished();
    resize(dlgWidth,dlgHeight);
}

void ProjManageDlg::OnImportBtnClicked()    //导入项目
{
    QMessageBox msgBox;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }

    QString importFileName=QFileDialog::getOpenFileName(this,tr("导入项目"),"",tr("Json files(*.json)"));

    QFile file;
    file.setFileName(importFileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray byteArray = file.readAll();
    file.close();
    // 构建 JSON 文档
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))  // 解析未发生错误
    {
        if (doucment.isArray())  // JSON 文档为数组
        {
            QJsonArray array = doucment.array();  // 转化为数组
            int nSize = array.size();  // 获取数组大小
            for (int i = 0; i < nSize; ++i)   // 遍历数组
            {
                QJsonValue value = array.at(i);
                if (value.type() == QJsonValue::Object)
                {
                    QJsonObject obj = value.toObject();
                    QStringList keysList=obj.keys();
                    for(int k=0;k<keysList.size();k++)
                    {
                        if(keysList[k]=="clearSolderItem")  //清锡命令
                        {
                            if(obj.value("clearSolderItem").toObject().isEmpty())
                                continue;
                            CLEAR_SOLDER clearSolderItem;
                            clearSolderItem.projectName=obj.value("clearSolderItem").toObject().value("projectName").toString();
                            clearSolderItem.atspeed=obj.value("clearSolderItem").toObject().value("atspeed").toDouble();
                            clearSolderItem.Zh=obj.value("clearSolderItem").toObject().value("Zh").toDouble();
                            clearSolderItem.clear_H=obj.value("clearSolderItem").toObject().value("clear_H").toDouble();
                            clearSolderItem.clearTime=obj.value("clearSolderItem").toObject().value("clearTime").toInt();
                            for(int j=0;j<8;j++)
                            {
                                clearSolderItem.clearPos[j]=obj.value("clearSolderItem").toObject().value("clearPos").toArray().at(j).toDouble();
                            }
                            if(!db.SaveClearSolderToDb(clearSolderItem.projectName,clearSolderItem))
                            {
                                msgBox.setText(tr("%1项目清锡命令写入数据库失败.").arg(clearSolderItem.projectName));
                                msgBox.exec();
                                return;
                            }
                        }
                        else    //焊接项目
                        {
                            QList <PROJ_ITEM> AllItem;
                            QString projectName=keysList[k];
                            QJsonArray commandArray=obj.value(projectName).toArray();
                            for (int n = 0; n < nSize; n++)   // 遍历数组
                            {
                                PROJ_ITEM commandItem;
                                commandItem.commNumber=commandArray.at(n).toObject().value("commNumber").toInt();
                                commandItem.command=commandArray.at(n).toObject().value("command").toString();
                                commandItem.technics=commandArray.at(n).toObject().value("technics").toString();
                                commandItem.direction=commandArray.at(n).toObject().value("direction").toString();
                                commandItem.Zh=commandArray.at(n).toObject().value("Zh").toDouble();
                                commandItem.optimize=commandArray.at(n).toObject().value("optimize").toInt();
                                for(int m=0;m<8;m++)
                                {
                                    commandItem.pointCoordinate1[m]=commandArray.at(n).toObject().value("pointCoordinate1").toArray().at(m).toDouble();
                                    commandItem.pointCoordinate2[m]=commandArray.at(n).toObject().value("pointCoordinate2").toArray().at(m).toDouble();
                                }
                                AllItem.append(commandItem);
                            }
                            if(!db.SaveProjAllItemToDb(projectName,AllItem))
                            {
                                msgBox.setText(tr("%1项目数据写入数据库失败.").arg(projectName));
                                msgBox.exec();
                                return;
                            }
                            QList<QListWidgetItem *> itemList=listWidget->findItems(projectName,Qt::MatchFixedString);
                            if(itemList.size()==0)
                                listWidget->addItem(projectName);
                        }
                    }
                }
            }
        }
        msgBox.setText(tr("导入项目完毕."));
        msgBox.exec();
    }
}

void ProjManageDlg::OnExportChkClicked()    //导出检查框
{
    QMessageBox msgBox;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }

    if(emportCheck->checkState()!=Qt::Checked)
    {
        delButton->setEnabled(true);
        copyButton->setEnabled(true);
        changeButton->setEnabled(true);
        importButton->setEnabled(true);
        exportButton->setEnabled(false);
        QStringList projNameList;
        if(db.ReadProjNameFromDb(projNameList))
        {
            listWidget->clear();
            for(int i=0;i<projNameList.size();i++)
            {
                QListWidgetItem *item=new QListWidgetItem("0");
                item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                item->setText(projNameList[i]);
                listWidget->addItem(item);
            }
        }
    }
    else
    {
        delButton->setEnabled(false);
        copyButton->setEnabled(false);
        changeButton->setEnabled(false);
        importButton->setEnabled(false);
        exportButton->setEnabled(true);
        QStringList projNameList;
        if(db.ReadProjNameFromDb(projNameList))
        {
            listWidget->clear();
            listCheckBox.clear();
            for(int i=0;i<projNameList.size();i++)
            {
                QCheckBox *exCheck=new QCheckBox;
                listCheckBox.append(exCheck);
                QListWidgetItem *item=new QListWidgetItem("0");
                item->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                item->setText(QString("   ")+projNameList[i]);
                listWidget->addItem(item);
                listWidget->setItemWidget(item,exCheck);
            }
        }
    }
    db.closeDB();
}

void ProjManageDlg::OnExportBtnClicked()    //导出项目
{
    QMessageBox msgBox,msgBox1;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    QStringList selectedProjNameList;
    for(int i=0;i<listWidget->count();i++)
    {
        if(listCheckBox[i]->checkState()==Qt::Checked)
        {
            selectedProjNameList.append(listWidget->item(i)->text().right(listWidget->item(i)->text().length()-3));
        }
    }
    if(selectedProjNameList.size()==0)
    {
        msgBox.setText(tr("请选择要导出的项目."));
        msgBox.exec();
        return;
    }

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }

    QJsonArray projArray;

    for(int i=0;i<selectedProjNameList.size();i++)
    {
        QJsonObject projJsonObt;
        QList <PROJ_ITEM> AllItem;
        CLEAR_SOLDER clearSolderItem;
        clearSolderItem.projectName.clear();
        if(!db.ReadProjAllItemFromDb(selectedProjNameList[i],AllItem))
        {
            msgBox.setText(tr("从数据库中读取焊接项目%1的数据失败,停止导出").arg(selectedProjNameList[i]));
            msgBox.exec();
            return;
        }
        db.ReadClearSolderFromDb(selectedProjNameList[i],clearSolderItem);
        projJsonObt=createProjectJson(selectedProjNameList[i],AllItem,clearSolderItem);
        projArray.append(QJsonValue(projJsonObt));
    }

    db.closeDB();

    QDateTime datetime=QDateTime::currentDateTime();
    QString strDateTime=datetime.toString("yyyyMMddhhmmss");
    QString exportFileName1=tr("导出项目%1.json").arg(strDateTime);
    QString exportFileName=QFileDialog::getSaveFileName(this,tr("导出项目"),exportFileName1,tr("Json files(*.json)"));

    QJsonDocument jsonDoc(projArray);
    QByteArray ba = jsonDoc.toJson();
    QFile file(exportFileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        msgBox.setText(tr("向磁盘写入文件失败."));
        msgBox.exec();
        return ;
    }
    file.write(ba);
    file.close();
    msgBox.setText(tr("导出成功，已导出的工程项目文件名为\n“%1”").arg(exportFileName));
    msgBox.exec();
}

QJsonObject ProjManageDlg::createCommandJson(PROJ_ITEM item)
{
    QJsonObject commandJson;
    QJsonArray pointCoordinate1,pointCoordinate2;
    for(int i=0;i<8;i++)
    {
        pointCoordinate1.append(QJsonValue(item.pointCoordinate1[i]));
        pointCoordinate2.append(QJsonValue(item.pointCoordinate2[i]));
    }

    commandJson.insert("commNumber",item.commNumber);
    commandJson.insert("command",item.command);
    commandJson.insert("technics",item.technics);
    commandJson.insert("pointCoordinate1",QJsonValue(pointCoordinate1));
    commandJson.insert("pointCoordinate2",QJsonValue(pointCoordinate2));
    commandJson.insert("direction",item.direction);
    commandJson.insert("Zh",item.Zh);
    commandJson.insert("optimize",item.optimize);

    return commandJson;
}

QJsonObject ProjManageDlg::createClearJson(CLEAR_SOLDER clearSolderItem)
{
    QJsonObject clearSolderJson;
    QJsonArray pointCoordinate;
    if(clearSolderItem.projectName.isEmpty())
        return clearSolderJson;

    for(int i=0;i<8;i++)
    {
        pointCoordinate.append(QJsonValue(clearSolderItem.clearPos[i]));
    }

    clearSolderJson.insert("projectName",clearSolderItem.projectName);
    clearSolderJson.insert("atspeed",clearSolderItem.atspeed);
    clearSolderJson.insert("Zh",clearSolderItem.Zh);
    clearSolderJson.insert("clear_H",clearSolderItem.clear_H);
    clearSolderJson.insert("clearTime",clearSolderItem.clearTime);
    clearSolderJson.insert("clearPos",QJsonValue(pointCoordinate));

    return clearSolderJson;
}

QJsonObject ProjManageDlg::createProjectJson(QString projName,QList <PROJ_ITEM> AllIteme,CLEAR_SOLDER clearSolderItem)
{
    QJsonObject projectdJson;
    QJsonArray commandArray;

    for(int i=0;i<AllIteme.size();i++)
    {
        commandArray.append(QJsonValue(createCommandJson(AllIteme[i])));
    }

    projectdJson.insert(projName,QJsonValue(commandArray));
    projectdJson.insert("clearSolderItem",QJsonValue(createClearJson(clearSolderItem)));

    return projectdJson;
}
