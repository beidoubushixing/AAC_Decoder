#include "DecoderAAC.h"
#include <stdio.h>

namespace MediaSDK
{

	DecoderAAC::DecoderAAC() : m_hDecoder(nullptr)
	{


		TRANSPORT_TYPE transportType = TT_MP4_ADTS;
		m_hDecoder = aacDecoder_Open(transportType, 1);

		if (m_hDecoder == nullptr)
		{

		}
	}


	DecoderAAC::~DecoderAAC()
	{

		if (m_hDecoder != nullptr)
		{
			aacDecoder_Close(m_hDecoder);
		}
	}


	short DecoderAAC::ByteArrayToShortBE(unsigned char *byteArray)
	{
		short num;

		num = num & 0x0000;
		num = num | (byteArray[0] << 8);
		num = num | byteArray[1];

		return num;
	}


	int DecoderAAC::ThreeBytesIntoIntBE(unsigned char *byteArray){
		return (byteArray[2] & 0xFF) | ((byteArray[1] & 0xFF) << 8) | ((byteArray[0] & 0xFF) << 16);
	}


	void DecoderAAC::CreateConfBuf(int sampleRate, int numOfChannels, unsigned char *conf)
	{
		short sampleMode = 0;

		switch (sampleRate)
		{
		case 96000:
			sampleMode = 0;
			break;

		case 88200:
			sampleMode = 1;
			break;

		case 64000:
			sampleMode = 2;
			break;

		case 48000:
			sampleMode = 3;
			break;

		case 44100:
			sampleMode = 4;
			break;

		case 32000:
			sampleMode = 5;
			break;

		case 24000:
			sampleMode = 6;
			break;

		case 22050:
			sampleMode = 7;
			break;

		case 16000:
			sampleMode = 8;
			break;

		case 12000:
			sampleMode = 9;
			break;

		case 11025:
			sampleMode = 10;
			break;

		case 8000:
			sampleMode = 11;
			break;

		case 7350:
			sampleMode = 12;
			break;

		default:

			break;
		}

		short channelMode = numOfChannels;
		if (numOfChannels == 8)	{
			channelMode = 7;
		}

		short config = 0;
		config |= 2 << (sizeof(short) * 8 - 5);                                           // 5 bits object type , 2 == AAC LC
		config |= sampleMode << (sizeof(short) * 8 - 5 - 4);                              // 4 bits frequency type
		config |= channelMode << (sizeof(short) * 8 - 5 - 4 - 4);                         // 4 bits channel configuration

		conf[0] = config >> 8;
		conf[1] = config;
	}


	bool DecoderAAC::SetParameters(int sampleRate, int numberOfChannels)
	{
		

		if (aacDecoder_SetParam(m_hDecoder, AAC_PCM_OUTPUT_INTERLEAVED, 1))
		{
			
			return false;
		}

		if (aacDecoder_SetParam(m_hDecoder, AAC_PCM_MIN_OUTPUT_CHANNELS, numberOfChannels))
		{
			
			return false;
		}

		if (aacDecoder_SetParam(m_hDecoder, AAC_PCM_MAX_OUTPUT_CHANNELS, numberOfChannels))
		{
			
			return false;
		}

		if (aacDecoder_SetParam(m_hDecoder, AAC_PCM_OUTPUT_CHANNEL_MAPPING, 1))
		{
			
			return false;
		}

		const unsigned int confSize = 2;
		unsigned char conf[confSize];
		unsigned char *conf2 = conf;

		CreateConfBuf(sampleRate, numberOfChannels, conf);

		if (aacDecoder_ConfigRaw(m_hDecoder, &conf2, &confSize))
		{
			
			return false;
		}

		

		return true;
	}


	int DecoderAAC::DecodeAudio(unsigned char *inputDataBuffer, unsigned int inputDataSize, short *outputDataBuffer)
	{

		m_nRC = aacDecoder_Fill(m_hDecoder, &inputDataBuffer, (const unsigned int *)&inputDataSize, (unsigned int *)&inputDataSize);

		if (m_nRC != AAC_DEC_OK)
		{
			printf("aacDecoder_Fill\n");
			return false;
		}

		m_nRC = aacDecoder_DecodeFrame(m_hDecoder, outputDataBuffer, AUDIO_MAX_FRAME_LENGTH_IN_BYTE, 0);

		if (m_nRC != AAC_DEC_OK)
		{
			printf("aacDecoder_DecodeFrame\n");
			return false;
		}
		CStreamInfo* sInfo = aacDecoder_GetStreamInfo(m_hDecoder);


		printf("#aac# SampleRate: %d\n", sInfo->sampleRate);
		return sInfo->frameSize * sInfo->numChannels;
	}

} //namespace MediaSDK
