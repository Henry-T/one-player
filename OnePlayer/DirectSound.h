#pragma once

#include <mmsystem.h>
#include <dsound.h>

#define BUFSIZE 400000
enum AUDIO_FORMAT
{
	AUDIO_FORMAT_PCM = 1,
	AUDIO_FORMAT_ADPCM_MS = 2,
	AUDIO_FORMAT_ADPCM_IMA,
	AUDIO_FORMAT_ALAW,
	AUDIO_FORMAT_ULAW,
};

typedef struct _AUDIO_CONFIG
{
	WORD wFormatTag;
	WORD nChannels;
	DWORD nSamplesPerSec;
	DWORD nAvgBytesPerSec; 
	WORD  nBlockAlign; 
	WORD  wBitsPerSample; 
	WORD  cbSize; 
} AUDIO_CONFIG; 


class CDirectSound
{
public:
	// ����DirectSoundBuffer�ӿڣ��Գ�Ա������ʼ��
	CDirectSound();
	// �ͷų�Ա��������ͻ������ռ�
	~CDirectSound();

	BOOL WriteToBuf();
	// д���ݵ�������
	int WriteDataToBuf(BYTE* pBuf, DWORD dwLen);
	// ����������
	BOOL CreateDSound(AUDIO_CONFIG* WaveHead, DWORD dwHeadLen);
	// ���ò����ٶ�
	BOOL SetPlaySpeed(float fSpeed);

public:
	void Play(INT nTime = 0, BOOL bLoop = TRUE);
	void Stop();
	void Pause();
	void Continue();

public:
	BOOL IsValid() const;
	// ��ȡ��������
	BOOL GetWaveData(void *pHeader, WAVEFORMATEX * & pWaveHeader, DWORD &dwHeadSize);
	BOOL GetWaveData(void *pRes, WAVEFORMATEX * & pWaveHeader, void * & pbWaveData, DWORD & cbWaveSize);
	// �����������
	BOOL SetSoundData(void *pSoundData, DWORD SoundSize);
	BOOL SetSoundData(DWORD Offset, void * pSoundData, DWORD dwSoundSize);

	// ����������
	BOOL CreateSoundBuffer(WAVEFORMATEX * pcmwf);
	// 3D��Ч�趨
	BOOL SetListenerPosition(D3DVALUE x, D3DVALUE y, D3DVALUE z);
	BOOL SetListenerVelocity(D3DVALUE x, D3DVALUE y, D3DVALUE z);
	BOOL SetListenerOrientation(D3DVALUE x, D3DVALUE y, D3DVALUE z, D3DVALUE topX, D3DVALUE topY, D3DVALUE topZ);

	BOOL SetDistanceFactor(D3DVALUE flDistanceFactor);
	BOOL GetDistanceFactor(D3DVALUE *flDistanceFactor);
	
	BOOL SetDopplerFactor(D3DVALUE flDopplerFactor);
	BOOL GetDopplerFactor(D3DVALUE *flDopplerFactor);
	
	BOOL SetRooloffFactor(D3DVALUE flRooloffFactor);
	BOOL GetRooloffFactor(D3DVALUE *flRooloffFactor);

public:
	DSBPOSITIONNOTIFY DSNotify[16];
	AUDIO_CONFIG m_config;
	float m_fRate;
	int m_rate;
	int i;
	CString file[255];
	CString filemp;
	DWORD m_nSamplesPerSec;			// ������
	DWORD m_nAvgBytesPerSec;		// ÿ������ֽ���
	DWORD m_dwTotalSize;			// ??? δʹ��
	DWORD m_dwHeadLen;				// ͷ������
	DWORD m_dwBufNum;				// ����Ƭ������
	DWORD m_dwBufLen;				// ����������
	DWORD m_dwBufSize;				// ��������С
	DWORD m_dwOffset;				// ƫ����

	HANDLE notify_events[16];		// ֪ͨ�¼�
	HANDLE m_hPlayThread;			// �����߳�
	HANDLE m_sysncObj;				// �ļ������߳�
	HANDLE m_hEvent;					

	HWND m_hWnd;					// ���ھ����ΪʲôҪ����???
	BYTE* m_pTheSound;				// �����ļ������ڴ�
	bool m_bStop;					// ֹͣ���ź�����Ա����
	bool m_bFirst;					// �Ƿ��״�д�뻺��
	bool m_bEnabled;				// �Ƿ����
	bool m_bPlay;					// ���ź�����Ա����
	bool m_play_flag, m_pause_flag;	// ���ź���ͣ��ʶ��

	// 3D����
	D3DVALUE m_fDopplerFactor;
	D3DVALUE m_fRolloffFactor;
	D3DVALUE m_fMinDistance;
	D3DVALUE m_fMaxDistance;

public:		// TODO ��ʱ����
	static LPDIRECTSOUND m_lpDirectSound;
	static LPDIRECTSOUNDBUFFER m_pDsb;
	static LPDIRECTSOUND3DBUFFER8 m_pDs3db;
	static DS3DBUFFER m_Ds3dbParams;			// 3D����������
	static LPDIRECTSOUND3DLISTENER8 m_pDs3dl;
	static DS3DLISTENER m_Ds3dblParams;
	static LPDIRECTSOUNDNOTIFY8 m_pDsNotify;
	static DWORD m_dwInstances;
};
