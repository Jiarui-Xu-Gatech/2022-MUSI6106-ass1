
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"
#define PI (3.1415926)
#include "AudioFileIf.h"
#include "CombFilterIf.h"
/**
 * 
| _ 3rdparty: (3rd party dependencies)
| _ sndlib : sndfile library(3rdparty with ugly code and lots of warnings)
| _ cmake.modules : (cmake scripts)
| _ inc : global headers
| _ src : source code
| _ AudioFileIO : library wrapping sndfile(3rdparty)
| _ CombFilter : combfilter library(assignment 1)
| _ inc : internal headers
| _ MUSI6106Exec : code for executable binary

*/

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();
void errorMsg(char *msg);
void runDefaultTest();
/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string sInputFilePath,                 //!< file paths
                sOutputFilePath,
				sOutputWavFilePath,
				sPath;
	sPath = "data\\";
    static const int kBlockSize = 1024;

    clock_t time = 0;
	int l;
	int ndelay;
	char opt = '\0';

    float **ppfAudioData = 0;
	float **ppfAudioDataOut = 0;
	CCombFilterIf *pCCombFilter = 0;
    CAudioFileIf *phAudioFile = 0;
	CAudioFileIf *phAudioFile2 = 0;
    std::fstream hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;

    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
	if (argc != 4) {
		errorMsg("error in number of arguments.");
		printf("Run test:\n");
		runDefaultTest();
	}
    else
    {
		
		sInputFilePath = argv[1];
		l = strlen(argv[2]);
		if (l == 2 && argv[2][0] == '-' && (argv[2][1] == 'f' || argv[2][1] == 'i')) {
			opt = argv[2][1];
			ndelay = atoi(argv[3]);
			if (opt == 'f') {
				sOutputWavFilePath = "FIR_" + sInputFilePath;
				sOutputFilePath = "FIR_"+ sInputFilePath + ".txt";
			}
			else {
				sOutputWavFilePath = "IIR_" + sInputFilePath;
				sOutputFilePath = "IIR_" + sInputFilePath + ".txt";
			}
		}
		else {
			errorMsg("Wrong switch.");
			return -1;
		}

		sInputFilePath = sPath + sInputFilePath;
		sOutputWavFilePath = sPath + sOutputWavFilePath;
		sOutputFilePath = sPath + sOutputFilePath;

    }
	
    //////////////////////////////////////////////////////////////////////////////
	
    // open the input wave file
    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phAudioFile->isOpen())
    {
        cout << "Wave file open error!";
        CAudioFileIf::destroy(phAudioFile);
        return -1;
    }
    phAudioFile->getFileSpec(stFileSpec);
	CCombFilterIf::create(pCCombFilter);
	if (pCCombFilter) {
		if (opt == 'i')
			pCCombFilter->init(CCombFilterIf::kCombIIR, 1.0*ndelay/1000.0f, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);
		else
			pCCombFilter->init(CCombFilterIf::kCombFIR, 1.0*ndelay/1000.0f, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);
	}
	else {
		return -1;
	}
	// open the output wav file
	CAudioFileIf::create(phAudioFile2);
	phAudioFile2->openFile(sOutputWavFilePath, CAudioFileIf::kFileWrite,&stFileSpec);

    // open the output text file
    hOutputFile.open(sOutputFilePath.c_str(), std::ios::out);
    if (!hOutputFile.is_open())
    {
        cout << "Text file open error!";
        CAudioFileIf::destroy(phAudioFile);
        return -1;
    }

    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    ppfAudioData = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfAudioData[i] = new float[kBlockSize];

	ppfAudioDataOut = new float*[stFileSpec.iNumChannels];
	for (int i = 0; i < stFileSpec.iNumChannels; i++)
		ppfAudioDataOut[i] = new float[kBlockSize];

    if (ppfAudioData == 0)
    {
        CAudioFileIf::destroy(phAudioFile);
        hOutputFile.close();
        return -1;
    }
    if (ppfAudioData[0] == 0)
    {
        CAudioFileIf::destroy(phAudioFile);
        hOutputFile.close();
        return -1;
    }

    time = clock();

    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output text file (one column per channel)
    while (!phAudioFile->isEof())
    {
        // set block length variable
        long long iNumFrames = kBlockSize;

        // read data (iNumOfFrames might be updated!)
        phAudioFile->readData(ppfAudioData, iNumFrames);
		//if (pCCombFilter) 
		{
			pCCombFilter->process(ppfAudioData, ppfAudioDataOut, iNumFrames, stFileSpec.iNumChannels);
			if (phAudioFile2)
				phAudioFile2->writeData(ppfAudioDataOut, iNumFrames);
		}
        cout << "\r" << "reading and writing";

        // write
        for (int i = 0; i < iNumFrames; i++)
        {
			
            for (int c = 0; c < stFileSpec.iNumChannels; c++)
            {
                hOutputFile << ppfAudioData[c][i] << "\t";
				hOutputFile << ppfAudioDataOut[c][i] << "\t";
            }
            hOutputFile << endl;
        }
    }

    cout << "\nreading/writing done in: \t" << (clock() - time) * 1.F / CLOCKS_PER_SEC << " seconds." << endl;
	phAudioFile2->closeFile();
    //////////////////////////////////////////////////////////////////////////////
    // clean-up (close files and free memory)
    CAudioFileIf::destroy(phAudioFile);
    hOutputFile.close();

	for (int i = 0; i < stFileSpec.iNumChannels; i++) {
		delete[] ppfAudioData[i];
		delete[] ppfAudioDataOut[i];
	}
	delete[] ppfAudioDataOut;
    delete[] ppfAudioData;
    ppfAudioData = 0;
	ppfAudioDataOut = 0;
    // all done
    return 0;

}


