/********************************************************************************
** Form generated from reading UI file 'imagepseudocolor.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEPSEUDOCOLOR_H
#define UI_IMAGEPSEUDOCOLOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImagePseudoColorClass
{
public:
    QAction *actionAbout;
    QAction *actionImageBinning;
    QAction *actionConvertTo8Bit;
    QAction *actionHistogram;
    QAction *actionThreashold;
    QWidget *centralWidget;
    QLineEdit *PhotographLineEdit;
    QPushButton *pushButtonPhotograph;
    QLineEdit *LuminescenceLineEdit;
    QPushButton *pushButtonLuminescence;
    QLineEdit *SubstractLineEdit;
    QPushButton *pushButtonClear;
    QPushButton *pushButtonSave;
    QPushButton *pushButtonFilter;
    QLineEdit *FilterLineEdit;
    QPushButton *pushButtonQuit;
    QPushButton *pushButtonFusion;
    QPushButton *pushButtonSubstract;
    QSpinBox *spinBoxHighValue;
    QSpinBox *spinBoxLowValue;
    QLabel *photographLabel;
    QLabel *luminescenceLabel;
    QLabel *fusionImageLabel;
    QPushButton *pushButtonPseudoColor;
    QLabel *clorbarHighLabel;
    QLabel *colorbarLowLabel;
    QLabel *colorbarImageLabel;
    QMenuBar *menuBar;
    QMenu *menuAbout;
    QMenu *menuTools;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ImagePseudoColorClass)
    {
        if (ImagePseudoColorClass->objectName().isEmpty())
            ImagePseudoColorClass->setObjectName(QString::fromUtf8("ImagePseudoColorClass"));
        ImagePseudoColorClass->setWindowModality(Qt::WindowModal);
        ImagePseudoColorClass->resize(950, 588);
        ImagePseudoColorClass->setMinimumSize(QSize(950, 588));
        ImagePseudoColorClass->setMaximumSize(QSize(950, 588));
        QIcon icon;
        icon.addFile(QString::fromUtf8("ImagePseudoColor.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ImagePseudoColorClass->setWindowIcon(icon);
        actionAbout = new QAction(ImagePseudoColorClass);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionImageBinning = new QAction(ImagePseudoColorClass);
        actionImageBinning->setObjectName(QString::fromUtf8("actionImageBinning"));
        actionConvertTo8Bit = new QAction(ImagePseudoColorClass);
        actionConvertTo8Bit->setObjectName(QString::fromUtf8("actionConvertTo8Bit"));
        actionHistogram = new QAction(ImagePseudoColorClass);
        actionHistogram->setObjectName(QString::fromUtf8("actionHistogram"));
        actionThreashold = new QAction(ImagePseudoColorClass);
        actionThreashold->setObjectName(QString::fromUtf8("actionThreashold"));
        centralWidget = new QWidget(ImagePseudoColorClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        PhotographLineEdit = new QLineEdit(centralWidget);
        PhotographLineEdit->setObjectName(QString::fromUtf8("PhotographLineEdit"));
        PhotographLineEdit->setGeometry(QRect(10, 80, 81, 21));
        PhotographLineEdit->setReadOnly(true);
        pushButtonPhotograph = new QPushButton(centralWidget);
        pushButtonPhotograph->setObjectName(QString::fromUtf8("pushButtonPhotograph"));
        pushButtonPhotograph->setGeometry(QRect(10, 110, 81, 31));
        LuminescenceLineEdit = new QLineEdit(centralWidget);
        LuminescenceLineEdit->setObjectName(QString::fromUtf8("LuminescenceLineEdit"));
        LuminescenceLineEdit->setGeometry(QRect(10, 10, 81, 21));
        LuminescenceLineEdit->setReadOnly(true);
        pushButtonLuminescence = new QPushButton(centralWidget);
        pushButtonLuminescence->setObjectName(QString::fromUtf8("pushButtonLuminescence"));
        pushButtonLuminescence->setGeometry(QRect(10, 40, 81, 31));
        SubstractLineEdit = new QLineEdit(centralWidget);
        SubstractLineEdit->setObjectName(QString::fromUtf8("SubstractLineEdit"));
        SubstractLineEdit->setGeometry(QRect(10, 150, 81, 21));
        pushButtonClear = new QPushButton(centralWidget);
        pushButtonClear->setObjectName(QString::fromUtf8("pushButtonClear"));
        pushButtonClear->setGeometry(QRect(10, 470, 81, 31));
        pushButtonSave = new QPushButton(centralWidget);
        pushButtonSave->setObjectName(QString::fromUtf8("pushButtonSave"));
        pushButtonSave->setGeometry(QRect(10, 430, 81, 31));
        pushButtonFilter = new QPushButton(centralWidget);
        pushButtonFilter->setObjectName(QString::fromUtf8("pushButtonFilter"));
        pushButtonFilter->setGeometry(QRect(10, 250, 81, 31));
        FilterLineEdit = new QLineEdit(centralWidget);
        FilterLineEdit->setObjectName(QString::fromUtf8("FilterLineEdit"));
        FilterLineEdit->setGeometry(QRect(10, 220, 81, 21));
        pushButtonQuit = new QPushButton(centralWidget);
        pushButtonQuit->setObjectName(QString::fromUtf8("pushButtonQuit"));
        pushButtonQuit->setGeometry(QRect(10, 510, 81, 31));
        pushButtonFusion = new QPushButton(centralWidget);
        pushButtonFusion->setObjectName(QString::fromUtf8("pushButtonFusion"));
        pushButtonFusion->setGeometry(QRect(10, 390, 81, 31));
        pushButtonSubstract = new QPushButton(centralWidget);
        pushButtonSubstract->setObjectName(QString::fromUtf8("pushButtonSubstract"));
        pushButtonSubstract->setGeometry(QRect(10, 180, 81, 31));
        spinBoxHighValue = new QSpinBox(centralWidget);
        spinBoxHighValue->setObjectName(QString::fromUtf8("spinBoxHighValue"));
        spinBoxHighValue->setGeometry(QRect(10, 290, 81, 22));
        spinBoxHighValue->setMinimum(1);
        spinBoxHighValue->setMaximum(65535);
        spinBoxHighValue->setSingleStep(100);
        spinBoxLowValue = new QSpinBox(centralWidget);
        spinBoxLowValue->setObjectName(QString::fromUtf8("spinBoxLowValue"));
        spinBoxLowValue->setGeometry(QRect(10, 360, 81, 22));
        spinBoxLowValue->setMaximum(65534);
        spinBoxLowValue->setSingleStep(100);
        photographLabel = new QLabel(centralWidget);
        photographLabel->setObjectName(QString::fromUtf8("photographLabel"));
        photographLabel->setGeometry(QRect(110, 280, 250, 250));
        photographLabel->setAlignment(Qt::AlignCenter);
        luminescenceLabel = new QLabel(centralWidget);
        luminescenceLabel->setObjectName(QString::fromUtf8("luminescenceLabel"));
        luminescenceLabel->setGeometry(QRect(110, 10, 250, 250));
        luminescenceLabel->setAlignment(Qt::AlignCenter);
        fusionImageLabel = new QLabel(centralWidget);
        fusionImageLabel->setObjectName(QString::fromUtf8("fusionImageLabel"));
        fusionImageLabel->setGeometry(QRect(370, 10, 520, 520));
        fusionImageLabel->setLayoutDirection(Qt::LeftToRight);
        fusionImageLabel->setAlignment(Qt::AlignCenter);
        pushButtonPseudoColor = new QPushButton(centralWidget);
        pushButtonPseudoColor->setObjectName(QString::fromUtf8("pushButtonPseudoColor"));
        pushButtonPseudoColor->setGeometry(QRect(10, 320, 81, 31));
        clorbarHighLabel = new QLabel(centralWidget);
        clorbarHighLabel->setObjectName(QString::fromUtf8("clorbarHighLabel"));
        clorbarHighLabel->setGeometry(QRect(900, 10, 40, 12));
        clorbarHighLabel->setAlignment(Qt::AlignCenter);
        colorbarLowLabel = new QLabel(centralWidget);
        colorbarLowLabel->setObjectName(QString::fromUtf8("colorbarLowLabel"));
        colorbarLowLabel->setGeometry(QRect(900, 520, 40, 12));
        colorbarLowLabel->setAlignment(Qt::AlignCenter);
        colorbarImageLabel = new QLabel(centralWidget);
        colorbarImageLabel->setObjectName(QString::fromUtf8("colorbarImageLabel"));
        colorbarImageLabel->setGeometry(QRect(900, 30, 40, 481));
        colorbarImageLabel->setAlignment(Qt::AlignCenter);
        ImagePseudoColorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ImagePseudoColorClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 950, 23));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        ImagePseudoColorClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(ImagePseudoColorClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ImagePseudoColorClass->setStatusBar(statusBar);

        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuAbout->addAction(actionAbout);
        menuTools->addAction(actionImageBinning);
        menuTools->addAction(actionConvertTo8Bit);
        menuTools->addAction(actionHistogram);
        menuTools->addAction(actionThreashold);

        retranslateUi(ImagePseudoColorClass);

        QMetaObject::connectSlotsByName(ImagePseudoColorClass);
    } // setupUi

    void retranslateUi(QMainWindow *ImagePseudoColorClass)
    {
        ImagePseudoColorClass->setWindowTitle(QApplication::translate("ImagePseudoColorClass", "ImagePseudoColor", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("ImagePseudoColorClass", "About", 0, QApplication::UnicodeUTF8));
        actionImageBinning->setText(QApplication::translate("ImagePseudoColorClass", "ImageBinning", 0, QApplication::UnicodeUTF8));
        actionConvertTo8Bit->setText(QApplication::translate("ImagePseudoColorClass", "ConvertTo8Bit", 0, QApplication::UnicodeUTF8));
        actionHistogram->setText(QApplication::translate("ImagePseudoColorClass", "Histogram", 0, QApplication::UnicodeUTF8));
        actionThreashold->setText(QApplication::translate("ImagePseudoColorClass", "Threashold", 0, QApplication::UnicodeUTF8));
        pushButtonPhotograph->setText(QApplication::translate("ImagePseudoColorClass", "Photograph", 0, QApplication::UnicodeUTF8));
        pushButtonLuminescence->setText(QApplication::translate("ImagePseudoColorClass", "Luminescence", 0, QApplication::UnicodeUTF8));
        SubstractLineEdit->setText(QApplication::translate("ImagePseudoColorClass", "0", 0, QApplication::UnicodeUTF8));
        pushButtonClear->setText(QApplication::translate("ImagePseudoColorClass", "Clear", 0, QApplication::UnicodeUTF8));
        pushButtonSave->setText(QApplication::translate("ImagePseudoColorClass", "Save", 0, QApplication::UnicodeUTF8));
        pushButtonFilter->setText(QApplication::translate("ImagePseudoColorClass", "Filter", 0, QApplication::UnicodeUTF8));
        FilterLineEdit->setText(QApplication::translate("ImagePseudoColorClass", "1", 0, QApplication::UnicodeUTF8));
        pushButtonQuit->setText(QApplication::translate("ImagePseudoColorClass", "Quit", 0, QApplication::UnicodeUTF8));
        pushButtonFusion->setText(QApplication::translate("ImagePseudoColorClass", "Fusion", 0, QApplication::UnicodeUTF8));
        pushButtonSubstract->setText(QApplication::translate("ImagePseudoColorClass", "Substract", 0, QApplication::UnicodeUTF8));
        photographLabel->setText(QApplication::translate("ImagePseudoColorClass", "Photograph", 0, QApplication::UnicodeUTF8));
        luminescenceLabel->setText(QApplication::translate("ImagePseudoColorClass", "Luminescence", 0, QApplication::UnicodeUTF8));
        fusionImageLabel->setText(QApplication::translate("ImagePseudoColorClass", "Fusion", 0, QApplication::UnicodeUTF8));
        pushButtonPseudoColor->setText(QApplication::translate("ImagePseudoColorClass", "PseudoColor", 0, QApplication::UnicodeUTF8));
        clorbarHighLabel->setText(QApplication::translate("ImagePseudoColorClass", "1", 0, QApplication::UnicodeUTF8));
        colorbarLowLabel->setText(QApplication::translate("ImagePseudoColorClass", "0", 0, QApplication::UnicodeUTF8));
        colorbarImageLabel->setText(QApplication::translate("ImagePseudoColorClass", "LValue", 0, QApplication::UnicodeUTF8));
        menuAbout->setTitle(QApplication::translate("ImagePseudoColorClass", "Help", 0, QApplication::UnicodeUTF8));
        menuTools->setTitle(QApplication::translate("ImagePseudoColorClass", "Tools", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ImagePseudoColorClass: public Ui_ImagePseudoColorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEPSEUDOCOLOR_H
