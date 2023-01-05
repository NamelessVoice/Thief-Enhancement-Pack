#pragma once
#include "afxwin.h"

#include <gl/gl.h>
#include <gl/glu.h>
#include <il/il.h>
#include <il/ilut.h>
#include <gl/freeglut.h>		// Header File For The GLaux Library
#include <string>
using namespace std;

class COpenGLControl :
	public CWnd
{
public:
	COpenGLControl(void);
	~COpenGLControl(void);

	UINT_PTR m_unpTimer;
	void oglCreate(CRect rect, CWnd *parent);
	void oglInitialize(void);
	afx_msg void OnDraw(CDC *pDC);
	void oglDrawScene(void);
	void COpenGLControl::SetOriginalTexture(const char* pszTexturePath);
	void COpenGLControl::SetNewTexture(const char* pszTexturePath);

	GLuint COpenGLControl::LoadATexture(const char* sTexturePath, int &width, int &height);

	bool m_bEnabled;

	float m_fX;
	float m_fY;
	boolean m_bReverseAlphaFade;
	float m_fAlpha;
	GLuint m_uOriginalTexture;
	GLuint m_uNewTexture;

	int m_iNewRealX;
	int m_iNewRealY;
	int m_iNewX;
	int m_iNewY;

	int m_iOldX;
	int m_iOldY;

	float m_fNewTileX;
	float m_fNewTileY;

private:
	// Window information
	CWnd	 *hWnd;
	HDC	  hdc;
	HGLRC	hrc;
	int	  m_nPixelFormat;
	CRect	m_rect;
	CRect	m_oldWindow;
	CRect	m_originalRect;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
