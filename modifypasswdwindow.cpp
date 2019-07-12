#include "modifypasswdwindow.h"
#include "ui_modifypasswdwindow.h"

ModifyPasswdWindow::ModifyPasswdWindow(QWidget *parent)
	: BaseWindow(parent)
    , ui(new Ui::ModifyPasswdWindow)
{
    ui->setupUi(this);
	initWindow();
    initTitleBar(this);
}

ModifyPasswdWindow::~ModifyPasswdWindow()
{

}


void ModifyPasswdWindow::initWindow()
{
	//背景GIG图;
	QLabel* pBack = new QLabel(this);
    QMovie *movie = new QMovie(this);
	movie->setFileName(":/Resources/NetSetWindow/headBack.gif");
	pBack->setMovie(movie);
	movie->start();
    pBack->move(0, 0);

    connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(slot_btnOk_clicked()));

    signalMapper = new QSignalMapper(this);
    connect(ui->btnCancel, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(ui->btnCancel, 0);
}

void ModifyPasswdWindow::paintEvent(QPaintEvent *event)
{
	// 绘制背景图;
    QPainter painter(this);
    QPainterPath pathBack;
    pathBack.setFillRule(Qt::WindingFill);
    pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(pathBack, QBrush(QColor(235, 242, 249)));
	
    QPainterPath pathBottom;
    pathBottom.setFillRule(Qt::WindingFill);
    pathBottom.addRoundedRect(QRect(0, 300, this->width(), this->height() - 300), 3, 3);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(pathBottom, QBrush(QColor(205, 226, 242)));

    painter.setPen(QPen(QColor(160 , 175 , 189)));
    painter.drawRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
}


void ModifyPasswdWindow::showEvent(QShowEvent *)
{
    ui->lineEdit_username->clear();
    ui->lineEdit_oldpasswd->clear();
    ui->lineEdit_newpasswd->clear();
    ui->lineEdit_repeatpasswd->clear();

    ui->lineEdit_username->setFocus();
}

void ModifyPasswdWindow::slot_btnOk_clicked()
{
    QString userName = ui->lineEdit_username->text();
    QString oldPwd = ui->lineEdit_oldpasswd->text();
    QString newPwd = ui->lineEdit_newpasswd->text();
    QString pwdConfirm = ui->lineEdit_repeatpasswd->text();


    if(userName.isEmpty() || oldPwd.isEmpty())
    {
        myHelper::ShowMessageBoxInfo("修改失败，用户名/密码不能为空！");
        return;
    }
    if(newPwd.isEmpty() || pwdConfirm.isEmpty())
    {
        myHelper::ShowMessageBoxInfo("修改失败，新密码不能为空！");
        return;
    }
    if(newPwd.compare(pwdConfirm) != 0)
    {
        myHelper::ShowMessageBoxInfo("修改失败，两次密码输入不一致！");
        return;
    }
    if(oldPwd.compare(newPwd) == 0)
    {
        myHelper::ShowMessageBoxInfo("修改失败，新旧密码不能一致！");
        return;
    }
    if(userName.compare(newPwd) == 0)
    {
        myHelper::ShowMessageBoxInfo("修改失败，用户名和密码不允许相同！");
        return;
    }

    User_Info info;
    if(DataModel::getInstance()->findUserInfoByName(userName, info))
    {
        if(info.password.compare(oldPwd, Qt::CaseSensitive) != 0)
        {
            myHelper::ShowMessageBoxInfo("修改失败，登录密码不正确！");
            return;
        }
        QString activeTime = DataModel::getInstance()->getCurrentSysDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString sql_pwd = QString(SQL_UPDATE_USER_PWD)
          .arg(newPwd)
          .arg(activeTime)
          .arg(info.userId);

        if(SingletonDBHelper->execSqlStr(sql_pwd))
        {
            ui->btnCancel->click();
        }
    }
    else
    {
        myHelper::ShowMessageBoxInfo("修改失败，该用户不存在，请重新输入！");
        return;
    }
}
