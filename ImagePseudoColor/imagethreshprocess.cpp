#include "imagethreshprocess.h"
#include "QMessageBox"
#include "qalgorithms.h"
#include "qfile.h"


using namespace std;
ImageThreshProcess::ImageThreshProcess(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	isProcessed=false;
	isSave=false;

	QObject::connect(ui.pushButton_Preoss,SIGNAL(clicked()),this,SLOT(on_pushButton_process_clicked()));
	QObject::connect(ui.pushButton_SaveQuit,SIGNAL(clicked()),this,SLOT(on_pushButton_quit_clicked()));
	QObject::connect(ui.pushButton_Reset,SIGNAL(clicked()),this,SLOT(on_pushButton_reset_clicked()));
	(void) new QwtPlotMagnifier( ui.qwtPlot->canvas() );
	(void) new QwtPlotPanner( ui.qwtPlot->canvas() );
	for (double x = 0; x < 65535; x+=1)
	{
		xs.append(x);
		ys.append(x*x);
	}
	curve.attach(ui.qwtPlot);//把曲线附加到plot上
	curve.setSamples(xs,ys);
	curve.setStyle(QwtPlotCurve::Lines);//设置曲线上是点还是线，默认是线，所以此行可不加
//	curve.setCurveAttribute(QwtPlotCurve::Fitted, true);//使曲线更光滑，不加这句曲线会很硬朗，有折点
	curve.setPen(QPen(Qt::blue));//设置画笔

}

ImageThreshProcess::~ImageThreshProcess()
{
	if(isSave&&isProcessed)
	{
		isSave=false;
		isProcessed=false;
		emit done();
	}
	else
	{
		QFile::remove(".//tempFiles//thresholdProcessed.tif");
		isSave=false;
		isProcessed=false;
		emit doneWithoutProcessing();
	}
}
void ImageThreshProcess::showProcessedImage(QString path)
{
// 	img=new QImage;
// 	if(!img->load(path));
// 	{
// 		QMessageBox::information(this,tr("Open image error"),tr("Open image error!"));
// 		isProcessed=false;
// 		return;
// 	}
// 	QImage* imgScaled=new QImage;
// 	*imgScaled=img->scaled(ui.label_OrignianlImage->width(),ui.label_OrignianlImage->height(),Qt::KeepAspectRatio);
// 	ui.label_OrignianlImage->setPixmap(QPixmap::fromImage(*imgScaled));
}
void ImageThreshProcess::showImage(QString path)
{
	img=new QImage;
	if(!img->load(path))
	{
		QMessageBox::information(this,tr("Open image error"),tr("Open image error!"));
		return;
	}
	QImage* imgScaled=new QImage;
	*imgScaled=img->scaled(ui.label_OrignianlImage->width(),ui.label_OrignianlImage->height(),Qt::KeepAspectRatio);
	ui.label_OrignianlImage->setPixmap(QPixmap::fromImage(*imgScaled));

	if(!isProcessed)
	{
		originalImage=imread(string((const char *)luminescneceFileName.toLocal8Bit()),CV_LOAD_IMAGE_ANYCOLOR|CV_LOAD_IMAGE_ANYDEPTH);
		PixelType originalHSize=originalImage.rows;//原始图像的行
		PixelType originalVSize=originalImage.cols;//原始图像的列

		double tempMax,tempMin;
		minMaxLoc(originalImage,&tempMin,&tempMax);
		ui.spinBox_ThresholdValue->setValue((int)tempMax);
		ui.spinBox_ThresholdValue->setMaximum((int)tempMax);
		ui.spinBox_SetToValue->setValue((int)tempMax);
		ui.spinBox_SetToValue->setMaximum((int)tempMax);

		Mat histImage;//直方图数据
		int histsize=1024;
		float range[] = {0, 65535};
		const float *ranges[] = {range};
		calcHist(&originalImage,1,0,Mat(),histImage,1,&histsize,ranges,true,false);

		xs.clear();
		ys.clear();
		for(double i=0;i<1024;++i)
		{
			xs.append(i*64);
			ys.append(histImage.at<float>(i,0));
		}
		curve.setSamples(xs,ys);//设置数据
	}
	

}
void ImageThreshProcess::on_pushButton_process_clicked()
{
	PixelType thresholdValue=ui.spinBox_ThresholdValue->value();
	PixelType setToValue=ui.spinBox_SetToValue->value();
	PixelType originalHSize=originalImage.rows;//原始图像的行
	PixelType originalVSize=originalImage.cols;//原始图像的列
	processedImage=Mat(originalHSize,originalVSize,CV_16U);
	for(int i=0;i<processedImage.rows;i++)
		for(int j=0;j<processedImage.cols;j++)
			processedImage.at<PixelType>(i,j)=0;

	for (PixelType i=0;i<originalHSize;++i)
	{
		for (PixelType j=0;j<originalVSize;++j)
		{
			if(originalImage.at<PixelType>(i,j) >= thresholdValue)
				processedImage.at<PixelType>(i,j) = (PixelType)setToValue;
			else
				processedImage.at<PixelType>(i,j) = originalImage.at<PixelType>(i,j);
		}
	}
	imwrite(".//tempFiles//thresholdProcessed.tif",processedImage);
	isProcessed=true;
	//showProcessedImage(luminescneceFileName);
	showImage(".//tempFiles//thresholdProcessed.tif");
	
}
void ImageThreshProcess::on_pushButton_quit_clicked()
{
	isSave=true;
	this->close();
}
void ImageThreshProcess::on_pushButton_reset_clicked()
{
	QFile::remove(".//tempFiles//thresholdProcessed.tif");
	QFile::remove(".//tempFiles//thresholdProcessed.tif");
	isSave=false;
	isProcessed=false;
	showImage(luminescneceFileName);
}