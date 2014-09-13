#ifndef IMAGETHRESHPROCESS_H
#define IMAGETHRESHPROCESS_H

#include <QWidget>

#include "ui_imagethreshprocess.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "QImage"
#include <QVector>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_legend.h>
#include <qwt_point_data.h>

typedef unsigned short PixelType;
using namespace cv;
class ImageThreshProcess : public QWidget
{
	Q_OBJECT

public:
	ImageThreshProcess(QWidget *parent = 0);
	~ImageThreshProcess();
	void showProcessedImage(QString path);
	void showImage(QString path);
	void setFileName(QString path){luminescneceFileName=path;}
//	void saveImage(QString path);

private:
	Ui::ImageThreshProcess ui;
	QString luminescneceFileName;
	Mat originalImage;
	Mat processedImage;
	
	QImage* img;

	QwtPlotCurve curve;
	QVector<double> xs;
	QVector<double> ys;

	bool isProcessed;
	bool isSave;

private slots:
	void on_pushButton_quit_clicked();
	void on_pushButton_process_clicked();
	void on_pushButton_reset_clicked();

signals:
	void done();
	void doneWithoutProcessing();
};

#endif // IMAGETHRESHPROCESS_H
