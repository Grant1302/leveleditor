#include "CConfigDialog.h"

CConfigDialog::CConfigDialog(QMap<QString, short> &Config, QWidget *Parent) :
    QDialog(Parent)
{
    QLabel *widthLabel = new QLabel("Width:");
    WidthEdit = new QLineEdit(QString("%1").arg(Config["width"]));
    WidthEdit->setValidator(new QIntValidator(1, 1000));
    QLabel *heightLabel = new QLabel("Height:");
    HeightEdit = new QLineEdit(QString("%1").arg(Config["height"]));
    HeightEdit->setValidator(new QIntValidator(1, 1000));
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(widthLabel, 0, 0, 1, 1);
    layout->addWidget(WidthEdit, 0, 1, 1, 1);
    layout->addWidget(heightLabel, 1, 0, 1, 1);
    layout->addWidget(HeightEdit, 1, 1, 1, 1);
    layout->addWidget(buttonBox, 3, 0, 1, 2);
    setLayout(layout);

    setWindowTitle(tr("Config"));

    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

QMap<QString, short> CConfigDialog::Result()
{
    QMap<QString, short> ret;

    ret["width"] = WidthEdit->text().toInt();
    ret["height"] = HeightEdit->text().toInt();

    return ret;
}
