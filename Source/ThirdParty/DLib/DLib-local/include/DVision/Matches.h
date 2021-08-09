/**
 * File: Matches.h
 * Project: DVision library
 * Author: Dorian Galvez-Lopez
 * Date: October 4, 2010
 * Description: Function to manage correspondences
 * License: see the LICENSE.txt file
 *
 */
 
#ifndef __D_MATCHES__
#define __D_MATCHES__

#include <vector>
#include <string>

namespace DVision {

/// Manages files of corresponding points
class Matches
{
public:

  /**
   * Saves two correspondence vectors in filename, w/o keypoints
   * @param filename
   * @param c1
   * @param c2 must be as long as c1
   */
  static void Save(const std::string &filename,
    const std::vector<int> &c1, const std::vector<int> &c2);
  
  /**
   * Loads two correspondence vectors from filename, w/o keypoints
   * @param filename
   * @param c1
   * @param c2
   */
  static void Load(const std::string &filename,
    std::vector<int> &c1, std::vector<int> &c2);
  
  /**
   * Loads two correspondence vectors from filename, w/o keypoints
   * @param filename
   * @param c1
   * @param c2
   */
  static void Load(const std::string &filename,
    std::vector<unsigned int> &c1, std::vector<unsigned int> &c2);

protected:
  
  /** 
   * Adss the correspondence vectors to the file storage
   * @param fs
   * @param c1
   * @param c2
   */
  static void save(cv::FileStorage &fs, const std::vector<int> &c1, 
    const std::vector<int> &c2);

  /** 
   * Loads the correspondence vectors from the file storage
   * @param fs
   * @param c1
   * @param c2
   */
  static void load(cv::FileStorage &fs, std::vector<int> &c1, 
    std::vector<int> &c2);

};

}

#endif
