#include "iter_set_menu.h"
#include "ui_iter_set_menu.h"

#include "seq_iterations.h"

iter_set_menu::iter_set_menu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::iter_set_menu)
{
    ui->setupUi(this);
}

iter_set_menu::~iter_set_menu()
{
    delete ui;
}

void iter_set_menu::on_buttonBox_accepted()
{
    if (ui->iter_Edit->text()!=""){
        iters= ui->iter_Edit->text().toInt();
    }else{
        iters= 0;
    }

    if (ui->scope_Edit->text()!=""){
        scope= ui->iter_Edit->text().toDouble();
    }else{
        scope = 0.0000000000001;
    }
}

void iter_set_menu::on_buttonBox_rejected()
{
    iters =-1;
    scope =-1;
}
