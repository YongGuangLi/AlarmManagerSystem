#ifndef VersionDialog_H
#define VersionDialog_H

#include "basewindow.h"
#include <QDebug>
#include <QMouseEvent>

namespace Ui {
class VersionDialog;
}

class VersionDialog : public BaseWindow
{
    Q_OBJECT

public:
    explicit VersionDialog(QWidget *parent = 0);
    ~VersionDialog();


private:
    Ui::VersionDialog *ui;
};

#endif // VersionDialog_H
