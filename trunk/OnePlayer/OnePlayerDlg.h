// OnePlayerDlg.h : 头文件
//

#pragma once

#include "DirectSound.h"
#include <MMSystem.h>
#include <math.h>
#pragma comment(lib,"winmm.lib")

// COnePlayerDlg 对话框
class COnePlayerDlg : public CDialog
{
// 构造
public:
	CString m_soundfile;
	CString m_filename;
	
	int m_nState;
	BOOL m_bStop;
	HANDLE m_hThread;
	FILE* fp;
	BOOL m_bContinue;
	// 3D音效
	BOOL m_b3DMotion;

	void ReadFileProc();

	CDirectSound* m_sndSound1;

	// 位图按钮
	CBitmapButton* cbb_Open;
	CBitmapButton* cbb_Play;
	CBitmapButton* cbb_Pause;
	CBitmapButton* cbb_Stop;
	CBitmapButton* cbb_Fast;
	CBitmapButton* cbb_Normal;
	CBitmapButton* cbb_Slow;
	CBitmapButton* cbb_Quit;
	// 设备枚举
	CComboBox *pDevicesCombo;
	// 3D设置控件变量
	CSliderCtrl *pMinDestSlider;
	CSliderCtrl *pMaxDestSlider;
	CSliderCtrl *pDopplerSlider;
	CSliderCtrl *pRolloffSlider;
	CSliderCtrl *pPosXSlider;
	CSliderCtrl *pPosYSlider;
	int m_nGridW, m_nGridH;
	HBITMAP m_hGrid;


	COnePlayerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ONEPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	// 3D音效设置函数
	afx_msg void OnMinDestSliding(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMaxDestSliding(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDopplerSliding(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRolloffSliding(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPosXSliding(UINT nSBCode, UINT nPos, CScrollBar *pScollBar);
	afx_msg void OnPosYSliding(UINT nSBCode, UINT nPos, CScrollBar *pScollBar);
	void UpdateGrid(FLOAT x, FLOAT y );
	// 回调函数
	static	BOOL CALLBACK DSEnumProc(LPGUID lpGUID,
		LPCWSTR lpszDesc,
		LPCWSTR lpszDvrName,
		LPVOID lpContent);
	afx_msg void OnBnClickedMotionCheck();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnBnClickedGetdevices();
};
