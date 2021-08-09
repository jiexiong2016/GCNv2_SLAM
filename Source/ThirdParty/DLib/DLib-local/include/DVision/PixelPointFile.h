/**
 * File: PixelPointFile.h
 * Project: DVision library
 * Author: Dorian Galvez-Lopez
 * Date: October 8, 2010
 * Description: manages structures of pixels + 3d
 * License: see the LICENSE.txt file
 *
 */

#ifndef __PIXEL_POINT__
#define __PIXEL_POINT__

#include <vector>

namespace DVision {

/// Manages files of 2d+3d points
class PixelPointFile
{
public:

  /// Single 2d+3d point
  class PixelPoint
  {
    public:
      float u, v; // pixel coordinates
      float x, y, z; // 3d coordinates
      int idx; // an arbitrary global index
      
    public:
      /**
       * Creates a 2d+3d point
       * @param _u pixel u-coord
       * @param _v pixel v-coord
       * @param _x 3d x-coord
       * @param _y 3d y-coord
       * @param _z 3d z-coord
       * @param _idx index
       */
      PixelPoint(float _u, float _v, float _x, float _y, float _z, int _idx):
        u(_u), v(_v), x(_x), y(_y), z(_z), idx(_idx){}
      
      PixelPoint(){}
  };

public:

  /**
   * Saves a set of pixel points
   * @param filename
   * @param points
   */
  static void saveFile(const std::string &filename,
    const std::vector<PixelPoint> &points);
  
  /**
   * Loads a set of pixel points
   * @param filename
   * @param points
   */
  static void readFile(const std::string &filename,
    std::vector<PixelPoint> &points);

};

}

#endif
