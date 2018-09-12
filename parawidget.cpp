#include "parawidget.h"
#include "msqldb.h"

ParaWidget::ParaWidget(QWidget *parent) :
    QDialog(parent)
{
}

void ParaWidget::initDialog()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT);
    ft.setItalic(false);

    QPushButton *delBtn=new QPushButton(tr("删除所选命令"));
    delBtn->setFont(ft);
    connect(delBtn, SIGNAL(clicked()), this, SLOT(OnDelBtnClicked()));
    QPushButton *cancelDelBtn=new QPushButton(tr("撤销"));
    cancelDelBtn->setFont(ft);
    connect(cancelDelBtn, SIGNAL(clicked()), this, SLOT(OnCancelDelBtnClicked()));
    selectAllCheckbox=new QCheckBox(tr("全选"));
    selectAllCheckbox->setFont(ft);
    connect(selectAllCheckbox, SIGNAL(clicked()), this, SLOT(OnSelectAllClicked()));
    saveBtn=new QPushButton(tr("保存"));
    saveBtn->setFont(ft);
    connect(saveBtn, SIGNAL(clicked()), this, SLOT(OnSaveBtnClicked()));
    saveBtn->setEnabled(false);
    QPushButton *closeBtn=new QPushButton(tr("关闭"));
    closeBtn->setFont(ft);
    connect(closeBtn, SIGNAL(clicked()), this, SLOT(OnCloseBtnClicked()));

    createDataWidget();
    createTechnicsBox();
    createCoordinateBox();

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(tableView);
    leftLayout->addWidget(selectAllCheckbox);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(delBtn);
    rightLayout->addSpacing(50);
    rightLayout->addWidget(cancelDelBtn);
    rightLayout->addSpacing(50);
    rightLayout->addWidget(chgCoordinateGroup);
    rightLayout->addWidget(chgTechnicsGroup);

    QHBoxLayout *scLayout = new QHBoxLayout;
    scLayout->addSpacing(100);
    scLayout->addWidget(saveBtn);    //保存
    scLayout->addSpacing(20);
    scLayout->addWidget(closeBtn);    //关闭

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(leftLayout);
    layout->addLayout(rightLayout);

    QVBoxLayout *tLayout = new QVBoxLayout;
    tLayout->addLayout(layout);
    scLayout->addSpacing(100);
    tLayout->addLayout(scLayout);
    setLayout(tLayout);
    setWindowTitle(tr("多选调整项目"));

    //this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    this->move(screenRect.width()/2,screenRect.height()/20);
    resize(screenRect.width()/4,screenRect.height()*3/4);

    QMessageBox msgBox;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    //QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("无法打开数据库，请检查。"));
        msgBox.exec();
        return;
    }

    QString projName=weldProject->getProjName();
    if(!db.ReadProjAllItemFromDb(projName,AllItem))
    {
        msgBox.setText(tr("无法读取项目命令记录，请检查。"));
        msgBox.exec();
        return;
    }
    lastAllItem=AllItem;

    //刷新数据
    refreshTableview();

    program=new QProcess(this);
    program->start("C:/Progra~1/Common~1/Micros~1/ink/TabTip.exe");
    program->waitForStarted();
    program->waitForFinished();
}
void ParaWidget::createDataWidget()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(20,120,120));
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT);
    ft.setItalic(false);

    dataModel = new QStandardItemModel(0,3);
    dataModel->setHeaderData(0, Qt::Horizontal, QObject::tr("  序号  "));
    dataModel->setHeaderData(1, Qt::Horizontal, QObject::tr("  指令  "));
    dataModel->setHeaderData(2, Qt::Horizontal, QObject::tr("  工艺参数  "));
    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////
    tableView=new QTableView(this);
    tableView->setSortingEnabled(false);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->verticalHeader()->hide();
    tableView->horizontalHeader()->setFont(ft);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);//需要时加滚动条ScrollBarAsNeeded
    //palette.setColor(QPalette::Background, QColor(0,0,0));
    tableView->setPalette(palette);
    tableView->setFont(ft);
    tableView->setModel(dataModel);
    tableView->resizeColumnsToContents();
    tableView->setFrameShape(QFrame::Panel);
    tableView->setShowGrid(true);
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->setAlternatingRowColors(true);
    tableView->setStyleSheet("QTableView{background-color:rgb(200,200,200) ; alternate-background-color: lightYellow;} \
                             QTableView::item:!selected{background: lightYellow;} \
                             QTableView::item:alternate:!selected{ background: lightBlue; }");
    connect(tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(OnModelIndexClicked(QModelIndex)));
}

