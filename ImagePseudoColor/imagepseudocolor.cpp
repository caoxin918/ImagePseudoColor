#include "imagepseudocolor.h"
#include "QFileDialog"
#include "QMessageBox"
#include "qfile.h"
#include "qpixmap.h"
#include <direct.h>
#include <stdio.h>
#include <io.h>
#include <iostream>


#include <vector>
#include <algorithm>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;

ImageType::Pointer FilterThread::readImage(QString filename)
{
	string temp1=filename.toStdString();
	const char* temp2=temp1.c_str();
	ReaderType::Pointer reader=ReaderType::New();
	reader->SetFileName(temp2);
	reader->SetImageIO(tiffIO);
	reader->Update();
	return reader->GetOutput();
}

void FilterThread::writeImage(ImageType* outputImage, const char* filename)
{
	WriterType::Pointer writer=WriterType::New();
	writer->SetFileName(filename);
	writer->SetImageIO(tiffIO);
	writer->SetInput(outputImage);
	writer->Update();
}
void FilterThread::run()
{	
	//以下是利用ITK来进行中值滤波的代码
	if(kernelSize==0)
	{
		Mat tempLuminesenceData=imread(".//tempFiles//substractData.tif",CV_LOAD_IMAGE_ANYCOLOR|CV_LOAD_IMAGE_ANYDEPTH);
		imwrite(".//tempFiles//filterData.tif",tempLuminesenceData);
	}
	else
	{
		tempITKLuminesenceData=ImageType::New();
		tiffIO=TIFFIOType::New();
		tempITKLuminesenceData=readImage(".//tempFiles//substractData.tif");
		ImageType::SizeType filterRadius;
		filterRadius[0]=filterRadius[1]=kernelSize;
		filter=MedianFilterType::New();
		filter->SetInput(tempITKLuminesenceData);
		filter->SetRadius(filterRadius);
		filter->Update();
		writeImage(filter->GetOutput(),".//tempFiles//filterData.tif");
	}
	emit done();
}

