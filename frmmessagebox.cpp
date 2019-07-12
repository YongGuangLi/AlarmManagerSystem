#include "frmmessagebox.h"
#include "ui_frmmessagebox.h"
#include "iconhelper.h"
#include "myhelper.h"

frmMessageBox::frmMessageBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmMessageBox)
{
    ui->setupUi(this);

    this->mousePressed = false;
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint);
    //设置窗体关闭时自动释放内存
    this->setAttribute(Qt::WA_DeleteOnClose);
    //设置图形字体
    IconHelper::Instance()->setIcon(ui->lab_Ico, QChar(0xf015), 12);
    IconHelper::Instance()->setIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    //关联关闭按钮
    connect(ui->btnMenu_Close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    //窗体居中显示
    myHelper::FormInCenter(this);
}

frmMessageBox::~frmMessageBox()
{
    delete ui;
}

void frmMessageBox::SetMessage(const QString &msg, int type)
{
    if (type == 0) {
        ui->btnCancel->setVisible(false);
        ui->lab_Title->setText("提示");
    } else if (type == 1) {
        ui->lab_Title->setText("询问");
    } else if (type == 2) {
        ui->btnCancel->setVisible(false);
        ui->lab_Title->setText("错误");
    }

    ui->labInfo->setText(msg);
}

void frmMessageBox::on_btnOk_clicked()
{
    done(QMessageBox::Ok);
    this->close();
}

void frmMessageBox::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton)) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void frmMessageBox::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void frmMessageBox::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}
