/**
 * File: IO.h
 * Project: DUtilsCV library
 * Author: Dorian Galvez
 * Date: October 11, 2010
 * Description: OpenCV-related IO functions
 * License: see the LICENSE.txt file
 *
 */

#ifndef __D_CV_IO__
#define __D_CV_IO__

#include <iostream>
#include <vector>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

namespace DUtilsCV
{

/// Input/Output functions for storage and printing
class IO
{
public:

  /** 
   * Saves a set of keypoints in the opencv-format
   * @param filename
   * @param keys
   * @param nodename name of the node of the set of keypoints to store
   *  then in xml/yaml format
   */
  static void saveKeyPoints(const std::string &filename,
    const std::vector<cv::KeyPoint> &keys,
    const std::string &nodename = "keys");
  
  /**
   * Loads a set of keypoints
   * @param filename
   * @param keys
   * @param nodename
   */
  static void loadKeyPoints(const std::string &filename,
    std::vector<cv::KeyPoint> &keys,
    const std::string &nodename = "keys");
  
  /**
   * Prints a mat of scalar values
   * @param m
   * @param name optional name given when printing
   * @param f stream where m is printed out
   */
  static void print(const cv::Mat &m,
    const std::string &name = "", std::ostream &f = std::cout);
  
  /**
   * Prints any mat of the type given
   * @param m
   * @param name optional name given when printing
   * @param f stream where m is printed out
   */
  template<class T>
  static void print(const cv::Mat &m, 
    const std::string &name = "", std::ostream &f = std::cout);
  
  /** 
   * Prints the size of the given matrix
   * @param m
   * @param name optional name given when printing
   * @param f stream
   */
  static void printSize(const cv::Mat &m,
    const std::string &name = "", std::ostream &f = std::cout);
  
  /**
   * Prints the data type of the given matrix
   * @param m 
   * @param name optional name given when printing
   * @param f stream
   */
  static void printType(const cv::Mat &m,
    const std::string &name = "", std::ostream &f = std::cout);
  
  /**
   * Saves any type of data that supports the "write" function
   * @param filename
   * @param c the data structure to save
   * @param nodename
   */
  template<class T>
  static void save(const std::string &filename, const T& c,
    const std::string &nodename = "data");

  /**
   * Loads any type of data that supports the "read" function
   * @param filename
   * @param c the data structure to load
   * @param nodename
   */
  template<class T>
  static void load(const std::string &filename, T& c,
    const std::string &nodename = "data");
  
  // Save and Load functions to make calling easier

};

// ---------------------------------------------------------------------------

template<class T>
void IO::print(const cv::Mat &m, const std::string &name, std::ostream &f)
{
  if(!name.empty()) f << name << " = ";
  f << "[ " << std::endl;
  for(int r = 0; r < m.rows; ++r)
  {
    for(int c = 0; c < m.cols; ++c)
    {
      f << m.at<T>(r, c) << " ";
    }
    f << std::endl;
  }
  f << "]" << std::endl;
}

// ---------------------------------------------------------------------------

template<class T>
void IO::save(const std::string &filename, const T& c,
  const std::string &nodename)
{
  cv::FileStorage fs(filename, cv::FileStorage::WRITE);
  c.write(fs, nodename);
}

// ---------------------------------------------------------------------------

template<class T>
void IO::load(const std::string &filename, T& c,
  const std::string &nodename)
{
  cv::FileStorage fs(filename, cv::FileStorage::READ);
  c.read(fs[nodename]);
}

// ---------------------------------------------------------------------------

}

#endif

