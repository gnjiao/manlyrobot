#include "optimizedlg.h"
#include "msqldb.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 工艺参数类
OptimizeDlg::OptimizeDlg(QWidget *parent) :
    QDialog(parent)
{
    dlgWidth=300;
    dlgHeight=500;
    checkBoxNum=0;
    checkBoxCount=0;
}

void OptimizeDlg::initDialog()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT);
    ft.setItalic(false);

    QLabel *capLabel=new QLabel(tr("焊接操作步骤"));
    capLabel->setFont(ft);

    createDataWidget();
    createButtonLayout();

    QGridLayout *layout=new QGridLayout;
    layout->addWidget(capLabel,0,0,1,2);
    layout->addWidget(tableView,1,0,8,2);
    layout->addLayout(btnLayout,9,0,1,2);

    setLayout(layout);
    setWindowTitle(tr("优化设置"));
    resize(dlgWidth,dlgHeight);

    int row=0;
    int cnt=weldProject->listCommand.size();
    listCheckBox=new QButtonGroup;
    for(int i=0;i<cnt;i++)
    {
        if((weldProject->listCommand[i] == QString(tr("点焊"))) || (weldProject->listCommand[i] == QString(tr("拖焊")))
                 || (weldProject->listCommand[i] == QString(tr("多拖1"))) || (weldProject->listCommand[i] == QString(tr("多拖2"))) || (weldProject->listCommand[i] == QString(tr("多拖3"))))
        {
            QStandardItem *item=new QStandardItem(tr("优化"));
            item->setCheckable(true);
            item->setTristate(true);
            dataModel->setItem(row, 0, new QStandardItem(QString("%1").arg(i)));
            dataModel->setItem(row, 1, new QStandardItem(weldProject->listCommand[i]));
            dataModel->setItem(row, 2,item);
            if(weldProject->optimize[i]==1)
            {
                item->setCheckState(Qt::PartiallyChecked);
                item->setText(tr("优化"));
            }
            else if(weldProject->optimize[i]==2)
            {
                item->setCheckState(Qt::Checked);
                item->setText(tr("高速"));
            }
            else
            {
                item->setCheckState(Qt::Unchecked);
                item->setText(tr("正常"));
            }
            //listCheckBox->addButton(mCheckBox,row);
            row++;
        }
    }
    checkBoxCount=row;
}

void OptimizeDlg::createDataWidget()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(20,120,120));
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT);
    ft.setItalic(false);

    dataModel = new QStandardItemModel(0,3);
    dataModel->setHeaderData(0, Qt::Horizontal, QObject::tr("指令序号"));
    dataModel->setHeaderData(1, Qt::Horizontal, QObject::tr("焊接类型"));
    dataModel->setHeaderData(2, Qt::Horizontal, QObject::tr("优化或高速"));

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
    connect(tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(setCheckItem(const QModelIndex &)));
}

void OptimizeDlg::createButtonLayout()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT);
    ft.setItalic(false);

    QPushButton *okButton=new QPushButton(tr("确定"));
    okButton->setFont(ft);
    connect(okButton, SIGNAL(clicked()), this, SLOT(OnOkBtnClicked()));
    QPushButton *cancelButton=new QPushButton(tr("取消"));
    cancelButton->setFont(ft);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(OnCancelBtnClicked()));

    btnLayout=new QHBoxLayout;
    btnLayout->addWidget(okButton);
    btnLayout->addWidget(cancelButton);
}

void OptimizeDlg::OnLoadOptCmd()
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
}

void OptimizeDlg::OnCancelBtnClicked()
{
    done(QDialog::Rejected);
}

void OptimizeDlg::OnOkBtnClicked()
{
    int rows=dataModel->rowCount();
    int *optm=new int[rows];
    for(int i=0;i<rows;i++)
    {
        QStandardItem *item=dataModel->item(i,2);
        //QCheckBox* chkBox=(QCheckBox*)(listCheckBox->button(i));
        if(item->checkState()==Qt::PartiallyChecked)
            optm[i]=1;
        else if(item->checkState()==Qt::Checked)
            optm[i]=2;
        else
            optm[i]=0;
    }
    //QStandardItem *item=dataModel->item(1,0);
    for(int i=0;i<rows;i++)
    {
        int val=dataModel->item(i,0)->data(Qt::DisplayRole).toInt();
        weldProject->optimize[val]=optm[i];
    }
    weldProject->saveProject();

    delete optm;
    done(QDialog::Accepted);
}

void OptimizeDlg::setCheckItem(const QModelIndex &index)
{
    int col=index.column();
    if(col<2)   //用户点击的是非选择项目
        return;
    QStandardItem *item=dataModel->itemFromIndex(index);
    Qt::CheckState st=item->checkState();
    if(st==Qt::Unchecked)
    {
        item->setCheckState(Qt::PartiallyChecked);
        item->setText(tr("优化"));
    }
    else if(st==Qt::PartiallyChecked)
    {
        item->setCheckState(Qt::Checked);
        item->setText(tr("高速"));
    }
    else
    {
        item->setText(tr("正常"));
        item->setCheckState(Qt::Unchecked);
    }
}
