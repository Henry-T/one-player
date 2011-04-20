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
	CDirectSound();						// 初始化成员
	~CDirectSound();					// 清理

	// ==============================================================================
	// DirectSound核心对象创建和使用
	// ==============================================================================
	BOOL CreateDirectSound(LPWAVEFORMATEX WaveHead);//, DWORD dwHeadLen);	// 创建缓冲区
	BOOL IsValid() const;								// 确定音频设备和住缓冲区是否就位
	BOOL WriteToBuf();									// 写入缓冲
	int WriteDataToBuf(BYTE* pBuf, DWORD dwLen);		// 写数据到缓冲区
	void Play(INT nTime = 0, BOOL bLoop = TRUE);		// 开始播放
	void Stop();										// 停止播放
	void Pause();										// 暂停播放
	void Continue();									// 继续播放

	// ==============================================================================
	// 声音文件处理
	// ==============================================================================
	// 获取声音数据
	BOOL GetWaveData(void *pHeader, WAVEFORMATEX * & pWaveHeader, DWORD &dwHeadSize);
	BOOL GetWaveData(void *pRes, WAVEFORMATEX * & pWaveHeader, void * & pbWaveData, DWORD & cbWaveSize);
	// 填充声音数据
	BOOL SetSoundData(void *pSoundData, DWORD SoundSize);
	BOOL SetSoundData(DWORD Offset, void * pSoundData, DWORD dwSoundSize);

	// ==============================================================================
	// 常规设定
	// ==============================================================================
	BOOL SetPlaySpeed(float fSpeed);				// 设置播放速度

	// ==============================================================================
	// 3D音效设定
	// ==============================================================================
	BOOL SetListenerPosition(D3DVALUE x, D3DVALUE y, D3DVALUE z);			// 设置听者位置
	BOOL SetListenerVelocity(D3DVALUE x, D3DVALUE y, D3DVALUE z);			// 设置听者速度
	BOOL SetListenerOrientation(D3DVALUE x, D3DVALUE y, D3DVALUE z, D3DVALUE topX, D3DVALUE topY, D3DVALUE topZ);	// 设置听者朝向

	BOOL SetDistanceFactor(D3DVALUE flDistanceFactor);		// 设置距离因子
	BOOL GetDistanceFactor(D3DVALUE *flDistanceFactor);		// 获取距离因子

	BOOL SetDopplerFactor(D3DVALUE flDopplerFactor);		// 设置多普勒因子
	BOOL GetDopplerFactor(D3DVALUE *flDopplerFactor);		// 获取多普勒因子

	BOOL SetRooloffFactor(D3DVALUE flRooloffFactor);		// 设置滚降因子
	BOOL GetRooloffFactor(D3DVALUE *flRooloffFactor);		// 获取滚降因子

public:
	DSBPOSITIONNOTIFY DSNotify[16];
	WAVEFORMATEX m_waveFormat;
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

private:	
	static LPDIRECTSOUND m_lpDirectSound;			// DSound设备对象
	static LPDIRECTSOUNDBUFFER m_pDSBuffer;			// DSound主缓冲区
	static LPDIRECTSOUND3DBUFFER8 m_pDS3DBuffer;	// DSound次级3D缓冲
	static DS3DBUFFER m_DS3DBParams;				// 3D缓冲区参数
	static LPDIRECTSOUND3DLISTENER8 m_pDS3DListener;// 3D听者
	static DS3DLISTENER m_Ds3dblParams;				// 3D听者参数
	static LPDIRECTSOUNDNOTIFY8 m_pDSNotify;		// DSound通知
	static DWORD m_dwInstances;						// DSound设备使用计数
};
