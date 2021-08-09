/**
 * File: PMVSCamera.h
 * Project: DVision library
 * Author: Dorian Galvez-Lopez
 * Date: October 7, 2010
 * Description: Class to read camera info from PMVS output files
 * License: see the LICENSE.txt file
 *
 */

#ifndef __PMVS_CAMERA__
#define __PMVS_CAMERA__
 
#include <vector>
#include <opencv2/core/core.hpp>
#include <string>
 
namespace DVision {
namespace PMVS {

/// Manages PMVS camera files
class CameraFile
{
public:

  /// A camera from a camera file
  class Camera
  {
  public:
    /// Projection matrix so that (u v s)' = P * (x y z 1)'
    cv::Mat P; // 3x4, 64-bit double
  };

public:
  /** 
   * Returns the camera information from the given file
   * @param filename
   * @param camera
   */
  static void readFile(const std::string &filename, Camera &camera);
  
  /** 
   * Returns the camera information from all the txt files in the given dir
   * @param filedir the directory
   * @param cameras
   */
  static void readFile(const std::string &filedir, 
    std::vector<Camera> &cameras);
    
  /** 
   * Saves the camera into the given filename
   * @param filename
   * @param cameras
   */
  static void saveFile(const std::string &filename, 
    const Camera &cameras);

  /** 
   * Saves the camera into the given directory, each in a file
   * with the given format (%08d.txt by default)
   * @param filedir directory to store the cameras in
   * @param cameras cameras to store
   * @param format format of file names
   */
  static void saveFile(const std::string &filedir, 
    const std::vector<Camera> &cameras,
    const std::string& format = "%08d.txt");
};

}
}

#endif

