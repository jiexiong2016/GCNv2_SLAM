/**
 * File: Types.cpp
 * Project: DUtilsCV library
 * Author: Dorian Galvez-Lopez
 * Date: October 20, 2011
 * Description: OpenCV-related data types functions
 * License: see the LICENSE.txt file
 *
 */

#include <opencv2/core/core.hpp>
#include <string>
#include "Types.h"

using namespace DUtilsCV;

// ----------------------------------------------------------------------------

std::string Types::type(const cv::Mat &m)
{
  std::string ret;

  switch(m.type())
  {
    case CV_8U: ret = "CV_8U"; break;
    case CV_8S: ret = "CV_8S"; break;
    case CV_16U: ret = "CV_16U"; break;
    case CV_16S: ret = "CV_16S"; break;
    case CV_32S: ret = "CV_32S"; break;
    case CV_32F: ret = "CV_32F"; break;
    case CV_64F: ret = "CV_64F"; break;
    default: ret = ""; break;
  }
  
  return ret;
}

// ----------------------------------------------------------------------------


