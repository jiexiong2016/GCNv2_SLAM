/**
 * File: IO.cpp
 * Project: DUtilsCV library
 * Author: Dorian Galvez-Lopez
 * Date: October 11, 2010
 * Description: OpenCV-related IO functions
 * License: see the LICENSE.txt file
 *
 */

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include "IO.h"
#include "Types.h"

using namespace std;
using namespace DUtilsCV;

// ----------------------------------------------------------------------------

void IO::saveKeyPoints(const std::string &filename,
    const std::vector<cv::KeyPoint> &keys,
    const std::string &nodename)
{
  cv::FileStorage fs(filename, cv::FileStorage::WRITE);
  cv::write(fs, nodename, keys);
}

// ----------------------------------------------------------------------------
  
void IO::loadKeyPoints(const std::string &filename,
    std::vector<cv::KeyPoint> &keys,
    const std::string &nodename)
{
  keys.resize(0);
  cv::FileStorage fs(filename, cv::FileStorage::READ);
  cv::read(fs[nodename], keys);
}

// ----------------------------------------------------------------------------

void IO::print(const cv::Mat &m, const std::string &name, std::ostream &f)
{
  switch(m.type())
  {
    case CV_8U: IO::print<unsigned char>(m, name, f); break;
    case CV_8S: IO::print<char>(m, name, f); break;
    case CV_16U: IO::print<unsigned short>(m, name, f); break;
    case CV_16S: IO::print<short>(m, name, f); break;
    case CV_32S: IO::print<int>(m, name, f); break;
    case CV_32F: IO::print<float>(m, name, f); break;
    case CV_64F: IO::print<double>(m, name, f); break;
  }
}

// ----------------------------------------------------------------------------

void IO::printSize(const cv::Mat &m, const std::string &name, std::ostream &f)
{
  if(!name.empty()) f << name << ": ";
  if(m.channels() == 1)
    f << m.rows << " x " << m.cols;
  else
    f << m.rows << " x " << m.cols << " x " << m.channels();
  f << std::endl;
}

// ----------------------------------------------------------------------------

void IO::printType(const cv::Mat &m, const std::string &name, std::ostream &f)
{
  if(!name.empty()) f << name << ": ";
  f << Types::type(m) << std::endl;
}

// ----------------------------------------------------------------------------

