
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

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
/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string sInputFilePath,                 //!< file paths
                sOutputFilePath,
				sOutputWavFilePath,
				sPath;
	sPath = ".\\data\\";
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
		errorMsg("Wrong number of arguments.");
		return -1;
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

