#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QtGui>

class CConfigDialog : public QDialog
{
    Q_OBJECT

private:
    QLineEdit *WidthEdit;
    QLineEdit *HeightEdit;
    
public:
    explicit CConfigDialog(QMap<QString, short> &Config, QWidget *Parent = NULL);
    QMap<QString, short> Result();
};

#endif // CONFIGDIALOG_H