ImageType::Pointer PseudocolorThread::readImage(QString filename)
{
	string temp1=filename.toStdString();
	const char* temp2=temp1.c_str();
	ReaderType::Pointer reader=ReaderType::New();
	reader->SetFileName(temp2);
	reader->SetImageIO(tiffIO);
	reader->Update();
	return reader->GetOutput();
}
void PseudocolorThread::copyImageData(ImageType::Pointer inputData,ImageType::Pointer outputData)
{
	outputData->SetRegions(inputData->GetRequestedRegion());
	outputData->CopyInformation(inputData);
	outputData->Allocate();

	IteratorType outputIt(outputData,outputData->GetRequestedRegion());
	ConstIteratorType inputIt(inputData,inputData->GetRequestedRegion());
	while(!inputIt.IsAtEnd())
	{
		outputIt.Set(inputIt.Get());
		++inputIt;
		++outputIt;
	}
}
UnChImageType::Pointer PseudocolorThread::rescaleImage(ImageType::Pointer imageData,PixelType minValue,PixelType maxValue)
{
	RescaleFilterType::Pointer rescaleFilter=RescaleFilterType::New();
	rescaleFilter->SetInput(imageData);
	rescaleFilter->SetOutputMinimum(minValue);
	rescaleFilter->SetOutputMaximum(maxValue);
	rescaleFilter->Update();
	CastFilterType::Pointer castFilter=CastFilterType::New();
	castFilter->SetInput(rescaleFilter->GetOutput());
	castFilter->Update();
	return castFilter->GetOutput();
}
void PseudocolorThread::writeRGBImage(RGBImageType* outputImage,const char* filename)
{
	RGBWriterType::Pointer writer=RGBWriterType::New();
	writer->SetFileName(filename);
	writer->SetImageIO(tiffIO);
	writer->SetInput(outputImage);
	writer->Update();
}
void PseudocolorThread::sliceInputLuminescneceImage(ImageType::Pointer inputImage,PixelType HValue,PixelType LValue)
{
	UnChImageType::Pointer sliced8BitImageData=UnChImageType::New();
	/******************************************************
下面的一段是将荧光图像映射到0-255区间内，得到0-255的灰度图
这里记录下基本原理，以防遗忘：
这里的inputimage是指上一步，也就是滤波处理后生成的图像数据；
首先，自定义一个临时图像数据，tempImage,最后会在这个的基础上
生成8位的灰度图；
然后的while循环有两个目的，1是找到图像中的最大和最小像素点的数值，2是将图像的所有像素点的数值约束在给定的colorbar数值范围内，这里是指当图像的最大值大于colorbar上限，或图像最小值低于colorbar下限的情况
最后，将每个像素点映射到0-255区间中的一个点，该数值是原始图像中像素点之余给点的colorbar，归一到0-255区间后的数值
注意：为了防止设置的colorbar上下区间太大，从而致使每个颜色区间的数值太高，进而致使低像素点被归类到0这一数值，有必要进行相关的矫正（在最后）
	*****************************************************/
	ImageType::Pointer tempImage=ImageType::New();
	tempImage->CopyInformation(inputImage);
	copyImageData(inputImage,tempImage);
	IteratorType tempIt(tempImage,tempImage->GetRequestedRegion());
	PixelType minImageValue,maxImageValue;
	minImageValue=65535;
	maxImageValue=0;
	while(!tempIt.IsAtEnd())
	{
		if(tempIt.Get()>=HValue)
			tempIt.Set(HValue);
		if(tempIt.Get()<=LValue)
			tempIt.Set(LValue);
		if(tempIt.Get()>maxImageValue)
			maxImageValue=tempIt.Get();
		if(tempIt.Get()<minImageValue)
			minImageValue=tempIt.Get();
		++tempIt;
	}
	int sliceMin,sliceMax;
	sliceMax=(int)(((float)maxImageValue-LValue)/((float)HValue-LValue)*255);
	sliceMin=(int)(((float)minImageValue-LValue)/((float)HValue-LValue)*255);
	if(sliceMin==0)
		sliceMin=1;//方便进行接下来的矫正，为了避免并不小于lvalue的像素点被划分为0层的情况
	if(sliceMax<2)
		sliceMax=2;
	sliced8BitImageData=rescaleImage(tempImage,sliceMin,sliceMax);
	UnChIteratorType sliceIt(sliced8BitImageData,sliced8BitImageData->GetRequestedRegion());
	ConstIteratorType inputIt(inputImage,inputImage->GetRequestedRegion());
	while(!inputIt.IsAtEnd())
	{
		if(inputIt.Get()<LValue)
		{
			sliceIt.Set(0);
		}
		if(inputIt.Get()>HValue)
		{
			sliceIt.Set(255);
		}
		++inputIt;
		++sliceIt;
	}
	write8BitImage(sliced8BitImageData,".//tempFiles//luminescnece8BitImage.tif");
}
void PseudocolorThread::write8BitImage(UnChImageType* outputImage, const char* filename)
{
	UnChWriterType::Pointer writer=UnChWriterType::New();
	writer->SetFileName(filename);
	//TIFFIOType *tiffIO=TIFFIOType::New();
	writer->SetImageIO(tiffIO);
	writer->SetInput(outputImage);
	writer->Update();
}
void PseudocolorThread::writeRGBAImage(RGBAImageType* outputImage,const char* filename)
{
	RGBAWriterType::Pointer writer=RGBAWriterType::New();
	writer->SetFileName(filename);
	writer->SetImageIO(PNGIOType::New());
	writer->SetInput(outputImage);
	writer->Update();
}
void PseudocolorThread::run()
{
	inputImageData=ImageType::New();
	tiffIO=TIFFIOType::New();
	inputImageData=readImage(".//tempFiles//filterData.tif");
	sliceInputLuminescneceImage(inputImageData,colorbarHighValue,colorbarLowValue);//将荧光图片做成0-255范围的灰度图，保存为luminescnece8BitImage.tif
	UnChImageType* unCharImageData=UnChImageType::New();
	UnChReaderType::Pointer UnChReader=UnChReaderType::New();
	UnChReader->SetFileName(".//tempFiles//luminescnece8BitImage.tif");
	UnChReader->SetImageIO(tiffIO);
	UnChReader->Update();
	unCharImageData=UnChReader->GetOutput();
	RGBFilterType::Pointer rgbfilter = RGBFilterType::New();
	rgbfilter->SetInput(unCharImageData);
	rgbfilter->SetColormap( RGBFilterType::Jet );
	rgbfilter->Update();
	RGBImageType* rgbImageData=RGBImageType::New();
	rgbImageData=rgbfilter->GetOutput();
	UnChIteratorType It(unCharImageData,unCharImageData->GetRequestedRegion());
	RGBIteratortype RGBIt(rgbImageData,rgbImageData->GetRequestedRegion());
	//初始化colormap数据
	float colorMapJetData[255][3]=
	{
		0,0,0.5156,
		0,0,0.5313,
		0,0,0.5469,
		0,0,0.5625,
		0,0,0.5781,
		0,0,0.5938,
		0,0,0.6094,
		0,0,0.6250,
		0,0,0.6406,
		0,0,0.6563,
		0,0,0.6719,
		0,0,0.6875,
		0,0,0.7031,
		0,0,0.7188,
		0,0,0.7344,
		0,0,0.7500,
		0,0,0.7656,
		0,0,0.7813,
		0,0,0.7969,
		0,0,0.8125,
		0,0,0.8281,
		0,0,0.8438,
		0,0,0.8594,
		0,0,0.8750,
		0,0,0.8906,
		0,0,0.9063,
		0,0,0.9219,
		0,0,0.9375,
		0,0,0.9531,
		0,0,0.9688,
		0,0,0.9844,
		0,0,1.0000,
		0,0.0156,1.0000,
		0,0.0313,1.0000,
		0,0.0469,1.0000,
		0,0.0625,1.0000,
		0,0.0781,1.0000,
		0,0.0938,1.0000,
		0,0.1094,1.0000,
		0,0.1250,1.0000,
		0,0.1406,1.0000,
		0,0.1563,1.0000,
		0,0.1719,1.0000,
		0,0.1875,1.0000,
		0,0.2031,1.0000,
		0,0.2188,1.0000,
		0,0.2344,1.0000,
		0,0.2500,1.0000,
		0,0.2656,1.0000,
		0,0.2813,1.0000,
		0,0.2969,1.0000,
		0,0.3125,1.0000,
		0,0.3281,1.0000,
		0,0.3438,1.0000,
		0,0.3594,1.0000,
		0,0.3750,1.0000,
		0,0.3906,1.0000,
		0,0.4063,1.0000,
		0,0.4219,1.0000,
		0,0.4375,1.0000,
		0,0.4531,1.0000,
		0,0.4688,1.0000,
		0,0.4844,1.0000,
		0,0.5000,1.0000,
		0,0.5156,1.0000,
		0,0.5313,1.0000,
		0,0.5469,1.0000,
		0,0.5625,1.0000,
		0,0.5781,1.0000,
		0,0.5938,1.0000,
		0,0.6094,1.0000,
		0,0.6250,1.0000,
		0,0.6406,1.0000,
		0,0.6563,1.0000,
		0,0.6719,1.0000,
		0,0.6875,1.0000,
		0,0.7031,1.0000,
		0,0.7188,1.0000,
		0,0.7344,1.0000,
		0,0.7500,1.0000,
		0,0.7656,1.0000,
		0,0.7813,1.0000,
		0,0.7969,1.0000,
		0,0.8125,1.0000,
		0,0.8281,1.0000,
		0,0.8438,1.0000,
		0,0.8594,1.0000,
		0,0.8750,1.0000,
		0,0.8906,1.0000,
		0,0.9063,1.0000,
		0,0.9219,1.0000,
		0,0.9375,1.0000,
		0,0.9531,1.0000,
		0,0.9688,1.0000,
		0,0.9844,1.0000,
		0,1.0000,1.0000,
		0.0156,1.0000,0.9844,
		0.0313,1.0000,0.9688,
		0.0469,1.0000,0.9531,
		0.0625,1.0000,0.9375,
		0.0781,1.0000,0.9219,
		0.0938,1.0000,0.9063,
		0.1094,1.0000,0.8906,
		0.1250,1.0000,0.8750,
		0.1406,1.0000,0.8594,
		0.1563,1.0000,0.8438,
		0.1719,1.0000,0.8281,
		0.1875,1.0000,0.8125,
		0.2031,1.0000,0.7969,
		0.2188,1.0000,0.7813,
		0.2344,1.0000,0.7656,
		0.2500,1.0000,0.7500,
		0.2656,1.0000,0.7344,
		0.2813,1.0000,0.7188,
		0.2969,1.0000,0.7031,
		0.3125,1.0000,0.6875,
		0.3281,1.0000,0.6719,
		0.3438,1.0000,0.6563,
		0.3594,1.0000,0.6406,
		0.3750,1.0000,0.6250,
		0.3906,1.0000,0.6094,
		0.4063,1.0000,0.5938,
		0.4219,1.0000,0.5781,
		0.4375,1.0000,0.5625,
		0.4531,1.0000,0.5469,
		0.4688,1.0000,0.5313,
		0.4844,1.0000,0.5156,
		0.5000,1.0000,0.5000,
		0.5156,1.0000,0.4844,
		0.5313,1.0000,0.4688,
		0.5469,1.0000,0.4531,
		0.5625,1.0000,0.4375,
		0.5781,1.0000,0.4219,
		0.5938,1.0000,0.4063,
		0.6094,1.0000,0.3906,
		0.6250,1.0000,0.3750,
		0.6406,1.0000,0.3594,
		0.6563,1.0000,0.3438,
		0.6719,1.0000,0.3281,
		0.6875,1.0000,0.3125,
		0.7031,1.0000,0.2969,
		0.7188,1.0000,0.2813,
		0.7344,1.0000,0.2656,
		0.7500,1.0000,0.2500,
		0.7656,1.0000,0.2344,
		0.7813,1.0000,0.2188,
		0.7969,1.0000,0.2031,
		0.8125,1.0000,0.1875,
		0.8281,1.0000,0.1719,
		0.8438,1.0000,0.1563,
		0.8594,1.0000,0.1406,
		0.8750,1.0000,0.1250,
		0.8906,1.0000,0.1094,
		0.9063,1.0000,0.0938,
		0.9219,1.0000,0.0781,
		0.9375,1.0000,0.0625,
		0.9531,1.0000,0.0469,
		0.9688,1.0000,0.0313,
		0.9844,1.0000,0.0156,
		1.0000,1.0000,0,
		1.0000,0.9844,0,
		1.0000,0.9688,0,
		1.0000,0.9531,0,
		1.0000,0.9375,0,
		1.0000,0.9219,0,
		1.0000,0.9063,0,
		1.0000,0.8906,0,
		1.0000,0.8750,0,
		1.0000,0.8594,0,
		1.0000,0.8438,0,
		1.0000,0.8281,0,
		1.0000,0.8125,0,
		1.0000,0.7969,0,
		1.0000,0.7813,0,
		1.0000,0.7656,0,
		1.0000,0.7500,0,
		1.0000,0.7344,0,
		1.0000,0.7188,0,
		1.0000,0.7031,0,
		1.0000,0.6875,0,
		1.0000,0.6719,0,
		1.0000,0.6563,0,
		1.0000,0.6406,0,
		1.0000,0.6250,0,
		1.0000,0.6094,0,
		1.0000,0.5938,0,
		1.0000,0.5781,0,
		1.0000,0.5625,0,
		1.0000,0.5469,0,
		1.0000,0.5313,0,
		1.0000,0.5156,0,
		1.0000,0.5000,0,
		1.0000,0.4844,0,
		1.0000,0.4688,0,
		1.0000,0.4531,0,
		1.0000,0.4375,0,
		1.0000,0.4219,0,
		1.0000,0.4063,0,
		1.0000,0.3906,0,
		1.0000,0.3750,0,
		1.0000,0.3594,0,
		1.0000,0.3438,0,
		1.0000,0.3281,0,
		1.0000,0.3125,0,
		1.0000,0.2969,0,
		1.0000,0.2813,0,
		1.0000,0.2656,0,
		1.0000,0.2500,0,
		1.0000,0.2344,0,
		1.0000,0.2188,0,
		1.0000,0.2031,0,
		1.0000,0.1875,0,
		1.0000,0.1719,0,
		1.0000,0.1563,0,
		1.0000,0.1406,0,
		1.0000,0.1250,0,
		1.0000,0.1094,0,
		1.0000,0.0938,0,
		1.0000,0.0781,0,
		1.0000,0.0625,0,
		1.0000,0.0469,0,
		1.0000,0.0313,0,
		1.0000,0.0156,0,
		1.0000,0,0,
		0.9844,0,0,
		0.9688,0,0,
		0.9531,0,0,
		0.9375,0,0,
		0.9219,0,0,
		0.9063,0,0,
		0.8906,0,0,
		0.8750,0,0,
		0.8594,0,0,
		0.8438,0,0,
		0.8281,0,0,
		0.8125,0,0,
		0.7969,0,0,
		0.7813,0,0,
		0.7656,0,0,
		0.7500,0,0,
		0.7344,0,0,
		0.7188,0,0,
		0.7031,0,0,
		0.6875,0,0,
		0.6719,0,0,
		0.6563,0,0,
		0.6406,0,0,
		0.6250,0,0,
		0.6094,0,0,
		0.5938,0,0,
		0.5781,0,0,
		0.5625,0,0,
		0.5469,0,0,
		0.5313,0,0,
		0.5156,0,0
	};
	RGBPixelType rgbPixelTemp;
	UnChPixelType uhPixelTemp;
	RGBAImageType::Pointer rgbaImageData=RGBAImageType::New();
	RGBAImageType::IndexType start;
	start[0]=0;
	start[1]=0;
	RGBAImageType::SizeType size;
	size=unCharImageData->GetRequestedRegion().GetSize();
	RGBAImageType::RegionType region;
	region.SetSize(size);
	region.SetIndex(start);
	rgbaImageData->SetRegions(region);
	rgbaImageData->Allocate();
	RGBAPixelType rgbaPixelTemp;
	RGBAIteratortype RGBAIt(rgbaImageData,rgbaImageData->GetRequestedRegion());

	while(!It.IsAtEnd())
	{
		uhPixelTemp=It.Get();
		if(uhPixelTemp>0)
		{
			rgbPixelTemp.SetRed((UnChPixelType)(colorMapJetData[uhPixelTemp-1][0] * 255.0));
			rgbPixelTemp.SetGreen((UnChPixelType)(colorMapJetData[uhPixelTemp-1][1] * 255.0));
			rgbPixelTemp.SetBlue((UnChPixelType)(colorMapJetData[uhPixelTemp-1][2] * 255.0));
			rgbaPixelTemp.SetRed((UnChPixelType)(colorMapJetData[uhPixelTemp-1][0] * 255.0));
			rgbaPixelTemp.SetGreen((UnChPixelType)(colorMapJetData[uhPixelTemp-1][1] * 255.0));
			rgbaPixelTemp.SetBlue((UnChPixelType)(colorMapJetData[uhPixelTemp-1][2] * 255.0));
			rgbaPixelTemp.SetAlpha(255);
		}
		else
		{
			rgbPixelTemp.SetRed((UnChPixelType)(colorMapJetData[0][0] * 255.0));
			rgbPixelTemp.SetGreen((UnChPixelType)(colorMapJetData[0][1] * 255.0));
			rgbPixelTemp.SetBlue((UnChPixelType)(colorMapJetData[0][2] * 255.0));
			rgbaPixelTemp.SetRed((UnChPixelType)(colorMapJetData[0][0] * 255.0));
			rgbaPixelTemp.SetGreen((UnChPixelType)(colorMapJetData[0][1] * 255.0));
			rgbaPixelTemp.SetBlue((UnChPixelType)(colorMapJetData[0][2] * 255.0));
			rgbaPixelTemp.SetAlpha(0);
		}
		RGBIt.Set(rgbPixelTemp);
		RGBAIt.Set(rgbaPixelTemp);
		++It;
		++RGBIt;
		++RGBAIt;
	}
	writeRGBImage(rgbImageData,".//tempFiles//pseudocolorData.tif");
	writeRGBAImage(rgbaImageData,".//tempFiles//pseudocolorData.png");
	emit done();
}


