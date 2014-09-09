#include "imagesoftwarebinningclass.h"
#include "QMessageBox"
#include "qfile.h"


ImageSoftwareBinningclass::ImageSoftwareBinningclass(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);

	QObject::connect(ui.pushButtonSaveClose,SIGNAL(clicked()),this,SLOT(on_pushButton_quit_clicked()));
	QObject::connect(ui.radioButton_2,SIGNAL(clicked()),this,SLOT(on_radioButton_checked()));
	QObject::connect(ui.radioButton_4,SIGNAL(clicked()),this,SLOT(on_radioButton_checked()));
	QObject::connect(ui.radioButton_8,SIGNAL(clicked()),this,SLOT(on_radioButton_checked()));
	QObject::connect(ui.radioButton_16,SIGNAL(clicked()),this,SLOT(on_radioButton_checked()));
	QObject::connect(ui.radioButton_Custom,SIGNAL(clicked()),this,SLOT(on_radioButton_checked()));
	QObject::connect(ui.pushButtonOK,SIGNAL(clicked()),this,SLOT(on_pushButton_ok_clicked()));

	ui.radioButton_2->setChecked(false);
	ui.radioButton_4->setChecked(false);
	ui.radioButton_8->setChecked(false);
	ui.radioButton_16->setChecked(false);
	ui.radioButton_Custom->setChecked(false);
	ui.lineEdit_HBinning->setText("");
	ui.lineEdit_VBinning->setText("");
	ui.lineEdit_HBinning->setDisabled(true);
	ui.lineEdit_VBinning->setDisabled(true);
	ui.checkBox_resize->setChecked(false);
	
	QRegExp regx("[0-9]+$");
	QValidator *validator1 = new QRegExpValidator(regx,ui.lineEdit_HBinning);
	ui.lineEdit_HBinning->setValidator(validator1);
	QValidator *validator2 = new QRegExpValidator(regx,ui.lineEdit_VBinning);
	ui.lineEdit_VBinning->setValidator(validator2);

	luminescneceFileName="";
	binningCustom=false;
	binningValue=0;
	originalHSize=0;
	originalVSize=0;
	binningHSize=0;
	binningVSize=0;
	HBinningValue=0;
	VBinningValue=0;
	saveBinningImg=false;
	binningProcessed=false;
	isReisedToOriginal=false;
}

