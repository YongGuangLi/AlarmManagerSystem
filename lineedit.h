#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QLineEdit>
#include <QColor>
#include <QDebug>
#include <QFont>

#define ELLIPSE_LINEEDIT 16
#define HEIGHT_LINEEDIT  25

class LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit LineEdit(QString,QWidget *parent = 0);
    void paintEvent(QPaintEvent *);

    void setText(const QString &text);

    void setColor(const QColor &color);

    QString text() const;

    int getFontWidth(QString str);

private:
    QString text_;
    QColor color_;
signals:

public slots:

};

#endif // LINEEDIT_H
