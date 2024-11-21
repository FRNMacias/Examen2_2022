#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//------------------------------------------

    CargarGraficar();
    Analisis();
    //comparacion_area();
    Energy();
    Guardar_energy();
    Energy_neta();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::setupCustomPlot(QWidget &view) {
    QCustomPlot *customPlot = qobject_cast<QCustomPlot*>(&view);
    if(!customPlot){
        qDebug() << "Error: El Widget pasado no es un QCustomPlot";
        return ;
    }
    // Configurar interacciones
    customPlot->setInteraction(QCP::iRangeDrag, true);  // Permitir arrastrar
    customPlot->setInteraction(QCP::iRangeZoom, true);  // Permitir zoom
    //ui->plot2->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);  // Drag en ambos ejes
   // ui->plot2->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);  // Zoom en ambos ejes
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal);
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal);
    // Configurar el factor de zoom (opcional)
    customPlot->axisRect()->setRangeZoomFactor(1.2);  // Zoom más rápido
}
void MainWindow::plotter(QVector<double> &x, QVector<double> &y,QWidget &view, const QColor &color){
    //verifico si esta ventana pertenece a CustomPlot
    QCustomPlot *customPlot = qobject_cast<QCustomPlot*>(&view);
    if(!customPlot){
        qDebug() << "Error: El Widget pasado no es un QCustomPlot";
        return ;
    }
    int n = customPlot->graphCount();
    customPlot->addGraph();
    QPen pen;
    pen.setColor(color);
    customPlot->graph(n)->setPen(pen);
    customPlot->graph(n)->setData(x,y);
    customPlot->rescaleAxes();
    ui->plot->replot();
}
void MainWindow::showData(struct data *data){
    qDebug() <<"---------------------------------";
    qDebug() << "Cantidad de datos cargados:" << data->a;
    for(int i=0; i<data->a; i++){
        qDebug() << "time: "<< data->time[i] << " power: " << data->power[i];
    }
}
void MainWindow::llenar(struct data &v, FILE *f){
    int tam;
    fseek(f,0,SEEK_END);
    tam = ftell(f);
    rewind(f);
    v.a = tam/16;
    v.time.resize(v.a);
    v.power.resize(v.a);
    for(int i=0; i<(v.a); i++){
        fread(&v.time[i],sizeof(double),1,f);
        fread(&v.power[i],sizeof(double),1,f);
    }
    fclose(f);
    showData(&v);
}
void MainWindow::CargarGraficar(){
    FILE *f = fopen("C:\\Users\\macia\\OneDrive\\Documentos\\InformaticaII\\Info 2 Historico de Examenes\\Examen 2 - 2022\\pv_power.dat","rb");
    if(f){
        llenar(pv_power,f);
        plotter(pv_power.time, pv_power.power, *ui->plot, QColor(Qt::red));
    }
    f = fopen("C:\\Users\\macia\\OneDrive\\Documentos\\InformaticaII\\Info 2 Historico de Examenes\\Examen 2 - 2022\\utn_power.dat","rb");
    if(f){
        llenar(utn_power,f);
        plotter(utn_power.time,utn_power.power, *ui->plot, QColor(Qt::blue));
    }
    setupCustomPlot(*ui->plot);
}
void MainWindow::Analisis(){
    double max, min, media;
    for(int i=0; i<(utn_power.a -1); i++){
        if(i==0){
            max=utn_power.power[i];
            min=utn_power.power[i];
        }else{
            if(utn_power.power[i] < min){
                min=utn_power.power[i];
            }
            if(utn_power.power[i] > max){
                max = utn_power.power[i];
            }
        }
    }
    media = calculate_media(&utn_power);
    ui->Pmax1->setText(QString("Potencia maxima: %1").arg(max));
    ui->Pmin1->setText(QString("Potencia minima: %1").arg(min));
    ui->prom1->setText(QString("Promedio: %1").arg(media));
    for(int i=0; i<(pv_power.a - 1); i++){
        if(i==0){
            max=pv_power.power[i];
            min=pv_power.power[i];
        }else{
            if(pv_power.power[i] < min){
                min=pv_power.power[i];
            }
            if(pv_power.power[i] > max){
                pv_power.power[i];
            }
        }
    }
    media = calculate_media(&pv_power);
    ui->Pmax2->setText(QString("Potencia maxima: %1").arg(max));
    ui->Pmin2->setText(QString("Potencia minima: %1").arg(min));
    ui->prom2->setText(QString("Promedio: %1").arg(media));
}
double MainWindow::calculate_media(struct data *data){
    double media = 0, div = (data->time[(data->a-1)] - data->time[0])*2;
    for(int i=0; i<(data->a -1); i++){
        media += ((data->time[i+1] -  data->time[i])*(data->power[i+1] + data->power[i]))/div;
    }
    return media;
}
double MainWindow::calculate_area_t(struct data *data, int a){
    return ((data->time[a+1] -  data->time[a])*(((data->power[a+1] - data->power[a])/2)+data->power[a]));
}
double MainWindow::calculate_area_r(struct data *data, int a){
    return ((data->time[a+1] - data->time[a])*(data->power[a+1]));
}
void MainWindow::comparacion_area(){
    QVector<double> area_t(30), area_r(30);
    qDebug() << "comparacion de areas por dos métodos:";
    qDebug() << "Trapecio  ----------  Rectángulo";
    for(int i=0; i<30; i++){
        area_t[i] = calculate_area_t(&pv_power,i);
        area_r[i] = calculate_area_r(&pv_power,i);
        qDebug() << area_t[i] << "            " << area_r[i];
    }
}
void MainWindow::Energy(){
    int tam = utn_power.a - 1;
    utn_power.energy.resize(tam);
    for(int i=0; i<tam; i++){
        utn_power.energy[i] = (calculate_area_t(&utn_power,i))/3600000;
    }
    plotter(utn_power.time, utn_power.energy,*ui->plot2,QColor(Qt::blue));

    tam = pv_power.a - 1;
    pv_power.energy.resize(tam);
    for(int i=0; i<tam; i++){
        pv_power.energy[i] = (calculate_area_t(&pv_power,i))/3.6e+6;
    }
    plotter(pv_power.time, pv_power.energy,*ui->plot2,QColor(Qt::red));
    setupCustomPlot(*ui->plot2);
}
void MainWindow::Guardar_energy(){
    FILE *f = fopen("C:\\Users\\macia\\OneDrive\\Documentos\\InformaticaII\\Info 2 Historico de Examenes\\Examen 2 - 2022\\energy.dat","wb");
    if(f !=NULL){
        int tam;
        //formato T1 E1 T2 E2 T3 E3 ... Tn En.
        if(pv_power.a == utn_power.a){ //si son del mismo tamaño, recorro todo.
            tam = pv_power.a;
        }else{
            if(pv_power.a < utn_power.a)
                tam = pv_power.a;
            if(pv_power.a > utn_power.a)
                tam = utn_power.a;
        }
        for(int i=0; i<(tam -1); i++){ //recorro hasta -1 ya que energy tiene un elemento menos
            double dif = (pv_power.energy[i])-(utn_power.energy[i]);
            fwrite(&pv_power.time,sizeof(double),1,f);
            fwrite(&dif,sizeof(double),1,f);
        }
        fclose(f);
    }else{
        qDebug() << "Error. No es posible abrir el archivo 'Energy' para escritura";
        return ;
    }
}
int MainWindow::ind_cercano(const QVector<double> &vector, double x){
    int a=0;
    for(int i=0; i<vector.size(); i++){
        if(vector[i]<=x){
            a=i;
        }
    }
    return a;
}
double MainWindow::img_linealizada(const QVector<double> &v_x,const QVector<double> &v_y, double x, int a){
    double img = 0;
    if(v_x[a] < x){
        img = (((x - v_x[a])*(v_y[a+1] - v_y[a]))/(v_x[a+1] - v_x[a])) + v_y[a];
    }else if(v_x[a] > x){
        img = (((x-v_x[a-1])*(v_y[a] - v_y[a-1]))/(v_y[a] - v_y[a-1]))+ + v_y[a-1];
    }else if(v_x[a] == x){
        img = v_y[a];
    }
    return img;
}
void MainWindow::Energy_neta(){
    int tam = pv_power.a + utn_power.a;
    QVector<double>energy_neto(tam);
    QVector<double>img_lin(pv_power.a);
    for(int i=0; i<pv_power.a; i++){
        int ind = ind_cercano(utn_power.time, pv_power.time[i]);
        img_lin[i] = img_linealizada(utn_power.time,utn_power.power,pv_power.time[i],ind);
    }
    for(int i=0; i<tam; i++){
        energy_neto[i] = img_lin[i] - pv_power.energy[i];
    }
    plotter(pv_power.time,energy_neto,*ui->plot2, QColor(Qt::magenta));
}
