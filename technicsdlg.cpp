#include "technicsdlg.h"
#include "pointwelddlg.h"
#include "linewelddlg.h"
#include "msqldb.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 工艺参数类
TechnicsDlg::TechnicsDlg(QWidget *parent) :
    QDialog(parent)
{
    type=0;
    saveFlag=0;
    saveAsEdit=0;
    program=new QProcess(this);
    keybaordOpenedFlag=0;
}

void TechnicsDlg::initDialog()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT+4);
    ft.setItalic(false);

    createButtonLayout();
    listWidget=new QListWidget;
    listWidget->setFont(ft);

    QHBoxLayout *layout=new QHBoxLayout;
    layout->addWidget(listWidget);
    layout->addStretch();
    layout->addLayout(btnLayout);
    layout->addStretch();

    setLayout(layout);
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

    if(type==POINT_WELD_TECHNICS)
    {
        setWindowTitle(tr("点焊工艺"));
    }
    else
    {
        setWindowTitle(tr("拖焊工艺"));
    }
    resize(400,500);
}
void TechnicsDlg::createButtonLayout()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT+4);
    ft.setItalic(false);

    loadButton=new QPushButton(tr("加载"));
    loadButton->setFont(ft);
    connect(loadButton, SIGNAL(clicked()), this, SLOT(OnLoadBtnClicked()));
    cancelButton=new QPushButton(tr("关闭"));
    cancelButton->setFont(ft);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(OnCancelBtnClicked()));
    newTechnicsButton=new QPushButton(tr("新建"));
    newTechnicsButton->setFont(ft);
    connect(newTechnicsButton, SIGNAL(clicked()), this, SLOT(OnNewTechnicsBtnClicked()));
    editButton=new QPushButton(tr("编辑"));
    editButton->setFont(ft);
    connect(editButton, SIGNAL(clicked()), this, SLOT(OnEditBtnClicked()));
    deleteButton=new QPushButton(tr("删除"));
    deleteButton->setFont(ft);
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(OnDeleteBtnClicked()));
    saveAsButton=new QPushButton(tr("另存为..."));
    saveAsButton->setFont(ft);
    connect(saveAsButton, SIGNAL(clicked()), this, SLOT(OnSaveAsBtnClicked()));
    QPushButton *renameButton=new QPushButton(tr("重命名"));
    renameButton->setFont(ft);
    connect(renameButton, SIGNAL(clicked()), this, SLOT(OnRenameBtnClicked()));
    importButton=new QPushButton(tr("导入工艺"));
    importButton->setFont(ft);
    connect(importButton, SIGNAL(clicked()), this, SLOT(OnImportBtnClicked()));
    emportCheck=new QCheckBox(tr("导出"));
    emportCheck->setFont(ft);
    connect(emportCheck, SIGNAL(clicked()), this, SLOT(OnExportChkClicked()));
    exportButton=new QPushButton(tr("导出工艺"));
    exportButton->setFont(ft);
    exportButton->setEnabled(false);
    connect(exportButton, SIGNAL(clicked()), this, SLOT(OnExportBtnClicked()));

    saveAsEdit=new QLineEdit;
    saveAsEdit->setFont(ft);
    saveAsEdit->hide();
    QHBoxLayout* saveLayout=new QHBoxLayout;
    saveLayout->addWidget(saveAsEdit);
    saveLayout->addWidget(saveAsButton);

    btnLayout=new QVBoxLayout;
    btnLayout->addWidget(loadButton);
    btnLayout->addSpacing(20);
    btnLayout->addWidget(newTechnicsButton);
    btnLayout->addWidget(editButton);
    btnLayout->addLayout(saveLayout);
    btnLayout->addWidget(deleteButton);
    btnLayout->addWidget(importButton);
    btnLayout->addWidget(emportCheck);
    btnLayout->addWidget(exportButton);
    btnLayout->addWidget(cancelButton);
    //btnLayout->addWidget(copyButton);
    //btnLayout->addWidget(renameButton);
}

bool TechnicsDlg::event(QEvent *e)
{
    if(saveAsEdit==0) return QWidget::event(e);
    if(saveAsEdit->hasFocus())
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

void TechnicsDlg::OnLoadBtnClicked()
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

    if(listWidget->currentRow()==-1)
    {
        msgBox.setText(tr("请选择要加载的工艺参数名称."));
        msgBox.exec();
        return;
    }
    currTechnicsName=listWidget->currentItem()->text();
    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }
    if(type==POINT_WELD_TECHNICS)   //点焊工艺参数
    {
        if(!db.ReadPointWeldFromDb(currTechnicsName,weldProject->currPointWeldPara))
        {
            msgBox.setText(tr("加载点焊工艺参数失败."));
            msgBox.exec();
            db.closeDB();
            return;
        }
        else
        {
            weldProject->currPointTechnicsName=currTechnicsName;
        }
    }
    else        //拖焊工艺参数
    {
        if(!db.ReadLineWeldFromDb(currTechnicsName,weldProject->currLineWeldPara))
        {
            msgBox.setText(tr("加载拖焊工艺参数失败."));
            msgBox.exec();
            db.closeDB();
            return;
        }
        else
        {
            weldProject->currLineTechnicsName=currTechnicsName;
        }
    }
    db.closeDB();

    done(QDialog::Accepted);
}

void TechnicsDlg::OnCancelBtnClicked()
{
    done(QDialog::Rejected);
}