void ParaWidget::createTechnicsBox()
{
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT);
    ft.setItalic(false);
    chgTechnicsGroup=new QGroupBox(tr("改变工艺"));
    pointTechnicsComb=new QComboBox;
    pointTechnicsComb->addItem(tr("请选择拖焊工艺"));
    pointTechnicsComb->setCurrentIndex(0);
    pointTechnicsComb->setFont(ft);
    lineTechnicsComb=new QComboBox;
    lineTechnicsComb->addItem(tr("请选择拖焊工艺"));
    lineTechnicsComb->setCurrentIndex(0);
    lineTechnicsComb->setFont(ft);
    QPushButton *updateBtn=new QPushButton(tr("更新工艺"));
    updateBtn->setFont(ft);
    connect(updateBtn, SIGNAL(clicked()), this, SLOT(OnUpdateTichnicsBtnClicked()));

    QHBoxLayout *pointLayout = new QHBoxLayout;
    pointLayout->addWidget(new QLabel(tr("点焊工艺")));
    pointLayout->addWidget(pointTechnicsComb);
    QHBoxLayout *lineLayout = new QHBoxLayout;
    lineLayout->addWidget(new QLabel(tr("拖焊工艺")));
    lineLayout->addWidget(lineTechnicsComb);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addSpacing(20);
    layout->addLayout(pointLayout);
    layout->addLayout(lineLayout);
    layout->addSpacing(50);
    layout->addWidget(updateBtn);
    layout->addSpacing(20);
    chgTechnicsGroup->setLayout(layout);


    QMessageBox msgBox;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    //QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("无法打开数据库。"));
        msgBox.exec();
        return;
    }
    QStringList pointWeldName,lineWeldName;
    if(!db.ReadPointTechNameFromDb(pointWeldName))
    {
        msgBox.setText(tr("无法读取点焊工艺名称。"));
        msgBox.exec();
    }
    else
    {
        for(int i=0;i<pointWeldName.size();i++)
        {
            pointTechnicsComb->addItem(pointWeldName[i]);
        }
    }
    if(!db.ReadLineTechNameFromDb(lineWeldName))
    {
        msgBox.setText(tr("无法读取拖焊工艺名称。"));
        msgBox.exec();
    }
    else
    {
        for(int i=0;i<lineWeldName.size();i++)
        {
            lineTechnicsComb->addItem(lineWeldName[i]);
        }
    }
}

void ParaWidget::createCoordinateBox()
{
    chgCoordinateGroup=new QGroupBox(tr("坐标偏移"));
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT);
    ft.setItalic(false);
    QLabel *posLabel[6]={new QLabel(tr("X轴:")),new QLabel(tr("Y轴:")),new QLabel(tr("Z轴:")),\
                        new QLabel(tr("R轴:")),new QLabel(tr("R1轴:")),new QLabel(tr("R2轴:"))};
    for(int i=0;i<6;i++)
        posLabel[i]->setFont(ft);
    QDoubleValidator *validator=new QDoubleValidator(-1000,1000,3);
    for(int i=0;i<6;i++)
    {
        coordinateEdit[i]=new QLineEdit();
        coordinateEdit[i]->setFont(ft);
        coordinateEdit[i]->setValidator(validator);
    }
    QPushButton *updateBtn=new QPushButton(tr("更新坐标"));
    updateBtn->setFont(ft);
    connect(updateBtn, SIGNAL(clicked()), this, SLOT(OnUpdateCoordinateBtnClicked()));

    QGridLayout *posLayout = new QGridLayout;
    posLayout->addWidget(posLabel[0], 0, 0);
    posLayout->addWidget(coordinateEdit[0], 0, 1);
    posLayout->addWidget(posLabel[1], 1, 0);
    posLayout->addWidget(coordinateEdit[1], 1, 1);
    posLayout->addWidget(posLabel[2], 2, 0);
    posLayout->addWidget(coordinateEdit[2], 2, 1);
    posLayout->addWidget(posLabel[3], 3, 0);
    posLayout->addWidget(coordinateEdit[3], 3, 1);
    posLayout->addWidget(posLabel[4], 4, 0);
    posLayout->addWidget(coordinateEdit[4], 4, 1);
    posLayout->addWidget(posLabel[5], 5, 0);
    posLayout->addWidget(coordinateEdit[5], 5, 1);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addSpacing(20);
    layout->addLayout(posLayout);
    layout->addSpacing(50);
    layout->addWidget(updateBtn);
    layout->addSpacing(20);
    chgCoordinateGroup->setLayout(layout);
}

void ParaWidget::OnModelIndexClicked(const QModelIndex &index)
{
    int row=index.row();

    QStandardItem *item=dataModel->item(row,0);
    if(item->checkState()!=Qt::Unchecked)
        item->setCheckState(Qt::Unchecked);
    else
        item->setCheckState(Qt::Checked);
}

void ParaWidget::OnSelectAllClicked()
{
    int cnt=weldProject->listCommand.size();
    for(int i=0;i<cnt;i++)
    {
        QStandardItem *item=dataModel->item(i,0);
        if(selectAllCheckbox->isChecked())
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
    }
}