ImagePseudoColor::ImagePseudoColor(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	QObject::connect(ui.pushButtonPhotograph,SIGNAL(clicked()),this,SLOT(on_pushButton_photograph_clicked()));
	QObject::connect(ui.pushButtonLuminescence,SIGNAL(clicked()),this,SLOT(on_pushButton_luminescence_clicked()));
	QObject::connect(ui.pushButtonSubstract,SIGNAL(clicked()),this,SLOT(on_pushButton_substract_clicked()));
	QObject::connect(ui.pushButtonFilter,SIGNAL(clicked()),this,SLOT(on_pushButton_filter_clicked()));
	QObject::connect(ui.pushButtonFusion,SIGNAL(clicked()),this,SLOT(on_pushButton_fusion_clicked()));
	QObject::connect(ui.pushButtonPseudoColor,SIGNAL(clicked()),this,SLOT(on_pushButton_pseudocolor_clicked()));
	QObject::connect(ui.pushButtonSave,SIGNAL(clicked()),this,SLOT(on_pushButton_save_clicked()));
	QObject::connect(ui.pushButtonClear,SIGNAL(clicked()),this,SLOT(on_pushButton_clear_clicked()));
	QObject::connect(ui.pushButtonQuit,SIGNAL(clicked()),this,SLOT(on_pushButton_quit_clicked()));
	QObject::connect(ui.actionAbout,SIGNAL(triggered()),this,SLOT(on_menuHelp_clicked()));
	initial();
}

