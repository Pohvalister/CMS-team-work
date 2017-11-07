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

   {
       srand(QDateTime::currentDateTime().toTime_t());
       ui->setupUi(this);

       //ui->AllRValues->setInteractions();
       ui->AllRValues->xAxis->setRange(0, 228);
       ui->AllRValues->yAxis->setRange(-1, 1);
       ui->AllRValues->yAxis->setTicks(false);
       ui->AllRValues->axisRect()->setupFullAxesBox();

       ui->AllRValues->plotLayout()->insertRow(0);
       QCPTextElement *title = new QCPTextElement(ui->AllRValues, "R value borders", QFont("sans", 10, QFont::Bold));
       ui->AllRValues->plotLayout()->addElement(0, 0, title);

     //  ui->customPlot->legend->setVisible(true);
       QFont legendFont = font();
       legendFont.setPointSize(10);
       ui->AllRValues->legend->setFont(legendFont);
       ui->AllRValues->legend->setSelectedFont(legendFont);
       ui->AllRValues->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items
       ui->AllRValues->rescaleAxes();

       ///some points need to be inserted
       const size_t amount = 1;
       double values_mas[] = {10.1};
       double h = 1;
       for (size_t i =0; i< amount;i++){
           QVector<double> x, y;
           const int N = 4;
           const double step = (h + h) / N;
           for (double cur_y = -h; cur_y <= h; cur_y += step) {
              x.push_back(values_mas[i]);
              y.push_back(cur_y);
           }
           ui->AllRValues->addGraph();
           ui->AllRValues->graph()->setData(x, y);
           ui->AllRValues->replot();
       }
   }

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
   need_to_show[0] = simple_it_vis->isChecked();
   need_to_show[1] = phi_it_vis->isChecked();
   need_to_show[2] = seq_vis->isChecked();
   need_to_show[3] = bifur_diag->isChecked();
   accept();
}

void Dialog::onClosePress()
{
   is_closed = true;
   accept();
}
