// OnePlayerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OnePlayer.h"
#include "OnePlayerDlg.h"
#include "mmsystem.h"		// WaveFile��д�ú�ȥ����һ��

#include <WindowsX.h>
// �������ͷ�ļ���֧��:
// ComboBox_AddString
// ComboBox_SetItemData ��

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Ϊʲô���������
#define  IDT_MOTION_TIMER 1

#define ORBIT_MAX_RADIUS        5.0f

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// COnePlayerDlg �Ի���




COnePlayerDlg::COnePlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COnePlayerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COnePlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COnePlayerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPEN, OnBnClickedOpen)
	ON_BN_CLICKED(IDC_PLAY, OnBnClickedPlay)
	ON_BN_CLICKED(IDC_PAUSE, OnBnClickedPause)
	ON_BN_CLICKED(IDC_STOP, OnBnClickedStop)
	ON_BN_CLICKED(IDC_SLOW, OnBnClickedSlow)
	ON_BN_CLICKED(IDC_NORMAL, OnBnClickedNormal)
	ON_BN_CLICKED(IDC_FAST, OnBnClickedFast)
	ON_WM_DESTROY()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_QUIT, OnBnClickedQuit)
	// 3D������Ϣӳ��
	/*ON_WM_HSCROLL(IDC_MinDest_Slider, OnMinDestSliding)
	ON_WM_HSCROLL(IDC_MaxDest_Slider, OnMaxDestSliding)
	ON_WM_HSCROLL(IDC_Rolloff_Slider, OnRolloffSliding)
	ON_WM_HSCROLL(IDC_Doppler_Slider, OnDopplerSliding)
	ON_WM_HSCROLL(IDC_PosX_Slider, OnPosXSliding)
	ON_WM_VSCROLL(IDC_PosY_Slider, OnPosYSliding)*/
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_MinDest_Slider, OnMinDestSliding)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_MaxDest_Slider, OnMaxDestSliding)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_Doppler_Slider, OnDopplerSliding)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_Rolloff_Slider, OnRolloffSliding)

	ON_BN_CLICKED(IDC_MOTION_CHECK, OnBnClickedMotionCheck)
	ON_WM_TIMER()
//	ON_BN_CLICKED(IDC_GetDevices, &COnePlayerDlg::OnBnClickedGetdevices)
END_MESSAGE_MAP()


// COnePlayerDlg ��Ϣ�������

BOOL COnePlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_hThread = 0;
	m_bStop = 0;
	m_bContinue = TRUE;
	m_sndSound1 = new CDirectSound;

	// ֧���ļ���ק��
	DragAcceptFiles();
	
	// ������Դ
	cbb_Open = new CBitmapButton();
	cbb_Play = new CBitmapButton();
	cbb_Pause = new CBitmapButton();
	cbb_Stop = new CBitmapButton();
	cbb_Fast = new CBitmapButton();
	cbb_Normal = new CBitmapButton();
	cbb_Slow = new CBitmapButton();
	cbb_Quit = new CBitmapButton();
	VERIFY(cbb_Open->SubclassDlgItem(IDC_OPEN,this));
	VERIFY(cbb_Play->SubclassDlgItem(IDC_PLAY,this));
	VERIFY(cbb_Pause->SubclassDlgItem(IDC_PAUSE,this));
	VERIFY(cbb_Stop->SubclassDlgItem(IDC_STOP,this));
	VERIFY(cbb_Fast->SubclassDlgItem(IDC_FAST,this));
	VERIFY(cbb_Normal->SubclassDlgItem(IDC_NORMAL,this));
	VERIFY(cbb_Slow->SubclassDlgItem(IDC_SLOW,this));
	VERIFY(cbb_Quit->SubclassDlgItem(IDC_QUIT,this));
	cbb_Open->LoadBitmaps(IDB_OPEN);
	cbb_Play->LoadBitmaps(IDB_PLAY);
	cbb_Pause->LoadBitmaps(IDB_PAUSE);
	cbb_Stop->LoadBitmaps(IDB_STOP);
	cbb_Fast->LoadBitmaps(IDB_FAST);
	cbb_Normal->LoadBitmaps(IDB_NORMAL);
	cbb_Slow->LoadBitmaps(IDB_SLOW);
	cbb_Quit->LoadBitmaps(IDB_QUIT);

	// ��ʼ��3D���ÿؼ�����
	pMinDestSlider = (CSliderCtrl*)GetDlgItem(IDC_MinDest_Slider);
	pMinDestSlider->SetRange(0, 100, TRUE);
	pMinDestSlider->SetPos(50);
	pMaxDestSlider = (CSliderCtrl*)GetDlgItem(IDC_MaxDest_Slider);
	pMaxDestSlider->SetRange(0, 100, TRUE);
	pMaxDestSlider->SetPos(50);
	pDopplerSlider = (CSliderCtrl*)GetDlgItem(IDC_Doppler_Slider);
	pDopplerSlider->SetRange(0, 100, TRUE);
	pDopplerSlider->SetPos(50);
	pRolloffSlider = (CSliderCtrl*)GetDlgItem(IDC_Rolloff_Slider);
	pRolloffSlider->SetRange(0, 100, TRUE);
	pRolloffSlider->SetPos(50);
	pPosXSlider = (CSliderCtrl*)GetDlgItem(IDC_PosX_Slider);
	pPosXSlider->SetRange(0, 100, TRUE);
	pPosXSlider->SetPos(50);
	pPosYSlider = (CSliderCtrl*)GetDlgItem(IDC_PosY_Slider);
	pPosYSlider->SetRange(0, 100, TRUE);
	pPosYSlider->SetPos(50);

	// ���ü�ʱ��
	SetTimer(IDT_MOTION_TIMER, 0, NULL);

	// ��ȡ�豸�б�
	pDevicesCombo = (CComboBox*)GetDlgItem(IDC_SOUNDDEVICES);
	ComboBox_AddString((HWND)pDevicesCombo, L"OK");
	if(FAILED(DirectSoundEnumerate((LPDSENUMCALLBACK)DSEnumProc, (LPVOID *)pDevicesCombo)))
	{
		EndDialog(TRUE);
		return TRUE;
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void COnePlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void COnePlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR COnePlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


CString FileNameFromPath(CString path)
{
	int nPos = path.ReverseFind('\\');
	return path.Mid(nPos+1);
}
void COnePlayerDlg::OnBnClickedOpen()
{
	LPCTSTR lpszFilter = L"Wave File(*.wav)|*.wav|ALL Files|*.*||";
	// ��ָ����������ʽ
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, lpszFilter);
	// ����򿪶Ի�����ʽ
	if(dlg.DoModal() == IDOK)
	{
		m_soundfile = dlg.GetPathName();
		
		GetDlgItem(IDC_MUSICNAME)->SetWindowText((LPCWSTR)FileNameFromPath(m_soundfile));
		
		GetDlgItem(IDC_PLAY)->EnableWindow(TRUE);
	}
}


