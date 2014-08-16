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
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImagePseudoColorClass
{
public:
    QWidget *centralWidget;
    QLineEdit *PhotographLineEdit;
    QPushButton *pushButtonPhotograph;
    QLineEdit *LuminescenceLineEdit;
    QPushButton *pushButtonLuminescence;
    QLineEdit *SubstractLineEdit;
    QPushButton *pushButtonClear;
    QPushButton *pushButtonSave;
    QPushButton *pushButtonSmooth;
    QLineEdit *SmoothLineEdit;
    QPushButton *pushButtonQuit;
    QPushButton *pushButtonFusion;
    QPushButton *pushButtonSubstract;
    QSpinBox *spinBoxHighValue;
    QSpinBox *spinBoxLowValue;
    QLabel *photographyLabel;
    QLabel *luminescenceLabel;
    QLabel *fusionImageLabel;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ImagePseudoColorClass)
    {
        if (ImagePseudoColorClass->objectName().isEmpty())
            ImagePseudoColorClass->setObjectName(QString::fromUtf8("ImagePseudoColorClass"));
        ImagePseudoColorClass->resize(934, 597);
        ImagePseudoColorClass->setMaximumSize(QSize(934, 597));
        centralWidget = new QWidget(ImagePseudoColorClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        PhotographLineEdit = new QLineEdit(centralWidget);
        PhotographLineEdit->setObjectName(QString::fromUtf8("PhotographLineEdit"));
        PhotographLineEdit->setGeometry(QRect(10, 10, 81, 31));
        PhotographLineEdit->setReadOnly(true);
        pushButtonPhotograph = new QPushButton(centralWidget);
        pushButtonPhotograph->setObjectName(QString::fromUtf8("pushButtonPhotograph"));
        pushButtonPhotograph->setGeometry(QRect(10, 50, 81, 31));
        LuminescenceLineEdit = new QLineEdit(centralWidget);
        LuminescenceLineEdit->setObjectName(QString::fromUtf8("LuminescenceLineEdit"));
        LuminescenceLineEdit->setGeometry(QRect(10, 90, 81, 31));
        LuminescenceLineEdit->setReadOnly(true);
        pushButtonLuminescence = new QPushButton(centralWidget);
        pushButtonLuminescence->setObjectName(QString::fromUtf8("pushButtonLuminescence"));
        pushButtonLuminescence->setGeometry(QRect(10, 130, 81, 31));
        SubstractLineEdit = new QLineEdit(centralWidget);
        SubstractLineEdit->setObjectName(QString::fromUtf8("SubstractLineEdit"));
        SubstractLineEdit->setGeometry(QRect(10, 170, 81, 31));
        pushButtonClear = new QPushButton(centralWidget);
        pushButtonClear->setObjectName(QString::fromUtf8("pushButtonClear"));
        pushButtonClear->setGeometry(QRect(10, 470, 81, 31));
        pushButtonSave = new QPushButton(centralWidget);
        pushButtonSave->setObjectName(QString::fromUtf8("pushButtonSave"));
        pushButtonSave->setGeometry(QRect(10, 430, 81, 31));
        pushButtonSmooth = new QPushButton(centralWidget);
        pushButtonSmooth->setObjectName(QString::fromUtf8("pushButtonSmooth"));
        pushButtonSmooth->setGeometry(QRect(10, 290, 81, 31));
        SmoothLineEdit = new QLineEdit(centralWidget);
        SmoothLineEdit->setObjectName(QString::fromUtf8("SmoothLineEdit"));
        SmoothLineEdit->setGeometry(QRect(10, 250, 81, 31));
        pushButtonQuit = new QPushButton(centralWidget);
        pushButtonQuit->setObjectName(QString::fromUtf8("pushButtonQuit"));
        pushButtonQuit->setGeometry(QRect(10, 510, 81, 31));
        pushButtonFusion = new QPushButton(centralWidget);
        pushButtonFusion->setObjectName(QString::fromUtf8("pushButtonFusion"));
        pushButtonFusion->setGeometry(QRect(10, 390, 81, 31));
        pushButtonSubstract = new QPushButton(centralWidget);
        pushButtonSubstract->setObjectName(QString::fromUtf8("pushButtonSubstract"));
        pushButtonSubstract->setGeometry(QRect(10, 210, 81, 31));
        spinBoxHighValue = new QSpinBox(centralWidget);
        spinBoxHighValue->setObjectName(QString::fromUtf8("spinBoxHighValue"));
        spinBoxHighValue->setGeometry(QRect(10, 330, 81, 22));
        spinBoxHighValue->setMinimum(1);
        spinBoxHighValue->setMaximum(65535);
        spinBoxHighValue->setSingleStep(100);
        spinBoxLowValue = new QSpinBox(centralWidget);
        spinBoxLowValue->setObjectName(QString::fromUtf8("spinBoxLowValue"));
        spinBoxLowValue->setGeometry(QRect(10, 360, 81, 22));
        spinBoxLowValue->setMaximum(65534);
        spinBoxLowValue->setSingleStep(100);
        photographyLabel = new QLabel(centralWidget);
        photographyLabel->setObjectName(QString::fromUtf8("photographyLabel"));
        photographyLabel->setGeometry(QRect(110, 20, 250, 250));
        photographyLabel->setAlignment(Qt::AlignCenter);
        luminescenceLabel = new QLabel(centralWidget);
        luminescenceLabel->setObjectName(QString::fromUtf8("luminescenceLabel"));
        luminescenceLabel->setGeometry(QRect(110, 280, 250, 250));
        luminescenceLabel->setAlignment(Qt::AlignCenter);
        fusionImageLabel = new QLabel(centralWidget);
        fusionImageLabel->setObjectName(QString::fromUtf8("fusionImageLabel"));
        fusionImageLabel->setGeometry(QRect(380, 10, 524, 524));
        fusionImageLabel->setLayoutDirection(Qt::LeftToRight);
        fusionImageLabel->setAlignment(Qt::AlignCenter);
        ImagePseudoColorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ImagePseudoColorClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 934, 23));
        ImagePseudoColorClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(ImagePseudoColorClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ImagePseudoColorClass->setStatusBar(statusBar);

        retranslateUi(ImagePseudoColorClass);

        QMetaObject::connectSlotsByName(ImagePseudoColorClass);
    } // setupUi

    void retranslateUi(QMainWindow *ImagePseudoColorClass)
    {
        ImagePseudoColorClass->setWindowTitle(QApplication::translate("ImagePseudoColorClass", "ImagePseudoColor", 0, QApplication::UnicodeUTF8));
        pushButtonPhotograph->setText(QApplication::translate("ImagePseudoColorClass", "Photograph", 0, QApplication::UnicodeUTF8));
        pushButtonLuminescence->setText(QApplication::translate("ImagePseudoColorClass", "Luminescence", 0, QApplication::UnicodeUTF8));
        pushButtonClear->setText(QApplication::translate("ImagePseudoColorClass", "Clear", 0, QApplication::UnicodeUTF8));
        pushButtonSave->setText(QApplication::translate("ImagePseudoColorClass", "Save", 0, QApplication::UnicodeUTF8));
        pushButtonSmooth->setText(QApplication::translate("ImagePseudoColorClass", "Smooth", 0, QApplication::UnicodeUTF8));
        pushButtonQuit->setText(QApplication::translate("ImagePseudoColorClass", "Quit", 0, QApplication::UnicodeUTF8));
        pushButtonFusion->setText(QApplication::translate("ImagePseudoColorClass", "Fusion", 0, QApplication::UnicodeUTF8));
        pushButtonSubstract->setText(QApplication::translate("ImagePseudoColorClass", "Substract", 0, QApplication::UnicodeUTF8));
        photographyLabel->setText(QApplication::translate("ImagePseudoColorClass", "Photography", 0, QApplication::UnicodeUTF8));
        luminescenceLabel->setText(QApplication::translate("ImagePseudoColorClass", "Luminescence", 0, QApplication::UnicodeUTF8));
        fusionImageLabel->setText(QApplication::translate("ImagePseudoColorClass", "Fusion", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ImagePseudoColorClass: public Ui_ImagePseudoColorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEPSEUDOCOLOR_H
