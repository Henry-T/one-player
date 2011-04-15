#include "StdAfx.h"
#include "DirectSound.h"

#pragma comment(lib, "dsound.lib")
#pragma comment(lib,"dxguid.lib")

void TKL_Trace(char * pFormat, ...)
{
	// TODO �������߼�
}
static void DSError( HRESULT hRes ) {
	switch(hRes) {
	case DS_OK: TKL_Trace("NO ERROR\n"); break;
	case DSERR_ALLOCATED: TKL_Trace("ALLOCATED\n"); break;
	case DSERR_INVALIDPARAM: TKL_Trace("INVALIDPARAM\n"); break;
	case DSERR_OUTOFMEMORY: TKL_Trace("OUTOFMEMORY\n"); break;
	case DSERR_UNSUPPORTED: TKL_Trace("UNSUPPORTED\n"); break;
	case DSERR_NOAGGREGATION: TKL_Trace("NOAGGREGATION\n"); break;
	case DSERR_UNINITIALIZED: TKL_Trace("UNINITIALIZED\n"); break;
	case DSERR_BADFORMAT: TKL_Trace("BADFORMAT\n"); break;
	case DSERR_ALREADYINITIALIZED: TKL_Trace("ALREADYINITIALIZED\n"); break;
	case DSERR_BUFFERLOST: TKL_Trace("BUFFERLOST\n"); break;
	case DSERR_CONTROLUNAVAIL: TKL_Trace("CONTROLUNAVAIL\n"); break;
	case DSERR_GENERIC: TKL_Trace("GENERIC\n"); break;
	case DSERR_INVALIDCALL: TKL_Trace("INVALIDCALL\n"); break;
	case DSERR_OTHERAPPHASPRIO: TKL_Trace("OTHERAPPHASPRIO\n"); break;
	case DSERR_PRIOLEVELNEEDED: TKL_Trace("PRIOLEVELNEEDED\n"); break;
	default: TKL_Trace("%lu\n",hRes);break;
	}
}


LPDIRECTSOUND CDirectSound::m_lpDirectSound;
LPDIRECTSOUNDBUFFER  CDirectSound::m_pDsb;
LPDIRECTSOUND3DBUFFER8  CDirectSound::m_pDs3db;
DS3DBUFFER CDirectSound::m_Ds3dbParams;			// 3D����������
LPDIRECTSOUND3DLISTENER8 CDirectSound::m_pDs3dl;
LPDIRECTSOUNDNOTIFY8 CDirectSound::m_pDsNotify;
DWORD CDirectSound::m_dwInstances;

CDirectSound::CDirectSound()
{
	::ZeroMemory(&m_config, sizeof(AUDIO_CONFIG));

	m_pDs3db = 0;
	m_pDs3db = 0;
	m_bFirst = TRUE; 
	m_lpDirectSound = 0;
	m_pDsb = 0;
	m_pTheSound = 0;
	m_bEnabled = TRUE;

	m_bPlay = 0;
	++m_dwInstances;
	m_hPlayThread = 0;
	m_dwBufNum = 4;
	m_dwBufLen = 4192; // 8192;
	m_dwOffset = 0;

	m_dwBufSize = 0;
	m_bStop = 0;
	m_pTheSound = (BYTE*)malloc(BUFSIZE);

	m_sysncObj = CreateMutex(0,0,0);

}

CDirectSound::~CDirectSound()
{
	if(m_hPlayThread)
	{
		WaitForSingleObject(m_hPlayThread,2000);
		CloseHandle(m_hPlayThread);
		m_hPlayThread = NULL;
	}

	free(m_pTheSound);

	if(m_pDs3db)
	{
		m_pDs3db->Release();
		m_pDs3db = NULL;
	}
	if(m_pDsb)
	{
		m_pDsb->Release();
		m_pDsb = NULL;
	}


	if( !--m_dwInstances && m_lpDirectSound ) {
		m_lpDirectSound->Release();
		m_lpDirectSound = 0;
	}

	if(m_sysncObj)
	{
		CloseHandle(m_sysncObj);
		m_sysncObj = NULL;
	}
}

