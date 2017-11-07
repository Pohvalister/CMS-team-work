#include "dialog.h"
#include "ui_dialog.h"
#include <string>

Dialog::Dialog(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::Dialog)
{
   ui->setupUi(this);
   {
      QFont fnt( "Arial", 20);
      ui->header->setFont(fnt);
   }
   text = new QLabel(tr("constant r"));
   r_field = new QLineEdit();
   simple_it_vis = new QCheckBox("Iration method vis");
   phi_it_vis = new QCheckBox("phi(x) = r*x*(1-x) vis");
   seq_vis = new QCheckBox("visualisation of sequence");
   bifur_diag = new QCheckBox("bifurcation diagram");

   layout = new QFormLayout;
   layout->addWidget(ui->header);
   layout->addRow(text, r_field);
   layout->addRow(simple_it_vis);
   layout->addRow(phi_it_vis);
   layout->addRow(seq_vis);
   layout->addRow(bifur_diag);

   connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onStartPress()));
   connect(ui->closeButton, SIGNAL(clicked(bool)), this, SLOT(onClosePress()));

   setLayout(layout);
}

Dialog::~Dialog()
{
   delete ui;
   delete r_field;
   delete text;
   delete simple_it_vis;
   delete phi_it_vis;
   delete layout;
   delete seq_vis;
   delete bifur_diag;
}

void Dialog::onStartPress()
{
   value = r_field->text().toDouble();
//   QMessageBox::information(NULL, "Test", std::to_string(value).c_str());

   if (simple_it_vis->isChecked()) need_to_show_id = 0;
   else if (phi_it_vis->isChecked()) need_to_show_id = 1;
   else if (seq_vis->isChecked()) need_to_show_id = 2;
   else if (bifur_diag->isChecked()) need_to_show_id = 3;

   accept();
}

void Dialog::onClosePress()
{
   is_closed = true;
   accept();
}
