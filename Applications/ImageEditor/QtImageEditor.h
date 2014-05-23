/*=========================================================================

Library:   TubeTK

Copyright 2010 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/

#ifndef __QtImageEditor_h
#define __QtImageEditor_h

//Qt includes
#include <QDialog>
#include <QLineEdit>
#include <QString>

//itk includes
#include "itkComplexToRealImageFilter.h"
#include "itkComplexToImaginaryImageFilter.h"
#include "itkComplexToModulusImageFilter.h"
#include "itkFFTShiftImageFilter.h"
#include "itkForwardFFTImageFilter.h"
#include "itktubeGaussianDerivativeImageSource.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkInverseFFTImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkRecursiveGaussianImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkTimeProbe.h"


//QtImageViewer includes
#include "ui_QtSlicerGUI.h"
#include "ui_QtSlicerHelpGUI.h"

//TubeTK includes
#include "QtOverlayControlsWidget.h"


namespace tube {

class QtImageEditor : public QDialog, public Ui::GuiDialogBase
{
  Q_OBJECT
public:

  QtImageEditor( QWidget* parent = 0, Qt::WindowFlags fl = 0 );
  ~QtImageEditor();
  typedef itk::Image< double, 3 >      ImageType;
  typedef double                       InputPixelType;

  typedef itk::Image< InputPixelType, 3 >              InputImageType;
  typedef itk::Image< InputPixelType, 3 >              OutputImageType;
  typedef itk::Image<unsigned char, 3>                 UnsignedCharImageType;
  typedef itk::ImageFileReader<ImageType>              ReaderType;
  typedef itk::ImageFileWriter<UnsignedCharImageType>  WriterType;
  typedef itk::ForwardFFTImageFilter<InputImageType>   FFTType;
  typedef FFTType::OutputImageType                     ComplexImageType;
  typedef itk::tube::GaussianDerivativeImageSource< InputImageType >
  GaussianDerivativeSourceType;

  typedef itk::FFTShiftImageFilter< InputImageType,
  InputImageType > FFTShiftFilterType;
  typedef itk::InverseFFTImageFilter<ComplexImageType,
  OutputImageType> InverseFFTType;
  typedef itk::RecursiveGaussianImageFilter<ComplexImageType,
  ComplexImageType >  FilterType;
  typedef itk::ComplexToRealImageFilter<ComplexImageType,
  OutputImageType> RealFilterType;
  typedef itk::RescaleIntensityImageFilter< InputImageType,
  UnsignedCharImageType > RescaleFilterType;
  typedef itk::ComplexToImaginaryImageFilter<ComplexImageType,
  OutputImageType> ImaginaryFilterType;
  typedef itk::ComplexToModulusImageFilter<ComplexImageType,
  OutputImageType> ModulusFilterType;
  typedef itk::MultiplyImageFilter< ComplexImageType,
  InputImageType, ComplexImageType > MultiplyFilterType;

  GaussianDerivativeSourceType::Pointer createGaussianDerivative(
      GaussianDerivativeSourceType::VectorType order);
public slots:
  void hideHelp();
  void showHelp(bool checked);
  void setMaximumSlice();
  void setDisplaySigma(QString value);
  void setInputImage(ImageType *newImData);
  void setDisplaySliceNumber(int number);
  bool loadImage(QString filePathToLoad = QString());
  void loadOverlay(QString overlayImagePath = QString());
  void applyFFT();
  void applyInverseFFT();
  void applyFilter();
  void displayFFT();
  void blurFilter();

private:
  QLineEdit                  *m_SigmaLineEdit;
  ImageType                  *m_ImageData;
  FFTType::OutputImageType   *m_ComplexeImage;
  QtOverlayControlsWidget    *m_OverlayWidget;
  FFTType::Pointer            m_FFTFilter;
  FilterType::Pointer         m_FilterX;
  FilterType::Pointer         m_FilterY;
  FilterType::Pointer         m_FilterZ;
  InverseFFTType::Pointer     m_InverseFFTFilter;
  QDialog                    *m_HelpDialog;
};

} // End namespace tube

#endif
