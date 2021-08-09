/**
 * File: Types.h
 * Project: DUtilsCV library
 * Author: Dorian Galvez-Lopez
 * Date: November 17, 2010
 * Description: OpenCV-related data types functions
 * License: see the LICENSE.txt file
 *
 */

#ifndef __D_CV_TYPES__
#define __D_CV_TYPES__

#include <opencv2/core/core.hpp>
#include <string>

namespace DUtilsCV
{

/// Functions to visualize and convert between OpenCV types
class Types
{
public:

  /**
   * Copies a mat to an array
   * @param V col/row vector or 2D matrix. 
   * @param r array where the data, casted to T and in row-major order, 
   * will be copied to
   */
  template<class T>
  static void vectorize(const cv::Mat &V, T* r);
  
  /**
   * Returns the data type of the matrix in a readable string
   * @param m matrix
   */
  static std::string type(const cv::Mat &m);

protected:
  
  /**
   * Copies a mat to an array
   * @param V col/row vector or 2D matrix of type TM.
   * @param r array where the data, casted to T and in row-major order, 
   * will be copied to
   */
  template<class TM, class T>
  static void _vectorize(const cv::Mat &V, T* r);

};

// ----------------------------------------------------------------------------

template<class T>
void Types::vectorize(const cv::Mat &V, T* r)
{
  switch(V.type())
  {
    case CV_8U: Types::_vectorize<unsigned char, T>(V, r); break;
    case CV_8S: Types::_vectorize<char, T>(V, r); break;
    case CV_16U: Types::_vectorize<unsigned short, T>(V, r); break;
    case CV_16S: Types::_vectorize<short, T>(V, r); break;
    case CV_32S: Types::_vectorize<int, T>(V, r); break;
    case CV_32F: Types::_vectorize<float, T>(V, r); break;
    case CV_64F: Types::_vectorize<double, T>(V, r); break;
  }
}

// ----------------------------------------------------------------------------

template<class TM, class T>
void Types::_vectorize(const cv::Mat &V, T* r)
{
  if(V.empty() || V.cols == 0 || V.rows == 0) return;
  
  if(V.rows > 1 && V.cols > 1)
  {
    int i = 0;
    for(int row = 0; row < V.rows; ++row)
      for(int col = 0; col < V.cols; ++col, ++i)
        r[i] = V.at<TM>(row, col);
  }
  else
  {
    if(V.rows > 1)
    {
      for(int i = 0; i < V.rows; ++i) r[i] = (T)V.at<TM>(i, 0);
    }
    else
    {
      for(int i = 0; i < V.cols; ++i) r[i] = (T)V.at<TM>(0, i);
    }
  }

}

// ----------------------------------------------------------------------------

}

#endif

