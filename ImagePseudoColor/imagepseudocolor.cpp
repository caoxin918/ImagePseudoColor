#include "imagepseudocolor.h"
#include "QFileDialog"
#include "QMessageBox"
#include "qpixmap.h"
#include <direct.h>
#include <stdio.h>
#include <io.h>
#include <iostream>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;



using namespace std;
ImagePseudoColor::ImagePseudoColor(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	QObject::connect(ui.pushButtonPhotograph,SIGNAL(clicked()),this,SLOT(on_pushButton_photograph_clicked()));
	QObject::connect(ui.pushButtonLuminescence,SIGNAL(clicked()),this,SLOT(on_pushButton_luminescence_clicked()));
	QObject::connect(ui.pushButtonSubstract,SIGNAL(clicked()),this,SLOT(on_pushButton_substract_clicked()));
	QObject::connect(ui.pushButtonFilter,SIGNAL(clicked()),this,SLOT(on_pushButton_filter_clicked()));
	QObject::connect(ui.pushButtonFusion,SIGNAL(clicked()),this,SLOT(on_pushButton_fusion_clicked()));


	initial();
}

ImagePseudoColor::~ImagePseudoColor()
{

}

void ImagePseudoColor::initial()
{
	photographFlag=false;
	luminescenceFlag=false;
	substractFlag=false;
	filterFlag=false;
	fusionFlag=false;

	photographImage=NULL;
	luminescneceImage=NULL;
	substractedImage=NULL;
	filteredImage=NULL;
	fusionImage=NULL;
	imgScaled=NULL;

	tempITKLuminesenceData=NULL;
	tempITKPhotographData=NULL;
	tiffIO=TIFFIOType::New();

	filter=MedianFilterType::New();

	photographFileName="";
	luminescneceFileName="";

	QRegExp regx("[0-9]+$");
	QValidator *validator1 = new QRegExpValidator(regx,ui.SubstractLineEdit);
	ui.SubstractLineEdit->setValidator(validator1);
	QValidator *validator2 = new QRegExpValidator(regx,ui.FilterLineEdit);
	ui.FilterLineEdit->setValidator(validator2);
	if(_access(".//tempFiles",0)==-1)
	{
		_mkdir(".//tempFiles");
	}
}

void ImagePseudoColor::on_pushButton_photograph_clicked()
{
	QString path=QFileDialog::getOpenFileName(this,"Open photograph",".","tiff Files(*.tif)");
	if (path.length()==0)
	{
		QMessageBox::information(NULL,"Warning","You didn't select any files.");
		if(photographFlag==true)
		{
			photographFlag=false;
			ui.photographLabel->setText("photograph");
			delete photographImage;
			photographFileName="";
		}
	}
	else
	{
		photographFlag=true;
		if(!showPhotographData(path))
		{
			delete photographImage;
			photographFlag=false;
			return;
		}
		photographFileName=path;//将白光图像的完整路径复制给photographFileName变量
	}	
}

void ImagePseudoColor::on_pushButton_luminescence_clicked()
{
	QString path=QFileDialog::getOpenFileName(this,"Open luminescence",".","tiff Files(*.tif)");
	if (path.length()==0)
	{
		QMessageBox::information(NULL,"Warning","You didn't select any files.");
		if(luminescenceFlag==true)
		{
			luminescenceFlag=false;
			ui.luminescenceLabel->setText("luminescence");
			delete luminescneceImage;
			luminescneceFileName="";
		}
	}
	else
	{
		luminescenceFlag=true;
		if(!showLuminescenceData(path))
		{
			delete luminescneceImage;
			luminescenceFlag=false;
			return;
		}
		luminescneceFileName=path;
	}	
}

