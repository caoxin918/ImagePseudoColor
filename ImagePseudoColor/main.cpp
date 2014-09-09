#include "imagepseudocolor.h"
#include <QtGui/QApplication>
#include "imagesoftwarebinningclass.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ImagePseudoColor w;
	w.show();
// 	ImageSoftwareBinningclass ww;
// 	ww.show();
	return a.exec();
}