void showClInfo()
{
    cout << "MUSI6106 Assignment Executable" << endl;
    cout << "(c) 2014-2022 by Alexander Lerch" << endl;
    cout  << endl;
    return;
}
void errorMsg(char *msg)
{
	printf("%s", msg);
	printf("\r\n");
	printf("the Program syntax: ./MUSI6106Exec inputfile.wav -f/i n\r\n");
	printf("-t f means use FIR, -t i means use IIR.\r\n");
	printf("n specify the max delay in millsecond.\r\n,some examples is below\r\n");
	printf("./MUSI6106Exec la.wav -f 100\r\n");
	printf("./MUSI6106Exec la.wav -i 150\r\n");
}

///////test function for when there is no argments
//1 FIR:Output is zero if input freq matches feedforward
bool t1_Is_FIR_Zero_when_Input_match_feedforward() {
	printf("t1_Isn_FIR_Zero_when_Input_match_feedforward\n");
	float fMaxDelayLengthInS = 0.01;
	int fSampleRateInHz = 44100;
	int nTotal = fSampleRateInHz;
	int nTotalFarwordCycle = 441; //fMaxDelayLengthInS * fSampleRateInHz;  //here let input freq matches feedforward
	float** input = new float*[1];
	float** output = new float*[1];
	input[0] = new float[nTotal];
	output[0] = new float[nTotal];

	for (int i = 0; i < nTotal; ++i) {
		input[0][i] =sin(2 * PI * i / (nTotalFarwordCycle));
	}

	/* Get a FIR combfilter */
	CCombFilterIf* combFilterIf = nullptr;
	CCombFilterIf::create(combFilterIf);
	combFilterIf->init(
		CCombFilterIf::CombFilterType_t::kCombFIR, fMaxDelayLengthInS, (float)fSampleRateInHz
		,1
	);
	combFilterIf->setParam(CCombFilterIf::FilterParam_t::kParamGain, -1);

	combFilterIf->process(input, output, nTotal,1);

	bool bCheckResult = true;
	for (int i = nTotalFarwordCycle; i < nTotal && bCheckResult; ++i) {
		if (output[0][i] > 1e-4F) {
			printf("%d: %.8f\n", i, output[0][i]);
			bCheckResult = false;
		}
	}
	delete[] input[0];
	delete[] output[0];
	delete[] input;
	delete[] output;
	CCombFilterIf::destroy(combFilterIf);
	return bCheckResult;
}