void ParaWidget::OnDelBtnClicked()
{
    if(dataModel->rowCount()==0)
        return;

    QMessageBox msgBox1;
    msgBox1.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox1.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox1.setFont(ft);
    msgBox1.setIcon(QMessageBox::Question);
    msgBox1.setWindowTitle(tr(APPLICATION_NAME));
    msgBox1.addButton(tr("是"), QMessageBox::YesRole);
    msgBox1.addButton(tr("否"), QMessageBox::NoRole);
    msgBox1.setText(tr("您是否真的要删除所选择的命令?"));
    if(msgBox1.exec()==QMessageBox::RejectRole)
    {
        return;
    }

    int sel=0;
    int rowCount=dataModel->rowCount();
    for(int i=rowCount-1;i>=0;i--)
    {
        QStandardItem *item=dataModel->item(i,0);
        if(item->checkState()==Qt::Checked)
        {
            sel=1;
            dataModel->removeRow(i);
            AllItem.removeAt(i);
            if(i<AllItem.size()-1)
            {
                for(int k=AllItem.size()-1;k>=i;k--)
                    --AllItem[k].commNumber;
            }
        }
    }
    if(sel)
        saveBtn->setEnabled(true);

}

void ParaWidget::OnCancelDelBtnClicked()    //撤销
{
    AllItem=lastAllItem;
    //刷新数据
    refreshTableview();
    saveBtn->setEnabled(false);
}

void ParaWidget::OnUpdateTichnicsBtnClicked()    //更新工艺
{
    int sel=0;
    int rowCount=dataModel->rowCount();
    for(int i=0;i<rowCount;i++)
    {
        QStandardItem *item=dataModel->item(i,0);
        if(item->checkState()==Qt::Checked)
        {
            sel=1;
             if(AllItem[i].command==tr("点焊"))
            {
                dataModel->item(i,0)->setText(pointTechnicsComb->currentText());
                AllItem[i].technics=pointTechnicsComb->currentText();
            }
            else if((AllItem[i].command==QString(tr("拖焊"))) || (AllItem[i].command==QString(tr("多拖1")))
                    || (AllItem[i].command==QString(tr("多拖2"))) || (AllItem[i].command==QString(tr("多拖3"))))
            {
                 dataModel->item(i,0)->setText(lineTechnicsComb->currentText());
                 AllItem[i].technics=lineTechnicsComb->currentText();
            }
        }
    }
    //刷新数据
    refreshTableview();
    if(sel)
        saveBtn->setEnabled(true);

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

    if(sel==0)
    {
        msgBox.setText(tr("请选择要更新工艺的命令。"));
        msgBox.exec();
    }
}

void ParaWidget::OnUpdateCoordinateBtnClicked()    //更新坐标
{
    for(int i=0;i<6;i++)
        coordinateOffset[i]=coordinateEdit[i]->text().toDouble();
    int sel=0;
    int rowCount=dataModel->rowCount();
    for(int i=0;i<rowCount;i++)
    {
        QStandardItem *item=dataModel->item(i,0);
        if(item->checkState()==Qt::Checked)
        {
            sel=1;
            for(int k=0;k<6;k++)
            {
                if(k<4)
                {
                    AllItem[i].pointCoordinate1[k]+=coordinateOffset[k];
                    AllItem[i].pointCoordinate2[k]+=coordinateOffset[k];
                }
                else
                {
                    AllItem[i].pointCoordinate1[k+1]+=coordinateOffset[k];
                    AllItem[i].pointCoordinate2[k+1]+=coordinateOffset[k];
                }
            }
        }
    }
    //刷新数据
    //refreshTableview();
    if(sel)
        saveBtn->setEnabled(true);

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

    if(sel)
        msgBox.setText(tr("坐标已更新。"));
    else
        msgBox.setText(tr("请选择要更新坐标的命令。"));
    msgBox.exec();
}

void ParaWidget::OnSaveBtnClicked() //保存
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
        msgBox.setText(tr("无法打开数据库，保存失败。"));
        msgBox.exec();
        return;
    }

    QString projName=weldProject->getProjName();

    if(AllItem.size()>0)
    {
        if(!db.SaveProjAllItemToDb(projName,AllItem))
        {
            saveBtn->setEnabled(true);
            msgBox.setText(tr("无法写入数据库，保存失败。"));
            msgBox.exec();
            return;
        }
        else
        {
            saveBtn->setEnabled(false);
            weldProject->clear();
            weldProject->loadProject(projName);
            emit weldProject->reloadTableSig();
            msgBox.setText(tr("数据保存成功。"));
            msgBox.exec();
        }
    }
    db.closeDB();
    lastAllItem=AllItem;

    //刷新数据
    refreshTableview();
}

void ParaWidget::OnCloseBtnClicked()
{
    close();
    //done(QDialog::Accepted);
}

void ParaWidget::refreshTableview()
{
    //删除旧的表记录
    if(dataModel->rowCount()>0)
        dataModel->removeRows(0,dataModel->rowCount(),QModelIndex());
    int row=0;
    int cnt=AllItem.size();
    for(int i=0;i<cnt;i++)
    {
            QStandardItem *item=new QStandardItem("0");
            item->setCheckable(true);
            item->setTristate(false);   //true是3中选中状态，false是两种状态
            dataModel->setItem(row, 0, item);
            dataModel->setItem(row, 1, new QStandardItem(AllItem[i].command));
            dataModel->setItem(row, 2, new QStandardItem(AllItem[i].technics));
            item->setCheckState(Qt::Unchecked);
            item->setText(QString("%1").arg(i));
            row++;
    }
}