unsigned int WINAPI PlaySoundProc(LPVOID pOwner)
{
	CDirectSound* pThis = (CDirectSound *) pOwner;
	pThis->WriteToBuf();
	return 1;
}
// ���崴��������������
BOOL CDirectSound::CreateDSound(AUDIO_CONFIG* WaveHead, DWORD dwHeadLen)
{
	if(m_config.nSamplesPerSec == NULL)
	{
		m_fRate = 1.0f;
		m_config = *WaveHead;							// Wave ͷ�ļ�ָ��
		m_nAvgBytesPerSec = m_config.nAvgBytesPerSec;	// ����ƽ��ÿ������ֽ���
		m_nSamplesPerSec = m_config.nSamplesPerSec;		// ������
	}

	m_dwBufLen = dwHeadLen;
	if(m_lpDirectSound)
	{
		return true;
	}

	HRESULT hRes = DS_OK;
	short nRes = 0;
	do 
	{
		if(nRes)
			::Sleep(500);
		hRes = ::DirectSoundCreate(0, &m_lpDirectSound, 0); 
		++nRes;
	} while (nRes < 10 && (hRes == DSERR_ALLOCATED || hRes == DSERR_NODRIVER));

	if(hRes != DS_OK)
		return FALSE;

	// ����Э��������ΪDSSCL_NORMAL
	m_lpDirectSound->SetCooperativeLevel(::GetDesktopWindow(), DSSCL_NORMAL);
	m_rate = WaveHead->nAvgBytesPerSec;

	// �������������ø�ʽ����
	if(!CreateSoundBuffer((WAVEFORMATEX*)(&m_config)))
		return false;

	m_bStop = 0;
	m_dwBufSize = 0;

	unsigned int dwID;
	m_hPlayThread = (HANDLE)_beginthreadex(0,0,PlaySoundProc, this, 0, &dwID);
	return TRUE;
}


// �����ȡWave���ݺ���
BOOL CDirectSound::GetWaveData(void *pRes, WAVEFORMATEX * & pWaveHeader, void * & pbWaveData, DWORD & cbWaveSize)
{
	pWaveHeader = 0;
	pbWaveData = 0;
	cbWaveSize = 0;
	DWORD *pdw = (DWORD *)pRes;
	DWORD dwRiff = *pdw++;
	DWORD dwLength = *pdw++;
	DWORD dwType = *pdw++;

	if( dwRiff != mmioFOURCC('R', 'I', 'F', 'F') )
		return FALSE;      // ���� RIFF

	if( dwType != mmioFOURCC('W', 'A', 'V', 'E') )
		return FALSE;      // ���� WAV

	DWORD * pdwEnd = (DWORD *)((BYTE *)pdw + 1000-4);

	while( pdw < pdwEnd ) {
		dwType = *pdw++;
		dwLength = *pdw++;

		switch( dwType ) {
			case mmioFOURCC('f', 'm', 't', ' '):
				if( !pWaveHeader ) {
					if( dwLength < sizeof(WAVEFORMAT) )
						return FALSE;      // ���� WAV

					pWaveHeader = (WAVEFORMATEX *)pdw;

					if( pbWaveData && cbWaveSize )
						return TRUE;
				}
				break;

			case mmioFOURCC('d', 'a', 't', 'a'):
				pbWaveData = LPVOID(pdw);
				cbWaveSize = dwLength;		// ���ӵ����ݴ�С

				if( pWaveHeader )
					return TRUE;
				break;
		}
		pdw = (DWORD *)((BYTE *)pdw + ((dwLength+1)&~1));
	}
	return FALSE;
}
// �����λ���������
BOOL CDirectSound::CreateSoundBuffer(WAVEFORMATEX * pcmwf)
{
	DSBUFFERDESC dsbdesc;		// ����DSBUFFERDESC�ṹ��
	memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC);			// ��ȡ�ṹ����ܰ�
	dsbdesc.dwFlags =								// ����������
		DSBCAPS_CTRL3D
		| DSBCAPS_CTRLVOLUME
		| DSBCAPS_GETCURRENTPOSITION2			/* Better position accuracy */
		| DSBCAPS_CTRLPOSITIONNOTIFY		/* We need notification */
		| DSBCAPS_GLOBALFOCUS
		| DSBCAPS_STATIC;				/* Allows background playing */
	dsbdesc.dwBufferBytes =m_dwBufNum*m_dwBufLen;
	dsbdesc.lpwfxFormat = pcmwf;    // ����������
	HRESULT hRes;
	if( DS_OK != (hRes = m_lpDirectSound->CreateSoundBuffer(&dsbdesc, &m_pDsb, 0)) ) {
		// Failed.
		DSError(hRes);
		m_pDsb = 0;
		return FALSE;
	}

	for (int i=0;i<m_dwBufNum;i++)
	{
		// ����������֪ͨ���Զ������¼�
		m_hEvent=CreateEvent( NULL, FALSE, FALSE, NULL );
		DSNotify[i].hEventNotify=m_hEvent; 

		DSNotify[i].dwOffset=i*(m_dwBufLen);	// ֪ͨ����
	}

	// ��ȡ������3D������
	hRes = m_pDsb->QueryInterface(IID_IDirectSound3DBuffer8,(void **)&m_pDs3db); 
	if (FAILED(hRes)) //�ɹ�
	{
		return FALSE;
	}

	::ZeroMemory(&m_Ds3dbParams, sizeof(DS3DBUFFER));
	m_Ds3dbParams.dwSize = sizeof(DS3DBUFFER);
	m_pDs3db->GetAllParameters(&m_Ds3dbParams);
	m_Ds3dbParams.dwMode = DS3DMODE_HEADRELATIVE;
	m_pDs3db->SetAllParameters(&m_Ds3dbParams, DS3D_IMMEDIATE);

	// ����֪ͨ
	hRes = m_pDsb->QueryInterface(IID_IDirectSoundNotify8, (LPVOID *)&m_pDsNotify);
	hRes=m_pDsNotify->SetNotificationPositions(m_dwBufNum,DSNotify); 
	if (FAILED(hRes)) //�ɹ�
	{
		return FALSE;
	}

	for (int n=0;n< m_dwBufNum;n++)
	{
		notify_events[n]=DSNotify[n].hEventNotify;
		ResetEvent(notify_events[n]);	// �����¼�
	}

	// ��ȡ3D����
	hRes = m_pDsb->QueryInterface(IID_IDirectSound3DListener8, (LPVOID *)&m_pDs3dl);
	if(FAILED(hRes))
	{
		return FALSE;
	}

	// ���ö�Doppler���ӡ�Rolloff���ӡ����������С����Ȳ���

	return TRUE;
}