ImagePseudoColor::~ImagePseudoColor()
{
	if(!removeDirWithContent(".//tempFiles"))
	{
		QMessageBox::information(NULL,"Warning","Delete tempfiles failed.");
	}
}
void ImagePseudoColor::initial()
{
	photographFlag=false;
	luminescenceFlag=false;
	substractFlag=false;
	filterFlag=false;
	pseudocolorFlag=false;
	fusionFlag=false;
	initialFlag=false;

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


	filterThread=new FilterThread;
	pseudocolorThread=new PseudocolorThread;
	connect(filterThread,SIGNAL(done()),this,SLOT(receiveFilterSignal()));
	connect(pseudocolorThread,SIGNAL(done()),this,SLOT(receivePseudocolorSignal()));

	QRegExp regx("[0-9]+$");
	QValidator *validator1 = new QRegExpValidator(regx,ui.SubstractLineEdit);
	ui.SubstractLineEdit->setValidator(validator1);
	QValidator *validator2 = new QRegExpValidator(regx,ui.FilterLineEdit);
	ui.FilterLineEdit->setValidator(validator2);
	QDir targetDir(".//tempFiles");//创建临时文件夹
	if(!targetDir.exists())
	{
		initialFlag=false;
		if(!targetDir.mkdir(targetDir.absolutePath()))
		{
			QMessageBox::information(NULL,"Warning","Cannot create the temp file.");
			initialFlag=false;
		}
		else
		{
			initialFlag=true;
		}
	}
	else
	{
		initialFlag=true;
	}
	{
		colorbarImage=new QImage;
		colorbarImage->load("JetColormapFunction.png");
		imgScaled=new QImage;
		*imgScaled=colorbarImage->scaled(ui.colorbarImageLabel->width(),ui.colorbarImageLabel->height());
		ui.colorbarImageLabel->setPixmap(QPixmap::fromImage(*imgScaled));
		ui.clorbarHighLabel->setText("");
		ui.colorbarLowLabel->setText("");
	}

}

