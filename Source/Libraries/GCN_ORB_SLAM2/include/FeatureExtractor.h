
#ifndef FEATURE_EXTRACTOR_H
#define FEATURE_EXTRACTOR_H

#include <vector>
#include <list>
#include <opencv/cv.h>


namespace ORB_SLAM2
{
  class FeatureExtractor
  {
public:
    
    enum {HARRIS_SCORE=0, FAST_SCORE=1 };

    
    const int PATCH_SIZE = 31;
    const int HALF_PATCH_SIZE = 15;
    const int EDGE_THRESHOLD = 19;

    FeatureExtractor(int nfeatures, float scaleFactor, int nlevels,
                 int iniThFAST, int minThFAST);

    virtual ~FeatureExtractor(){}

  virtual void operator()( cv::InputArray image, cv::InputArray mask,
      std::vector<cv::KeyPoint>& keypoints,
      cv::OutputArray descriptors) = 0;

    int GetLevels() const {
        return nlevels;}

    float GetScaleFactor() const {
        return scaleFactor;}

    std::vector<float> GetScaleFactors() const {
        return mvScaleFactor;
    }

    std::vector<float> GetInverseScaleFactors() const {
        return mvInvScaleFactor;
    }

    std::vector<float> GetScaleSigmaSquares() const {
        return mvLevelSigma2;
    }

    std::vector<float> GetInverseScaleSigmaSquares() const {
        return mvInvLevelSigma2;
    }

    std::vector<cv::Mat> mvImagePyramid;

  protected:

    int nfeatures;
    double scaleFactor;
    int nlevels;
    int iniThFAST;
    int minThFAST;

  std::vector<int> mnFeaturesPerLevel;

    std::vector<int> umax;

    std::vector<float> mvScaleFactor;
    std::vector<float> mvInvScaleFactor;    
    std::vector<float> mvLevelSigma2;
    std::vector<float> mvInvLevelSigma2;

  };
}

#endif
