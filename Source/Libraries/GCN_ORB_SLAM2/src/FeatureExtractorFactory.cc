#include "FeatureExtractorFactory.h"

#include"ORBextractor.h"

#ifdef USE_GCN
#include"GCNextractor.h"
#endif

namespace ORB_SLAM2
{

  FeatureExtractor* createFeatureExtractor(int nfeatures, float scaleFactor, int nlevels,
					 int iniThFAST, int minThFAST)
  {
#ifdef USE_GCN
    if (getenv("USE_ORB") == nullptr)
    {
      return new GCNextractor(nfeatures,scaleFactor,nlevels,iniThFAST,minThFAST);
    }
#endif
    return new ORBextractor(nfeatures,scaleFactor,nlevels,iniThFAST,minThFAST);
  }
}
