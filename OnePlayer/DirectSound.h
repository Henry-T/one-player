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
	// 创建DirectSoundBuffer接口，对成员变量初始化
	CDirectSound();
	// 释放成员变量对象和缓冲区空间
	~CDirectSound();

	BOOL WriteToBuf();
	// 写数据到缓冲区
	int WriteDataToBuf(BYTE* pBuf, DWORD dwLen);
	// 创建缓冲区
	BOOL CreateDSound(AUDIO_CONFIG* WaveHead, DWORD dwHeadLen);
	// 设置播放速度
	BOOL SetPlaySpeed(float fSpeed);

public:
	void Play(INT nTime = 0, BOOL bLoop = TRUE);
	void Stop();
	void Pause();
	void Continue();

protected:
	BOOL IsValid() const;
	// 获取声音数据
	BOOL GetWaveData(void *pHeader, WAVEFORMATEX * & pWaveHeader, DWORD &dwHeadSize);
	BOOL GetWaveData(void *pRes, WAVEFORMATEX * & pWaveHeader, void * & pbWaveData, DWORD & cbWaveSize);
	// 填充声音数据
	BOOL SetSoundData(void *pSoundData, DWORD SoundSize);
	BOOL SetSoundData(DWORD Offset, void * pSoundData, DWORD dwSoundSize);

	// 创建缓冲区
	BOOL CreateSoundBuffer(WAVEFORMATEX * pcmwf);
	// 3D音效设定
	BOOL SetListenerPosition(D3DVALUE x, D3DVALUE y, D3DVALUE z);
	BOOL SetListenerVelocity(D3DVALUE x, D3DVALUE y, D3DVALUE z);
	BOOL SetListenerOrientation(D3DVALUE x, D3DVALUE y, D3DVALUE z, D3DVALUE topX, D3DVALUE topY, D3DVALUE topZ);

	BOOL SetDistanceFactor(D3DVALUE flDistanceFactor);
	BOOL GetDistanceFactor(D3DVALUE *flDistanceFactor);
	
	BOOL SetDopplerFactor(D3DVALUE flDopplerFactor);
	BOOL GetDopplerFactor(D3DVALUE *flDopplerFactor);
	
	BOOL SetRooloffFactor(D3DVALUE flRooloffFactor);
	BOOL GetRooloffFactor(D3DVALUE *flRooloffFactor);

private:
	DSBPOSITIONNOTIFY DSNotify[16];
	AUDIO_CONFIG m_config;
	float m_fRate;
	int m_rate;
	int i;
	CString file[255];
	CString filemp;
	DWORD m_nSamplesPerSec;			// 采样率
	DWORD m_nAvgBytesPerSec;		// 每秒采样字节数
	DWORD m_dwTotalSize;			// ??? 未使用
	DWORD m_dwHeadLen;				// 头部长度
	DWORD m_dwBufNum;				// 缓冲片段数量
	DWORD m_dwBufLen;				// 缓冲区长度
	DWORD m_dwBufSize;				// 缓冲区大小
	DWORD m_dwOffset;				// 偏移量

	HANDLE notify_events[16];		// 通知事件
	HANDLE m_hPlayThread;			// 播放线程
	HANDLE m_sysncObj;				// 文件载入线程
	HANDLE m_hEvent;					

	HWND m_hWnd;					// 窗口句柄，为什么要定义???
	BYTE* m_pTheSound;				// 声音文件填充的内存
	bool m_bStop;					// 停止播放函数成员变量
	bool m_bFirst;					// 是否首次写入缓冲
	bool m_bEnabled;				// 是否可用
	bool m_bPlay;					// 播放函数成员变量
	bool m_play_flag, m_pause_flag;	// 播放和暂停标识符

	// 3D参数
	D3DVALUE m_fDopplerFactor;
	D3DVALUE m_fRolloffFactor;
	D3DVALUE m_fMinDistance;
	D3DVALUE m_fMaxDistance;

public:		// TODO 临时公开
	static LPDIRECTSOUND m_lpDirectSound;
	static LPDIRECTSOUNDBUFFER m_pDsb;
	static LPDIRECTSOUND3DBUFFER8 m_pDs3db;
	static DS3DBUFFER m_Ds3dbParams;			// 3D缓冲区参数
	static LPDIRECTSOUND3DLISTENER8 m_pDs3dl;
	static LPDIRECTSOUNDNOTIFY8 m_pDsNotify;
	static DWORD m_dwInstances;
};
