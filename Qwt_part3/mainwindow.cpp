#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Создать поле со шкалами для отображения графика
    addPlot();

    // Включить масштабную сетку
    addPlotGrid();

    // Кривая
    addCurve();

    // Включить возможность приближения/удаления графика
    enableMagnifier();

    // Включить возможность перемещения по графику
    enableMovingOnPlot();

    // Включить отображение координат курсора и двух перпендикулярных
    // линий в месте его отображения
    enablePicker();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addPlot()
{
    // #include <qwt_plot.h>
    ui->Qwt_Widget->setTitle( "Qwt demonstration" );
    ui->Qwt_Widget->setCanvasBackground( Qt::white );

    // Параметры осей координат
    ui->Qwt_Widget->setAxisTitle(QwtPlot::yLeft, "Y");
    ui->Qwt_Widget->setAxisTitle(QwtPlot::xBottom, "X");
    ui->Qwt_Widget->insertLegend( new QwtLegend() );

}

void MainWindow::addPlotGrid()
{
    // #include <qwt_plot_grid.h>
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setMajorPen(QPen( Qt::gray, 2 )); // цвет линий и толщина
    grid->attach( ui->Qwt_Widget );
}

void MainWindow::addCurve()
{
    //#include <qwt_plot_curve.h>
    curve = new QwtPlotCurve();
    curve->setTitle( "Demo Curve" );
    curve->setPen( Qt::blue, 6 ); // цвет и толщина кривой

    // Маркеры кривой
    // #include <qwt_symbol.h>
    QwtSymbol *symbol = new QwtSymbol( QwtSymbol::Ellipse,
        QBrush( Qt::yellow ), QPen( Qt::red, 2 ), QSize( 8, 8 ) );
    curve->setSymbol( symbol );

    // Добавить точки на ранее созданную кривую
    // Значения точек записываются в массив, затем считываются
    // из этого массива
    for (int i = 0; i < 5; i++) {
        points << QPointF( 1.0 * i, 1.0 * i); // произвольное заполнение
    }

    curve->setSamples( points ); // ассоциировать набор точек с кривой

    curve->attach( ui->Qwt_Widget ); // отобразить кривую на графике
}

void MainWindow::enableMagnifier()
{
    // #include <qwt_plot_magnifier.h>
    QwtPlotMagnifier *magnifier =
            new QwtPlotMagnifier(ui->Qwt_Widget->canvas());
    // клавиша, активирующая приближение/удаление
    magnifier->setMouseButton(Qt::MidButton);
}

void MainWindow::enableMovingOnPlot()
{
    // #include <qwt_plot_panner.h>
    QwtPlotPanner *d_panner = new QwtPlotPanner( ui->Qwt_Widget->canvas() );
    // клавиша, активирующая перемещение
    d_panner->setMouseButton( Qt::RightButton );
}

void MainWindow::enablePicker()
{
    // #include <qwt_plot_picker.h>
    // настройка функций
    QwtPlotPicker *d_picker =
            new QwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft, // ассоциация с осями
    QwtPlotPicker::CrossRubberBand, // стиль перпендикулярных линий
    QwtPicker::AlwaysOn, // всегда включен
    ui->Qwt_Widget->canvas() ); // ассоциация с полем

    // Цвет перпендикулярных линий
    d_picker->setRubberBandPen( QColor( Qt::red ) );

    // цвет координат положения указателя
    d_picker->setTrackerPen( QColor( Qt::black ) );

    // непосредственное включение вышеописанных функций
    d_picker->setStateMachine( new QwtPickerDragPointMachine() );

    connect( d_picker, SIGNAL( appended( const QPoint & ) ),
            SLOT( click_on_canvas( const QPoint & ) ) );
}

void MainWindow::click_on_canvas( const QPoint &pos )
{
    // считываем значения координат клика
    double x = ui->Qwt_Widget->invTransform(QwtPlot::xBottom, pos.x());
    double y = ui->Qwt_Widget->invTransform(QwtPlot::yLeft, pos.y());

    statusBar()->showMessage("x= " + QString::number(x) +
                             "; y = " + QString::number(y));
}


void MainWindow::on_moveByXButton_clicked()
{
    double x = 0.0;
    // Выполняется преобразование ',' на '.' для того, чтобы
    // текст spinBox мог бы быть преобразован в double
    QString spinBoxText = ui->changeXSpinBox->text().replace(
                QLocale().decimalPoint(), QLatin1Char('.'));

    double xChangeValue = spinBoxText.toDouble();

    for (int i = 0; i <points.size(); i++) {
        x = points[i].x();
        points[i].setX(x + xChangeValue);
    }

    curve->setSamples(points);
    ui->Qwt_Widget->replot();
}