void ImagePseudoColor::on_pushButton_photograph_clicked()
{
	if(!initialFlag)
	{
		QMessageBox::information(NULL,"Warning","Initial failed.");
		return;
	}
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
		Mat tempData=imread(string((const char *)path.toLocal8Bit()),CV_LOAD_IMAGE_ANYCOLOR|CV_LOAD_IMAGE_ANYDEPTH);
		if(tempData.channels()!=1)
		{
			photographFlag=false;
			QMessageBox::information(NULL,"Warning","The channel of input image must be 1.");
			return;
		}
		photographFlag=true;
		if(!showPhotographData(path))
		{
			delete photographImage;
			photographFlag=false;
			return;
		}
		photographFileName=path;//将白光图像的完整路径复制给photographFileName变量
		ui.PhotographLineEdit->setText(path);
	}	
}

void ImagePseudoColor::on_pushButton_luminescence_clicked()
{
	if(!initialFlag)
	{
		QMessageBox::information(NULL,"Warning","Initial failed.");
		return;
	}
	clearFusionWindow();
	QString path=QFileDialog::getOpenFileName(this,"Open luminescence",".","tiff Files(*.tif)");
	if (path.length()==0)
	{
		QMessageBox::information(NULL,"Warning","You didn't select any files.");
		if(luminescenceFlag==true)
		{
			luminescenceFlag=false;
			substractFlag=false;
			filterFlag=false;
			pseudocolorFlag=false;
			fusionFlag=false;
			ui.luminescenceLabel->setText("luminescence");
			delete luminescneceImage;
			luminescneceFileName="";
		}
	}
	else
	{
		Mat tempData=imread(string((const char *)path.toLocal8Bit()),CV_LOAD_IMAGE_ANYCOLOR|CV_LOAD_IMAGE_ANYDEPTH);
		if(tempData.channels()!=1)
		{
			photographFlag=false;
			QMessageBox::information(NULL,"Warning","The channel of input image must be 1.");
			return;
		}
		luminescenceFlag=true;
		if(!showLuminescenceData(path))
		{
			delete luminescneceImage;
			luminescenceFlag=false;
			pseudocolorFlag=false;
			substractFlag=false;
			filterFlag=false;
			fusionFlag=false;
			return;
		}
		substractFlag=false;
		filterFlag=false;
		pseudocolorFlag=false;
		fusionFlag=false;
		luminescneceFileName=path;
		ui.LuminescenceLineEdit->setText(path);
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
		pseudocolorFlag=false;
		fusionFlag=false;
		return;
	}
	if(temp>65535)
	{
		QMessageBox::information(NULL,"Warning","The input must be smaller than 65535.");
		substractFlag=false;
		filterFlag=false;
		pseudocolorFlag=false;
		fusionFlag=false;
		return;
	}
	substractValue=temp;
	string temp1=luminescneceFileName.toStdString();
	const char* temp2=temp1.c_str();
	Mat inputImageData=imread(temp1,CV_LOAD_IMAGE_ANYCOLOR|CV_LOAD_IMAGE_ANYDEPTH);
	Mat outputImageData=inputImageData-substractValue;
	imwrite(".//tempFiles//substractData.tif",outputImageData);
	showLuminescenceData(".//tempFiles//substractData.tif");
	substractFlag=true;
	filterFlag=false;
	pseudocolorFlag=false;
	fusionFlag=false;
}

