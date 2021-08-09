/**
 * File: BRIEF.h
 * Author: Dorian Galvez-Lopez
 * Date: May 2020
 * Description: implementation of BRIEF (Binary Robust Independent 
 *   Elementary Features) descriptor by 
 *   Michael Calonder, Vincent Lepetit and Pascal Fua
 *   + close binary tests (by Dorian Galvez-Lopez)
 *
 * If you use this code with the RANDOM_CLOSE descriptor version, please cite:
  @INPROCEEDINGS{GalvezIROS11,
    author={Galvez-Lopez, Dorian and Tardos, Juan D.},
    booktitle={Intelligent Robots and Systems (IROS), 2011 IEEE/RSJ International Conference on},
    title={Real-time loop detection with bags of binary words},
    year={2011},
    month={sept.},
    volume={},
    number={},
    pages={51 -58},
    keywords={},
    doi={10.1109/IROS.2011.6094885},
    ISSN={2153-0858}
  }
 *
 * License: see the LICENSE.txt file
 *
 */

#ifndef __D_BRIEF_256__
#define __D_BRIEF_256__

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <vector>
#include <bitset>

namespace DVision {

/// BRIEF descriptor
template<int Bits>
class BRIEF_t
{
public:

  /// Bitset type
  typedef std::bitset<Bits> bitset;

  /// Type of pairs
  enum Type
  {
    RANDOM, // random pairs (Calonder's original version)
    RANDOM_CLOSE // random but close pairs (used in GalvezIROS11)
  };
  
public:

  /**
   * Creates the BRIEF a priori data for descriptors of nbits length
   * @param patch_size 
   * @param type type of pairs to generate
   */
  BRIEF_t(int patch_size = 48, Type type = RANDOM_CLOSE) : m_patch_size(patch_size), m_type(type)
  {
    generateTestPoints();
  }
  
  /**
   * Returns the descriptor length in bits
   * @return descriptor length in bits
   */
  inline int getDescriptorLengthInBits() const
  {
    return Bits;
  }
  
  /**
   * Returns the type of classifier
   */
  inline Type getType() const
  {
    return m_type;
  }
  
  /**
   * Returns the size of the patch
   */
  inline int getPatchSize() const
  {
    return m_patch_size;
  }
  
  /**
   * Returns the BRIEF descriptors of the given keypoints in the given image
   * @param image
   * @param points
   * @param descriptors 
   * @param treat_image (default: true) if true, the image is converted to 
   *   grayscale if needed and smoothed. If not, it is assumed the image has
   *   been treated by the user
   * @note this function is similar to BRIEF::compute
   */
  inline void operator() (const cv::Mat &image, 
    const std::vector<cv::KeyPoint> &points,
    std::vector<bitset> &descriptors,
    bool treat_image = true) const
  {
    compute(image, points, descriptors, treat_image);
  }
  
  /**
   * Returns the BRIEF descriptors of the given keypoints in the given image
   * @param image
   * @param points
   * @param descriptors 
   * @param treat_image (default: true) if true, the image is converted to 
   *   grayscale if needed and smoothed. If not, it is assumed the image has
   *   been treated by the user
   * @note this function is similar to BRIEF::operator()
   */ 
  void compute(const cv::Mat &image,
    const std::vector<cv::KeyPoint> &points,
    std::vector<bitset> &descriptors,
    bool treat_image = true) const;
  
  /**
   * Exports the test pattern
   * @param x1 x1 coordinates of pairs
   * @param y1 y1 coordinates of pairs
   * @param x2 x2 coordinates of pairs
   * @param y2 y2 coordinates of pairs
   */
  inline void exportPairs(std::vector<int> &x1, std::vector<int> &y1,
    std::vector<int> &x2, std::vector<int> &y2) const
  {
    x1 = m_x1;
    y1 = m_y1;
    x2 = m_x2;
    y2 = m_y2;
  }
  
  /**
   * Sets the test pattern
   * @param x1 x1 coordinates of pairs
   * @param y1 y1 coordinates of pairs
   * @param x2 x2 coordinates of pairs
   * @param y2 y2 coordinates of pairs
   */
  inline void importPairs(const std::vector<int> &x1, 
    const std::vector<int> &y1, const std::vector<int> &x2, 
    const std::vector<int> &y2)
  {
    m_x1 = x1;
    m_y1 = y1;
    m_x2 = x2;
    m_y2 = y2;
    assert(Bits == static_cast<int>(x1.size()));
  }
  
