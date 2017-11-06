#include "dialog.h"
#include "ui_dialog.h"
#include <string>

Dialog::Dialog(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::Dialog)
{
   ui->setupUi(this);

   text = new QLabel(tr("constant r"));
   r_field = new QLineEdit();
   simple_it_vis = new QCheckBox("Iration method visualization");

   QGridLayout *layout = new QGridLayout;
   layout->addWidget(text, 0, 0, Qt::AlignTop);
   layout->addWidget(r_field, 0, 1);
   layout->addWidget(simple_it_vis, 1, 0, Qt::AlignTop);

   connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onStartPress()));

   setLayout(layout);
}

Dialog::~Dialog()
{
   delete ui;
}

void Dialog::onStartPress()
{
   value = r_field->text().toDouble();
//   QMessageBox::information(NULL, "Test", std::to_string(value).c_str());
   need_to_show[0] = simple_it_vis->isChecked();
//   QMessageBox::information(NULL, "Test", std::to_string(need_to_show[0]).c_str());
   accept();
}