// ����������������
BOOL CDirectSound::SetSoundData(void * pSoundData,DWORD SoundSize)
{
	LPVOID lpvPtr1;		// ����дָ��
	DWORD dwBytes1;
	 
	if (!pSoundData) return FALSE;	

	// ���дָ��
	HRESULT hr = m_pDsb->Lock(0, SoundSize, &lpvPtr1, &dwBytes1, 0, 0, DSBLOCK_ENTIREBUFFER);    
	// ��� DSERR_BUFFERLOST ������, �ָ�����������
	if(DSERR_BUFFERLOST == hr) {
		m_pDsb->Restore();
		hr = m_pDsb->Lock(0, SoundSize, &lpvPtr1, &dwBytes1, 0, 0, DSBLOCK_ENTIREBUFFER);
	}
	if(DS_OK == hr) {
		// Write to pointers.
		::CopyMemory(lpvPtr1, pSoundData, SoundSize);
		// Release the data back to DirectSound.
		hr = m_pDsb->Unlock(lpvPtr1, dwBytes1, 0, 0);
		if(DS_OK == hr)
			return TRUE;
	}
	// Lock, Unlock, or Restore failed.
	return FALSE;

}

BOOL CDirectSound::SetSoundData( DWORD Offset, void * pSoundData, DWORD dwSoundSize )
{
	LPVOID lpPart1;
	LPVOID lpPart2;
	DWORD  Part1Len,Part2Len;


	DWORD off_read,off_write;
	//char buf[128];
	m_pDsb->GetCurrentPosition(&off_read,&off_write);
	TKL_Trace("offset = %d,off_read=%d off_write=%d\n",Offset,off_read,off_write);

	// Obtain write pointer.
	HRESULT hr = m_pDsb->Lock(Offset, dwSoundSize, &lpPart1, &Part1Len, &lpPart2, &Part2Len, 0);    

	// If DSERR_BUFFERLOST is returned, restore and retry lock.
	if(DSERR_BUFFERLOST == hr) {
		m_pDsb->Restore();
		hr = m_pDsb->Lock(Offset, dwSoundSize, &lpPart1, &Part1Len, &lpPart2, &Part2Len, 0);
	}
	if(DS_OK == hr) {
		// Write to pointers.
		::CopyMemory(lpPart1, pSoundData, Part1Len);
		if (lpPart2)
		{
			::CopyMemory(lpPart2, (BYTE*)pSoundData+Part1Len, Part2Len);
		}


		// Release the data back to DirectSound.
		hr = m_pDsb->Unlock(lpPart1, Part1Len, lpPart2, Part2Len);

	}
	// Lock, Unlock, or Restore failed.

	return FALSE;
}


