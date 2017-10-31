// AACTest.cpp : Defines the entry point for the console application.
//

#include "DecoderAAC.h"
#include <stdio.h>

#define SAMPLE_RATE 48000
#define NUMBER_OF_CHANNELS 2

using namespace MediaSDK;

int main()
{
	unsigned char ucaEncodedFrame[10000];
	int iEncodedFrameSize, iDecodedFrameSize=0;
	short saDecodedFrame[10000];
	FILE *encodedFile;
	FILE *decodedFile;
	encodedFile = fopen("EncodedWithAACDump.aac", "rb");
	decodedFile = fopen("DecodedWithAAC.pcm", "wb");

	//AAC
	DecoderAAC *m_cAacDecoder = new DecoderAAC();
	m_cAacDecoder->SetParameters(SAMPLE_RATE, NUMBER_OF_CHANNELS);
	printf("start decoding\n");
	//while (fread(&iEncodedFrameSize, sizeof(int), 1, encodedFile)==1)
	//{
	//	printf("EncodedFrameSize = %d\n", iEncodedFrameSize);
	//	fread(ucaEncodedFrame, sizeof(char), iEncodedFrameSize, encodedFile);
	//	iDecodedFrameSize = m_cAacDecoder->DecodeAudio(ucaEncodedFrame, iEncodedFrameSize, saDecodedFrame);
	//	fwrite(saDecodedFrame, sizeof(short), iDecodedFrameSize, decodedFile);
	//}
	fseek(encodedFile, 0, SEEK_END);
	long long temp = ftell(encodedFile);
	printf("pointer = %lld\n", temp);
	fseek(encodedFile, 0, SEEK_SET);
	long long temp2 = ftell(encodedFile);
	printf("pointer 2 = %lld\n", temp2);
	iEncodedFrameSize = 300;
		for (int i = 0; i < temp; i += iEncodedFrameSize){
			fread(ucaEncodedFrame, sizeof(char), iEncodedFrameSize, encodedFile);
			iDecodedFrameSize = m_cAacDecoder->DecodeAudio(ucaEncodedFrame, iEncodedFrameSize, saDecodedFrame);
			if (iDecodedFrameSize)fwrite(saDecodedFrame, sizeof(short), iDecodedFrameSize, decodedFile);
		}

	printf("stop decoding\n");
	fclose(encodedFile);
	fclose(decodedFile);



	return 0;
}

