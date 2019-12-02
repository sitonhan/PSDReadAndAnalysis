#ifndef MYWIDGET_H
#define MYWIDGET_H


#include <qmath.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_legend.h>
#include <qwt_point_data.h>
#include <qwt_text_label.h>
#include <QTimer>
#include <QVector>
#include <QDateTime>
#include <QString>
#include <qwt_plot_grid.h>
#include <QPushButton>
#include "ad7606.h"


using namespace std;

namespace Ui {
    class myWidget;
}
class myWidget : public QWidget
{
    Q_OBJECT
public:
    explicit myWidget(QWidget *parent = 0);
    ~myWidget();
private slots:
    void timerUpdate();
    void getADCStart();
    void closeADC();

private:
    Ui::myWidget *ui;

    //QString *text;

    QwtPlotCurve *point0;
    QVector<double>pointX = {0};
    QVector<double>pointY = {0};

    QwtPlotCurve *curve0;
    QwtPlotCurve *curve1;
    QwtPlotCurve *curve2;
    QwtPlotCurve *curve3;

    //X轴
    QVector<double> time = { 0, 1, 2, 3, 4, 5, 6, 7};
    //Y轴
    QVector<double>val0 = {0,0,0,0,0,0,0,0};
    QVector<double>val1 = {0,0,0,0,0,0,0,0};
    QVector<double>val2 = {0,0,0,0,0,0,0,0};
    QVector<double>val3 = {0,0,0,0,0,0,0,0};

    //QVector<double> temp = {0,0,0,0,0,0,0,0};
    BYTE Index;
    WORD RawData[8];
    BYTE Result;


};
#endif // MYWIDGET_H
