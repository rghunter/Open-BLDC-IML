#include "serialselect.h"
#include "ui_serialselect.h"

serialSelect::serialSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::serialSelect)
{
    ui->setupUi(this);
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    for (int i=0; i < ports.size(); i++)
    {
            ui->portBox->addItem(ports.at(i).physName);
    }


}

QString serialSelect::getPort()
{
        return ui->portBox->currentText();
}

serialSelect::~serialSelect()
{
    delete ui;
}
