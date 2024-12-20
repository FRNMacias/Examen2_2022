#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "muParser.h"
//#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
struct data{
    QVector<double> time;
    QVector<double> power;
    QVector<double> energy;
    int a;
};
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
// ----------------------------

public slots:

public:
    void setupCustomPlot(QWidget &view);
    void plotter(QVector<double> &x, QVector<double> &y, QWidget &view, const QColor &color);
    void showData(struct data *data);
    void llenar(struct data &v, FILE *f); //retorna el tamaño de cada vector
    void comparacion_area();
    int ind_cercano(const QVector<double> &vector, double x);
    double img_linealizada(const QVector<double> &v_x,const QVector<double> &v_y, double x, int a);
private:
    mu::Parser parser;
    struct data pv_power, utn_power;
    void CargarGraficar();
    void Analisis();
    double calculate_media(struct data *data);
    double calculate_area_t(struct data *data, int a);
    double calculate_area_r(struct data *data, int a);
    void Energy();
    void Guardar_energy();
    void Energy_neta();
};
#endif // MAINWINDOW_H
