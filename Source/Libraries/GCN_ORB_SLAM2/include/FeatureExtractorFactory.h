#ifndef FEATURE_EXTRACTOR_FACTORY_H
#define FEATURE_EXTRACTOR_FACTORY_H

#include "FeatureExtractor.h"

namespace ORB_SLAM2
{

FeatureExtractor* createFeatureExtractor(int nfeatures, float scaleFactor, int nlevels,
					 int iniThFAST, int minThFAST);

}
#endif