ImageSoftwareBinningclass::~ImageSoftwareBinningclass()
{
	if(saveBinningImg && binningProcessed)
	{
		saveBinningImg=false;
		binningProcessed=false;
		emit done(isReisedToOriginal);
		
	}
	else
	{
		QFile::remove(".//tempFiles//binningLuminescenceImage.tif");
		QFile::remove(".//tempFiles//binningLuminescenceImageAfterResize.tif");
		saveBinningImg=false;
		binningProcessed=false;
	}
	
}
void ImageSoftwareBinningclass::on_radioButton_checked()
{
	ui.lineEdit_HBinning->setDisabled(true);
	ui.lineEdit_VBinning->setDisabled(true);
	binningValue=0;
	binningCustom=false;
	if (sender()==ui.radioButton_2)
	{
		binningValue=2;
	}
	else if (sender()==ui.radioButton_4)
	{
		binningValue=4;
	}
	else if (sender()==ui.radioButton_8)
	{
		binningValue=8;
	}
	else if (sender()==ui.radioButton_16)
	{
		binningValue=16;
	}
	else if(sender()==ui.radioButton_Custom)
	{
		binningCustom=true;
		ui.lineEdit_HBinning->setDisabled(false);
		ui.lineEdit_VBinning->setDisabled(false);
	}
	else
	{
		return;
	}
}
void ImageSoftwareBinningclass::on_pushButton_ok_clicked()
{
	if(binningValue==0&&binningCustom==false)
	{
		QMessageBox::information(this,tr("Error"),tr("Please choose a binning value first!"));
		return;
	}
	if(binningCustom)
	{
		HBinningValue=(ui.lineEdit_HBinning->text().toULong());
		VBinningValue=(ui.lineEdit_VBinning->text().toULong());
		binningHSize=originalHSize/(HBinningValue);
		binningVSize=originalVSize/(VBinningValue);
		
		if(originalHSize%(HBinningValue)!=0 || originalVSize%(VBinningValue)!=0)
		{
			QMessageBox::information(this,tr("Error"),tr("Binning size error!"));
			return;
		}
		
	}
	else
	{
		binningHSize=originalHSize/binningValue;
		binningVSize=originalVSize/binningValue;
		HBinningValue=VBinningValue=binningValue;
		if(originalHSize%(HBinningValue)!=0 || originalVSize%(VBinningValue)!=0)
		{
			QMessageBox::information(this,tr("Error"),tr("Binning size error!"));
			return;
		}
		
	}
	if(originalHSize<=HBinningValue||originalVSize<=VBinningValue)
	{
		QMessageBox::information(this,tr("Error"),tr("Binning size error!"));
		return;
	}
	binningImage=Mat(binningHSize,binningVSize,CV_16U);
	for(int i=0;i<binningImage.rows;i++)
		for(int j=0;j<binningImage.cols;j++)
			binningImage.at<PixelType>(i,j)=0;
	Mat binningWithResizeImag;//这个存放缩放后的图像数据
	imageBinning();
	imageResize();
	if(ui.checkBox_resize->isChecked())//是否要进行差值放大到原来的大小
	{
		cv::resize(binningImage, binningWithResizeImag, Size(originalHSize,originalVSize)); //默认就是双线性插值
		imwrite(".//tempFiles//binningLuminescenceImageAfterResize.tif",binningWithResizeImag);
		showImage(".//tempFiles//binningLuminescenceImageAfterResize.tif");
		isReisedToOriginal=true;
	}
	imwrite(".//tempFiles//binningLuminescenceImage.tif",binningImage);
	showImage(".//tempFiles//binningLuminescenceImage.tif");
	
	binningProcessed=true;
}
PixelType ImageSoftwareBinningclass::addImagePixel(int pixelLoacationI,int pixelLocationJ)
{
	double temp=0;
	for(int i=0;i<(HBinningValue);++i)
	{
		for (int j=0;j<(VBinningValue);++j)
		{
			temp+=originalImage.at<PixelType>(pixelLoacationI+i,pixelLocationJ+j);
		}
	}
	if(temp>65535)
		temp=65535;
	return (PixelType)temp;
}
void ImageSoftwareBinningclass::imageBinning()
{
	for(int i=0;i<binningHSize;++i)
		for (int j=0;j<binningVSize;++j)
		{
			if(i==0)
			{
				int jjj=0;
			}
			binningImage.at<PixelType>(i,j)=addImagePixel(i*HBinningValue,j*VBinningValue);
		}
}
void ImageSoftwareBinningclass::showImage(QString path)
{
	QImage *luminescneceImage=new QImage;
	if(!luminescneceImage->load(path))
	{
		QMessageBox::information(this,tr("Open image error"),tr("Open image error!"));
		return;
	}
	QImage* imgScaled=new QImage;
	*imgScaled=luminescneceImage->scaled(ui.label_binningImage->width(),ui.label_binningImage->height(),Qt::KeepAspectRatio);
	ui.label_binningImage->setPixmap(QPixmap::fromImage(*imgScaled));
}
void ImageSoftwareBinningclass::showOriginalImage(QString path)
{
	QImage *luminescneceImage=new QImage;
	if(!luminescneceImage->load(path))
	{
		QMessageBox::information(this,tr("Open image error"),tr("Open image error!"));
		return;
	}
	QImage* imgScaled=new QImage;
	*imgScaled=luminescneceImage->scaled(ui.label_originalImage->width(),ui.label_originalImage->height(),Qt::KeepAspectRatio);
	ui.label_originalImage->setPixmap(QPixmap::fromImage(*imgScaled));
	originalImage=imread(string((const char *)luminescneceFileName.toLocal8Bit()),CV_LOAD_IMAGE_ANYCOLOR|CV_LOAD_IMAGE_ANYDEPTH);
	originalHSize=originalImage.rows;//行
	originalVSize=originalImage.cols;//列
	
}

void ImageSoftwareBinningclass::imageResize()
{
	
}
void ImageSoftwareBinningclass::saveBinningImage()
{
	
}

void ImageSoftwareBinningclass::on_pushButton_quit_clicked()
{
	saveBinningImg=true;
	this->close();
}