unsigned int WINAPI FileReadProc(LPVOID pOwner)
{
	COnePlayerDlg* pThis = (COnePlayerDlg*)pOwner;
	pThis->ReadFileProc();

	return 1;
}

void COnePlayerDlg::ReadFileProc()
{
	// BYTE buf[1025];
	int n = 0;
	while(1)
	{
		BYTE buf[1025];
		int nlen;
		if(m_bStop)
			break;
		nlen = fread(buf, 1, 1024, fp);

		if(!nlen)
			break;

		while(m_sndSound1 ->WriteDataToBuf(buf, nlen) == -1)
		{
			Sleep(100);
		}
	}
	if(fp)
	{
		fclose(fp);
		fp = NULL;
	}

}

void COnePlayerDlg::OnBnClickedPlay()
{
	WAVEFORMATEX WaveHead;	// ��������ͷ�ļ�
	WaveHead.wFormatTag = 0;			// 1
	WaveHead.nChannels = 0;				// 2 
	WaveHead.nSamplesPerSec = 0;	// 44100
	WaveHead.nAvgBytesPerSec = 0;	// 176400
	WaveHead.nBlockAlign = 0;			// 4
	WaveHead.wBitsPerSample = 0;		// 16

	// ���ò�����״̬
	m_bContinue = 1;
	m_bStop = 0;

	if(m_soundfile.IsEmpty())
	{
		MessageBox(L"���ȴ�һ����Ƶ�ļ�");
		return;
	}

	char* a = (char*)(LPCWSTR)m_soundfile;
	 fp = _wfopen((wchar_t*)(LPCWSTR)m_soundfile, L"rb");
	//fp = _wfopen(L".\a.wav1", L"rb");

	fseek(fp, 20, 0);	// ����RIFFͷ����20Byte

	// ��ȡWave��ʽ��Ϣ
	int nLen = 0;
	nLen = fread(&WaveHead, 1, sizeof(WAVEFORMATEX), fp);

	// �����ļ�׼������
	// TODO ���ﲻӦ���ؽ�DirectSound��ֻ��Ҫ�޸����е�������ʽϸ��
	m_sndSound1->CreateDirectSound(&WaveHead);//, 8000);


	// ��ʼ�����߳�
	fseek(fp, 20+sizeof(WAVEFORMATEX),0);
	unsigned int dwReadID;
	m_hThread = (HANDLE)_beginthreadex(0,0,FileReadProc, this, 0, &dwReadID);

	// ����
	Sleep(200);
	m_sndSound1->Play();

	GetDlgItem(IDC_PLAY)->EnableWindow(FALSE);
	GetDlgItem(IDC_PAUSE)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_FAST)->EnableWindow(TRUE);
	GetDlgItem(IDC_NORMAL)->EnableWindow(TRUE);
	GetDlgItem(IDC_SLOW)->EnableWindow(TRUE);
}

