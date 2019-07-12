#include "loginnetsetwindow.h"
#include "ui_loginnetsetwindow.h"

LoginNetSetWindow::LoginNetSetWindow(QWidget *parent)
	: BaseWindow(parent)
    ,ui(new Ui::LoginNetSetWindow)
{
    ui->setupUi(this);
	initWindow();
    initTitleBar(this);
}

LoginNetSetWindow::~LoginNetSetWindow()
{

}



void LoginNetSetWindow::initWindow()
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

    connect(ui->btnOk, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(ui->btnOk, 0);

    connect(ui->btnCancel, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(ui->btnCancel, 0);
}

void LoginNetSetWindow::paintEvent(QPaintEvent *event)
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

void LoginNetSetWindow::showEvent(QShowEvent *)
{
    ui->lineEdit_mysqlip->setText(SingletonConfig->getIpMySql());
    ui->lineEdit_mysqlport->setText(QString::number(SingletonConfig->getPortMySql()));
    ui->lineEdit_mysqluser->setText(SingletonConfig->getUserMysql());
    ui->lineEdit_mysqlpasswd->setText(SingletonConfig->getPasswdMysql());

    ui->lineEdit_redisip->setText(SingletonConfig->getIpRedis());
    ui->lineEdit_redisport->setText(QString::number(SingletonConfig->getPortRedis()));
    ui->lineEdit_redispasswd->setText(SingletonConfig->getPasswdRedis());

}

void LoginNetSetWindow::slot_btnOk_clicked()
{
    SingletonConfig->setIpMySql(ui->lineEdit_mysqlip->text());
    SingletonConfig->setPortMySql(ui->lineEdit_mysqlport->text().toInt());
    SingletonConfig->setUserMySql(ui->lineEdit_mysqluser->text());
    SingletonConfig->setPasswdMySql(ui->lineEdit_mysqlpasswd->text());

    SingletonConfig->setIpRedis(ui->lineEdit_redisip->text());
    SingletonConfig->setPortRedis(ui->lineEdit_redisport->text().toInt());
    SingletonConfig->setPasswdRedis(ui->lineEdit_redispasswd->text());

    SingletonConfig->databaseConfigWriteToFile();


    qApp->exit(RETCODE_RESTART);
}
