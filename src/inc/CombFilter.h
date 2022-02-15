#pragma once

//IIR class CCombFilterBase;
#include <math.h>
#include <algorithm> 
#include <complex>
using namespace std;
#include "CombFilterIf.h"

class CIIR
{
private:
	float m_fMaxDelayLengthInS;
	float m_fSampleRateInHz;
	int m_iNumChannels;
	int m_M;
	float *m_pDelayline;
public:
	CIIR(float fMaxDelayLengthInS, float fSampleRateInHz, int iNumChannels);
	~CIIR();
	void reset();
	void filter(float data_in[], float data_out[], int len);
	Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames, int iNumChannel);
};
class CFIR
{
private:
	float m_fMaxDelayLengthInS;
	float m_fSampleRateInHz;
	int m_iNumChannels;
	int m_M;
	float *m_pDelayline;
public:
	CFIR(float fMaxDelayLengthInS, float fSampleRateInHz, int iNumChannels);
	~CFIR();
	void reset();
	void filter(float data_in[], float data_out[], int len);
	Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames, int iNumChannel);
};

class CCombFilterBase :public  CCombFilterIf
{
public:
	CCombFilterBase() :CCombFilterIf(), m_Filter_IIR(NULL), m_Filter_FIR(NULL)
	{

	};
	virtual ~CCombFilterBase() {
		if (m_Filter_IIR)
			delete m_Filter_IIR;
		if (m_Filter_FIR)
			delete m_Filter_FIR;
	}
	/*void reset_IIR() {
		float b[5] = { 0.001836, 0.007344, 0.011016, 0.007344, 0.001836 };
		float a[5] = { 1.0, -3.0544, 3.8291, -2.2925, 0.55075 };
		m_Filter_IIR->setPara(b, 4, a, 4);
	}
	void reset_IIR(float fMaxDelayLengthInS, float fSampleRateInHz, int iNumChannels) {
		
		if (m_Filter_IIR)
			delete m_Filter_IIR;
		m_Filter_IIR = new IIR_II();
		//set a test filter 4½×ÇÐ±ÈÑ©·òµÍÍ¨ÂË²¨Æ÷
		reset_IIR();
	}*/
	void init_IIR(float fMaxDelayLengthInS, float fSampleRateInHz, int iNumChannels) {

		if (m_Filter_IIR)
			delete m_Filter_IIR;
		m_Filter_IIR = new CIIR(fMaxDelayLengthInS, fSampleRateInHz, iNumChannels);
	}
	void reset_IIR() {
		if (m_Filter_IIR)
			m_Filter_IIR->reset();
	}
	void process_IIR(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames,int iNumChannels)
	{
		if (m_Filter_IIR) {
			for (int i = 0;i < iNumChannels;i++) {
				m_Filter_IIR->process(ppfInputBuffer, ppfOutputBuffer, iNumberOfFrames, i);
			}
		}
	}

	void init_FIR(float fMaxDelayLengthInS, float fSampleRateInHz, int iNumChannels) {

		if (m_Filter_FIR)
			delete m_Filter_FIR;
		m_Filter_FIR = new CFIR(fMaxDelayLengthInS, fSampleRateInHz, iNumChannels);
	
	}
	void reset_FIR() {
		if (m_Filter_FIR)
			m_Filter_FIR->reset();
	}
	void process_FIR(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames, int iNumChannels)
	{
		if (m_Filter_FIR) {
			for (int i = 0;i < iNumChannels;i++) {
				m_Filter_FIR->process(ppfInputBuffer, ppfOutputBuffer, iNumberOfFrames, i);
			}
		}
	}
private:
	CIIR * m_Filter_IIR;
	CFIR * m_Filter_FIR;
};