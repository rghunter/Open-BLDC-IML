#ifndef SERIALSELECT_H
#define SERIALSELECT_H

#include <QDialog>
#include <qextserialenumerator.h>

namespace Ui {
    class serialSelect;
}

class serialSelect : public QDialog
{
    Q_OBJECT

public:
    explicit serialSelect(QWidget *parent = 0);
    ~serialSelect();
    QString getPort(void);

private:
    Ui::serialSelect *ui;
};

#endif // SERIALSELECT_H