void TechnicsDlg::OnNewTechnicsBtnClicked()
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

    if(type==POINT_WELD_TECHNICS)
    {
        PointWeldDlg *dlg=new PointWeldDlg();
        dlg->type=2;    //新建点焊工艺参数
        dlg->pointWeldPara.atspeed=weldProject->currPointWeldPara.atspeed;
        dlg->pointWeldPara.Dlt_X=weldProject->currPointWeldPara.Dlt_X;
        dlg->pointWeldPara.Dlt_Y=weldProject->currPointWeldPara.Dlt_Y;
        dlg->pointWeldPara.Dlt_Z=weldProject->currPointWeldPara.Dlt_Z;
        dlg->pointWeldPara.Dlt_speed=weldProject->currPointWeldPara.Dlt_speed;
        dlg->pointWeldPara.FT_Pre_Length=weldProject->currPointWeldPara.FT_Pre_Length;
        dlg->pointWeldPara.FT_Pre_Speed=weldProject->currPointWeldPara.FT_Pre_Speed;
        dlg->pointWeldPara.FT_Pre_Modle=weldProject->currPointWeldPara.FT_Pre_Modle;
        dlg->pointWeldPara.Soldering_style=weldProject->currPointWeldPara.Soldering_style;
        dlg->pointWeldPara.ShakeSolder_Length=weldProject->currPointWeldPara.ShakeSolder_Length;
        dlg->pointWeldPara.ShakeSolder_speed=weldProject->currPointWeldPara.ShakeSolder_speed;
        dlg->pointWeldPara.S1_Length=weldProject->currPointWeldPara.S1_Length;
        dlg->pointWeldPara.S1_T_Length=weldProject->currPointWeldPara.S1_T_Length;
        dlg->pointWeldPara.S_speed=weldProject->currPointWeldPara.S_speed;
        dlg->pointWeldPara.S2_Length=weldProject->currPointWeldPara.S2_Length;
        dlg->pointWeldPara.S2_T_Length=weldProject->currPointWeldPara.S2_T_Length;
        dlg->pointWeldPara.Dlt_TX=weldProject->currPointWeldPara.Dlt_TX;
        dlg->pointWeldPara.Dlt_TY=weldProject->currPointWeldPara.Dlt_TY;
        dlg->pointWeldPara.Dlt_TZ=weldProject->currPointWeldPara.Dlt_TZ;
        dlg->pointWeldPara.T1_Delay_time=weldProject->currPointWeldPara.T1_Delay_time;
        dlg->pointWeldPara.T2_Delay_time=weldProject->currPointWeldPara.T2_Delay_time;
        dlg->pointWeldPara.T3_Delay_time=weldProject->currPointWeldPara.T3_Delay_time;
        dlg->pointWeldPara.v_Over_Time_count=weldProject->currPointWeldPara.v_Over_Time_count;
        dlg->initDialog();
        if(dlg->exec()==QDialog::Accepted)
        {
            S_P pointWeldPara;
            pointWeldPara.atspeed =dlg->pointWeldPara.atspeed;
            pointWeldPara.Dlt_X = dlg->pointWeldPara.Dlt_X;
            pointWeldPara.Dlt_Y = dlg->pointWeldPara.Dlt_Y;
            pointWeldPara.Dlt_Z = dlg->pointWeldPara.Dlt_Z;
            pointWeldPara.Dlt_speed = dlg->pointWeldPara.Dlt_speed;
            pointWeldPara.FT_Pre_Length = dlg->pointWeldPara.FT_Pre_Length;
            pointWeldPara.FT_Pre_Speed = dlg->pointWeldPara.FT_Pre_Speed;
            pointWeldPara.FT_Pre_Modle = dlg->pointWeldPara.FT_Pre_Modle;
            pointWeldPara.Soldering_style = dlg->pointWeldPara.Soldering_style;
            pointWeldPara.ShakeSolder_Length = dlg->pointWeldPara.ShakeSolder_Length;
            pointWeldPara.ShakeSolder_speed = dlg->pointWeldPara.ShakeSolder_speed;
            pointWeldPara.S1_Length = dlg->pointWeldPara.S1_Length;
            pointWeldPara.S1_T_Length = dlg->pointWeldPara.S1_T_Length;
            pointWeldPara.S_speed = dlg->pointWeldPara.S_speed;
            pointWeldPara.S2_Length = dlg->pointWeldPara.S2_Length;
            pointWeldPara.S2_T_Length = dlg->pointWeldPara.S2_T_Length;
            pointWeldPara.Dlt_TX = dlg->pointWeldPara.Dlt_TX;
            pointWeldPara.Dlt_TY = dlg->pointWeldPara.Dlt_TY;
            pointWeldPara.Dlt_TZ = dlg->pointWeldPara.Dlt_TZ;
            pointWeldPara.T1_Delay_time = dlg->pointWeldPara.T1_Delay_time;
            pointWeldPara.T2_Delay_time = dlg->pointWeldPara.T2_Delay_time;
            pointWeldPara.T3_Delay_time = dlg->pointWeldPara.T3_Delay_time;
            pointWeldPara.v_Over_Time_count = dlg->pointWeldPara.v_Over_Time_count;

            SqlDB db;
            if(!db.openDB("robot.db"))
            {
                msgBox.setText(tr("打开数据库失败."));
                msgBox.exec();
                return;
            }
            QList<QListWidgetItem *> list=listWidget->findItems(dlg->weldTechnicsName,Qt::MatchFixedString);
            if(list.size()>0)
            {
                msgBox1.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
                msgBox1.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
                QFont ft;
                ft.setPointSize(MESSAGEBOX_FONT);
                ft.setItalic(false);
                msgBox1.setFont(ft);
                msgBox1.setIcon(QMessageBox::Question);
                msgBox1.setWindowTitle(tr("APPLICATION_NAME"));
                msgBox1.addButton(tr("是"), QMessageBox::YesRole);
                msgBox1.addButton(tr("否"), QMessageBox::NoRole);
                msgBox1.setText(tr("发现相同的工艺参数名称,您是否要覆盖已有的参数?"));
                if(msgBox1.exec()==QMessageBox::RejectRole)
                {
                    db.closeDB();
                    return;
                }
                else
                {
                    if(!db.SavePointWeldToDb(dlg->weldTechnicsName,pointWeldPara))
                    {
                        msgBox.setText(tr("保存新建点焊工艺参数失败."));
                        msgBox.exec();
                        db.closeDB();
                        return;
                    }
                }
            }
            else
            {
                if(!db.SavePointWeldToDb(dlg->weldTechnicsName,pointWeldPara))
                {
                    msgBox.setText(tr("保存新建点焊工艺参数失败."));
                    msgBox.exec();
                    db.closeDB();
                    return;
                }
                listWidget->addItem(dlg->weldTechnicsName);
            }
            db.closeDB();
        }
    }
    else
    {
        LineWeldDlg *dlg=new LineWeldDlg();
        dlg->type=2;    //新建点焊工艺参数
        dlg->lineWeldPara.atspeed=weldProject->currLineWeldPara.atspeed;
        dlg->lineWeldPara.Dlt_X=weldProject->currLineWeldPara.Dlt_X;
        dlg->lineWeldPara.Dlt_Y=weldProject->currLineWeldPara.Dlt_Y;
        dlg->lineWeldPara.Dlt_Z=weldProject->currLineWeldPara.Dlt_Z;
        dlg->lineWeldPara.Dlt_speed=weldProject->currLineWeldPara.Dlt_speed;
        dlg->lineWeldPara.FT_Pre_Length=weldProject->currLineWeldPara.FT_Pre_Length;
        dlg->lineWeldPara.FT_Pre_Speed=weldProject->currLineWeldPara.FT_Pre_Speed;
        dlg->lineWeldPara.TH_speed=weldProject->currLineWeldPara.TH_speed;
        dlg->lineWeldPara.S1_Length=weldProject->currLineWeldPara.S1_Length;
        dlg->lineWeldPara.S1_T_Length=weldProject->currLineWeldPara.S1_T_Length;
        dlg->lineWeldPara.S1_speed=weldProject->currLineWeldPara.S1_speed;
        dlg->lineWeldPara.S2_Length=weldProject->currLineWeldPara.S2_Length;
        dlg->lineWeldPara.S2_T_Length=weldProject->currLineWeldPara.S2_T_Length;
        dlg->lineWeldPara.S2_speed=weldProject->currLineWeldPara.S2_speed;
        dlg->lineWeldPara.Dlt_TX=weldProject->currLineWeldPara.Dlt_TX;
        dlg->lineWeldPara.Dlt_TY=weldProject->currLineWeldPara.Dlt_TY;
        dlg->lineWeldPara.Dlt_TZ=weldProject->currLineWeldPara.Dlt_TZ;
        dlg->lineWeldPara.T1_Delay_time=weldProject->currLineWeldPara.T1_Delay_time;
        dlg->lineWeldPara.T2_Delay_time=weldProject->currLineWeldPara.T2_Delay_time;
        dlg->lineWeldPara.T3_Delay_time=weldProject->currLineWeldPara.T3_Delay_time;
        dlg->lineWeldPara.v_Over_Time_count=weldProject->currLineWeldPara.v_Over_Time_count;
        dlg->initDialog();
        if(dlg->exec()==QDialog::Accepted)
        {
            L_P lineWeldPara;
            lineWeldPara.atspeed =dlg->lineWeldPara.atspeed;
            lineWeldPara.Dlt_X = dlg->lineWeldPara.Dlt_X;
            lineWeldPara.Dlt_Y = dlg->lineWeldPara.Dlt_Y;
            lineWeldPara.Dlt_Z = dlg->lineWeldPara.Dlt_Z;
            lineWeldPara.Dlt_speed = dlg->lineWeldPara.Dlt_speed;
            lineWeldPara.FT_Pre_Length = dlg->lineWeldPara.FT_Pre_Length;
            lineWeldPara.FT_Pre_Speed = dlg->lineWeldPara.FT_Pre_Speed;
            lineWeldPara.TH_speed = dlg->lineWeldPara.TH_speed;
            lineWeldPara.S1_Length = dlg->lineWeldPara.S1_Length;
            lineWeldPara.S1_T_Length = dlg->lineWeldPara.S1_T_Length;
            lineWeldPara.S1_speed = dlg->lineWeldPara.S1_speed;
            lineWeldPara.S2_Length = dlg->lineWeldPara.S2_Length;
            lineWeldPara.S2_T_Length = dlg->lineWeldPara.S2_T_Length;
            lineWeldPara.S2_speed = dlg->lineWeldPara.S2_speed;
            lineWeldPara.Dlt_TX = dlg->lineWeldPara.Dlt_TX;
            lineWeldPara.Dlt_TY = dlg->lineWeldPara.Dlt_TY;
            lineWeldPara.Dlt_TZ = dlg->lineWeldPara.Dlt_TZ;
            lineWeldPara.T1_Delay_time = dlg->lineWeldPara.T1_Delay_time;
            lineWeldPara.T2_Delay_time = dlg->lineWeldPara.T2_Delay_time;
            lineWeldPara.T3_Delay_time = dlg->lineWeldPara.T3_Delay_time;
            lineWeldPara.v_Over_Time_count = dlg->lineWeldPara.v_Over_Time_count;

            SqlDB db;
            if(!db.openDB("robot.db"))
            {
                msgBox.setText(tr("打开数据库失败."));
                msgBox.exec();
                return;
            }
            QList<QListWidgetItem *> list=listWidget->findItems(dlg->weldTechnicsName,Qt::MatchFixedString);
            if(list.size()>0)
            {
                msgBox1.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
                msgBox1.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
                QFont ft;
                ft.setPointSize(MESSAGEBOX_FONT);
                ft.setItalic(false);
                msgBox1.setFont(ft);
                msgBox1.setIcon(QMessageBox::Question);
                msgBox1.setWindowTitle(tr("APPLICATION_NAME"));
                msgBox1.addButton(tr("是"), QMessageBox::YesRole);
                msgBox1.addButton(tr("否"), QMessageBox::NoRole);
                msgBox1.setText(tr("发现相同的工艺参数名称,您是否要覆盖已有的参数?"));
                if(msgBox1.exec()==QMessageBox::RejectRole)
                {
                    db.closeDB();
                    return;
                }
                else
                {
                    if(!db.SaveLineWeldToDb(dlg->weldTechnicsName,lineWeldPara))
                    {
                        msgBox.setText(tr("保存新建拖焊工艺参数失败."));
                        msgBox.exec();
                        db.closeDB();
                        return;
                    }
                }
            }
            else
            {
                if(!db.SaveLineWeldToDb(dlg->weldTechnicsName,lineWeldPara))
                {
                    msgBox.setText(tr("保存新建拖焊工艺参数失败."));
                    msgBox.exec();
                    db.closeDB();
                    return;
                }
                listWidget->addItem(dlg->weldTechnicsName);
            }
            db.closeDB();
        }
    }
}

