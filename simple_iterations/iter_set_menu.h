#ifndef ITER_SET_MENU_H
#define ITER_SET_MENU_H

#include <QDialog>

namespace Ui {
class iter_set_menu;
}

class iter_set_menu : public QDialog
{
    Q_OBJECT

public:
    explicit iter_set_menu(QWidget *parent = 0);
    ~iter_set_menu();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::iter_set_menu *ui;
public:
   size_t iters;
   double scope;
};

#endif // ITER_SET_MENU_H
