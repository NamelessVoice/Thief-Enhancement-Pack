#include "StdAfx.h"
#include "OpenGLControl.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

#define CIRCLE_SEGS 16

COpenGLControl::COpenGLControl(void)
{
	m_fX = 0;
	m_fY = 0;
	m_bReverseAlphaFade = false;
	m_fAlpha = 0.00f;

	GLuint m_uOriginalTexture = 0;
	GLuint m_uNewTexture = 0;

	m_iNewRealX = 2;
	m_iNewRealY = 2;
	m_iNewX = 128;
	m_iNewY = 128;

	m_fNewTileX = 1;
	m_fNewTileY = 1;

	m_bEnabled = false;
}

COpenGLControl::~COpenGLControl(void)
{
}


void COpenGLControl::oglCreate(CRect rect, CWnd *parent)
{
	CString className = AfxRegisterWndClass(CS_HREDRAW |
		CS_VREDRAW | CS_OWNDC, NULL,
		(HBRUSH)GetStockObject(BLACK_BRUSH), NULL);

	CreateEx(0, className, "OpenGL", WS_CHILD | WS_VISIBLE |
				WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, parent, 0);

	// Set initial variables' values
	m_oldWindow	 = rect;
	m_originalRect = rect;

	hWnd = parent;
}BEGIN_MESSAGE_MAP(COpenGLControl, CWnd)
ON_WM_PAINT()
ON_WM_CREATE()
ON_WM_TIMER()
ON_WM_SIZE()
END_MESSAGE_MAP()

void COpenGLControl::OnPaint()
{
	// Don't do this, we'll handle the painting of this element manually

	//	CPaintDC dc(this); // device context for painting
	 ValidateRect(NULL);
}

int COpenGLControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	oglInitialize();	// Initialise OpenGL

	return 0;
}


void COpenGLControl::oglInitialize(void)
{
	// Initial Setup:
	//
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,	 // bit depth
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		16,	 // z-buffer depth
		0, 0, 0, 0, 0, 0, 0,
	};

	// Get device context only once.
	hdc = GetDC()->m_hDC;

	// Pixel format.
	m_nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, m_nPixelFormat, &pfd);

	// Create the OpenGL Rendering Context.
	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	// Basic Setup:
	//
	// Set color to use when clearing the background.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	// Turn on backface culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Turn on depth testing
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);

	// Turn on textures
	glEnable(GL_TEXTURE_2D);								// Enable Texture Mapping ( NEW )

	// Init ilut
	ilInit();
	ilutRenderer(ILUT_OPENGL);

	// Send draw request
	OnDraw(NULL);
}

void COpenGLControl::OnDraw(CDC *pDC)
{
	// TODO: Camera controls.
}

void COpenGLControl::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
		case 1:
		{
			// Clear color and depth buffer bits
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Draw OpenGL scene
			oglDrawScene();

			// Swap buffers
			SwapBuffers(hdc);

			break;
		}

		default:
			break;
	}

	CWnd::OnTimer(nIDEvent);
}

void COpenGLControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED) return;

	m_fX = (float)cx;
	m_fY = (float)cy;

	// Map the OpenGL coordinates.
	glViewport(0, 0, cx, cy);

	// Projection view
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	// Set our current view perspective
	//gluPerspective(35.0f, (float)cx / (float)cy, 0.01f, 2000.0f);
	
	glOrtho (0, 1, 1, 0, 0, 1);

	// Model view
	glMatrixMode(GL_MODELVIEW);
}