//2.IIR: amount of magnitude increase/decrease if input freq matches feedback
bool t2_Is_IIR_magnitude_change_when_Input_match_feedforward() {
	printf("t2_Is_IIR_magnitude_change_when_Input_match_feedforward\n");
	float fMaxDelayLengthInS = 0.01;
	int fSampleRateInHz = 44100;
	int nTotal = fSampleRateInHz;
	int nTotalFarwordCycle = fMaxDelayLengthInS * fSampleRateInHz;  //here let input freq matches feedforward
	float** input = new float*[1];
	float** output = new float*[1];
	input[0] = new float[nTotal];
	output[0] = new float[nTotal];

	for (int i = 0; i < nTotal; ++i) {
		input[0][i] = sin(2 * PI * i / (nTotalFarwordCycle));
	}

	
	/* Get a FIR combfilter */
	CCombFilterIf* combFilterIf = nullptr;
	CCombFilterIf::create(combFilterIf);
	combFilterIf->init(
		CCombFilterIf::CombFilterType_t::kCombIIR, fMaxDelayLengthInS, (float)fSampleRateInHz
		, 1
	);
	combFilterIf->setParam(CCombFilterIf::FilterParam_t::kParamGain, 1);

	combFilterIf->process(input, output, nTotal, 1);


	bool bCheckResult = true;

	for (int i = nTotalFarwordCycle; i < nTotal && bCheckResult; i += nTotalFarwordCycle) {
		int iCycle = i / nTotalFarwordCycle;
		for (int j = 0; j < nTotalFarwordCycle && bCheckResult; ++j) {
			int k = i + j;
			if (abs(output[0][k - nTotalFarwordCycle]) > 1e-5F) {
				if (abs(output[0][k] - output[0][k - nTotalFarwordCycle] - input[0][j]) > 1e-4F) {
					printf("%d: %.6f, %d: %.6f, %d: %.6f\n", k, output[0][k], k - nTotalFarwordCycle, output[0][k - nTotalFarwordCycle], j, input[0][j]);
					bCheckResult = false;
				}
			}
		}
	}
	
	delete[] input[0];
	delete[] output[0];
	delete[] input;
	delete[] output;
	CCombFilterIf::destroy(combFilterIf);

	return bCheckResult;
}

//4 FIR/IIR: correct processing for zero input signal
bool t4_proces_zero_input() {
	printf("t4_proces_zero_input\n");
	float fMaxDelayLengthInS = 0.01;
	int fSampleRateInHz = 44100;
	int nTotal = fSampleRateInHz;
	int nTotalFarwordCycle = 441;//fMaxDelayLengthInS * fSampleRateInHz;  //here let input freq matches feedforward
	float** input = new float*[1];
	float** output = new float*[1];
	input[0] = new float[nTotal];
	output[0] = new float[nTotal];

	memset(input[0], 0, sizeof(float) * nTotal);
	memset(output[0], 0, sizeof(float) * nTotal);

	bool bCheckResult1 = true;
	bool bCheckResult2 = true;
	/* Get a FIR combfilter */
	CCombFilterIf* combFilterIf = nullptr;
	CCombFilterIf::create(combFilterIf);
	combFilterIf->init(
		CCombFilterIf::CombFilterType_t::kCombFIR, fMaxDelayLengthInS, (float)fSampleRateInHz
		, 1
	);

	combFilterIf->setParam(CCombFilterIf::FilterParam_t::kParamGain, -1);
	combFilterIf->process(input, output, nTotal,1);

	for (int i = 0; i < nTotal && bCheckResult1; ++i) {
		if (output[0][i] > 1e-4F) {
			std::printf("%d: %.8f\n", i, output[0][i]);
			bCheckResult1 = false;
		}
	}
	

	memset(input[0], 0, sizeof(float) * nTotal);
	memset(output[0], 0, sizeof(float) * nTotal);

	/* Get a FIR combfilter */
	CCombFilterIf* combFilterIf2 = nullptr;
	CCombFilterIf::create(combFilterIf2);
	combFilterIf2->init(
		CCombFilterIf::CombFilterType_t::kCombIIR, fMaxDelayLengthInS, (float)fSampleRateInHz
		, 1
	);

	combFilterIf2->setParam(CCombFilterIf::FilterParam_t::kParamGain, -1);
	combFilterIf2->process(input, output, nTotal, 1);

	for (int i = 0; i < nTotal && bCheckResult2; ++i) {
		if (output[0][i] > 1e-4F) {
			std::printf("%d: %.8f\n", i, output[0][i]);
			bCheckResult2 = false;
		}
	}

	
	delete[] input[0];
	delete[] output[0];
	delete[] input;
	delete[] output;
	CCombFilterIf::destroy(combFilterIf);
	CCombFilterIf::destroy(combFilterIf2);

	return bCheckResult1 && bCheckResult2;
}


