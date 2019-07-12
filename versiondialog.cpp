#include "versiondialog.h"
#include "ui_versiondialog.h"

VersionDialog::VersionDialog(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::VersionDialog)
{
    ui->setupUi(this);

    ui->label_versioninfo->setText(QString::fromLocal8Bit("<h3>全景数据集成及智能告警系统<br/>V2.0&nbsp;&nbsp;&nbsp;&nbsp;</h3><br/>"
                                 "Copyright 2019<br/>"
                                 "<b>湖南大唐先一科技有限公司</b>"));

    ui->label_versioninfo->setStyleSheet("color:black;font-size:15px;");

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(hide()));

    initTitleBar(this, " 关于 全景数据集成及智能告警系统", ":Resources/Image/main_logo.png", QColor(99,99,99), false);
    connect(this, SIGNAL(signal_btnClose_Clicked()), this, SLOT(hide()));
}

VersionDialog::~VersionDialog()
{
    delete ui;
}


