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
    double xmax=100;
    double ymin=-20;
    double ymax=20;
    double w=1/sqrt(L*C);
    double del=(R*sqrt(C/L))/2;
    if(del==0)
    {
        imax=V/(L*w);
        xmax=(8*3.14*w)/2;
        for(double n=1;n<(100000*xmax);n++)
        {
            t[n]=n*h;
            i[n]=imax*sin(w*t[n]);
        }

        ymin=1.5*-1*imax;
        ymax=1.5*imax;


    }
    else
        if(del<1)
        {
            double a=sqrt(1-del*del)*w;
            double b=del*w;
            xmax=15/b;
            for(double n=1;n<(100000*xmax);n++)
            {
                t[n]=n*h;
                i[n]=V*exp(-1*del*w*t[n])*sin(sqrt(1-del*del)*w*t[n])/(sqrt(1-del*del)*L*w);
            }

            ymax=1.5*V*exp(-1*b*atan(a/b)/a)/(L*w);
            ymin=-1*V*exp(-1*b*atan(a/b)/a)/(L*w);

        }
    else if(del==1)
        {
            xmax=15/w;
            for(double n=1;n<(100000*xmax);n++)
            {
                t[n]=n*h;
                i[n]=V*t[n]*exp(-1*w*t[n])/L;
            }

            ymin=0;
            ymax=V/(2*L*w);
        }
    else if(del>1)
        {
            double s1=(-1*del*w)+(sqrt(del*del-1))*w;
            double s2=(-1*del*w)-(sqrt(del*del-1))*w;
            imax=V/(2*w*L*sqrt(del*del-1));
            double b=del*w;
            double a=w*sqrt(del*del-1);
            //xmax=(100*log(a+b/(b-a)))/(2*a);
            //xmax=15/(b-a);
            xmax=10/(b-a);
            ymax=1.5*imax*(pow((a+b)/(b-a),(a-b)/(2*a))-pow((a+b)/(b-a),-1*(a+b)/(2*a)));


            for(double n=1;n<(100000*xmax);n++)
            {
                t[n]=n*h;
                i[n]=imax*(exp(s1*t[n])-exp(s2*t[n]));
            }



            ymin=0;





        }


    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(t,i);
    ui->customPlot->xAxis->setLabel("t(s)");
    ui->customPlot->yAxis->setLabel("current(A)");
    ui->customPlot->xAxis->setRange(0,xmax);
    ui->customPlot->yAxis->setRange(ymin,ymax);
    ui->customPlot->replot();

}