void ImagePseudoColor::on_pushButton_filter_clicked()
{
	clearFusionWindow();
	unsigned long temp;
	temp=ui.FilterLineEdit->text().toULong();
	if(!substractFlag)
	{
		QMessageBox::information(NULL,"Warning","Please substract the background first.");
		filterFlag=false;;
		fusionFlag=false;
		return;
	}
	if(temp<0 || temp>100)//暂定
	{
		QMessageBox::information(NULL,"Warning","The input must be larger than 0 and smaller than 100.");
		filterFlag=false;
		fusionFlag=false;
		return;
	}
	ui.pushButtonFilter->setEnabled(false);
	filterValue=temp;
	filterThread->kernelSize=filterValue;
	filterThread->start();
}
void ImagePseudoColor::on_pushButton_pseudocolor_clicked()
{
	clearFusionWindow();
	colorbarHighValue=ui.spinBoxHighValue->value();
	colorbarLowValue=ui.spinBoxLowValue->value();
	if(!filterFlag)
	{
		QMessageBox::information(NULL,"Warning","Please filter the luminescnece image.");
		pseudocolorFlag=false;
		fusionFlag=false;
		return;
	}
	if(colorbarHighValue<=colorbarLowValue)
	{
		QMessageBox::information(NULL,"Warning","The colorbar_low must be smaller than colorbar_high.");
		pseudocolorFlag=false;
		fusionFlag=false;
		return;
	}
	ui.pushButtonPseudoColor->setEnabled(false);
	pseudocolorThread->setColorbarValues(colorbarLowValue,colorbarHighValue);
	pseudocolorThread->start();
}
void ImagePseudoColor::on_pushButton_fusion_clicked()
{
	if(!pseudocolorFlag)
	{
		QMessageBox::information(NULL,"Warning","Please pseudocolor the luminescnece image.");
		fusionFlag=false;
		return;
	};
	if(!photographFlag)
	{
		photographFileName=luminescneceFileName;//若是没有读取白光图像
	}
	tempITKLuminesenceData=ImageType::New();
	tempITKLuminesenceData=readImage(".//tempFiles//filterData.tif");
	Mat photo=imread(string((const char *)photographFileName.toLocal8Bit()),CV_LOAD_IMAGE_ANYCOLOR|CV_LOAD_IMAGE_GRAYSCALE);//这里刻意用8bit格式来读取，然后保存成8bit
	Mat lumi=imread(string((const char *)luminescneceFileName.toLocal8Bit()),CV_LOAD_IMAGE_ANYCOLOR|CV_LOAD_IMAGE_ANYDEPTH);
	if(photo.size!=lumi.size)
	{
		QMessageBox::information(NULL,"Warning","The sizes of photograph and luminescence are not the same.");
		fusionFlag=false;
		return;
	}
	imwrite(".//tempFiles//origianlPhotograph.tif",photo);
	imwrite(".//tempFiles//originalLuminescence.tif",lumi);
	ImageType::Pointer photographData=ImageType::New();
	photographData=readImage(".//tempFiles//origianlPhotograph.tif");

	RGBImageType::Pointer pseudocolorData=RGBImageType::New();
	pseudocolorData=readRGBImage(".//tempFiles//pseudocolorData.tif");
	IteratorType photographIt(photographData,photographData->GetRequestedRegion());
	IteratorType luminescenceIt(tempITKLuminesenceData,tempITKLuminesenceData->GetRequestedRegion());
	RGBIteratortype rgbIt(pseudocolorData,pseudocolorData->GetRequestedRegion());
	PixelType tempLumi;
	PixelType tempPhot;
	RGBPixelType tempRGB;
	while(!rgbIt.IsAtEnd())
	{
		tempLumi=luminescenceIt.Get();
		tempPhot=photographIt.Get();
		if(tempLumi<colorbarLowValue)
		{
			tempRGB.SetRed(tempPhot);
			tempRGB.SetBlue(tempPhot);
			tempRGB.SetGreen(tempPhot);
			rgbIt.Set(tempRGB);
		}
		++rgbIt;
		++photographIt;
		++luminescenceIt;
	}
	writeRGBImage(pseudocolorData,".//tempFiles//fusionData.tif");
	showFusionData(".//tempFiles//fusionData.tif");
	fusionFlag=true;
	ui.clorbarHighLabel->setText(ui.spinBoxHighValue->text());
	ui.colorbarLowLabel->setText(ui.spinBoxLowValue->text());

}
void ImagePseudoColor::on_pushButton_quit_clicked()
{
	this->close();
}
void ImagePseudoColor::clearFusionWindow()
{
	ui.clorbarHighLabel->setText("");
	ui.colorbarLowLabel->setText("");
	ui.fusionImageLabel->setText("Fusion");
}
void ImagePseudoColor::on_menuHelp_clicked()
{
	QMessageBox::information(this, tr("About"),QString("Author: Xin Cao\n")+"Email: caoxin918@gmail.com");  
}
bool ImagePseudoColor::copyDirectoryFiles(QString fromDir,QString toDir,bool cover)
{
	QDir sourceDir(fromDir);
	QDir targetDir(toDir);
	if(!targetDir.exists())
	{
		if(!targetDir.mkdir(targetDir.absolutePath()))
			return false;
	}
	QFileInfoList fileInfoList = sourceDir.entryInfoList(); 
	foreach(QFileInfo fileInfo,fileInfoList)
	{
		if(fileInfo.fileName()=="."||fileInfo.fileName()=="..")
			continue;
		if(cover&&targetDir.exists(fileInfo.fileName()))
		{
			targetDir.remove(fileInfo.fileName());
		}
		if(!QFile::copy(fileInfo.filePath(),targetDir.filePath(fileInfo.fileName())))
		{
			return false;
		}
	}
}
void ImagePseudoColor::on_pushButton_save_clicked()
{
	if(!fusionFlag)
	{
		QMessageBox::information(NULL,"Warning","No fusion images.");
		fusionFlag=false;
		return;
	}
	QString fileName=QFileDialog::getSaveFileName(this,tr("Save Results"),"");
	if(!fileName.isNull())
	{
		if(!copyDirectoryFiles(".//tempFiles",fileName,1))
		{
			QMessageBox::information(NULL,"Warning","Copy files errors.");
			return;
		}
	}
	else
	{
		return;
	}
}
void ImagePseudoColor::on_pushButton_clear_clicked()
{
	photographFlag=false;
	luminescenceFlag=false;
	substractFlag=false;
	filterFlag=false;
	pseudocolorFlag=false;
	fusionFlag=false;
	ui.LuminescenceLineEdit->setText("");
	ui.PhotographLineEdit->setText("");
	ui.SubstractLineEdit->setText("");
	ui.FilterLineEdit->setText("");
	ui.spinBoxHighValue->setValue(1);
	ui.spinBoxLowValue->setValue(0);
	ui.photographLabel->setText("Photograph");
	ui.luminescenceLabel->setText("Luminescence");
	ui.fusionImageLabel->setText("Fusion");
	ui.clorbarHighLabel->setText("1");
	ui.colorbarLowLabel->setText("0");
	luminescneceFileName="";
	photographFileName="";
}
bool ImagePseudoColor::removeDirWithContent(const QString &dirName)
{
	static QVector<QString> dirNames;  
	static QString funcErrMsg="删除[%1]失败.";  
	static QString funcInfFndMsg="发现[%1].";  
	static QString funcInfDelMsg="删除[%1]成功.";  
	QDir dir;  
	QFileInfoList filst;  
	QFileInfoList::iterator curFi;  
	//初始化
	dirNames.clear();  
	if(dir.exists()){  
		dirNames<<dirName;  
	}  
	else{  
		return true;  
	}  
	//遍历各级文件夹，并将这些文件夹中的文件删除  
	for(int i=0;i<dirNames.size();++i){  
		dir.setPath(dirNames[i]);  
		filst=dir.entryInfoList(QDir::Dirs|QDir::Files  
			|QDir::Readable|QDir::Writable  
			|QDir::Hidden|QDir::NoDotAndDotDot  
			,QDir::Name);  
		if(filst.size()>0){  
			curFi=filst.begin();  
			while(curFi!=filst.end()){  
				//遇到文件夹,则添加至文件夹列表dirs尾部  
				if(curFi->isDir()){  
					dirNames.push_back(curFi->filePath());  
				}else if(curFi->isFile()){  
					//遇到文件,则删除之  
					if(!dir.remove(curFi->fileName())){  
						return false;  
					}  
				}  
				curFi++;  
			}//end of while  
		}  
	}  
	//删除文件夹  
	for(int i=dirNames.size()-1;i>=0;--i){  
		dir.setPath(dirNames[i]);  
		if(!dir.rmdir(".")){  
			return false;  
		}  
	}   
	return true; 
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
void ImagePseudoColor::showFusionData(QString filename)
{
	luminescneceImage=new QImage;
	if(!luminescneceImage->load(filename))
	{
		QMessageBox::information(this,tr("Open image error"),tr("Open image error!"));
		return;
	}
	imgScaled=new QImage;
	*imgScaled=luminescneceImage->scaled(ui.fusionImageLabel->width(),ui.fusionImageLabel->height(),Qt::KeepAspectRatio);
	ui.fusionImageLabel->setPixmap(QPixmap::fromImage(*imgScaled));
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
RGBImageType::Pointer ImagePseudoColor::readRGBImage(QString filename)
{
	string temp1=filename.toStdString();
	const char* temp2=temp1.c_str();
	RGBReaderType::Pointer reader=RGBReaderType::New();
	reader->SetFileName(temp2);
	reader->SetImageIO(TIFFIOType::New());
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
void ImagePseudoColor::writeRGBImage(RGBImageType* outputImage,const char* filename)
{
	RGBWriterType::Pointer writer=RGBWriterType::New();
	writer->SetFileName(filename);
	writer->SetImageIO(tiffIO);
	writer->SetInput(outputImage);
	writer->Update();
}
void ImagePseudoColor::receiveFilterSignal()
{
	
	IplImage *Image1=cvLoadImage(".//tempFiles//filterData.tif" ,CV_LOAD_IMAGE_ANYCOLOR|CV_LOAD_IMAGE_ANYDEPTH );
	double minValue,maxValue;
	cvMinMaxLoc(Image1,&minValue,&maxValue);
	ui.spinBoxHighValue->setValue((int)(maxValue));
	ui.spinBoxLowValue->setValue((int)(minValue));
//	pseudocolorThread->setMaxMinImageValues(minValue,maxValue);

	showLuminescenceData(".//tempFiles//filterData.tif");
	filterFlag=true;
	pseudocolorFlag=false;
	fusionFlag=false;
	ui.pushButtonFilter->setEnabled(true);
}
void ImagePseudoColor::receivePseudocolorSignal()
{
	showLuminescenceData(".//tempFiles//pseudocolorData.tif");
	pseudocolorFlag=true;
	fusionFlag=false;
	ui.pushButtonPseudoColor->setEnabled(true);
}