//implemenet IIR && FIR  &&  class CCombFilterBase;
#include "CombFilter.h"

//-----------------------------------------------------------------------------//
//CFIR
CFIR::CFIR(float fMaxDelayLengthInS, float fSampleRateInHz, int iNumChannels)
	:m_fMaxDelayLengthInS(fMaxDelayLengthInS), m_pDelayline(NULL),
	m_fSampleRateInHz(fSampleRateInHz), m_iNumChannels(iNumChannels) {
	m_M = fMaxDelayLengthInS*fSampleRateInHz;
	m_pDelayline = NULL;
	if (m_M > 0) {
		m_pDelayline = new float[m_M];
		for (int i = 0; i < m_M;i++) {
			m_pDelayline[i] = 0.0f;
		}
	}
	else
		m_pDelayline = NULL;
};

CFIR::~CFIR() {
	if (m_pDelayline)
		delete m_pDelayline;
}

void CFIR::filter(float data_in[], float data_out[], int len)
{
	/*
	x=zeros(100,1);x(1)=1; % unit impulse signal of length 100
	g=0.5;
	Delayline=zeros(10,1);% memory allocation for length 10
	for n=1:length(x);
		y(n)=x(n)+g*Delayline(10);
		Delayline=[x(n);Delayline(1:10-1)];
	end;
	*/
	if (!m_pDelayline)
		return;
	
	float g = 0.5f;
	for (int i = 0; i < len;i++) {
		data_out[i] = data_in[i]+g*m_pDelayline[m_M-1];
		for(int j= m_M - 1;j>0;j--)
			m_pDelayline[j] = m_pDelayline[j-1];
		m_pDelayline[0] = data_in[i];
	}
}
Error_t CFIR::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames, int iNumChannel)
{
	filter(ppfInputBuffer[iNumChannel], ppfOutputBuffer[iNumChannel], iNumberOfFrames);
	return Error_t::kNoError;
}

void CFIR::reset()
{
	if(m_pDelayline)
		for (int i = 0; i < m_M;i++) {
			m_pDelayline[i] = 0.0f;
		}
}

//CIIR
CIIR::CIIR(float fMaxDelayLengthInS, float fSampleRateInHz, int iNumChannels)
	:m_fMaxDelayLengthInS(fMaxDelayLengthInS), m_pDelayline(NULL),
	m_fSampleRateInHz(fSampleRateInHz), m_iNumChannels(iNumChannels) {
	m_M = fMaxDelayLengthInS*fSampleRateInHz;
	m_pDelayline = NULL;
	if (m_M > 0) {
		m_pDelayline = new float[m_M];
		for (int i = 0; i < m_M;i++) {
			m_pDelayline[i] = 0.0f;
		}
	}
	else
		m_pDelayline = NULL;
};

CIIR::~CIIR() {
	if (m_pDelayline)
		delete m_pDelayline;
}

void CIIR::filter(float data_in[], float data_out[], int len)
{
	/*
	x=zeros(100,1);x(1)=1; % unit impulse signal of length 100
	g=0.5;
	Delayline=zeros(10,1); % memory allocation for length 10
	for n=1:length(x);
		y(n)=x(n)+g*Delayline(10);
		Delayline=[y(n);Delayline(1:10-1)];
	end;
	*/
	if (!m_pDelayline)
		return;

	float g = 0.5f;
	for (int i = 0; i < len;i++) {
		data_out[i] = data_in[i] + g*m_pDelayline[m_M - 1];
		for (int j = m_M - 1;j>0;j--)
			m_pDelayline[j] = m_pDelayline[j - 1];
		m_pDelayline[0] = data_out[i];
	}
}
Error_t CIIR::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames, int iNumChannel)
{
	filter(ppfInputBuffer[iNumChannel], ppfOutputBuffer[iNumChannel], iNumberOfFrames);
	return Error_t::kNoError;
}


void CIIR::reset()
{
	if (m_pDelayline)
		for (int i = 0; i < m_M;i++) {
			m_pDelayline[i] = 0.0f;
		}
}