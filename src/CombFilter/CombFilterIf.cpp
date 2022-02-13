
// standard headers

// project headers
#include "MUSI6106Config.h"

#include "ErrorDef.h"
#include "Util.h"

#include "CombFilterIf.h"
#include "CombFilter.h"
static const char*  kCMyProjectBuildDate = __DATE__;


CCombFilterIf::CCombFilterIf () :
    m_bIsInitialized(false),
    m_pCCombFilter(0),
    m_fSampleRate(0)
{
    // this should never hurt
    this->reset ();
}

CCombFilterIf::~CCombFilterIf ()
{
    this->reset ();
}

const int  CCombFilterIf::getVersion (const Version_t eVersionIdx)
{
    int iVersion = 0;

    switch (eVersionIdx)
    {
    case kMajor:
        iVersion    = MUSI6106_VERSION_MAJOR; 
        break;
    case kMinor:
        iVersion    = MUSI6106_VERSION_MINOR; 
        break;
    case kPatch:
        iVersion    = MUSI6106_VERSION_PATCH; 
        break;
    case kNumVersionInts:
        iVersion    = -1;
        break;
    }

    return iVersion;
}
const char*  CCombFilterIf::getBuildDate ()
{
    return kCMyProjectBuildDate;
}

Error_t CCombFilterIf::create (CCombFilterIf*& pCCombFilter)
{
	pCCombFilter = new CCombFilterBase();

	if (!pCCombFilter)
		return Error_t::kMemError;

    return Error_t::kNoError;
}

Error_t CCombFilterIf::destroy (CCombFilterIf*& pCCombFilter)
{
	delete pCCombFilter; //here may call CCombFilterBase's destory
	pCCombFilter = 0;
    return Error_t::kNoError;
}

Error_t CCombFilterIf::init (CombFilterType_t eFilterType, float fMaxDelayLengthInS, float fSampleRateInHz, int iNumChannels)
{
	CCombFilterBase * p = (CCombFilterBase *)this;
	this->m_eFilterType = eFilterType;
	this->m_bIsInitialized = true;
	switch (eFilterType) {
	case kCombIIR:
		p->init_IIR(fMaxDelayLengthInS, fSampleRateInHz, iNumChannels);
		break;
	case kCombFIR:
		p->init_FIR(fMaxDelayLengthInS, fSampleRateInHz, iNumChannels);
		break;
	}
    return Error_t::kNoError;
}

Error_t CCombFilterIf::reset ()
{
	CCombFilterBase * p = (CCombFilterBase *)this;
	switch (m_eFilterType) {
	case kCombIIR:
		p->reset_IIR();
		break;
	case kCombFIR:
		p->reset_FIR();
		break;
	}
    return Error_t::kNoError;
}

Error_t CCombFilterIf::process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames,int iNumChannels)
{
	if (!m_bIsInitialized) {
		return Error_t::kNotInitializedError;
	}
	CCombFilterBase * p = (CCombFilterBase *)this;
	switch (m_eFilterType) {
	case kCombIIR:
		p->process_IIR(ppfInputBuffer,  ppfOutputBuffer, iNumberOfFrames, iNumChannels);
		break;
	case kCombFIR:
		p->process_FIR(ppfInputBuffer, ppfOutputBuffer, iNumberOfFrames, iNumChannels);
		break;
	}
    return Error_t::kNoError;
}

Error_t CCombFilterIf::setParam (FilterParam_t eParam, float fParamValue)
{
    return Error_t::kNoError;
}

float CCombFilterIf::getParam (FilterParam_t eParam) const
{
    return 0;
}