//3.FIR/IIR: correct result for VARYING input block size
bool t3_Is_correct_when_varying_input_block_size() {
	printf("t3_Is_correct_when_varying_input_block_size\n");
	float fMaxDelayLengthInS = 0.01;
	int fSampleRateInHz = 44100;
	int nTotal = 10 * fSampleRateInHz;
	int nTotalFarwordCycle = 441;//fMaxDelayLengthInS * fSampleRateInHz;  //here let input freq matches feedforward
	float** input = new float*[1];
	float** output = new float*[1];
	float** refOutput = new float*[1];
	float** tempInput = new float*[1];
	float** tempOutput = new float*[1];
	input[0] = new float[nTotal];
	output[0] = new float[nTotal];
	refOutput[0] = new float[nTotal];
	tempInput[0]= new float[nTotal];
	tempOutput[0] = new float[nTotal];
	bool bCheckResult1 = true;
	bool bCheckResult2 = true;

	for (int i = 0; i < nTotal; ++i) {
		input[0][i] = sin(2 * PI * i / (nTotalFarwordCycle));
	}

	/* Get a FIR combfilter */
	CCombFilterIf* combFilterIf = nullptr;
	CCombFilterIf::create(combFilterIf);
	combFilterIf->init(
		CCombFilterIf::CombFilterType_t::kCombFIR, fMaxDelayLengthInS, (float)fSampleRateInHz
		, 1
	);
	combFilterIf->setParam(CCombFilterIf::FilterParam_t::kParamGain, 1);
	combFilterIf->process(input, output, nTotal, 1);

	

	memset(tempInput[0], 0, sizeof(float) * nTotal);
	memset(tempOutput[0], 0, sizeof(float) * nTotal);


	for (int blockSize = 1, i = 0; i < nTotal && blockSize < 65536; i += blockSize, blockSize <<= 1) {
		tempInput[0] = input[0] + i;
		tempOutput[0] = output[0] + i;
		combFilterIf->process(tempInput, tempOutput, blockSize,1);
	}

	combFilterIf->setParam(CCombFilterIf::FilterParam_t::kParamDelay, 1.0*nTotalFarwordCycle / fSampleRateInHz);
	combFilterIf->process(input, refOutput, nTotal,1);

	for (int i = 0; i < nTotal && bCheckResult1; ++i) {
		if (abs(output[0][i] - refOutput[0][i]) > 1e-8) {
			printf("output[0][%d]: %.8f, refOutput[0][%d]: %.8f\n", i, output[0][i], i, refOutput[0][i]);
			bCheckResult1 = false;
		}
	}
	CCombFilterIf::destroy(combFilterIf);

	{
		/* Get a IIR combfilter */
		CCombFilterIf* combFilterIf = nullptr;
		CCombFilterIf::create(combFilterIf);
		combFilterIf->init(
			CCombFilterIf::CombFilterType_t::kCombIIR, fMaxDelayLengthInS, (float)fSampleRateInHz
			, 1
		);
		combFilterIf->setParam(CCombFilterIf::FilterParam_t::kParamGain, 1);
		combFilterIf->process(input, output, nTotal, 1);



		memset(tempInput[0], 0, sizeof(float) * nTotal);
		memset(tempOutput[0], 0, sizeof(float) * nTotal);


		for (int blockSize = 1, i = 0; i < nTotal && blockSize < 65536; i += blockSize, blockSize <<= 1) {
			tempInput[0] = input[0] + i;
			tempOutput[0] = output[0] + i;
			combFilterIf->process(tempInput, tempOutput, blockSize, 1);
		}

		combFilterIf->setParam(CCombFilterIf::FilterParam_t::kParamDelay, 1.0*nTotalFarwordCycle / fSampleRateInHz);
		combFilterIf->process(input, refOutput, nTotal, 1);

		for (int i = 0; i < nTotal && bCheckResult2; ++i) {
			if (abs(output[0][i] - refOutput[0][i]) > 1e-8) {
				printf("output[0][%d]: %.8f, refOutput[0][%d]: %.8f\n", i, output[0][i], i, refOutput[0][i]);
				bCheckResult2 = false;
			}
		}
		CCombFilterIf::destroy(combFilterIf);
	}

	delete[] input[0];
	delete[] output[0];
	delete[] tempInput[0];
	delete[] tempOutput[0];
	delete[] refOutput[0];
	delete[] input;
	delete[] output;
	delete[] tempInput;
	delete[] tempOutput;
	delete[] refOutput;
	return  bCheckResult1 && bCheckResult2;;
}

void runDefaultTest()
{
	t1_Is_FIR_Zero_when_Input_match_feedforward();
	//t2_Is_IIR_magnitude_change_when_Input_match_feedforward();
	
	t4_proces_zero_input();
	t3_Is_correct_when_varying_input_block_size();
}
 