void TechnicsDlg::OnEditBtnClicked()
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

    if(listWidget->currentRow()==-1)
    {
        msgBox.setText(tr("请选择要编辑的工艺参数名称."));
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
    QString technicsName=listWidget->currentItem()->text();
    if(type==POINT_WELD_TECHNICS)
    {
        S_P pointWeldPara;
        if(!db.ReadPointWeldFromDb(technicsName,pointWeldPara))
        {
            msgBox.setText(tr("从数据库读取点焊工艺参数失败."));
            msgBox.exec();
            db.closeDB();
            return;
        }
        PointWeldDlg *dlg=new PointWeldDlg();
        dlg->type=3;    //编辑点焊工艺参数
        dlg->weldTechnicsName=listWidget->currentItem()->text();
        dlg->pointWeldPara.atspeed=pointWeldPara.atspeed;
        dlg->pointWeldPara.Dlt_X=pointWeldPara.Dlt_X;
        dlg->pointWeldPara.Dlt_Y=pointWeldPara.Dlt_Y;
        dlg->pointWeldPara.Dlt_Z=pointWeldPara.Dlt_Z;
        dlg->pointWeldPara.Dlt_speed=pointWeldPara.Dlt_speed;
        dlg->pointWeldPara.FT_Pre_Length=pointWeldPara.FT_Pre_Length;
        dlg->pointWeldPara.FT_Pre_Speed=pointWeldPara.FT_Pre_Speed;
        dlg->pointWeldPara.FT_Pre_Modle=pointWeldPara.FT_Pre_Modle;
        dlg->pointWeldPara.Soldering_style=pointWeldPara.Soldering_style;
        dlg->pointWeldPara.ShakeSolder_Length=pointWeldPara.ShakeSolder_Length;
        dlg->pointWeldPara.ShakeSolder_speed=pointWeldPara.ShakeSolder_speed;
        dlg->pointWeldPara.S1_Length=pointWeldPara.S1_Length;
        dlg->pointWeldPara.S1_T_Length=pointWeldPara.S1_T_Length;
        dlg->pointWeldPara.S_speed=pointWeldPara.S_speed;
        dlg->pointWeldPara.S2_Length=pointWeldPara.S2_Length;
        dlg->pointWeldPara.S2_T_Length=pointWeldPara.S2_T_Length;
        dlg->pointWeldPara.Dlt_TX=pointWeldPara.Dlt_TX;
        dlg->pointWeldPara.Dlt_TY=pointWeldPara.Dlt_TY;
        dlg->pointWeldPara.Dlt_TZ=pointWeldPara.Dlt_TZ;
        dlg->pointWeldPara.T1_Delay_time=pointWeldPara.T1_Delay_time;
        dlg->pointWeldPara.T2_Delay_time=pointWeldPara.T2_Delay_time;
        dlg->pointWeldPara.T3_Delay_time=pointWeldPara.T3_Delay_time;
        dlg->pointWeldPara.v_Over_Time_count=pointWeldPara.v_Over_Time_count;
        dlg->initDialog();
        if(dlg->exec()==QDialog::Accepted)
        {
            pointWeldPara.atspeed =dlg->pointWeldPara.atspeed;
            pointWeldPara.Dlt_X = dlg->pointWeldPara.Dlt_X;
            pointWeldPara.Dlt_Y = dlg->pointWeldPara.Dlt_Y;
            pointWeldPara.Dlt_Z = dlg->pointWeldPara.Dlt_Z;
            pointWeldPara.Dlt_speed = dlg->pointWeldPara.Dlt_speed;
            pointWeldPara.FT_Pre_Length = dlg->pointWeldPara.FT_Pre_Length;
            pointWeldPara.FT_Pre_Speed = dlg->pointWeldPara.FT_Pre_Speed;
            pointWeldPara.FT_Pre_Modle = dlg->pointWeldPara.FT_Pre_Modle;
            pointWeldPara.Soldering_style = dlg->pointWeldPara.Soldering_style;
            pointWeldPara.ShakeSolder_Length = dlg->pointWeldPara.ShakeSolder_Length;
            pointWeldPara.ShakeSolder_speed = dlg->pointWeldPara.ShakeSolder_speed;
            pointWeldPara.S1_Length = dlg->pointWeldPara.S1_Length;
            pointWeldPara.S1_T_Length = dlg->pointWeldPara.S1_T_Length;
            pointWeldPara.S_speed = dlg->pointWeldPara.S_speed;
            pointWeldPara.S2_Length = dlg->pointWeldPara.S2_Length;
            pointWeldPara.S2_T_Length = dlg->pointWeldPara.S2_T_Length;
            pointWeldPara.Dlt_TX = dlg->pointWeldPara.Dlt_TX;
            pointWeldPara.Dlt_TY = dlg->pointWeldPara.Dlt_TY;
            pointWeldPara.Dlt_TZ = dlg->pointWeldPara.Dlt_TZ;
            pointWeldPara.T1_Delay_time = dlg->pointWeldPara.T1_Delay_time;
            pointWeldPara.T2_Delay_time = dlg->pointWeldPara.T2_Delay_time;
            pointWeldPara.T3_Delay_time = dlg->pointWeldPara.T3_Delay_time;
            pointWeldPara.v_Over_Time_count = dlg->pointWeldPara.v_Over_Time_count;

            SqlDB db;
            if(!db.openDB("robot.db"))
            {
                msgBox.setText(tr("打开数据库失败."));
                msgBox.exec();
                return;
            }

            if(!db.SavePointWeldToDb(dlg->weldTechnicsName,pointWeldPara))
            {
                msgBox.setText(tr("保存点焊工艺参数失败."));
                msgBox.exec();
                db.closeDB();
                return;
            }
        }
    }
    else
    {
        L_P lineWeldPara;
        if(!db.ReadLineWeldFromDb(technicsName,lineWeldPara))
        {
            msgBox.setText(tr("从数据库读取拖焊工艺参数失败."));
            msgBox.exec();
            db.closeDB();
            return;
        }
        LineWeldDlg *dlg=new LineWeldDlg();
        dlg->type=3;    //编辑点焊工艺参数
        dlg->weldTechnicsName=listWidget->currentItem()->text();
        dlg->lineWeldPara.ZH=lineWeldPara.ZH;
        dlg->lineWeldPara.atspeed=lineWeldPara.atspeed;
        dlg->lineWeldPara.Dlt_X=lineWeldPara.Dlt_X;
        dlg->lineWeldPara.Dlt_Y=lineWeldPara.Dlt_Y;
        dlg->lineWeldPara.Dlt_Z=lineWeldPara.Dlt_Z;
        dlg->lineWeldPara.Dlt_speed=lineWeldPara.Dlt_speed;
        dlg->lineWeldPara.FT_Pre_Length=lineWeldPara.FT_Pre_Length;
        dlg->lineWeldPara.FT_Pre_Speed=lineWeldPara.FT_Pre_Speed;
        dlg->lineWeldPara.TH_speed=lineWeldPara.TH_speed;
        dlg->lineWeldPara.S1_Length=lineWeldPara.S1_Length;
        dlg->lineWeldPara.S1_T_Length=lineWeldPara.S1_T_Length;
        dlg->lineWeldPara.S1_speed=lineWeldPara.S1_speed;
        dlg->lineWeldPara.S2_Length=lineWeldPara.S2_Length;
        dlg->lineWeldPara.S2_T_Length=lineWeldPara.S2_T_Length;
        dlg->lineWeldPara.S2_speed=lineWeldPara.S2_speed;
        dlg->lineWeldPara.Dlt_TX=lineWeldPara.Dlt_TX;
        dlg->lineWeldPara.Dlt_TY=lineWeldPara.Dlt_TY;
        dlg->lineWeldPara.Dlt_TZ=lineWeldPara.Dlt_TZ;
        dlg->lineWeldPara.T1_Delay_time=lineWeldPara.T1_Delay_time;
        dlg->lineWeldPara.T2_Delay_time=lineWeldPara.T2_Delay_time;
        dlg->lineWeldPara.T3_Delay_time=lineWeldPara.T3_Delay_time;
        dlg->lineWeldPara.v_Over_Time_count=lineWeldPara.v_Over_Time_count;
        dlg->initDialog();
        if(dlg->exec()==QDialog::Accepted)
        {
            lineWeldPara.atspeed =dlg->lineWeldPara.atspeed;
            lineWeldPara.Dlt_X = dlg->lineWeldPara.Dlt_X;
            lineWeldPara.Dlt_Y = dlg->lineWeldPara.Dlt_Y;
            lineWeldPara.Dlt_Z = dlg->lineWeldPara.Dlt_Z;
            lineWeldPara.Dlt_speed = dlg->lineWeldPara.Dlt_speed;
            lineWeldPara.FT_Pre_Length = dlg->lineWeldPara.FT_Pre_Length;
            lineWeldPara.FT_Pre_Speed = dlg->lineWeldPara.FT_Pre_Speed;
            lineWeldPara.TH_speed = dlg->lineWeldPara.TH_speed;
            lineWeldPara.S1_Length = dlg->lineWeldPara.S1_Length;
            lineWeldPara.S1_T_Length = dlg->lineWeldPara.S1_T_Length;
            lineWeldPara.S1_speed = dlg->lineWeldPara.S1_speed;
            lineWeldPara.S2_Length = dlg->lineWeldPara.S2_Length;
            lineWeldPara.S2_T_Length = dlg->lineWeldPara.S2_T_Length;
            lineWeldPara.S2_speed = dlg->lineWeldPara.S2_speed;
            lineWeldPara.Dlt_TX = dlg->lineWeldPara.Dlt_TX;
            lineWeldPara.Dlt_TY = dlg->lineWeldPara.Dlt_TY;
            lineWeldPara.Dlt_TZ = dlg->lineWeldPara.Dlt_TZ;
            lineWeldPara.T1_Delay_time = dlg->lineWeldPara.T1_Delay_time;
            lineWeldPara.T2_Delay_time = dlg->lineWeldPara.T2_Delay_time;
            lineWeldPara.T3_Delay_time = dlg->lineWeldPara.T3_Delay_time;
            lineWeldPara.v_Over_Time_count = dlg->lineWeldPara.v_Over_Time_count;

            SqlDB db;
            if(!db.openDB("robot.db"))
            {
                msgBox.setText(tr("打开数据库失败."));
                msgBox.exec();
                return;
            }
            if(!db.SaveLineWeldToDb(dlg->weldTechnicsName,lineWeldPara))
            {
                msgBox.setText(tr("保存拖焊工艺参数失败."));
                msgBox.exec();
                db.closeDB();
                return;
            }
        }
    }
    db.closeDB();
}

