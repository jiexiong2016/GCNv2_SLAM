/**
 * File: Mat.cpp
 * Project: DUtilsCV library
 * Author: Dorian Galvez-Lopez
 * Date: February 21, 2012
 * Description: functions for matrices
 * License: see the LICENSE.txt file
 *
 */

#include "Mat.h"
#include <opencv2/core/core.hpp>
#include <vector>

using namespace std;
using namespace DUtilsCV;

// ---------------------------------------------------------------------------

void Mat::removeRows(cv::Mat &m, const std::vector<unsigned int> &rows)
{
  vector<unsigned int> copied_rows = rows;
  Mat::removeRows(m, copied_rows);
}

// ---------------------------------------------------------------------------

void Mat::removeRows(cv::Mat &m, std::vector<unsigned int> &rows)
{
  if(rows.empty()) return;
  
  // filter indices 
  
  // sort the index entries    
  std::sort(rows.begin(), rows.end()); // ascending order
  
  // remove those indices that exceed the data vector length
  {
    int i_idx = (int)rows.size() - 1;
    while( rows[i_idx] >= (unsigned int)m.rows ) i_idx--;
    rows.resize(i_idx+1);
  }
    
  // make sure there are no repeated indices
  {
    const std::vector<unsigned int>::iterator last =
      std::unique(rows.begin(), rows.end());
    rows.erase(last, rows.end());
  }
  
  if(!rows.empty()) Mat::_removeRows(m, rows);
}

// ---------------------------------------------------------------------------

void Mat::_removeRows(cv::Mat &m, const std::vector<unsigned int> &rows)
{
  if(m.isContinuous())
  {
    switch(m.type())
    {
      case CV_8U: Mat::_removeRowsContinuous<unsigned char>(m, rows); break;
      case CV_8S: Mat::_removeRowsContinuous<char>(m, rows); break;
      case CV_16U: Mat::_removeRowsContinuous<unsigned short>(m, rows); break;
      case CV_16S: Mat::_removeRowsContinuous<short>(m, rows); break;
      case CV_32S: Mat::_removeRowsContinuous<int>(m, rows); break;
      case CV_32F: Mat::_removeRowsContinuous<float>(m, rows); break;
      case CV_64F: Mat::_removeRowsContinuous<double>(m, rows); break;
    }
  }
  else
  {
    Mat::_removeRowsNonContinuous(m, rows);
  }
}

// ---------------------------------------------------------------------------

void Mat::_removeRowsNonContinuous(cv::Mat &m, 
  const std::vector<unsigned int> &rows)
{
  // always preserve the order of the rest of rows
  
  // remove rows in descending order, grouping when possible
  int end_row = m.rows;
  int i_idx = (int)rows.size() - 1;
  while(i_idx >= 0)
  {
    int j_idx = i_idx - 1;
    while(j_idx >= 0 && ((int)(rows[i_idx] - rows[j_idx]) == i_idx - j_idx))
    {
      j_idx--;
    }
    //data.erase(data.begin() + indices[j_idx + 1], 
    //  data.begin() + indices[i_idx] + 1);
    // ==
    //std::copy( m.ptr<T>(rows[i_idx]+1), m.ptr<T>(end_row),
    //  m.ptr<T>(rows[j_idx + 1]) );
        
    m.rowRange(rows[j_idx+1], rows[j_idx+1] + end_row-rows[i_idx]-1) = 
      m.rowRange(rows[i_idx]+1, end_row) * 1;
    
    end_row -= rows[i_idx] - rows[j_idx+1] + 1;
    
    i_idx = j_idx;
  }
  
  // remove last rows
  m.resize(end_row);
}

// ---------------------------------------------------------------------------

