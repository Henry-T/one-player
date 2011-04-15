// OnePlayerDlg.h : ͷ�ļ�
//

#pragma once

#include "DirectSound.h"
#include <MMSystem.h>
#include <math.h>
#pragma comment(lib,"winmm.lib")

// COnePlayerDlg �Ի���
class COnePlayerDlg : public CDialog
{
// ����
public:
	CString m_soundfile;
	CString m_filename;
	
	int m_nState;
	BOOL m_bStop;
	HANDLE m_hThread;
	FILE* fp;
	BOOL m_bContinue;
	// 3D��Ч
	BOOL m_b3DMotion;

	void ReadFileProc();

	CDirectSound* m_sndSound1;

	// λͼ��ť
	CBitmapButton* cbb_Open;
	CBitmapButton* cbb_Play;
	CBitmapButton* cbb_Pause;
	CBitmapButton* cbb_Stop;
	CBitmapButton* cbb_Fast;
	CBitmapButton* cbb_Normal;
	CBitmapButton* cbb_Slow;
	CBitmapButton* cbb_Quit;
	// �豸ö��
	CComboBox *pDevicesCombo;
	// 3D���ÿؼ�����
	CSliderCtrl *pMinDestSlider;
	CSliderCtrl *pMaxDestSlider;
	CSliderCtrl *pDopplerSlider;
	CSliderCtrl *pRolloffSlider;
	CSliderCtrl *pPosXSlider;
	CSliderCtrl *pPosYSlider;
	int m_nGridW, m_nGridH;
	HBITMAP m_hGrid;


	COnePlayerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ONEPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedSlow();
	afx_msg void OnBnClickedNormal();
	afx_msg void OnBnClickedFast();
	afx_msg void OnDestroy();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedQuit();
	// 3D��Ч���ú���
	afx_msg void OnMinDestSliding(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMaxDestSliding(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDopplerSliding(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRolloffSliding(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPosXSliding(UINT nSBCode, UINT nPos, CScrollBar *pScollBar);
	afx_msg void OnPosYSliding(UINT nSBCode, UINT nPos, CScrollBar *pScollBar);
	void UpdateGrid(FLOAT x, FLOAT y );
	// �ص�����
	static	BOOL CALLBACK DSEnumProc(LPGUID lpGUID,
		LPCWSTR lpszDesc,
		LPCWSTR lpszDvrName,
		LPVOID lpContent);
	afx_msg void OnBnClickedMotionCheck();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnBnClickedGetdevices();
};