void TechnicsDlg::OnDeleteBtnClicked()
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
    if(listWidget->currentItem()->text()==QString("default"))
    {
        msgBox.setText(tr("这是系统默认工艺参数,不能删除."));
        msgBox.exec();
        return;
    }

    msgBox1.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox1.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox1.setFont(ft);
    msgBox1.setIcon(QMessageBox::Question);
    msgBox1.setWindowTitle(tr(APPLICATION_NAME));
    msgBox1.addButton(tr("是"), QMessageBox::YesRole);
    msgBox1.addButton(tr("否"), QMessageBox::NoRole);
    msgBox1.setText(tr("您是否真的要删除所选择的工艺参数%1?").arg(listWidget->currentItem()->text()));
    if(msgBox1.exec()==QMessageBox::RejectRole)
    {
        return;
    }

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }
    if(type==POINT_WELD_TECHNICS)
    {
        if(!db.DeletePointTechFromDb(listWidget->currentItem()->text()))
        {
            msgBox.setText(tr("删除点焊工艺参数失败."));
            msgBox.exec();
            db.closeDB();
            return;
        }
    }
    else
    {
        if(!db.DeleteLineTechFromDb(listWidget->currentItem()->text()))
        {
            msgBox.setText(tr("删除拖焊工艺参数失败."));
            msgBox.exec();
            db.closeDB();
            return;
        }
    }
    if(!db.DeleteEnvParaFromDb(listWidget->currentItem()->text()))
    {
        msgBox.setText(tr("删除点焊工艺参数失败."));
        msgBox.exec();
        db.closeDB();
        return;
    }
    else
    {
        listWidget->takeItem(listWidget->currentRow());
    }
    db.closeDB();
}

