/**
 * File: PLYFile.h
 * Project: DVision library
 * Author: Dorian Galvez-Lopez
 * Date: October 8, 2010
 * Description: Manager of simple PLY files
 * License: see the LICENSE.txt file
 *
 */

#ifndef __PLY_FILE__
#define __PLY_FILE__

#include <string>
#include <vector>

namespace DVision {
namespace PMVS {

/// Manages a subset of PLY files
class PLYFile
{
public:
  /// Single PLY point
  struct PLYPoint
  {
    double x, y, z;
    double nx, ny, nz;
    int r, g, b;  
  };
  
public:
  
  /** 
   * Reads a simply PLY file and returns its 3D points and colors
   * @param filename
   * @param points
   */
  static void readFile(const std::string &filename, std::vector<PLYPoint>& points);
  
  /** 
   * Reads a simply PLY file and returns its 3D points and colors
   * @param filename
   * @param points
   */
  static void saveFile(const std::string &filename, const std::vector<PLYPoint>& points);
  
  /**
   * Reads the header of the given filename and returns the number of points
   * it contains
   * @param filename
   */
  static int getNumberOfPoints(const std::string &filename);
  
};

}
}

#endif
