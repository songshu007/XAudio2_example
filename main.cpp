#include <Windows.h>
#include <xaudio2.h>
#include "WavFile.h"

int main()
{
	HRESULT hr;
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (FAILED(hr))
		return hr;

	IXAudio2* pXAudio2 = nullptr;
	if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
		return hr;

	IXAudio2MasteringVoice* pMasterVoice = nullptr;
	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
		return hr;

	// ½âÎö wave
	XAUDIO2_BUFFER buffer = { 0 };
	WAVEFORMATEXTENSIBLE wfx = { 0 };
	WavFile Wav;
	Wav.LoadFromFile(L"C:\\Users\\Lenovo\\OneDrive\\×ÀÃæ\\Music\\sample1.wav");
	buffer.AudioBytes = Wav.GetHeadData()->dwDataSize;
	buffer.pAudioData = Wav.GetDataBuffer();
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

	wfx.Format.nChannels = Wav.GetHeadData()->usNumChannels;
	wfx.Format.nSamplesPerSec = Wav.GetHeadData()->dwSampleRate;
	wfx.Format.nAvgBytesPerSec = Wav.GetHeadData()->dwBytesPerSec;
	wfx.Format.nBlockAlign = Wav.GetHeadData()->usBlockAlign;
	wfx.Format.wBitsPerSample = Wav.GetHeadData()->usBitsPerSamp;
	wfx.Format.cbSize = Wav.GetHeadData()->dwDataSize;
	wfx.Format.wFormatTag = Wav.GetHeadData()->usAudioFormat;

	IXAudio2SourceVoice* pSourceVoice;
	if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx))) 
		return hr;

	if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer)))
		return hr;
	
	if (FAILED(hr = pSourceVoice->Start(0)))
		return hr;

	Sleep(1000000);

	return 0;
}