  /**
   * Returns the Hamming distance between two descriptors
   * @param a first descriptor vector
   * @param b second descriptor vector
   * @return hamming distance
   */
  inline static int distance(const bitset &a, const bitset &b)
  {
    return (a^b).count();
  }

protected:

  /**
   * Generates random points in the patch coordinates, according to 
   * m_patch_size and m_bit_length
   */
  void generateTestPoints();
  
protected:

  /// Patch size
  int m_patch_size;
  
  /// Type of pairs
  Type m_type;

  /// Coordinates of test points relative to the center of the patch
  std::vector<int> m_x1, m_x2;
  std::vector<int> m_y1, m_y2;

};

template<int Bits>
void BRIEF_t<Bits>::compute(const cv::Mat &image,
    const std::vector<cv::KeyPoint> &points,
    std::vector<bitset> &descriptors,
    bool treat_image) const
{
  const float sigma = 2.f;
  const cv::Size ksize(9, 9);

  cv::Mat im;
  if(treat_image)
  {
    cv::Mat aux;
    if(image.depth() == 3)
    {
      cv::cvtColor(image, aux, cv::COLOR_RGB2GRAY);
    }
    else
    {
      aux = image;
    }

    cv::GaussianBlur(aux, im, ksize, sigma, sigma);

  }
  else
  {
    im = image;
  }

  assert(im.type() == CV_8UC1);
  assert(im.isContinuous());

  // use im now
  const int W = im.cols;
  const int H = im.rows;

  descriptors.resize(points.size());
  typename std::vector<bitset>::iterator dit;

  std::vector<cv::KeyPoint>::const_iterator kit;

  int x1, y1, x2, y2;

  dit = descriptors.begin();
  for(kit = points.begin(); kit != points.end(); ++kit, ++dit)
  {
    dit->reset();

    for(unsigned int i = 0; i < m_x1.size(); ++i)
    {
      x1 = (int)(kit->pt.x + m_x1[i]);
      y1 = (int)(kit->pt.y + m_y1[i]);
      x2 = (int)(kit->pt.x + m_x2[i]);
      y2 = (int)(kit->pt.y + m_y2[i]);

      if(x1 >= 0 && x1 < W && y1 >= 0 && y1 < H
        && x2 >= 0 && x2 < W && y2 >= 0 && y2 < H)
      {
        if( im.ptr<unsigned char>(y1)[x1] < im.ptr<unsigned char>(y2)[x2] )
        {
          dit->set(i);
        }
      } // if (x,y)_1 and (x,y)_2 are in the image
    } // for each (x,y)
  } // for each keypoint
}

template<int Bits>
void BRIEF_t<Bits>::generateTestPoints()
{
  m_x1.resize(Bits);
  m_y1.resize(Bits);
  m_x2.resize(Bits);
  m_y2.resize(Bits);

  const float g_mean = 0.f;
  const float g_sigma = 0.2f * (float)m_patch_size;
  const float c_sigma = 0.08f * (float)m_patch_size;

  float sigma2 = (m_type == RANDOM ? g_sigma : c_sigma);

  const int max_v = m_patch_size / 2;

  DUtils::Random::SeedRandOnce();

  for(int i = 0; i < Bits; ++i)
  {
    int x1, y1, x2, y2;

    do
    {
      x1 = DUtils::Random::RandomGaussianValue(g_mean, g_sigma);
    } while( x1 > max_v || x1 < -max_v);

    do
    {
      y1 = DUtils::Random::RandomGaussianValue(g_mean, g_sigma);
    } while( y1 > max_v || y1 < -max_v);

    float meanx, meany;
    if(m_type == RANDOM)
      meanx = meany = g_mean;
    else
    {
      meanx = x1;
      meany = y1;
    }

    do
    {
      x2 = DUtils::Random::RandomGaussianValue(meanx, sigma2);
    } while( x2 > max_v || x2 < -max_v);

    do
    {
      y2 = DUtils::Random::RandomGaussianValue(meany, sigma2);
    } while( y2 > max_v || y2 < -max_v);

    m_x1[i] = x1;
    m_y1[i] = y1;
    m_x2[i] = x2;
    m_y2[i] = y2;
  }
}

typedef BRIEF_t<256> BRIEF256;

} // namespace DVision

#endif