void TechnicsDlg::OnSaveAsBtnClicked()
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

    if(saveFlag)
    {
        QString newName=saveAsEdit->text().trimmed();
        QString oldName=listWidget->currentItem()->text();
        if(newName.isEmpty()) return;
        int rtn=saveNewTechnicPara(oldName,newName);
        switch(rtn)
        {
        case 0:
            msgBox.setText(tr("保存完毕."));
            msgBox.exec();
            listWidget->addItem(newName);
            break;
        case -1:
            msgBox.setText(tr("打开数据库失败."));
            msgBox.exec();
            break;
        case -2:
            msgBox.setText(tr("在数据库中写入新点焊工艺参数失败."));
            msgBox.exec();
            break;
        case -3:
            msgBox.setText(tr("在数据库中读取旧点焊工艺参数失败."));
            msgBox.exec();
            break;
        case -4:
            msgBox.setText(tr("在数据库中写入新拖焊工艺数据失败."));
            msgBox.exec();
            break;
        case -5:
            msgBox.setText(tr("在数据库中读取旧拖焊工艺参数失败."));
            msgBox.exec();
            break;
        case -6:
            msgBox.setText(tr("在数据库中写入新工艺环境数据失败."));
            msgBox.exec();
            break;
        case -7:
            msgBox.setText(tr("在数据库中读取旧工艺环境数据失败."));
            msgBox.exec();
            break;
        }

        saveFlag=0;
        saveAsEdit->hide();
        saveAsButton->setText(tr("另存为..."));
    }
    else
    {
        saveFlag=1;
        saveAsEdit->setText(listWidget->currentItem()->text());
        saveAsEdit->show();
        saveAsButton->setText(tr("保存"));
    }
}