// �򿪲���
// ���Ų���
void CDirectSound::Play(INT nTime, BOOL bLoop)
{
	if( ! IsValid() /*|| ! IsEnabled()*/ )
		return;		// no chance to play the sound ...


	DWORD dwPlayCursor;
	DWORD dwWriteCursor;
	m_pDsb->GetCurrentPosition(&dwPlayCursor, &dwWriteCursor);

	int offByte = nTime*m_rate/1000;

	if(dwPlayCursor >0)
	{
		if( dwPlayCursor > m_dwBufLen* m_dwBufNum )
			dwPlayCursor = m_dwBufLen* m_dwBufNum;
	}
	if(dwPlayCursor > offByte)
	{
		if(dwPlayCursor  < -offByte)
		{
			dwPlayCursor = 0;
		}
		else
		{
			dwPlayCursor += offByte;
		}

	}

	else
		dwPlayCursor = m_dwBufLen* m_dwBufNum;

	m_pDsb->SetCurrentPosition(dwPlayCursor);

	if( DSERR_BUFFERLOST == m_pDsb->Play(0, 0, 1) ) 
	{
		// another application had stolen our buffer
		// Note that a "Restore()" is not enough, because
		// the sound data is invalid after Restore().
		SetSoundData(m_pTheSound, m_dwBufLen);


		// Try playing again
		m_pDsb->Play(0, 0, 1);
	}

}

// ֹͣ����
void CDirectSound::Stop()
{
	m_bStop = 1;
	if(m_hPlayThread)
	{
		WaitForSingleObject(m_hPlayThread,2000);
		CloseHandle(m_hPlayThread);
		m_hPlayThread = NULL;
	}
	if( IsValid() )
		m_pDsb->Stop();

	for (int i=0;i<m_dwBufNum;i++)
	{
		//create dsound buffer notify points
		//Auto reset event
		ResetEvent(notify_events[i]);
		CloseHandle(notify_events[i]);//=m_hEvent; 

	}
	if(m_pDs3db)
	{
		m_pDs3db->Release();
		m_pDs3db = NULL;
	}
	if(m_pDsb)
	{
		m_pDsb->Release();
		m_pDsb = NULL;
	}
	if(m_lpDirectSound)
	{
		m_lpDirectSound->Release();
		m_lpDirectSound = NULL;
	}
	m_bFirst = TRUE;
	m_dwBufSize = 0;
}

// ��ͣ����
void CDirectSound::Pause()
{
	if( IsValid() )
		m_pDsb->Stop();
}
// ��������
void CDirectSound::Continue()
{
	if( IsValid() ) {
		DWORD dwPlayCursor, dwWriteCursor;
		m_pDsb->GetCurrentPosition(&dwPlayCursor, &dwWriteCursor);
		Play(dwPlayCursor);
	}
}

BOOL CDirectSound::IsValid() const
{
	return (m_lpDirectSound && m_pDsb)? TRUE:FALSE;
}

int CDirectSound::WriteDataToBuf( BYTE* pBuf, DWORD dwLen )
{
	if(m_dwBufSize + dwLen > BUFSIZE)
		return -1;

	WaitForSingleObject(m_sysncObj, 2000);
	{
		memcpy(m_pTheSound + m_dwBufSize, pBuf, dwLen);
		m_dwBufSize += dwLen;
		ReleaseMutex(m_sysncObj);
	}

	return 1;
}

