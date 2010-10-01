/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    QtSlicer.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef QtSlicer_h
#define QtSlicer_h

#include "ui_QtSlicerGUI.h"
#include "ui_QtSlicerHelpGUI.h"
#include "itkImage.h"

class QtSlicer : public QDialog, Ui::Gui
{ 
public:
    
  QtSlicer( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
  ~QtSlicer();
  
  typedef itk::Image<float,3> ImageType;

  void DisplayPosition(int x,int y ,int z,float value);
  void Help();
  void SetInputImage(ImageType * newImData);
};

#endif