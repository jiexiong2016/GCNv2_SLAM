/**
 * File: Geometry.h
 * Project: DUtilsCV library
 * Author: Dorian Galvez-Lopez
 * Date: October 11, 2010
 * Description: OpenCV-related geometry functions
 * License: see the LICENSE.txt file
 *
 */

#ifndef __D_CV_GEOMETRY__
#define __D_CV_GEOMETRY__

#include <opencv2/core/core.hpp>

namespace DUtilsCV
{

/// Geometry functions
class Geometry
{
public:

  /**
	 * Calculates the squared Euclidean distance between two OpenCV vectors
	 * @param a row/col vector of N components
	 * @param b row/col vector of N components
	 * @return squared Euclidean distance
	 * @deprecated not efficient
	 */
	static double sqDistance(const cv::Mat &a, const cv::Mat &b);

};

}

#endif