int TechnicsDlg::saveNewTechnicPara(QString oldName,QString newName)
{
    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        return -1;
    }
    if(type==1) //点焊
    {
        S_P pointWeldPara;
        if(db.ReadPointWeldFromDb(oldName,pointWeldPara))
        {
            if(!db.SavePointWeldToDb(newName,pointWeldPara))
            {
                db.closeDB();
                return -2;
            }
        }
        else
        {
            db.closeDB();
            return -3;
        }
    }
    else
    {       //拖焊
        L_P lineWeldPara;
        if(db.ReadLineWeldFromDb(oldName,lineWeldPara))
        {
            if(!db.SaveLineWeldToDb(newName,lineWeldPara))
            {
                return -4;
            }
        }
        else
        {
            db.closeDB();
            return -5;
        }
    }
    ENV_PARA envPara;
    if(db.ReadEnvParaFromDb(oldName,envPara))
    {
        if(db.SaveEnvParaToDb(newName,envPara))
        {
            db.closeDB();
            return 0;       //正确结果
        }
        else
        {
            return -6;
        }
    }
    else
    {
        db.closeDB();
        return -7;
    }
    db.closeDB();
    return 0;
}

void TechnicsDlg::OnRenameBtnClicked()
{
}

void TechnicsDlg::OnImportBtnClicked()    //导入工艺
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

    QString importFileName=QFileDialog::getOpenFileName(this,tr("导入工艺"),"",tr("Json files(*.json)"));

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
                        if(keysList[k]=="envParaItem")  //焊接环境参数
                        {
                            if(obj.value("envParaItem").toObject().isEmpty())
                                continue;
                            ENV_PARA envParaItem;
                            QString technicsName=obj.value("envParaItem").toObject().value("technicsName").toString();
                            envParaItem.HanPan_HD=obj.value("envParaItem").toObject().value("HanPan_HD").toDouble();
                            envParaItem.HanPan_BMCL=obj.value("envParaItem").toObject().value("HanPan_BMCL").toString();
                            envParaItem.HanPan_FTXS=obj.value("envParaItem").toObject().value("HanPan_FTXS").toString();
                            envParaItem.HanPan_ZJ=obj.value("envParaItem").toObject().value("HanPan_ZJ").toDouble();
                            envParaItem.HanPan_KJ=obj.value("envParaItem").toObject().value("HanPan_KJ").toDouble();
                            envParaItem.HanXiSi_ZJ=obj.value("envParaItem").toObject().value("HanXiSi_ZJ").toDouble();
                            envParaItem.HanXiSi_ZL=obj.value("envParaItem").toObject().value("HanXiSi_ZL").toString();
                            envParaItem.HanXiSi_ZHJ=obj.value("envParaItem").toObject().value("HanXiSi_ZHJ").toDouble();
                            envParaItem.HanXiSi_FW=obj.value("envParaItem").toObject().value("HanXiSi_FW").toString();
                            envParaItem.LaoTie_WD=obj.value("envParaItem").toObject().value("LaoTie_WD").toDouble();
                            envParaItem.LaoTie_ZL=obj.value("envParaItem").toObject().value("LaoTie_ZL").toString();
                            if(!db.SaveEnvParaToDb(technicsName,envParaItem))
                            {
                                msgBox.setText(tr("%1焊接环境参数写入数据库失败.").arg(technicsName));
                                msgBox.exec();
                                return;
                            }
                        }
                        else    //焊接工艺
                        {
                            QString technicsName=keysList[k];
                            QJsonObject technicsJosn=obj.value(technicsName).toObject();
                            if(type==POINT_WELD_TECHNICS)
                            {
                                S_P pointTechnicsItem;
                                pointTechnicsItem.ZH=technicsJosn.value("ZH").toDouble();
                                pointTechnicsItem.atspeed=technicsJosn.value("atspeed").toDouble();
                                pointTechnicsItem.Dlt_X=technicsJosn.value("Dlt_X").toDouble();
                                pointTechnicsItem.Dlt_Y=technicsJosn.value("Dlt_Y").toDouble();
                                pointTechnicsItem.Dlt_Z=technicsJosn.value("Dlt_Z").toDouble();
                                pointTechnicsItem.Dlt_speed=technicsJosn.value("Dlt_speed").toDouble();
                                pointTechnicsItem.FT_Pre_Length=technicsJosn.value("FT_Pre_Length").toDouble();
                                pointTechnicsItem.FT_Pre_Speed=technicsJosn.value("FT_Pre_Speed").toDouble();
                                pointTechnicsItem.FT_Pre_Modle=(unsigned char)technicsJosn.value("FT_Pre_Modle").toInt();
                                pointTechnicsItem.Soldering_style=(unsigned char)technicsJosn.value("Soldering_style").toInt();
                                pointTechnicsItem.ShakeSolder_Length=technicsJosn.value("ShakeSolder_Length").toDouble();
                                pointTechnicsItem.ShakeSolder_speed=technicsJosn.value("ShakeSolder_speed").toDouble();
                                pointTechnicsItem.S1_Length=technicsJosn.value("S1_Length").toDouble();
                                pointTechnicsItem.S1_T_Length=technicsJosn.value("S1_T_Length").toDouble();
                                pointTechnicsItem.S_speed=technicsJosn.value("S_speed").toDouble();
                                pointTechnicsItem.S2_Length=technicsJosn.value("S2_Length").toDouble();
                                pointTechnicsItem.S2_T_Length=technicsJosn.value("S2_T_Length").toDouble();
                                pointTechnicsItem.Dlt_TX=technicsJosn.value("Dlt_TX").toDouble();
                                pointTechnicsItem.Dlt_TY=technicsJosn.value("Dlt_TY").toDouble();
                                pointTechnicsItem.Dlt_TZ=technicsJosn.value("Dlt_TZ").toDouble();
                                pointTechnicsItem.T1_Delay_time=technicsJosn.value("T1_Delay_time").toInt();
                                pointTechnicsItem.T2_Delay_time=technicsJosn.value("T2_Delay_time").toInt();
                                pointTechnicsItem.T3_Delay_time=technicsJosn.value("T3_Delay_time").toInt();
                                pointTechnicsItem.v_Over_Time_count=technicsJosn.value("v_Over_Time_count").toInt();
                                if(!db.SavePointWeldToDb(technicsName,pointTechnicsItem))
                                {
                                    msgBox.setText(tr("%1工艺数据写入数据库失败.").arg(technicsName));
                                    msgBox.exec();
                                    return;
                                }
                            }
                            else
                            {
                                L_P lineTechnicsItem;
                                lineTechnicsItem.ZH=technicsJosn.value("ZH").toDouble();
                                lineTechnicsItem.atspeed=technicsJosn.value("atspeed").toDouble();
                                lineTechnicsItem.Dlt_X=technicsJosn.value("Dlt_X").toDouble();
                                lineTechnicsItem.Dlt_Y=technicsJosn.value("Dlt_Y").toDouble();
                                lineTechnicsItem.Dlt_Z=technicsJosn.value("Dlt_Z").toDouble();
                                lineTechnicsItem.Dlt_speed=technicsJosn.value("Dlt_speed").toDouble();
                                lineTechnicsItem.FT_Pre_Length=technicsJosn.value("FT_Pre_Length").toDouble();
                                lineTechnicsItem.FT_Pre_Speed=technicsJosn.value("FT_Pre_Speed").toDouble();
                                lineTechnicsItem.TH_speed=technicsJosn.value("TH_speed").toDouble();
                                lineTechnicsItem.S1_Length=technicsJosn.value("S1_Length").toDouble();
                                lineTechnicsItem.S1_T_Length=technicsJosn.value("S1_T_Length").toDouble();
                                lineTechnicsItem.S1_speed=technicsJosn.value("S1_speed").toDouble();
                                lineTechnicsItem.S2_Length=technicsJosn.value("S2_Length").toDouble();
                                lineTechnicsItem.S2_T_Length=technicsJosn.value("S2_T_Length").toDouble();
                                lineTechnicsItem.S2_speed=technicsJosn.value("S2_speed").toDouble();
                                lineTechnicsItem.Dlt_TX=technicsJosn.value("Dlt_TX").toDouble();
                                lineTechnicsItem.Dlt_TY=technicsJosn.value("Dlt_TY").toDouble();
                                lineTechnicsItem.Dlt_TZ=technicsJosn.value("Dlt_TZ").toDouble();
                                lineTechnicsItem.T1_Delay_time=technicsJosn.value("T1_Delay_time").toInt();
                                lineTechnicsItem.T2_Delay_time=technicsJosn.value("T2_Delay_time").toInt();
                                lineTechnicsItem.T3_Delay_time=technicsJosn.value("T3_Delay_time").toInt();
                                lineTechnicsItem.v_Over_Time_count=technicsJosn.value("v_Over_Time_count").toInt();
                                if(!db.SaveLineWeldToDb(technicsName,lineTechnicsItem))
                                {
                                    msgBox.setText(tr("%1工艺数据写入数据库失败.").arg(technicsName));
                                    msgBox.exec();
                                    return;
                                }
                            }
                            QList<QListWidgetItem *> itemList=listWidget->findItems(technicsName,Qt::MatchFixedString);
                            if(itemList.size()==0)
                                listWidget->addItem(technicsName);
                        }
                    }
                }
            }
        }
        msgBox.setText(tr("导入工艺完毕."));
        msgBox.exec();
    }
}

