#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLineEdit>
#include <cmath>
#include <QToolButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix("C:/Users/ASUS/Desktop/xcircuit figures/rlc_circuit");
    ui->label_pic->setPixmap(pix);
    connect(ui->toolButton,SIGNAL(clicked()),this,SLOT(onclicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::onclicked()
{
    circuit_solver();
}
void MainWindow::circuit_solver()
{
    double h=1/(100000.0);
    double V=ui->lineEdit->text().toDouble();
    double R=ui->lineEdit_2->text().toDouble();
    double C=ui->lineEdit_3->text().toDouble();
    double L=ui->lineEdit_4->text().toDouble();
    QVector<double> t(10000002),i(10000002);
    t[0]=0;
    i[0]=0;
    double imax;
    double w=1/sqrt(L*C);
    double del=(R*sqrt(C/L))/2;
    if(del==0)
    {
        imax=V/(L*w);
        for(double n=1;n<10000001;n++)
        {
            t[n]=n*h;
            i[n]=imax*sin(w*t[n]);
        }

    }
    else
        if(del<1)
        {
            for(double n=1;n<10000001;n++)
            {
                t[n]=n*h;
                i[n]=V*exp(-1*del*w*t[n])*sin(sqrt(1-del*del)*w*t[n])/(sqrt(1-del*del)*L*w);
            }
        }
    else if(del==1)
        {
            for(double n=1;n<10000001;n++)
            {
                t[n]=n*h;
                i[n]=V*t[n]*exp(-1*w*t[n])/L;
            }
        }
    else if(del>1)
        {
            double s1=(-1*del*w)+(sqrt(del*del-1))*w;
            double s2=(-1*del*w)-(sqrt(del*del-1))*w;
            imax=V/(2*w*L*sqrt(del*del-1));
            for(double n=1;n<10000001;n++)
            {
                t[n]=n*h;
                i[n]=imax*(exp(s1*t[n])-exp(s2*t[n]));
            }

        }


    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(t,i);
    ui->customPlot->xAxis->setLabel("t");
    ui->customPlot->yAxis->setLabel("i");
    ui->customPlot->xAxis->setRange(0,100);
    ui->customPlot->yAxis->setRange(20,-20);
    ui->customPlot->replot();

}