void COnePlayerDlg::OnBnClickedPause()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_bContinue == 1)
	{
		m_sndSound1->Pause();
		m_bContinue = 0;
		GetDlgItem(IDC_PAUSE)->SetWindowTextW(L"����");
		GetDlgItem(IDC_FAST)->EnableWindow(FALSE);
		GetDlgItem(IDC_NORMAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLOW)->EnableWindow(FALSE);
	}
	else
	{
		m_sndSound1->Continue();
		m_bContinue = 1;
		GetDlgItem(IDC_PAUSE)->SetWindowTextW(L"��ͣ");
		GetDlgItem(IDC_FAST)->EnableWindow(TRUE);
		GetDlgItem(IDC_NORMAL)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLOW)->EnableWindow(TRUE);
	}
}

void COnePlayerDlg::OnBnClickedStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bStop = 1;
	if(m_hThread)
	{
		WaitForSingleObject(m_hThread, 2000);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	m_sndSound1->Stop();

	GetDlgItem(IDC_PLAY)->EnableWindow(TRUE);
	GetDlgItem(IDC_PAUSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_FAST)->EnableWindow(FALSE);
	GetDlgItem(IDC_NORMAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_SLOW)->EnableWindow(FALSE);
}

void COnePlayerDlg::OnBnClickedSlow()
{
	m_sndSound1->SetPlaySpeed(0.5);
}

void COnePlayerDlg::OnBnClickedNormal()
{
	m_sndSound1->SetPlaySpeed(1.0);
}

void COnePlayerDlg::OnBnClickedFast()
{
	m_sndSound1->SetPlaySpeed(2.0);
}




void COnePlayerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	m_bStop = 1;
	if(m_hThread)
	{
		WaitForSingleObject(m_hThread, 2000);
		CloseHandle(m_hThread);
		m_hThread = 0;
	}

	OnBnClickedStop();

	delete m_sndSound1;
}


void COnePlayerDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	TCHAR szFile[MAX_PATH];
	DragQueryFile(hDropInfo, 0, szFile, MAX_PATH);

	m_soundfile = szFile;

	GetDlgItem(IDC_MUSICNAME)->SetWindowText((LPCWSTR)FileNameFromPath(m_soundfile));
	GetDlgItem(IDC_PLAY)->EnableWindow(TRUE);

	CDialog::OnDropFiles(hDropInfo);
}


void COnePlayerDlg::OnBnClickedQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}


BOOL CALLBACK COnePlayerDlg::DSEnumProc(
	LPGUID lpGUID,
	LPCWSTR lpszDesc,
	LPCWSTR lpszDvrName,
	LPVOID lpContent)
{
	HWND hCombo = (HWND)lpContent;
	LPGUID lpTemp = NULL;
	if(lpGUID != NULL)
	{
		if((lpTemp = (LPGUID)malloc(sizeof(GUID))) == NULL)
		{
			return TRUE;
		}
		memcpy(lpTemp, lpGUID, sizeof(GUID));
	}
	ComboBox_AddString(hCombo, lpszDesc);
	ComboBox_SetItemData(hCombo, ComboBox_FindString(hCombo, 0, lpszDesc), lpTemp);
	free(lpTemp);
	return TRUE;
}

// 3D��Ч�趨��Ϣ����
void COnePlayerDlg::OnMinDestSliding(NMHDR *pNMHDR, LRESULT *pResult)
{
	//int nPos = ((CSliderCtrl *)pNMHDR)->GetPos();
	int nPos = ((CSliderCtrl *)GetDlgItem(IDC_MinDest_Slider))->GetPos();
	GetDlgItem(IDC_MinDest)->SetWindowText((LPCWSTR)nPos);
}
void COnePlayerDlg::OnMaxDestSliding(NMHDR *pNMHDR, LRESULT *pResult)
{
	// int nPos = ((CSliderCtrl *)pNMHDR)->GetPos();
	int nPos = ((CSliderCtrl *)GetDlgItem(IDC_MaxDest_Slider))->GetPos();
	GetDlgItem(IDC_MaxDest)->SetWindowText((LPCWSTR)nPos);
}
void COnePlayerDlg::OnDopplerSliding(NMHDR *pNMHDR, LRESULT *pResult)
{
	//int nPos = ((CSliderCtrl *)pNMHDR)->GetPos();
	int nPos = ((CSliderCtrl *)GetDlgItem(IDC_Doppler_Slider))->GetPos();
	GetDlgItem(IDC_Doppler)->SetWindowText((LPCWSTR)nPos);
}
void COnePlayerDlg::OnRolloffSliding(NMHDR *pNMHDR, LRESULT *pResult)
{
	//int nPos = ((CSliderCtrl *)pNMHDR)->GetPos();
	int nPos = ((CSliderCtrl *)GetDlgItem(IDC_Rolloff_Slider))->GetPos();
	GetDlgItem(IDC_Rolloff)->SetWindowText((LPCWSTR)nPos);
}
void COnePlayerDlg::OnPosXSliding(UINT nSBCode, UINT nPos, CScrollBar *pScollBar)
{

}
void COnePlayerDlg::OnPosYSliding(UINT nSBCode, UINT nPos, CScrollBar *pScollBar)
{

}