void TechnicsDlg::OnExportChkClicked()    //导出检查框
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
        loadButton->setEnabled(true);
        newTechnicsButton->setEnabled(true);
        editButton->setEnabled(true);
        deleteButton->setEnabled(true);

        importButton->setEnabled(true);
        exportButton->setEnabled(false);
        QStringList projNameList;
        if(type==POINT_WELD_TECHNICS)
        {
            if(db.ReadPointTechNameFromDb(projNameList))
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
            if(db.ReadLineTechNameFromDb(projNameList))
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
    }
    else
    {
        loadButton->setEnabled(false);
        newTechnicsButton->setEnabled(false);
        editButton->setEnabled(false);
        deleteButton->setEnabled(false);

        importButton->setEnabled(false);
        exportButton->setEnabled(true);
        QStringList projNameList;
        if(type==POINT_WELD_TECHNICS)
        {
            if(db.ReadPointTechNameFromDb(projNameList))
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
        else
        {
            if(db.ReadLineTechNameFromDb(projNameList))
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
    }
    db.closeDB();
}

void TechnicsDlg::OnExportBtnClicked()    //导出工艺
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

    QStringList selectedTechnicsNameList;
    for(int i=0;i<listWidget->count();i++)
    {
        if(listCheckBox[i]->checkState()==Qt::Checked)
        {
            selectedTechnicsNameList.append(listWidget->item(i)->text().right(listWidget->item(i)->text().length()-3));
        }
    }
    if(selectedTechnicsNameList.size()==0)
    {
        msgBox.setText(tr("请选择要导出的工艺."));
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
    QString exportFileName1;
    QDateTime datetime=QDateTime::currentDateTime();
    QString strDateTime=datetime.toString("yyyyMMddhhmmss");

    for(int i=0;i<selectedTechnicsNameList.size();i++)
    {
        QJsonObject technicsJsonObt;
        ENV_PARA envParaItem;
        db.ReadEnvParaFromDb(selectedTechnicsNameList[i],envParaItem);
        if(type==POINT_WELD_TECHNICS)
        {
            S_P pointTechnicsItem;
            if(!db.ReadPointWeldFromDb(selectedTechnicsNameList[i],pointTechnicsItem))
            {
                msgBox.setText(tr("从数据库中读取点焊工艺%1的数据失败,停止导出").arg(selectedTechnicsNameList[i]));
                msgBox.exec();
                return;
            }
            exportFileName1=tr("导出点焊工艺%1.json").arg(strDateTime);
            technicsJsonObt=createPointTechnicsJson(selectedTechnicsNameList[i],pointTechnicsItem,envParaItem);
        }
        else
        {
            L_P lineTechnicsItem;
            if(!db.ReadLineWeldFromDb(selectedTechnicsNameList[i],lineTechnicsItem))
            {
                msgBox.setText(tr("从数据库中读取拖焊工艺%1的数据失败,停止导出").arg(selectedTechnicsNameList[i]));
                msgBox.exec();
                return;
            }
            exportFileName1=tr("导出拖焊工艺%1.json").arg(strDateTime);
            technicsJsonObt=createLineTechnicsJson(selectedTechnicsNameList[i],lineTechnicsItem,envParaItem);
        }
        projArray.append(QJsonValue(technicsJsonObt));
    }

    db.closeDB();

    QString exportFileName=QFileDialog::getSaveFileName(this,tr("导出工艺"),exportFileName1,tr("Json files(*.json)"));

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
    msgBox.setText(tr("导出成功，已导出的工程工艺文件名为\n“%1”").arg(exportFileName));
    msgBox.exec();
}

QJsonObject TechnicsDlg::createPointTechnicsJson(S_P item)
{
    QJsonObject technicsJson;

    technicsJson.insert("ZH",item.ZH);
    technicsJson.insert("atspeed",item.atspeed);
    technicsJson.insert("Dlt_X",item.Dlt_X);
    technicsJson.insert("Dlt_Y",item.Dlt_Y);
    technicsJson.insert("Dlt_Z",item.Dlt_Z);
    technicsJson.insert("Dlt_speed",item.Dlt_speed);
    technicsJson.insert("FT_Pre_Length",item.FT_Pre_Length);
    technicsJson.insert("FT_Pre_Speed",item.FT_Pre_Speed);
    technicsJson.insert("FT_Pre_Modle",item.FT_Pre_Modle);
    technicsJson.insert("Soldering_style",item.Soldering_style);
    technicsJson.insert("ShakeSolder_Length",item.ShakeSolder_Length);
    technicsJson.insert("ShakeSolder_speed",item.ShakeSolder_speed);
    technicsJson.insert("S1_Length",item.S1_Length);
    technicsJson.insert("S1_T_Length",item.S1_T_Length);
    technicsJson.insert("S_speed",item.S_speed);
    technicsJson.insert("S2_Length",item.S2_Length);
    technicsJson.insert("S2_T_Length",item.S2_T_Length);
    technicsJson.insert("Dlt_TX",item.Dlt_TX);
    technicsJson.insert("Dlt_TY",item.Dlt_TY);
    technicsJson.insert("Dlt_TZ",item.Dlt_TZ);
    technicsJson.insert("T1_Delay_time",qint64(item.T1_Delay_time));
    technicsJson.insert("T2_Delay_time",qint64(item.T2_Delay_time));
    technicsJson.insert("T3_Delay_time",qint64(item.T3_Delay_time));
    technicsJson.insert("v_Over_Time_count",qint64(item.v_Over_Time_count));

    return technicsJson;
}

QJsonObject TechnicsDlg::createLineTechnicsJson(L_P item)
{
    QJsonObject technicsJson;

    technicsJson.insert("ZH",item.ZH);
    technicsJson.insert("atspeed",item.atspeed);
    technicsJson.insert("Dlt_X",item.Dlt_X);
    technicsJson.insert("Dlt_Y",item.Dlt_Y);
    technicsJson.insert("Dlt_Z",item.Dlt_Z);
    technicsJson.insert("Dlt_speed",item.Dlt_speed);
    technicsJson.insert("FT_Pre_Length",item.FT_Pre_Length);
    technicsJson.insert("FT_Pre_Speed",item.FT_Pre_Speed);
    technicsJson.insert("S_speed",item.TH_speed);
    technicsJson.insert("S1_Length",item.S1_Length);
    technicsJson.insert("S1_T_Length",item.S1_T_Length);
    technicsJson.insert("S_speed",item.S1_speed);
    technicsJson.insert("S2_Length",item.S2_Length);
    technicsJson.insert("S2_T_Length",item.S2_T_Length);
    technicsJson.insert("S_speed",item.S2_speed);
    technicsJson.insert("Dlt_TX",item.Dlt_TX);
    technicsJson.insert("Dlt_TY",item.Dlt_TY);
    technicsJson.insert("Dlt_TZ",item.Dlt_TZ);
    technicsJson.insert("T1_Delay_time",qint64(item.T1_Delay_time));
    technicsJson.insert("T2_Delay_time",qint64(item.T2_Delay_time));
    technicsJson.insert("T3_Delay_time",qint64(item.T3_Delay_time));
    technicsJson.insert("v_Over_Time_count",qint64(item.v_Over_Time_count));

    return technicsJson;
}

QJsonObject TechnicsDlg::createEnvParaJson(QString technicsName,ENV_PARA envParaItem)
{
    QJsonObject envParaJson;
    if(technicsName.isEmpty())
        return envParaJson;

    envParaJson.insert("TechnicsName",technicsName);
    envParaJson.insert("HanPan_HD",envParaItem.HanPan_HD);
    envParaJson.insert("HanPan_BMCL",envParaItem.HanPan_BMCL);
    envParaJson.insert("HanPan_FTXS",envParaItem.HanPan_FTXS);
    envParaJson.insert("HanPan_ZJ",envParaItem.HanPan_ZJ);
    envParaJson.insert("HanPan_KJ",envParaItem.HanPan_KJ);
    envParaJson.insert("HanXiSi_ZJ",envParaItem.HanXiSi_ZJ);
    envParaJson.insert("HanXiSi_ZL",envParaItem.HanXiSi_ZL);
    envParaJson.insert("HanXiSi_ZHJ",envParaItem.HanXiSi_ZHJ);
    envParaJson.insert("HanXiSi_FW",envParaItem.HanXiSi_FW);
    envParaJson.insert("LaoTie_WD",envParaItem.LaoTie_WD);
    envParaJson.insert("LaoTie_ZL",envParaItem.LaoTie_ZL);

    return envParaJson;
}

QJsonObject TechnicsDlg::createPointTechnicsJson(QString technicsName,S_P pointTechnicsItem,ENV_PARA envParaItem)
{
    QJsonObject technicsJson;
    QJsonObject pointTechnicsJson=createPointTechnicsJson(pointTechnicsItem);
    technicsJson.insert(technicsName,QJsonValue(pointTechnicsJson));
    technicsJson.insert("envParaItem",QJsonValue(createEnvParaJson(technicsName,envParaItem)));

    return technicsJson;
}

QJsonObject TechnicsDlg::createLineTechnicsJson(QString technicsName,L_P lineTechnicsItem,ENV_PARA envParaItem)
{
    QJsonObject technicsJson;
    QJsonObject lineTechnicsJson=createLineTechnicsJson(lineTechnicsItem);
    technicsJson.insert(technicsName,QJsonValue(lineTechnicsJson));
    technicsJson.insert("envParaItem",QJsonValue(createEnvParaJson(technicsName,envParaItem)));

    return technicsJson;
}
