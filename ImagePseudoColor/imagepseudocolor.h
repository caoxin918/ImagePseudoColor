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
typedef itk::ImageFileWriter<ImageType> WriterType;//��������Լ������ӫ��ͼ���ʽ������16bit tiffͼ��

typedef unsigned char UnChPixelType;//����İ׹�ͼ��Ϊ8bit
typedef itk::Image<UnChPixelType,Dimension> UnChImageType;//�׹�ͼ���ITK����

typedef itk::RGBPixel< unsigned char >        RGBPixelType;
typedef itk::Image< RGBPixelType, Dimension > RGBImageType;//α��ɫͼ���ITK����

typedef itk::ImageRegionConstIterator<ImageType> ConstIteratorType;
typedef itk::ImageRegionIterator<ImageType> IteratorType;//ͼ�������
typedef itk::TIFFImageIO TIFFIOType;//IO�޶�Ϊtiff��ʽͼ��
typedef itk::MedianImageFilter<ImageType,ImageType> MedianFilterType;//ͼ���˲���

class FilterThread:public QThread
{
public:
	int kernelSize;
	void run();
	ImageType::Pointer readImage(QString filename);//read a image file using ITK����ʱû�õ�
	void writeImage(ImageType* outputImage, const char* filename);//write a image to disk using ITK����ʱû�õ�
	
private:
	ImageType::Pointer tempITKLuminesenceData;//��ʱ��������ӫ��ͼ���ͼ�����ݣ�����ITK
	TIFFIOType::Pointer tiffIO;//ITK��ȡ�Լ��洢���ݵĸ�ʽ�޶�
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
	UnChImageType::Pointer tempITKPhotographData;//��ʱ��������׹�ͼ���ͼ�����ݣ�����ITK
	ImageType::Pointer tempITKLuminesenceData;//��ʱ��������ӫ��ͼ���ͼ�����ݣ�����ITK
	TIFFIOType::Pointer tiffIO;//ITK��ȡ�Լ��洢���ݵĸ�ʽ�޶�
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
	ImageType::Pointer readImage(QString filename);//read a image file using ITK����ʱû�õ�
	void writeImage(ImageType* outputImage, const char* filename);//write a image to disk using ITK����ʱû�õ�
	PixelType minusPixel(PixelType pixelValue,PixelType subValue){return pixelValue>subValue ? (pixelValue-subValue):0;}//û�õ�
	bool showLuminescenceData(QString fileName);
	bool showPhotographData(QString fileName);

};


#endif // IMAGEPSEUDOCOLOR_H
