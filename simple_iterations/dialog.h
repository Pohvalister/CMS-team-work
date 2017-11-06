#ifndef DIALOG_H
#define DIALOG_H

#include <QtWidgets>
#include <QDialog>
#include <QLineEdit>
#include <bitset>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
   Q_OBJECT

public:
   explicit Dialog(QWidget *parent = 0);
   double value;
   std::bitset<1> need_to_show;
   ~Dialog();

private slots:
   void onStartPress();

private:
   Ui::Dialog *ui;
   QLineEdit *r_field;
   QLabel *text;
   QCheckBox *simple_it_vis;
};

#endif // DIALOG_H
