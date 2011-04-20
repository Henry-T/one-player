#pragma once

#include <mmsystem.h>
#include <dsound.h>

#define BUFSIZE 400000
//enum AUDIO_FORMAT
//{
//	AUDIO_FORMAT_PCM = 1,
//	AUDIO_FORMAT_ADPCM_MS = 2,
//	AUDIO_FORMAT_ADPCM_IMA,
//	AUDIO_FORMAT_ALAW,
//	AUDIO_FORMAT_ULAW,
//};

class CDirectSound
{
public:
	CDirectSound();						// ��ʼ����Ա
	~CDirectSound();					// ����

	// ==============================================================================
	// DirectSound���Ķ��󴴽���ʹ��
	// ==============================================================================
	BOOL CreateDirectSound(LPWAVEFORMATEX WaveHead);//, DWORD dwHeadLen);	// ����������
	BOOL IsValid() const;								// ȷ����Ƶ�豸��ס�������Ƿ��λ
	BOOL WriteToBuf();									// д�뻺��
	int WriteDataToBuf(BYTE* pBuf, DWORD dwLen);		// д���ݵ�������
	void Play(INT nTime = 0, BOOL bLoop = TRUE);		// ��ʼ����
	void Stop();										// ֹͣ����
	void Pause();										// ��ͣ����
	void Continue();									// ��������

	// ==============================================================================
	// �����ļ�����
	// ==============================================================================
	// ��ȡ��������
	BOOL GetWaveData(void *pHeader, WAVEFORMATEX * & pWaveHeader, DWORD &dwHeadSize);
	BOOL GetWaveData(void *pRes, WAVEFORMATEX * & pWaveHeader, void * & pbWaveData, DWORD & cbWaveSize);
	// �����������
	BOOL SetSoundData(void *pSoundData, DWORD SoundSize);
	BOOL SetSoundData(DWORD Offset, void * pSoundData, DWORD dwSoundSize);

	// ==============================================================================
	// �����趨
	// ==============================================================================
	BOOL SetPlaySpeed(float fSpeed);				// ���ò����ٶ�

	// ==============================================================================
	// 3D��Ч�趨
	// ==============================================================================
	BOOL SetListenerPosition(D3DVALUE x, D3DVALUE y, D3DVALUE z);			// ��������λ��
	BOOL SetListenerVelocity(D3DVALUE x, D3DVALUE y, D3DVALUE z);			// ���������ٶ�
	BOOL SetListenerOrientation(D3DVALUE x, D3DVALUE y, D3DVALUE z, D3DVALUE topX, D3DVALUE topY, D3DVALUE topZ);	// �������߳���

	BOOL SetDistanceFactor(D3DVALUE flDistanceFactor);		// ���þ�������
	BOOL GetDistanceFactor(D3DVALUE *flDistanceFactor);		// ��ȡ��������

	BOOL SetDopplerFactor(D3DVALUE flDopplerFactor);		// ���ö���������
	BOOL GetDopplerFactor(D3DVALUE *flDopplerFactor);		// ��ȡ����������

	BOOL SetRooloffFactor(D3DVALUE flRooloffFactor);		// ���ù�������
	BOOL GetRooloffFactor(D3DVALUE *flRooloffFactor);		// ��ȡ��������

public:
	DSBPOSITIONNOTIFY DSNotify[16];
	WAVEFORMATEX m_waveFormat;
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

private:	
	static LPDIRECTSOUND m_lpDirectSound;			// DSound�豸����
	static LPDIRECTSOUNDBUFFER m_pDSBuffer;			// DSound��������
	static LPDIRECTSOUND3DBUFFER8 m_pDS3DBuffer;	// DSound�μ�3D����
	static DS3DBUFFER m_DS3DBParams;				// 3D����������
	static LPDIRECTSOUND3DLISTENER8 m_pDS3DListener;// 3D����
	static DS3DLISTENER m_Ds3dblParams;				// 3D���߲���
	static LPDIRECTSOUNDNOTIFY8 m_pDSNotify;		// DSound֪ͨ
	static DWORD m_dwInstances;						// DSound�豸ʹ�ü���
};
