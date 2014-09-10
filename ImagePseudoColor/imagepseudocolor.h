#ifndef IMAGEPSEUDOCOLOR_H
#define IMAGEPSEUDOCOLOR_H

#include <QtGui/QMainWindow>
#include "ui_imagepseudocolor.h"
#include "QImage"
#include "qthread.h"

#include "imagesoftwarebinningclass.h"

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkTIFFImageIO.h"
#include "itkPNGImageIO.h"
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
typedef itk::RescaleIntensityImageFilter<ImageType,ImageType> RescaleFilterType;
typedef itk::CastImageFilter<ImageType,UnChImageType> CastFilterType;
typedef itk::RGBPixel< unsigned char >        RGBPixelType;
typedef itk::Image< RGBPixelType, Dimension > RGBImageType;//α��ɫͼ���ITK����
typedef itk::ImageFileWriter<RGBImageType> RGBWriterType;//���8bit rgbͼ��
typedef itk::ImageFileReader<RGBImageType> RGBReaderType;
typedef itk::ImageFileWriter<UnChImageType> UnChWriterType;//���8bitͼ��
typedef itk::ImageFileReader<UnChImageType> UnChReaderType;
typedef itk::ImageRegionIterator<RGBImageType> RGBIteratortype;
typedef itk::ImageRegionIterator<UnChImageType> UnChIteratorType;
typedef itk::ScalarToRGBColormapImageFilter< UnChImageType, RGBImageType> RGBFilterType;
typedef itk::RGBAPixel<unsigned char> RGBAPixelType;//��������pngͼƬ�����ظ�ʽ
typedef itk::Image< RGBAPixelType, Dimension > RGBAImageType;
typedef itk::ImageRegionIterator<RGBAImageType> RGBAIteratortype;
typedef itk::ImageFileWriter<RGBAImageType> RGBAWriterType;

typedef itk::ImageRegionConstIterator<ImageType> ConstIteratorType;
typedef itk::ImageRegionIterator<ImageType> IteratorType;//ͼ�������
typedef itk::TIFFImageIO TIFFIOType;//IO�޶�Ϊtiff��ʽͼ��
typedef itk::PNGImageIO PNGIOType;
typedef itk::MedianImageFilter<ImageType,ImageType> MedianFilterType;//ͼ���˲���

class FilterThread:public QThread//ӫ��ͼ���˲��߳�
{
	Q_OBJECT
public:
	int kernelSize;
	void run();
	ImageType::Pointer readImage(QString filename);//read a image file using ITK
	void writeImage(ImageType* outputImage, const char* filename);//write a image to disk using ITK
	
signals:
	void done();
private:
	ImageType::Pointer tempITKLuminesenceData;//��ʱ��������ӫ��ͼ���ͼ�����ݣ�����ITK
	TIFFIOType::Pointer tiffIO;//ITK��ȡ�Լ��洢���ݵĸ�ʽ�޶�
	MedianFilterType::Pointer filter;
};
class PseudocolorThread:public QThread//ӫ��ͼ��α��ɫ�����߳�,ʹ��ITK
{
	Q_OBJECT
private:
	PixelType colorbarHighValue;
	PixelType colorbarLowValue;
	PixelType maxImageValue;
	PixelType minImageValue;
	ImageType::Pointer readImage(QString filename);
	void writeRGBImage(RGBImageType* outputImage,const char* filename);
	void sliceInputLuminescneceImage(ImageType::Pointer inputImage,PixelType HValue,PixelType LValue);//sliceInputLuminescneceImage()   �������ܣ��������16λӫ��ͼ����������colorbarӳ�䵽0-255,Ȼ����ڴ����ϣ�����luminescnece8BitImage.tif
	void copyImageData(ImageType::Pointer inputData,ImageType::Pointer outputData);
	UnChImageType::Pointer rescaleImage(ImageType::Pointer imageData,PixelType minValue,PixelType maxValue);//rescale the image to the value between assigned values
	void write8BitImage(UnChImageType* outputImage, const char* filename);
	void writeRGBAImage(RGBAImageType* outputImage,const char* filename);

	ImageType::Pointer inputImageData;
	TIFFIOType::Pointer tiffIO;
public:
	void setColorbarValues(PixelType low,PixelType high)
	{
		colorbarLowValue=low;
		colorbarHighValue=high;
	}
	void run();

signals:
	void done();
};


class ImagePseudoColor : public QMainWindow
{
	Q_OBJECT

public:
	ImagePseudoColor(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ImagePseudoColor();

private:
	Ui::ImagePseudoColorClass ui;
	FilterThread *filterThread;
	PseudocolorThread *pseudocolorThread;

	ImageSoftwareBinningclass *imageBinning;
	
	

private slots:
	void on_pushButton_photograph_clicked();
	void on_pushButton_luminescence_clicked();
	void on_pushButton_substract_clicked();
	void on_pushButton_filter_clicked();
	void on_pushButton_pseudocolor_clicked();
	void on_pushButton_fusion_clicked();
 	void on_pushButton_save_clicked();
 	void on_pushButton_clear_clicked();
 	void on_pushButton_quit_clicked();
	void receiveFilterSignal();
	void receivePseudocolorSignal();
	void on_menuHelp_clicked();

	void on_menuToolsImageBinning_clicked();
	void receiveBinningSignal(bool);
	void receiveNoBinningSignal();

private:
	bool photographFlag;
	bool luminescenceFlag;
	bool substractFlag;
	bool filterFlag;
	bool pseudocolorFlag;
	bool fusionFlag; 
	bool initialFlag;
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
	QImage* colorbarImage;

private: 
	QString photographFileName;
	QString luminescneceFileName;

private:
	void initial();
	ImageType::Pointer readImage(QString filename);//read a image file using ITK
	void writeImage(ImageType* outputImage, const char* filename);//write a image to disk using ITK
	RGBImageType::Pointer readRGBImage(QString filename);
	void writeRGBImage(RGBImageType* outputImage,const char* filename);


	PixelType minusPixel(PixelType pixelValue,PixelType subValue){return pixelValue>subValue ? (pixelValue-subValue):0;}//û�õ�
	bool showLuminescenceData(QString fileName);
	bool showPhotographData(QString fileName);
	void showFusionData(QString filename);
	void clearFusionWindow();

	bool copyDirectoryFiles(QString fromDir,QString toDir,bool cover);
	bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist);
	bool removeDirWithContent(const QString &dirName);
};


#endif // IMAGEPSEUDOCOLOR_H
