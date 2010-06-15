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
#ifndef __itkJointHistogramImageFunction_h
#define __itkJointHistogramImageFunction_h

#include "itkImageFunction.h"
#include "itkImage.h"

namespace itk
{

/** \class JointHistogramImageFunction
 *
 *  Using an input image an input mask, this function allows the computation of
 *  Z-Score values at a given point in the image. This is done by calling the
 *  Precompute function to build up a mean and standard deviation for each bin in
 *  a joint-histogram. That mean and standard deviation is used to compute the
 *  Z-Score at a point when Evaluate is called. The neighboorhood used in the
 *  computation of the joint histogram is determined by the feature width.
 *
 */
template<class TInputImage, class TCoordRep = float>
class ITK_EXPORT JointHistogramImageFunction :
    public ImageFunction< TInputImage, double, TCoordRep >
{
public:

  /** Class typedefs **/
  typedef JointHistogramImageFunction                  Self;
  typedef ImageFunction<TInputImage,double,TCoordRep>  Superclass;
  typedef SmartPointer<Self>                           Pointer;
  typedef SmartPointer<const Self>                     ConstPointer;
  typedef typename Superclass::InputImageType          InputImageType;
  typedef typename TInputImage::PixelType              PixelType;
  typedef typename Superclass::PointType               PointType;
  typedef typename Superclass::IndexType               IndexType;
  typedef typename Superclass::ContinuousIndexType     ContinuousIndexType;
  typedef itk::Image<float,2>                          HistogramType;

  /** Run-time type information (and related methods). */
  itkTypeMacro( JointHistogramImageFunction, ImageFunction );

  /** Standard New Macro. */
  itkNewMacro( Self );

  /** Constant for fetching the dimensions of the image. **/
  itkStaticConstMacro( ImageDimension, unsigned int, Superclass::ImageDimension );

  /** Get/Set the width of a significant feature. */
  itkGetMacro( FeatureWidth, double );
  itkSetMacro( FeatureWidth, double );

  /** Set the mask or second image used in the comparison. **/
  virtual void SetInputMask( const typename InputImageType::Pointer mask )
  {
    m_InputMask = mask;
  }

  /** Get the mask or second image used in the comparison. **/
  virtual typename InputImageType::Pointer GetInputMask() const
  {
    return m_InputMask;
  }

  /** Get the size of the histogram (It will be a size x size image ). */
  itkGetMacro( HistogramSize, unsigned int );

  /** 
   * Set the size of the histogram. This will reset the bins in the 
   * mean and standard deviation to zero. 
   */ 
  virtual void SetHistogramSize( const unsigned int& size );

  /** Get the Z-score at a given point. */
  virtual double Evaluate( const PointType& point ) const
  {
    IndexType index;
    this->ConvertPointToNearestIndex( point, index );
    return ( this->EvaluateAtIndex( index ) );
  }

  /** Get the Z-score at a given continuous index. */
  virtual double EvaluateAtContinuousIndex( const ContinuousIndexType & index ) const
  {
    IndexType nindex;

    this->ConvertContinuousIndexToNearestIndex( index, nindex );
    return this->EvaluateAtIndex( nindex );
  }

  /** Get the Z-score at a given index. */
  virtual double EvaluateAtIndex( const IndexType & index ) const;

  /** 
   * Add histograms (based on a given point) to the internals used to 
   * calculate the mean and standard deviation histograms when needed.
   */
  virtual void Precompute( const PointType& point ) const
  {
    IndexType index;
    this->ConvertPointToNearestIndex( point, index );
    this->PrecomputeAtIndex( index );
  }

  /** 
   * Add histograms (based on a given continuous index) to the internals used 
   * to calculate the mean and standard deviation histograms when needed.
   */
  virtual void PrecomputeAtContinuousIndex( const ContinuousIndexType & index ) const
  {
    IndexType nindex;

    this->ConvertContinuousIndexToNearestIndex( index, nindex );
    this->PrecomputeAtIndex( nindex );
  }

  /** 
   * Add histograms (based on a given index) to the internals used to 
   * calculate the mean and standard deviation histograms when needed.
   */
  virtual double PrecomputeAtIndex( const IndexType & index ) const;
  
protected:
  
  /** Default constructor */
  JointHistogramImageFunction();

  /** Default destructor */
  ~JointHistogramImageFunction() {}

  /** Printself function for introspection. **/
  void PrintSelf( std::ostream& os, Indent indent ) const;

  /** 
   * Compute the mean and standard deviation histograms for use in Z-score
   * calculation.
   */
  void ComputeMeanAndStandardDeviation();

  void ComputeHistogramAtIndex( const IndexType& index,
                                typename HistogramType::Pointer hist ) const;

  /** Data members **/
  typename InputImageType::Pointer       m_InputMask;
  typename HistogramType::Pointer        m_SumHistogram;
  typename HistogramType::Pointer        m_SumOfSquaresHistogram;
  typename HistogramType::Pointer        m_MeanHistogram;
  typename HistogramType::Pointer        m_StandardDeviationHistogram;
  double                                 m_FeatureWidth;
  unsigned int                           m_HistogramSize;
  mutable unsigned int                           m_NumberOfSamples;
  mutable unsigned int                           m_NumberOfComputedSamples;

private:
  JointHistogramImageFunction( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

}; // End class JointHistogramImageFunction

}// end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
# include "itkJointHistogramImageFunction.txx"
#endif

#endif
