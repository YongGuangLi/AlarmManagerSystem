#include "lineedit.h"



LineEdit::LineEdit(QString text, QWidget *parent) : QLineEdit(parent)
{
    text_ = text;
    this->setMinimumWidth(350);
}

void LineEdit::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.drawRect(rect().x(), rect().y() , rect().width() - 1, rect().height() -1);

    if(text_.startsWith("2"))              //是否是已日期开头,日期开头为告警结论
    {
        int index = text_.indexOf(' ', 20);               //把告警结论分割开，上下显示两行
        this->setFixedHeight(50);
        painter.drawText( 40, rect().height() / 2 - 5,text_.left(index));
        painter.drawText( 40, rect().height() / 2 + 15,text_.mid(index + 1));
    }
    else
    {
        painter.drawText( 40, rect().height() / 2 + 5,text_);
    }

    painter.setBrush(color_);
    painter.drawEllipse(10, rect().height() / 2 - ELLIPSE_LINEEDIT/2, ELLIPSE_LINEEDIT, ELLIPSE_LINEEDIT);
}

void LineEdit::setText(const QString &text)
{
    text_ = text;
}

void LineEdit::setColor(const QColor &color)
{
    color_ = color;
}

QString LineEdit::text() const
{
    return text_;
}

int LineEdit::getFontWidth(QString str)
{
    QFont wordfont;
    wordfont.setFamily("宋体");
    wordfont.setPointSize(10);
    QFontMetrics fm(wordfont);
    QRect rec = fm.boundingRect(str);
    return rec.width();                                             //这个就获得了字符串所占的像素宽度
}