BOOL CDirectSound::WriteToBuf()
{
	if(!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL))
	{
		return FALSE;
	}

	while(!m_bStop)
	{
		if(m_bFirst)
		{
			while(m_dwBufSize < 3*m_dwBufLen)
			{
				if(m_bStop)
					break;
				Sleep(10);
			}
			WaitForSingleObject(m_sysncObj, 20000);		// ???
			{
				SetSoundData(m_pTheSound, 3* m_dwBufLen);
				if(m_dwBufSize > 3*m_dwBufLen)
				{
					memcpy(m_pTheSound,m_pTheSound + 3*m_dwBufLen, m_dwBufSize - 3*m_dwBufLen);
				}
				m_dwBufSize -= 3*m_dwBufLen;

				ReleaseMutex(m_sysncObj);
			}
			m_bFirst = FALSE;
		}
		else
		{

			int id = WaitForMultipleObjects(m_dwBufNum,notify_events,0,INFINITE);

			//ParseBuffer();

			//GENRS_AutoLock lock(m_sysncObj);
			while (m_dwBufSize < m_dwBufLen)
			{
				if(m_bStop)
					break;
				Sleep(10);
			}

			WaitForSingleObject(m_sysncObj,2000);
			{

				TKL_Trace("ID = %d",id);
				if(id == 0)
					m_dwOffset = 3*m_dwBufLen;
				else
					m_dwOffset = (id-1)*m_dwBufLen;

				SetSoundData( m_dwOffset,m_pTheSound, m_dwBufLen);
				if(m_dwBufSize > m_dwBufLen)
					memcpy(m_pTheSound,m_pTheSound+m_dwBufLen,m_dwBufSize - m_dwBufLen);

				m_dwBufSize -= m_dwBufLen;
				ReleaseMutex(m_sysncObj);
			}
		}
	}
	return TRUE;
}

BOOL CDirectSound::SetPlaySpeed( float fSpeed )
{
	m_fRate = fSpeed;
	m_config.nSamplesPerSec = m_nSamplesPerSec * fSpeed;
	m_config.nAvgBytesPerSec = m_nAvgBytesPerSec * fSpeed;
	Stop();
	if(!CreateDSound(&m_config,m_dwBufLen))
		return FALSE;
	Play();
	return TRUE;
}


// 3D��Ч�趨
BOOL CDirectSound::SetListenerPosition(D3DVALUE x, D3DVALUE y, D3DVALUE z)
{
	m_pDs3dl->SetPosition(x, y, z, DS3D_IMMEDIATE);
	return TRUE;
}
BOOL CDirectSound::SetListenerVelocity(D3DVALUE x, D3DVALUE y, D3DVALUE z)
{
	m_pDs3dl->SetVelocity(x, y, z, DS3D_IMMEDIATE);
	return TRUE;
}
BOOL CDirectSound::SetListenerOrientation(D3DVALUE x, D3DVALUE y, D3DVALUE z, D3DVALUE topX, D3DVALUE topY, D3DVALUE topZ)
{
	m_pDs3dl->SetOrientation(x, y, z, topX, topY, topZ, DS3D_IMMEDIATE);
	return TRUE;
}

BOOL CDirectSound::SetDistanceFactor(D3DVALUE flDistanceFactor)
{
	m_pDs3dl->SetDistanceFactor(flDistanceFactor, DS3D_IMMEDIATE);
	return TRUE;
}
BOOL CDirectSound::GetDistanceFactor(D3DVALUE *flDistanceFactor)
{
	m_pDs3dl->GetDistanceFactor(flDistanceFactor);
	return TRUE;
}

BOOL CDirectSound::SetDopplerFactor(D3DVALUE flDopplerFactor)
{
	m_pDs3dl->SetDopplerFactor(flDopplerFactor, DS3D_IMMEDIATE);
	return TRUE;
}
BOOL CDirectSound::GetDopplerFactor(D3DVALUE *flDopplerFactor)
{
	m_pDs3dl->GetDopplerFactor(flDopplerFactor);
	return TRUE;
}

BOOL CDirectSound::SetRooloffFactor(D3DVALUE flRooloffFactor)
{
	m_pDs3dl->SetRolloffFactor(flRooloffFactor, DS3D_IMMEDIATE);
	return TRUE;
}
BOOL CDirectSound::GetRooloffFactor(D3DVALUE *flRooloffFactor)
{
	m_pDs3dl->GetRolloffFactor(flRooloffFactor);
	return TRUE;
}