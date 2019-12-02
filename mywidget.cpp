#include "mywidget.h"
#include "ui_mywidget.h"

myWidget::myWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myWidget)
{
    ui->setupUi(this);

// 点绘制模块
    ui->qwtPlot0->setTitle("PSD上点的位置"); // 设置图标题名称
    ui->qwtPlot0->resize(300,300);
    ui->qwtPlot0->setAxisTitle(QwtPlot::xBottom,"x->"); //设置X轴的名称
    ui->qwtPlot0->setAxisTitle(QwtPlot::yLeft,"y->"); //设置Y轴的名称
    ui->qwtPlot0->setAxisScale(QwtPlot::xBottom, -10.0, 10.0,2.0);//设置X轴坐标刻度
    ui->qwtPlot0->setAxisScale(QwtPlot::yLeft, -10.0, 10.0, 2.0);//设置Y轴坐标刻度
    QwtPlotGrid *grid0 = new QwtPlotGrid;// 设置网格
    grid0->enableX(true);
    grid0->enableY(true);
    grid0->setMajorPen(Qt::black,0,Qt::DotLine);
    grid0->attach(ui->qwtPlot0);

    point0 = new QwtPlotCurve();//配置点
    point0->setTitle("laser ponit");
    point0->setPen(Qt::red,2.0,Qt::SolidLine);
    point0->setStyle(QwtPlotCurve::Dots);
    point0->attach(ui->qwtPlot0);

// 曲线绘制模块
    ui->qwtPlot->setTitle("ADC"); // 设置曲线图标题名称
    ui->qwtPlot->resize(640,400); //widget中定义大小，不在此处定义
    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom,"x->"); //设置X轴的名称
    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft,"y->"); //设置Y轴的名称
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, 0.0, 8.0,1.0);//设置X轴坐标刻度
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft, -1.0, 1.0, 0.1);//设置Y轴坐标刻度
    //(void) new QwtPlotMagnifier(ui->qwtPlot->canvas()); //使能滚轮放大/缩小
    //(void) new QwtPlotPanner( ui->qwtPlot->canvas() ); //使能鼠标左键平移
    QwtPlotGrid *grid = new QwtPlotGrid;// 设置网格
    grid->enableX(true);
    grid->enableY(true);
    grid->setMajorPen(Qt::black,0,Qt::DotLine);
    grid->attach(ui->qwtPlot);

    ui->qwtPlot->insertLegend(new QwtLegend(), QwtPlot::BottomLegend); //设置曲线标签

// 配置曲线
    curve0 = new QwtPlotCurve();
    curve0->setTitle("ch0"); //设置曲线名称
    curve0->setPen(Qt::red,2); //配置曲线颜色宽度等
    curve0->setCurveAttribute(QwtPlotCurve::Fitted, true); //曲线平滑
    curve0->attach(ui->qwtPlot);//附着曲线
    curve1 = new QwtPlotCurve();
    curve1->setTitle("ch1");
    curve1->setPen(Qt::green,2);
    curve1->setCurveAttribute(QwtPlotCurve::Fitted, true);
    curve1->attach(ui->qwtPlot);
    curve2 = new QwtPlotCurve();
    curve2->setTitle("ch2");
    curve2->setPen(Qt::blue,2);
    curve2->setCurveAttribute(QwtPlotCurve::Fitted, true);
    curve2->attach(ui->qwtPlot);
    curve3 = new QwtPlotCurve();
    curve3->setTitle("ch3");
    curve3->setPen(Qt::gray,2);
    curve3->setCurveAttribute(QwtPlotCurve::Fitted, true);
    curve3->attach(ui->qwtPlot);

//配置文本框
//    QByteArray temp = {val0[0],val1[0],val2[0],val3[0]};
//    ui->TextLabel->setText(temp);

//pushButton设置
    ui->pushButton->setText("启动设备");
    connect(ui->pushButton,QPushButton::clicked,this,myWidget::getADCStart);
    ui->pushButton_2->setText("关闭设备");
    connect(ui->pushButton_2,QPushButton::clicked,this,myWidget::closeADC);
    Index = M3F20xm_OpenDevice();
}

// 开启adc
void myWidget::getADCStart()
{

    M3F20xm_Verify(Index,&Result);
    M3F20xm_InitFIFO(Index);//初始化储存器
    M3F20xm_ADCStart(Index);//开始采样
    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout, this, &myWidget::timerUpdate);
    timer->start(10); // 10ms触发一次
}

void myWidget::closeADC()
{
    M3F20xm_ADCStop(Index);

}

//析构函数
myWidget::~myWidget()
{
    M3F20xm_CloseDevice(Index);
    delete ui;

}

void myWidget::timerUpdate()
{
    M3F20xm_ADCRead(Index,RawData);//采集数据

    val0.erase(val0.begin());//解析数据
    val0.push_back(short(RawData[0])/32768.0);
    val1.erase(val1.begin());
    val1.push_back(short(RawData[1])/32768.0);
    val2.erase(val2.begin());
    val2.push_back(short(RawData[2])/32768.0);
    val3.erase(val3.begin());
    val3.push_back(short(RawData[3])/32768.0);

    curve0->setSamples(time,val0);//曲线赋值
    curve1->setSamples(time,val1);
    curve2->setSamples(time,val2);
    curve3->setSamples(time,val3);
    ui->qwtPlot->replot();

    pointX.replace(0,val0[0]/val2[0]);//点赋值
    pointY.replace(0,val1[0]/val3[0]);
    point0->setSamples(pointX,pointY);
    ui->qwtPlot0->replot();
}
