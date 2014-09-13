#ifndef IMAGESOFTWAREBINNINGCLASS_H
#define IMAGESOFTWAREBINNINGCLASS_H

#include <QWidget>
#include "ui_imagesoftwarebinningclass.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"

typedef unsigned short PixelType;
using namespace cv;

class ImageSoftwareBinningclass : public QWidget
{
	Q_OBJECT

public:
	ImageSoftwareBinningclass(QWidget *parent = 0);
	void setFileName(QString path){luminescneceFileName=path;}
	void showImage(QString path);
	void showOriginalImage(QString path);
	void saveBinningImage();
	void closeWindow();
	~ImageSoftwareBinningclass();

private slots:
	void on_pushButton_quit_clicked();
	void on_radioButton_checked();
	void on_pushButton_ok_clicked();


private:
	Ui::ImageSoftwareBinningclass ui;
	QString luminescneceFileName;
	Mat binningImage;
	Mat originalImage;
	int binningValue;
	int HBinningValue;
	int VBinningValue;
	bool binningCustom;
	int originalHSize;
	int originalVSize;
	int binningHSize;
	int binningVSize;
	bool saveBinningImg;
	bool binningProcessed;
	bool isResisedToOriginal;

	PixelType addImagePixel(int pixelLoacationI,int pixelLocationJ);
	void imageBinning();
	void imageResize();//图像双线性插值到原来的荧光图像大小

signals:
	void done(bool);
	void doneWithoutBinning();
};

#endif // IMAGESOFTWAREBINNINGCLASS_H
