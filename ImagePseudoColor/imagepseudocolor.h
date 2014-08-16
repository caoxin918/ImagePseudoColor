#ifndef IMAGEPSEUDOCOLOR_H
#define IMAGEPSEUDOCOLOR_H

#include <QtGui/QMainWindow>
#include "ui_imagepseudocolor.h"

class ImagePseudoColor : public QMainWindow
{
	Q_OBJECT

public:
	ImagePseudoColor(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ImagePseudoColor();

private:
	Ui::ImagePseudoColorClass ui;
};

#endif // IMAGEPSEUDOCOLOR_H
