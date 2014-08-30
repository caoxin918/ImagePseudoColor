#ifndef IMAGEPSEUDOCOLOR_H
#define IMAGEPSEUDOCOLOR_H

#include <QtGui/QMainWindow>
#include "ui_imagepseudocolor.h"
#include "QImage"
#include "qthread.h"

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkTIFFImageIO.h"
#include "itkMedianImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkScalarToRGBColormapImageFilter.h"

typedef unsigned short PixelType;
const unsigned int Dimension=2;
typedef itk::Image<PixelType,Dimension> ImageType;
typedef itk::ImageFileReader<ImageType> ReaderType;
typedef itk::ImageFileWriter<ImageType> WriterType;//这里，输入以及输出的荧光图像格式，都是16bit tiff图像

typedef unsigned char UnChPixelType;//输入的白光图像，为8bit
typedef itk::Image<UnChPixelType,Dimension> UnChImageType;//白光图像的ITK定义

typedef itk::RGBPixel< unsigned char >        RGBPixelType;
typedef itk::Image< RGBPixelType, Dimension > RGBImageType;//伪彩色图像的ITK定义

typedef itk::ImageRegionConstIterator<ImageType> ConstIteratorType;
typedef itk::ImageRegionIterator<ImageType> IteratorType;//图像迭代器
typedef itk::TIFFImageIO TIFFIOType;//IO限定为tiff格式图像
typedef itk::MedianImageFilter<ImageType,ImageType> MedianFilterType;//图像滤波器

class FilterThread:public QThread
{
public:
	int kernelSize;
	void run();
	ImageType::Pointer readImage(QString filename);//read a image file using ITK，暂时没用的
	void writeImage(ImageType* outputImage, const char* filename);//write a image to disk using ITK，暂时没用到
	
private:
	ImageType::Pointer tempITKLuminesenceData;//临时用来处理荧光图像的图像数据，基于ITK
	TIFFIOType::Pointer tiffIO;//ITK读取以及存储数据的格式限定
	MedianFilterType::Pointer filter;
};


class ImagePseudoColor : public QMainWindow
{
	Q_OBJECT

public:
	ImagePseudoColor(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ImagePseudoColor();

private:
	Ui::ImagePseudoColorClass ui;
	FilterThread filterThread;

private slots:
	void on_pushButton_photograph_clicked();
	void on_pushButton_luminescence_clicked();
	void on_pushButton_substract_clicked();
	void on_pushButton_filter_clicked();
	void on_pushButton_fusion_clicked();
// 	void on_pushButton_save_clicked();
// 	void on_pushButton_clear_clicked();
// 	void on_pushButton_quit_clicked();
// 	void on_spinBoxValueChanged();

private:
	bool photographFlag;
	bool luminescenceFlag;
	bool substractFlag;
	bool filterFlag;
	bool fusionFlag; 
	PixelType substractValue;
	PixelType filterValue;
	PixelType highValueInLuminescenceImage;
	PixelType lowValueInLuminescenceImage;
	PixelType colorbarHighValue;
	PixelType colorbarLowValue;
	UnChImageType::Pointer tempITKPhotographData;//临时用来处理白光图像的图像数据，基于ITK
	ImageType::Pointer tempITKLuminesenceData;//临时用来处理荧光图像的图像数据，基于ITK
	TIFFIOType::Pointer tiffIO;//ITK读取以及存储数据的格式限定
	MedianFilterType::Pointer filter;


	QImage* photographImage;
	QImage* luminescneceImage;
	QImage* substractedImage;
	QImage* filteredImage;
	QImage* fusionImage;
	QImage *imgScaled;

private: 
	QString photographFileName;
	QString luminescneceFileName;

private:
	void initial();
	ImageType::Pointer readImage(QString filename);//read a image file using ITK，暂时没用的
	void writeImage(ImageType* outputImage, const char* filename);//write a image to disk using ITK，暂时没用到
	PixelType minusPixel(PixelType pixelValue,PixelType subValue){return pixelValue>subValue ? (pixelValue-subValue):0;}//没用到
	bool showLuminescenceData(QString fileName);
	bool showPhotographData(QString fileName);

};


#endif // IMAGEPSEUDOCOLOR_H