void COpenGLControl::oglDrawScene(void)
{
	// Model view
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();									// Reset The Current Modelview Matrix
	
	// Wireframe Mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if ( !m_bEnabled )
	{
		return;
	}

	int iNewX = (int)(m_iNewX * m_fNewTileX);
	int iNewY = (int)(m_iNewY * m_fNewTileY);

	float fBigX = (float)m_iOldX;
	float fBigY = (float)m_iOldY;

	if ( iNewX > fBigX )
		fBigX = (float)iNewX;

	if ( iNewY > fBigY )
		fBigY = (float)iNewY;

	float fNewX = fBigX / iNewX;
	float fNewY = fBigY / iNewY;
	float fOldX = fBigX / m_iOldX;
	float fOldY = fBigY / m_iOldY;

	float vertX = 1.0f;
	float vertY = 1.0f;
	// Handle non-square images
	if ( iNewX > iNewY )
	{
		vertY /= ( iNewX/iNewY);
	}

	if ( iNewY > iNewX )
	{
		vertX /= ( iNewY/iNewX);
	}

	float fLowX = 0.50f - ( vertX/2);
	float fHighX = 0.50f + ( vertX/2);
	float fLowY = 0.50f - ( vertY/2);
	float fHighY = 0.50f + ( vertY/2);
	
	// Original texture
	glBindTexture(GL_TEXTURE_2D, m_uOriginalTexture);

	glBegin(GL_QUADS);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, fOldY);
		glVertex2f(fLowX, fLowY);
		glTexCoord2f(fOldX, fOldY);
		glVertex2f(fHighX, fLowY);
		glTexCoord2f(fOldX, 0);
		glVertex2f(fHighX, fHighY);
		glTexCoord2f(0, 0);
		glVertex2f(fLowX, fHighY);
	glEnd();

	if ( !m_bReverseAlphaFade )
	{
		m_fAlpha += 0.01f;
	} else {
		m_fAlpha -= 0.01f;
	}

	if ( m_fAlpha >= 1.20f || m_fAlpha <= -0.20f ) {
		m_bReverseAlphaFade = !m_bReverseAlphaFade;
	}

	// New texture
	glBindTexture(GL_TEXTURE_2D, m_uNewTexture);

	glBegin(GL_QUADS);
		glColor4f(1.0f, 1.0f, 1.0f, m_fAlpha);
		glTexCoord2f(0, fNewY);
		glVertex2f(fLowX, fLowY);
		glTexCoord2f(fNewX, fNewY);
		glVertex2f(fHighX, fLowY);
		glTexCoord2f(fNewX, 0);
		glVertex2f(fHighX, fHighY);
		glTexCoord2f(0, 0);
		glVertex2f(fLowX, fHighY);
	glEnd();
}

void COpenGLControl::SetOriginalTexture(const char* pszTexturePath) {
	m_uOriginalTexture = LoadATexture(pszTexturePath, m_iOldX, m_iOldY);
}

void COpenGLControl::SetNewTexture(const char* pszTexturePath) {
	m_uNewTexture = LoadATexture(pszTexturePath, m_iNewX, m_iNewY);

	m_iNewRealX = m_iNewX;
	m_iNewRealY = m_iNewY;

	m_fNewTileX = 1.0f;
	m_fNewTileY = 1.0f;

	string s(pszTexturePath);
	size_t pos = s.find_last_of('.');
	s = s.substr(0, pos) + ".mtl";
	ifstream mtlReader(s.c_str());

	if ( !mtlReader )
	{
		if ( m_iOldX > 0 && m_iOldY > 0 )
		{
			ofstream mtlWriter(s.c_str());

			char sScale[32];

			sprintf_s(sScale, "terrain_scale %i %i", m_iOldX, m_iOldY, 32);
			mtlWriter.write(sScale, (std::streamsize)strlen(sScale));
			mtlWriter.flush();
			mtlWriter.close();
			mtlReader.clear();
			mtlReader.open(s.c_str());
		}
	}
	
	if ( mtlReader )
	{
		string line;

		while ( !mtlReader.eof())
		{
			getline(mtlReader, line);
			string word;
			vector<string> strs;

			
			istringstream iss(line, istringstream::in);

			while (iss >> word ) {
				strs.push_back(word);
			}

			if (strs.size() > 0 )
			{
				if ( strs[0].compare("terrain_scale") == 0 )
				{
					if ( strs.size() == 2 )
					{
						istringstream ( strs[1] ) >> m_iNewX;
						m_iNewY = m_iNewX;
					} else if ( strs.size() == 3 )
					{
						istringstream ( strs[1] ) >> m_iNewX;
						istringstream ( strs[2] ) >> m_iNewY;
					}
				}
				else if ( strs[0].compare("tile_factor") == 0 )
				{
					if ( strs.size() == 2 )
					{
						istringstream ( strs[1] ) >> m_fNewTileX;
						m_fNewTileY = m_fNewTileX;
					} else if ( strs.size() == 3 )
					{
						istringstream ( strs[1] ) >> m_fNewTileX;
						istringstream ( strs[2] ) >> m_fNewTileY;
					}
				}
			}
		}
		mtlReader.close();
	}

}

GLuint COpenGLControl::LoadATexture(const char* pszTexturePath, int &width, int &height)
{
	// Set width and height to zero in case we can't load it
	width = 0;
	height = 0;

	GLuint texID = 0;
	ILuint newImage;
	ilGenImages(1, &newImage);
	ilBindImage(newImage);

	// Load the image using DevIL
	ILboolean loadedOkay = ilLoadImage((const ILstring)pszTexturePath);

	if ( loadedOkay == IL_TRUE )
	{
		// If the image loaded successfully, bind it into OpenGL
		GLuint textureID;
		textureID = ilutGLBindTexImage();

		width = ilGetInteger(IL_IMAGE_WIDTH);
		height = ilGetInteger(IL_IMAGE_HEIGHT);

		ilDeleteImage(newImage);

		texID = textureID;
	}

	return texID;
}
