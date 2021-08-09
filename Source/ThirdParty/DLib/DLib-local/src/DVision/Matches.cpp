/**
 * File: Matches.cpp
 * Project: DVision library
 * Author: Dorian Galvez-Lopez
 * Date: October 4, 2010
 * Description: Function to manage correspondences
 * License: see the LICENSE.txt file
 *
 */

#include <iostream>
#include <vector>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Matches.h"

#include "DUtilsCV.h" // defines macros CVXX


// ---------------------------------------------------------------------------

void DVision::Matches::Save(const std::string &filename,
    const std::vector<int> &c1, const std::vector<int> &c2)
{
  cv::FileStorage fs(filename, cv::FileStorage::WRITE);  
  DVision::Matches::save(fs, c1, c2);
}

// ---------------------------------------------------------------------------

void DVision::Matches::save(cv::FileStorage &fs, const std::vector<int> &c0, 
    const std::vector<int> &c1)
{
  #if CV24
    cv::write(fs, "c0", c0);
    cv::write(fs, "c1", c1);
  #else
    if(!c0.empty())
      fs << "c0" << "[" << c0 << "]";
    else
      fs << "c0" << "[" << "]";

    if(!c1.empty())
      fs << "c1" << "[" << c1 << "]";
    else
      fs << "c1" << "[" << "]";
  #endif
}

// ---------------------------------------------------------------------------

void DVision::Matches::Load(const std::string &filename,
    std::vector<int> &c1, std::vector<int> &c2)
{
  cv::FileStorage fs(filename, cv::FileStorage::READ);
  DVision::Matches::load(fs, c1, c2);
}

// ---------------------------------------------------------------------------

void DVision::Matches::Load(const std::string &filename,
  std::vector<unsigned int> &c1, std::vector<unsigned int> &c2)
{
  std::vector<int> a1, a2;
  DVision::Matches::Load(filename, a1, a2);
  
  c1.resize(a1.size());
  std::copy(a1.begin(), a1.end(), c1.begin());
  
  c2.resize(a2.size());
  std::copy(a2.begin(), a2.end(), c2.begin());
}

// ---------------------------------------------------------------------------

void DVision::Matches::load(cv::FileStorage &fs, std::vector<int> &c0, 
    std::vector<int> &c1)
{
  #if CV24
    cv::read(fs["c0"], c0);
    cv::read(fs["c1"], c1);
  #else
    fs["c0"] >> c0;
    fs["c1"] >> c1; 
  #endif
}

// ---------------------------------------------------------------------------

