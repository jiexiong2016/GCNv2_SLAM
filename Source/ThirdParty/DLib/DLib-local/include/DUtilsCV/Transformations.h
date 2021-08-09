/**
 * File: Transformations.h
 * Project: DUtilsCV library
 * Author: Dorian Galvez-Lopez
 * Date: October, 2010
 * Description: Functions to operate with transformation matrices
 * License: see the LICENSE.txt file
 *
 */

#ifndef __TRANSFORMATIONS__
#define __TRANSFORMATIONS__

#include <opencv2/core/core.hpp>

namespace DUtilsCV {

/// Spatial transformations
class Transformations
{
public:
  /**
   * Returns the transformation matrix of a generic rotation
   * @param axis axis of the rotation
   * @param theta angle of rotation in radians
   */
  static cv::Mat rotvec(const cv::Mat &axis, double theta);
  
  /**
   * Returns the transformation matrix of a generic rotation given its
   * Euler angles 
   * @param zphi phi angle (z asis)
   * @param ytheta theta angle (y asis)
   * @param zpsi psi angle (z asis)
   * @param tx optional x translation
   * @param ty optional y translation
   * @param tz optional z translation
   */
  //static cv::Mat euler(double zphi, double ytheta, double zpsi,
  //  double tx = 0, double ty = 0, double tz = 0);
  
  /** 
   * Creates a translational transform
   * @param X
   * @param Y
   * @param Z
   * @return transformation with the given translation
   */
  static cv::Mat transl(double X, double Y, double Z);
  
  /**
   * Returns the transformation matrix of a rotation around the x axis
   * @param theta rotation angle
   * @param X optional translation in x axis
   * @param Y optional translation in y axis
   * @param Z optional translation in z axis
   */
  static cv::Mat rotx(double theta, double X = 0, double Y = 0, double Z = 0);
  
  /**
   * Returns the transformation matrix of a rotation around the y axis
   * @param theta rotation angle
   * @param X optional translation in x axis
   * @param Y optional translation in y axis
   * @param Z optional translation in z axis
   */
  static cv::Mat roty(double theta, double X = 0, double Y = 0, double Z = 0);
  
  /**
   * Returns the transformation matrix of a rotation around the z axis
   * @param theta rotation angle
   * @param X optional translation in x axis
   * @param Y optional translation in y axis
   * @param Z optional translation in z axis
   */
  static cv::Mat rotz(double theta, double X = 0, double Y = 0, double Z = 0);

  /**
   * Returns the inverse of a transformation
   * @param aTb
   * @return bTa
   */
  static cv::Mat inv(const cv::Mat &aTb);
  
  /**
   * Returns a 4x4 transformation matrix composed of the given rotation and
   * translation
   * @param R 3x3 rotation matrix or 1x3 or 3x1 rotation vector (Rodrigues format)
   * @param t 3x1 or 1x3 or 4x1 or 1x4 translation vector
   * @return 4x4 matrix
   */
  static cv::Mat composeRt(const cv::Mat &R, const cv::Mat &t);
  
  /**
   * Decomposes a 4x4 transformation matrix into a rotation and a translation
   * @param T 4x4 transformation matrix
   * @param R 3x3 rotation matrix
   * @param t 4x1 or 3x1 translation vector. If the given size of t is 4x1 or 3x1,
   *   then it is kept. If not, a 4x1 vector is returned
   */
  static void decomposeRt(const cv::Mat &T, cv::Mat &R, cv::Mat &t);
  
};

}

#endif