void ImagePseudoColor::on_pushButton_substract_clicked()
{
	unsigned long temp;
	temp=ui.SubstractLineEdit->text().toULong();
	if(!luminescenceFlag)
	{
		QMessageBox::information(NULL,"Warning","You didn't select a luminescence image.");
		substractFlag=false;
		filterFlag=false;
		fusionFlag=false;
		return;
	}
	if(temp>65535)
	{
		QMessageBox::information(NULL,"Warning","The input must be smaller than 65535.");
		substractFlag=false;
		filterFlag=false;
		fusionFlag=false;
		return;
	}
	substractValue=temp;
	string temp1=luminescneceFileName.toStdString();
	const char* temp2=temp1.c_str();
	Mat inputImageData=imread(temp1,CV_LOAD_IMAGE_ANYCOLOR|CV_LOAD_IMAGE_ANYDEPTH);
	PixelType *temppp= inputImageData.ptr<PixelType>(1);
	Mat outputImageData=inputImageData-substractValue;
	//cvSub(inputImageData,);
	//medianBlur(inputImageData,outputImageData,substractValue);
	imwrite(".//tempFiles//substractData.tif",outputImageData);
	showLuminescenceData(".//tempFiles//substractData.tif");
	substractFlag=true;

// 	substractValue=temp;//assign the substract value
// 	tempITKLuminesenceData=NULL;
// 	tempITKLuminesenceData=ImageType::New();
// 	tempITKLuminesenceData=readImage(luminescneceFileName);
// 	IteratorType It(tempITKLuminesenceData,tempITKLuminesenceData->GetRequestedRegion());
// 	PixelType temp2=0;
// 	while(!It.IsAtEnd())
// 	{
// 		temp2=(minusPixel(It.Get(),substractValue));
// 		It.Set(temp2);
// 		++It;
// 	}
// 	writeImage(tempITKLuminesenceData,".//tempFiles//substractData.tif");
// 	showLuminescenceData(".//tempFiles//substractData.tif");
// 	substractFlag=true;
}

void ImagePseudoColor::on_pushButton_filter_clicked()
{
	unsigned long temp;
	temp=ui.FilterLineEdit->text().toULong();
	if(!substractFlag)
	{
		QMessageBox::information(NULL,"Warning","Please substract the background first.");
		filterFlag=false;;
		fusionFlag=false;
		return;
	}
	if(temp<1 || temp>20)
	{
		QMessageBox::information(NULL,"Warning","The input must be larger than 0 and smaller than 20.");
		filterFlag=false;
		fusionFlag=false;
		return;
	}
	filterValue=temp;
	tempITKLuminesenceData=NULL;
	tempITKLuminesenceData=ImageType::New();
	tempITKLuminesenceData=readImage(".//tempFiles//substractData.tif");
	ImageType::SizeType filterRadius;
	filterRadius[0]=filterRadius[1]=filterValue;
	filter->SetInput(tempITKLuminesenceData);
	filter->SetRadius(filterRadius);
	filter->Update();
	writeImage(filter->GetOutput(),".//tempFiles//filteredData.tif");
	showLuminescenceData(".//tempFiles//filteredData.tif");
}

void ImagePseudoColor::on_pushButton_fusion_clicked()
{
	;
}

bool ImagePseudoColor::showPhotographData(QString fileName)
{
	photographImage=new QImage;
	if(!photographImage->load(fileName))
	{
		QMessageBox::information(this,tr("Open image error"),tr("Open image error!"));
		return false;
	}
	imgScaled=new QImage;
	*imgScaled=photographImage->scaled(ui.photographLabel->width(),ui.photographLabel->height(),Qt::KeepAspectRatio);
	ui.photographLabel->setPixmap(QPixmap::fromImage(*imgScaled));
	return true;
}
bool ImagePseudoColor::showLuminescenceData(QString fileName)
{
	luminescneceImage=new QImage;
	if(!luminescneceImage->load(fileName))
	{
		QMessageBox::information(this,tr("Open image error"),tr("Open image error!"));
		return false;
	}
	imgScaled=new QImage;
	*imgScaled=luminescneceImage->scaled(ui.luminescenceLabel->width(),ui.luminescenceLabel->height(),Qt::KeepAspectRatio);
	ui.luminescenceLabel->setPixmap(QPixmap::fromImage(*imgScaled));
	return true;
}
ImageType::Pointer ImagePseudoColor::readImage(QString filename)
{
	string temp1=filename.toStdString();
	const char* temp2=temp1.c_str();
	ReaderType::Pointer reader=ReaderType::New();
	reader->SetFileName(temp2);
	reader->SetImageIO(tiffIO);
	reader->Update();
	return reader->GetOutput();
}

void ImagePseudoColor::writeImage(ImageType* outputImage, const char* filename)
{
	WriterType::Pointer writer=WriterType::New();
	writer->SetFileName(filename);
	writer->SetImageIO(tiffIO);
	writer->SetInput(outputImage);
	writer->Update();
}