void COnePlayerDlg::OnBnClickedMotionCheck()
{
	int val = ((CButton *)GetDlgItem(IDC_MOTION_CHECK))->GetCheck();
	if(val == 1)
	{
		m_b3DMotion = TRUE;
	}
	else
	{
		m_b3DMotion = FALSE;
	}
}


void COnePlayerDlg::UpdateGrid(  FLOAT x, FLOAT y )
{
	static LONG s_lX = 0;
	static LONG s_lY = 0;

	// ���WM_PAINT������������������μ��� (�������)
	if( ::GetUpdateRect( m_hWnd,NULL,FALSE ) )
		return;

	// Get access to the needed device contexts
	HWND hWndGrid = ::GetDlgItem( m_hWnd, IDC_SourcePos ); 
	HDC  hDC      = ::GetDC( hWndGrid );
	HDC  hDCbmp   = ::CreateCompatibleDC( hDC );

	// Select the grid bitmap into the off-screen DC
	::SelectObject( hDCbmp, m_hGrid );

	// Restrict painting to the grid area
	::IntersectClipRect( hDC, 0, 0, m_nGridW, m_nGridH );

	// Restore the area of the grid previously drawn upon
	BitBlt( hDC, s_lX-1, s_lY-1, 3, 3, hDCbmp, s_lX-1, s_lY-1, SRCCOPY ); 

	// Convert the world space x,y coordinates to pixel coordinates
	RECT rc;
	::GetClientRect( hWndGrid, &rc );
	s_lX = (LONG)( ( x/ORBIT_MAX_RADIUS + 1 ) * ( rc.left + rc.right ) / 2 );
	s_lY = (LONG)( (-y/ORBIT_MAX_RADIUS + 1 ) * ( rc.top + rc.bottom ) / 2 );

	// Draw a crosshair object in red pixels
	//SetPixel( hDC, s_lX-1, s_lY+0, 0x000000ff );
	//SetPixel( hDC, s_lX+0, s_lY-1, 0x000000ff );
	SetPixel( hDC, s_lX+0, s_lY+0, 0x000000ff );
	//SetPixel( hDC, s_lX+0, s_lY+1, 0x000000ff );
	SetPixel( hDC, s_lX+1, s_lY+0, 0x000000ff );

	::ReleaseDC( hWndGrid, hDC );
	::DeleteDC( hDCbmp );
}

void COnePlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(!m_sndSound1->m_bPlay)
		return;

	FLOAT fXScale;
	FLOAT fYScale;

	fXScale = (((CSliderCtrl*)GetDlgItem(IDC_PosX_Slider ))->GetPos())/100.0f;

	fYScale = (((CSliderCtrl*)GetDlgItem(IDC_PosY_Slider ))->GetPos())/100.0f;
	FLOAT t = timeGetTime()/1000.0f;

	// Χ���������ƶ���Դ�����뾶��27.5�ĵ�λ����
	D3DVECTOR vPosition;
	vPosition.x = ORBIT_MAX_RADIUS * fXScale * (FLOAT)sin(t);
	vPosition.y = 0.0f;
	vPosition.z = ORBIT_MAX_RADIUS * fYScale * (FLOAT)cos(t);

	D3DVECTOR vVelocity;
	vVelocity.x = ORBIT_MAX_RADIUS * fXScale * (FLOAT)sin(t+0.05f);
	vVelocity.y = 0.0f;
	vVelocity.z = ORBIT_MAX_RADIUS * fYScale * (FLOAT)cos(t+0.05f); 
	
	m_sndSound1->SetListenerPosition(vPosition.x, vPosition.y, vPosition.z);
	m_sndSound1->SetListenerVelocity(vVelocity.x, vVelocity.y, vVelocity.z);

	UpdateGrid(  vPosition.x, vPosition.z );

	CDialog::OnTimer(nIDEvent);
}


