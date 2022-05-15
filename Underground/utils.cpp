#include "main.h"

C_Offsets *Offsets;
CDraw Draw;
char *m_pszDllPath;

FORCEINLINE __declspec ( naked ) DWORD_PTR GetEBP ( void )
{
	__asm
	{
		mov eax, ebp
			retn
	}
}

FORCEINLINE __declspec ( naked ) DWORD_PTR GetESP ( void )
{
	__asm
	{
		mov eax, esp
			retn
	}
}

FORCEINLINE __declspec ( naked ) DWORD_PTR GetESI ( void )
{
	__asm
	{
		mov eax, esi
			retn
	}
}

FORCEINLINE __declspec ( naked ) DWORD_PTR GetEDI ( void )
{
	__asm
	{
		mov eax, edi
			retn
	}
}

void InterpolateVector( float frac, const Vector& src, const Vector& dest, Vector& output )
{
	int i;

	for ( i = 0; i < 3; i++ )
	{
		output[ i ] = src[ i ] + frac * ( dest[ i ] - src[ i ] );
	}
}
uint CountFuncs(void** vmt) 
{
	// Query memory and as long as the pointer lands in
	// PAGE_EXECUTE_READ or PAGE_EXECUTE_READWRITE memory
	// assume this is a valid function pointer.
	MEMORY_BASIC_INFORMATION mem;
	int i = -1;
	do 
	{
		i++; VirtualQuery(vmt[i], &mem, sizeof(MEMORY_BASIC_INFORMATION));
	} 
	while (mem.Protect == PAGE_EXECUTE_READ || mem.Protect == PAGE_EXECUTE_READWRITE);

	return i;
}

uint CountFuncs(void* begin, void* end, void** vmt) 
{
	int i = -1;
	do 
	{
		i++;
	} while ( begin < vmt[i] && vmt[i ]< end );
	return i;
}

int FindFunc(void** vmt, void* func, uint vfuncs) 
{
	if ( !vfuncs ) vfuncs = CountFuncs( vmt );
	for ( uint i = 0; i < vfuncs; i++ ) 
	{
		if (vmt[i] == func) 
		{
			return i;
		}
	}

	return -1;  
}

////////////////////////////////////////////////////////////////////

bool CDraw::WorldToScreen(const Vector &Origin,Vector &Screen)
{
	bool Result = false;
	const VMatrix& WorldToScreen = EngineSDK->WorldToScreenMatrix(); 

	float w = WorldToScreen[3][0] * Origin[0] + WorldToScreen[3][1] * Origin[1] + WorldToScreen[3][2] * Origin[2] + WorldToScreen[3][3]; 
	Screen[2] = 0.0f; 

	if( w > 0.001f ) 
	{ 
		float invw = 1.0f / w; 

		int ScreenSize[2];
		EngineSDK->GetScreenSize( ScreenSize[0], ScreenSize[1] );

		Screen[0] = (ScreenSize[0] / 2.0f) + ((((WorldToScreen[0][0] * Origin[0] + WorldToScreen[0][1] * Origin[1] + WorldToScreen[0][2] * Origin[2] + WorldToScreen[0][3]) * invw) / 2.0f) * ScreenSize[0] + 0.5f); 
		Screen[1] = (ScreenSize[1] / 2.0f) - ((((WorldToScreen[1][0] * Origin[0] + WorldToScreen[1][1] * Origin[1] + WorldToScreen[1][2] * Origin[2] + WorldToScreen[1][3]) * invw) / 2.0f) * ScreenSize[1] + 0.5f); 
		Result = true;
	} 

	return Result; 
}

void CDraw::Line( int x, int y, int x2, int y2, int w, Color dwColor )
{
	SurfaceSDK->DrawSetColor( dwColor ); 
	SurfaceSDK->DrawLine( x, y, x2, y2 ); 
}

void CDraw::FillRGBA( int x, int y, int w, int h, int r, int g, int b, int a )
{
	SurfaceSDK->DrawSetColor( r, g, b, a ); 
	SurfaceSDK->DrawFilledRect( x, y, x + w, y + h ); 
}

void CDraw::FillRGBA( int x, int y, int w, int h, Color dwColor )
{
	SurfaceSDK->DrawSetColor( dwColor ); 
	SurfaceSDK->DrawFilledRect( x, y, x + w, y + h ); 
}

void CDraw::OutlineArea(int x, int y, int w, int h, int r, int g, int b, int a)
{
	SurfaceSDK->DrawSetColor(r,g,b,a); 
	SurfaceSDK->DrawOutlinedRect(x,y,x+w,y+h); 
}

void CDraw::DrawLine(int startX, int startY, int endX, int endY, int r, int g, int b, int a)
{
	SurfaceSDK->DrawSetColor(r,g,b,a); 
	SurfaceSDK->DrawLine( startX, startY, endX, endY); 
}

void CDraw::Outline( int x, int y, int w, int h )
{
	FillRGBA( x, y, w, 1, 0, 0, 0, 255 );  //top
	FillRGBA( x, y + h - 1, w, 1, 0, 0, 0, 255 );  //bottom

	FillRGBA( x, y + 1, 1, h - 2, 0, 0, 0, 255 );  //left
	FillRGBA( x + w - 1, y + 1, 1, h - 2, 0, 0, 0, 255 );  //right
}

void CDraw::OutlineBox( int x, int y, int w, int h, int r, int g, int b, int a )
{
	SurfaceSDK->DrawSetColor( r, g, b, a ); 
	SurfaceSDK->DrawOutlinedRect( x, y, x + w + 1, y + h + 1 ); 
}

void CDraw::DrawBox( int x, int y, Color dwColor, int box_width, int radius )
{
	int radius2 = radius << 1; 

	//outlines
	OutlineBox( x - radius + box_width, y - radius + box_width, radius2 - box_width - 1, radius2 - box_width - 1, 0, 0, 0, 255 );  //inner
	OutlineBox( x - radius - 1, y - radius - 1, radius2 + ( box_width + 2) - 1, radius2 + ( box_width + 1), 0, 0, 0, 255 );  //outer

	//actual lines
	FillRGBA( x - radius + box_width, y - radius, radius2 - box_width, box_width, dwColor );  //top
	FillRGBA( x - radius, y + radius, radius2, box_width, (dwColor) );  //bottom
	FillRGBA( x - radius, y - radius, box_width, radius2, (dwColor) );  //left
	FillRGBA( x + radius, y - radius, box_width, radius2 + box_width, (dwColor) );  //right
}

void CDraw::DrawRect( int x, int y, int w, int h, Color dwColor )
{
	SurfaceSDK->DrawSetColor( dwColor  ); 
	SurfaceSDK->DrawFilledRect( x, y, x + w, y + h ); 
}

void CDraw::DrawOutlineRect( int x, int y, int w, int h, Color dwColor )
{
	SurfaceSDK->DrawSetColor(  dwColor  ); 
	SurfaceSDK->DrawOutlinedRect( x, y, x + w, y + h ); 
}

int TweakColor(int c1, int c2, int variation) 
{ 
	if (c1==c2) { return c1;  } 
	else if (c1<c2) { c1 += variation;  } 
	else { c1 -= variation;  } 
	return c1;  
}

void CDraw::tintAreaDegradeLeft(int x,int y,int w,int h, int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2, int variation)
{
	for( int i = 0;  i<=w;  i++ )
	{
		FillRGBA( x+i, y, 1, h, r1, g1, b1, a1 );  // FillRGBA
		r1 = TweakColor( r1, r2, variation); 
		g1 = TweakColor( g1, g2, variation); 
		b1 = TweakColor( b1, b2, variation); 
		a1 = TweakColor( a1, a2, variation); 
	}
}

void CDraw::tintAreaDegradeUp(int x,int y,int w,int h, int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2, int variation)
{
	for( int i = 0;  i<=h;  i++ )
	{
		FillRGBA( x, y+i, w, 1, r1, g1, b1, a1 );  // FillRGBA
		r1 = TweakColor( r1, r2, variation); 
		g1 = TweakColor( g1, g2, variation); 
		b1 = TweakColor( b1, b2, variation); 
		a1 = TweakColor( a1, a2, variation); 
	}
}

void CDraw::tintAreaDegradeRight(int x,int y,int w,int h, int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2, int variation)
{
	for( int i = w;  i>=0;  i-- )
	{
		FillRGBA( x+i, y, 1, h, r1, g1, b1, a1 );  // FillRGBA
		r1 = TweakColor( r1, r2, variation); 
		g1 = TweakColor( g1, g2, variation); 
		b1 = TweakColor( b1, b2, variation); 
		a1 = TweakColor( a1, a2, variation); 
	}
}

void CDraw::tintAreaDegradeDown(int x,int y,int w,int h, int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2, int variation)
{
	for( int i = h;  i>=0;  i-- )
	{
		FillRGBA( x, y+i, w, 1, r1, g1, b1, a1 );  // FillRGBA
		r1 = TweakColor( r1, r2, variation); 
		g1 = TweakColor( g1, g2, variation); 
		b1 = TweakColor( b1, b2, variation); 
		a1 = TweakColor( a1, a2, variation); 
	}
}

bool __fastcall ScreenProjectPoint(Vector &vIn, int*pOut)
{
	Vector vScreen;

	bool bOK = Draw.WorldToScreen(vIn, vScreen);
	if(bOK)
	{
		pOut[0] = vScreen.x;
		pOut[1] = vScreen.y;
	}

	return bOK;
}

static char box[6][4] =
{
	{ 0, 4, 6, 2 }, // +X
	{ 0, 1, 5, 4 }, // +Y
	{ 0, 2, 3, 1 }, // +Z
	{ 7, 5, 1, 3 }, // -X
	{ 7, 3, 2, 6 }, // -Y
	{ 7, 6, 4, 5 }, // -Z
};

void CDraw::RenderBox(Vector const& vMin, Vector const& vMax, matrix3x4_t const& xToWorld )
{
	INT iPosX[4],
		iPosY[4], iScreen[2];

	Vector vPos[8],
		vTmp;

	for(INT k = 0; k < 8; k++)
	{
		vTmp[0] = (k & 1) ? vMin[0] : vMax[0];
		vTmp[1] = (k & 2) ? vMin[1] : vMax[1];
		vTmp[2] = (k & 4) ? vMin[2] : vMax[2];
		VectorTransform(vTmp, xToWorld, vPos[k]);
	}

	for(INT k = 0; k < 6; k++)
	{
		bool bDraw = true;

		for(INT l = 0; l < 4 && bDraw; ++l)
		{
			if(ScreenProjectPoint( vPos[ box[k][l] ], iScreen))
			{
				iPosX[l] = iScreen[0];
				iPosY[l] = iScreen[1];
			}
			else
				bDraw = false;
		}

		if(bDraw)
		{
			SurfaceSDK->DrawPolyLine(iPosX, iPosY, 4);
		}
	}
}

void CDraw::PrintText(bool center,int x,int y,Color dwColor,const char *input,...)
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, input);
	_vsnprintf(buf, sizeof(buf), input, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);
	SurfaceSDK->DrawSetTextFont(Draw.ConsoleFont);	
	SurfaceSDK->DrawSetTextColor(dwColor);

	if(center)
	{
		int wide, tall;
		SurfaceSDK->GetTextSize(Draw.ConsoleFont, wbuf, wide, tall);
		x -= wide / 2;
		y -= tall / 2;
	}

	SurfaceSDK->DrawSetTextPos(x, y);
	SurfaceSDK->DrawPrintText(wbuf, wcslen(wbuf));
}

int CDraw::GetWidth(const char *input)
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, input);
	_vsnprintf(buf, sizeof(buf), input, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);
	int wide, tall;
	SurfaceSDK->GetTextSize(ConsoleFont, wbuf, wide, tall);

	return strlen((char*)input);
}
int CDraw::GetHeight(const char *input)
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, input);
	_vsnprintf(buf, sizeof(buf), input, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);
	int wide, tall;
	SurfaceSDK->GetTextSize(ConsoleFont, wbuf, wide, tall);

	return tall;
}

bool StringCompareI( const char*pszOrgString, const char*pszCompString )
{
	if( pszOrgString == 0 || pszCompString == 0 )
		return false;

	for( ;*pszOrgString != 0; ++pszOrgString, ++pszCompString )
	{
		if(*pszCompString == 0
			|| (*pszOrgString !=*pszCompString
			&&*pszOrgString != (*pszCompString + 32)
			&&*pszOrgString != (*pszCompString - 32)) )
			return false;
	}

	if( (*pszOrgString == 0 &&*pszCompString != 0) )
		return false;

	return true;
}

std::string szDirFile (const char* pszName )
{
	char appdata[0xFF];
	strcpy(appdata,getenv("AppData"));
	strcat(appdata, "\\sts\\");
	strcat(appdata,pszName);
	puts(appdata);

	return ( appdata ); 
}

std::string szDirFileConfig(const char* pszName)
{

	char appdata[0xFF];
	strcpy(appdata, getenv("AppData"));
	strcat(appdata, "\\sts\\");
	strcat(appdata, "\\Configs\\");
	strcat(appdata, pszName);
	puts(appdata);

	return (appdata);
}

std::string szDirFileDemos(const char* pszName)
{
	char appdata[0xFF];
	strcpy(appdata, getenv("AppData"));
	strcat(appdata, "\\sts\\");
	strcat(appdata, "\\Demos\\");
	strcat(appdata, pszName);
	puts(appdata);

	return (appdata);
}

std::string szDirFileConfigDll(const char* pszName)
{
	char appdata[0xFF];
	strcpy(appdata, m_pszDllPath);
	strcat(appdata, "\\Configs\\");
	strcat(appdata, pszName);
	puts(appdata);

	return (appdata);
}

std::string szDirFileDemosDll(const char* pszName)
{
	char appdata[0xFF];
	strcpy(appdata, m_pszDllPath);
	strcat(appdata, "\\Demos\\");
	strcat(appdata, pszName);
	puts(appdata);

	return (appdata);
}

//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------
static bool s_bMaterialsInitialized = false;
static IMaterial *s_pWireframe;
static IMaterial *s_pWireframeIgnoreZ;
static IMaterial *s_pVertexColor;
static IMaterial *s_pVertexColorIgnoreZ;


//-----------------------------------------------------------------------------
// Initializes standard materials
//-----------------------------------------------------------------------------
void InitializeStandardMaterials()
{
	if (s_bMaterialsInitialized)
		return;

	s_bMaterialsInitialized = true;

	KeyValues *pVMTKeyValues = new KeyValues("wireframe");
	pVMTKeyValues->SetInt("$vertexcolor", 1);
	s_pWireframe = MaterialSystemSDK->CreateMaterial("__utilWireframe2", pVMTKeyValues);
	s_pWireframe->IncrementReferenceCount();

	pVMTKeyValues = new KeyValues("wireframe");
	pVMTKeyValues->SetInt("$vertexcolor", 1);
	pVMTKeyValues->SetInt("$vertexalpha", 1);
	pVMTKeyValues->SetInt("$ignorez", 1);
	s_pWireframeIgnoreZ = MaterialSystemSDK->CreateMaterial("__utilWireframeIgnoreZ2", pVMTKeyValues);
	s_pWireframeIgnoreZ->IncrementReferenceCount();

	pVMTKeyValues = new KeyValues("unlitgeneric");
	pVMTKeyValues->SetInt("$vertexcolor", 1);
	pVMTKeyValues->SetInt("$vertexalpha", 1);
	s_pVertexColor = MaterialSystemSDK->CreateMaterial("__utilVertexColor2", pVMTKeyValues);
	s_pVertexColor->IncrementReferenceCount();

	pVMTKeyValues = new KeyValues("unlitgeneric");
	pVMTKeyValues->SetInt("$vertexcolor", 1);
	pVMTKeyValues->SetInt("$vertexalpha", 1);
	pVMTKeyValues->SetInt("$ignorez", 1);
	s_pVertexColorIgnoreZ = MaterialSystemSDK->CreateMaterial("__utilVertexColorIgnoreZ2", pVMTKeyValues);
	s_pVertexColorIgnoreZ->IncrementReferenceCount();
}

void ShutdownStandardMaterials()
{
	if (!s_bMaterialsInitialized)
		return;

	s_bMaterialsInitialized = false;

	s_pWireframe->DecrementReferenceCount();
	s_pWireframe = NULL;

	s_pWireframeIgnoreZ->DecrementReferenceCount();
	s_pWireframeIgnoreZ = NULL;

	s_pVertexColor->DecrementReferenceCount();
	s_pVertexColor = NULL;

	s_pVertexColorIgnoreZ->DecrementReferenceCount();
	s_pVertexColorIgnoreZ = NULL;
}


//-----------------------------------------------------------------------------
// Renders a wireframe sphere
//-----------------------------------------------------------------------------
void RenderWireframeSphere(const Vector &vCenter, float flRadius, int nTheta, int nPhi, Color c, bool bZBuffer)
{
	InitializeStandardMaterials();

	// Make one more coordinate because (u,v) is discontinuous.
	++nTheta;

	int nVertices = nPhi * nTheta;
	int nIndices = (nTheta - 1) * 4 * (nPhi - 1);

	CMatRenderContextPtr pRenderContext(MaterialSystemSDK);
	pRenderContext->Bind(bZBuffer ? s_pWireframe : s_pWireframeIgnoreZ);

	CMeshBuilder meshBuilder;
	IMesh* pMesh = pRenderContext->GetDynamicMesh();

	meshBuilder.Begin(pMesh, MATERIAL_LINES, nVertices, nIndices);

	unsigned char chRed = c.r();
	unsigned char chGreen = c.g();
	unsigned char chBlue = c.b();
	unsigned char chAlpha = c.a();

	int i, j;
	for (i = 0; i < nPhi; ++i)
	{
		for (j = 0; j < nTheta; ++j)
		{
			float u = j / (float)(nTheta - 1);
			float v = i / (float)(nPhi - 1);
			float theta = 2.0f * M_PI * u;
			float phi = M_PI * v;

			meshBuilder.Position3f(vCenter.x + (flRadius * sin(phi) * cos(theta)),
				vCenter.y + (flRadius * sin(phi) * sin(theta)),
				vCenter.z + (flRadius * cos(phi)));
			meshBuilder.Color4ub(chRed, chGreen, chBlue, chAlpha);
			meshBuilder.AdvanceVertex();
		}
	}

	for (i = 0; i < nPhi - 1; ++i)
	{
		for (j = 0; j < nTheta - 1; ++j)
		{
			int idx = nTheta * i + j;

			meshBuilder.Index(idx);
			meshBuilder.AdvanceIndex();

			meshBuilder.Index(idx + nTheta);
			meshBuilder.AdvanceIndex();

			meshBuilder.Index(idx);
			meshBuilder.AdvanceIndex();

			meshBuilder.Index(idx + 1);
			meshBuilder.AdvanceIndex();
		}
	}

	meshBuilder.End();
	pMesh->Draw();
}


//-----------------------------------------------------------------------------
// Draws a sphere
//-----------------------------------------------------------------------------
void RenderSphere(const Vector &vCenter, float flRadius, int nTheta, int nPhi, Color c, IMaterial *pMaterial)
{
	InitializeStandardMaterials();

	CMatRenderContextPtr pRenderContext(MaterialSystemSDK);

	unsigned char chRed = c.r();
	unsigned char chGreen = c.g();
	unsigned char chBlue = c.b();
	unsigned char chAlpha = c.a();

	// Two extra degenerate triangles per row (except the last one)
	int nTriangles = 2 * nTheta * (nPhi - 1);
	int nIndices = 2 * (nTheta + 1) * (nPhi - 1);
	if (nTriangles == 0)
		return;

	pRenderContext->Bind(pMaterial);
	IMesh *pMesh = pRenderContext->GetDynamicMesh();
	CMeshBuilder meshBuilder;

	meshBuilder.Begin(pMesh, MATERIAL_TRIANGLE_STRIP, nTriangles, nIndices);

	// Build the index buffer.
	float flOONPhi = 1.0f / (nPhi - 1);
	float flOONTheta = 1.0f / (nTheta - 1);

	int i, j;
	for (i = 0; i < nPhi; ++i)
	{
		for (j = 0; j < nTheta; ++j)
		{
			float u = j / (float)(nTheta - 1);
			float v = i / (float)(nPhi - 1);
			float theta = 2.0f * M_PI * u;
			float phi = M_PI * v;

			Vector vecPos;
			vecPos.x = flRadius * sin(phi) * cos(theta);
			vecPos.y = flRadius * sin(phi) * sin(theta);
			vecPos.z = flRadius * cos(phi);

			Vector vecNormal = vecPos;
			VectorNormalize(vecNormal);

			vecPos += vCenter;

			meshBuilder.Position3f(vecPos.x, vecPos.y, vecPos.z);
			meshBuilder.Normal3f(vecNormal.x, vecNormal.y, vecNormal.z);
			meshBuilder.Color4ub(chRed, chGreen, chBlue, chAlpha);
			meshBuilder.TexCoord2f(0, j * flOONTheta, i * flOONPhi);
			meshBuilder.AdvanceVertex();
		}
	}

	// Emit the triangle strips.
	int idx = 0;
	for (i = 0; i < nPhi - 1; ++i)
	{
		for (j = 0; j < nTheta; ++j)
		{
			idx = nTheta * i + j;

			meshBuilder.Index(idx + nTheta);
			meshBuilder.AdvanceIndex();

			meshBuilder.Index(idx);
			meshBuilder.AdvanceIndex();
		}

		// Emit a degenerate triangle to skip to the next row without a connecting triangle
		if (i < nPhi - 2)
		{
			meshBuilder.Index(idx);
			meshBuilder.AdvanceIndex();

			meshBuilder.Index(idx + nTheta + 1);
			meshBuilder.AdvanceIndex();
		}
	}

	meshBuilder.End();
	pMesh->Draw();
}

void RenderSphere(const Vector &vCenter, float flRadius, int nTheta, int nPhi, Color c, bool bZBuffer)
{
	IMaterial *pMaterial = bZBuffer ? s_pVertexColor : s_pVertexColorIgnoreZ;
	Color cActual(c.r(), c.g(), c.b(), c.a());
	RenderSphere(vCenter, flRadius, nTheta, nPhi, cActual, pMaterial);
}


//-----------------------------------------------------------------------------
// Box vertices
//-----------------------------------------------------------------------------
static int s_pBoxFaceIndices[6][4] =
{
	{ 0, 4, 6, 2 }, // -x
	{ 5, 1, 3, 7 }, // +x
	{ 0, 1, 5, 4 }, // -y
	{ 2, 6, 7, 3 }, // +y
	{ 0, 2, 3, 1 },	// -z
	{ 4, 5, 7, 6 }	// +z
};

static int s_pBoxFaceIndicesInsideOut[6][4] =
{
	{ 0, 2, 6, 4 }, // -x
	{ 5, 7, 3, 1 }, // +x
	{ 0, 4, 5, 1 }, // -y
	{ 2, 3, 7, 6 }, // +y
	{ 0, 1, 3, 2 },	// -z
	{ 4, 6, 7, 5 }	// +z
};

static void GenerateBoxVertices(const Vector &vOrigin, const QAngle& angles, const Vector &vMins, const Vector &vMaxs, Vector pVerts[8])
{
	// Build a rotation matrix from orientation
	matrix3x4_t fRotateMatrix;
	AngleMatrix(angles, fRotateMatrix);

	Vector vecPos;
	for (int i = 0; i < 8; ++i)
	{
		vecPos[0] = (i & 0x1) ? vMaxs[0] : vMins[0];
		vecPos[1] = (i & 0x2) ? vMaxs[1] : vMins[1];
		vecPos[2] = (i & 0x4) ? vMaxs[2] : vMins[2];

		VectorRotate(vecPos, fRotateMatrix, pVerts[i]);
		pVerts[i] += vOrigin;
	}
}


//-----------------------------------------------------------------------------
// Renders a wireframe box relative to an origin 
//-----------------------------------------------------------------------------
void RenderWireframeBox(const Vector &vOrigin, const QAngle& angles, const Vector &vMins, const Vector &vMaxs, Color c, bool bZBuffer)
{
	InitializeStandardMaterials();

	CMatRenderContextPtr pRenderContext(MaterialSystemSDK);
	pRenderContext->Bind(bZBuffer ? s_pWireframe : s_pWireframeIgnoreZ);

	Vector p[8];
	GenerateBoxVertices(vOrigin, angles, vMins, vMaxs, p);

	unsigned char chRed = c.r();
	unsigned char chGreen = c.g();
	unsigned char chBlue = c.b();
	unsigned char chAlpha = c.a();

	IMesh *pMesh = pRenderContext->GetDynamicMesh();
	CMeshBuilder meshBuilder;
	meshBuilder.Begin(pMesh, MATERIAL_LINES, 24);

	// Draw the box
	for (int i = 0; i < 6; i++)
	{
		int *pFaceIndex = s_pBoxFaceIndices[i];

		for (int j = 0; j < 4; ++j)
		{
			meshBuilder.Position3fv(p[pFaceIndex[j]].Base());
			meshBuilder.Color4ub(chRed, chGreen, chBlue, chAlpha);
			meshBuilder.AdvanceVertex();

			meshBuilder.Position3fv(p[pFaceIndex[(j == 3) ? 0 : j + 1]].Base());
			meshBuilder.Color4ub(chRed, chGreen, chBlue, chAlpha);
			meshBuilder.AdvanceVertex();
		}
	}

	meshBuilder.End();
	pMesh->Draw();
}


//-----------------------------------------------------------------------------
// Renders a solid box 
//-----------------------------------------------------------------------------
void RenderBox(const Vector& vOrigin, const QAngle& angles, const Vector& vMins, const Vector& vMaxs, Color c, IMaterial *pMaterial, bool bInsideOut)
{
	InitializeStandardMaterials();

	CMatRenderContextPtr pRenderContext(MaterialSystemSDK);
	pRenderContext->Bind(pMaterial);

	Vector p[8];
	GenerateBoxVertices(vOrigin, angles, vMins, vMaxs, p);

	unsigned char chRed = c.r();
	unsigned char chGreen = c.g();
	unsigned char chBlue = c.b();
	unsigned char chAlpha = c.a();

	IMesh *pMesh = pRenderContext->GetDynamicMesh();
	CMeshBuilder meshBuilder;
	meshBuilder.Begin(pMesh, MATERIAL_TRIANGLES, 12);

	// Draw the box
	Vector vecNormal;
	for (int i = 0; i < 6; i++)
	{
		vecNormal.Init();
		vecNormal[i / 2] = (i & 0x1) ? 1.0f : -1.0f;

		int *ppFaceIndices = bInsideOut ? s_pBoxFaceIndicesInsideOut[i] : s_pBoxFaceIndices[i];
		for (int j = 1; j < 3; ++j)
		{
			int i0 = ppFaceIndices[0];
			int i1 = ppFaceIndices[j];
			int i2 = ppFaceIndices[j + 1];

			meshBuilder.Position3fv(p[i0].Base());
			meshBuilder.Color4ub(chRed, chGreen, chBlue, chAlpha);
			meshBuilder.Normal3fv(vecNormal.Base());
			meshBuilder.TexCoord2f(0, 0.0f, 0.0f);
			meshBuilder.AdvanceVertex();

			meshBuilder.Position3fv(p[i2].Base());
			meshBuilder.Color4ub(chRed, chGreen, chBlue, chAlpha);
			meshBuilder.Normal3fv(vecNormal.Base());
			meshBuilder.TexCoord2f(0, 1.0f, (j == 1) ? 1.0f : 0.0f);
			meshBuilder.AdvanceVertex();

			meshBuilder.Position3fv(p[i1].Base());
			meshBuilder.Color4ub(chRed, chGreen, chBlue, chAlpha);
			meshBuilder.Normal3fv(vecNormal.Base());
			meshBuilder.TexCoord2f(0, (j == 1) ? 0.0f : 1.0f, 1.0f);
			meshBuilder.AdvanceVertex();
		}
	}

	meshBuilder.End();
	pMesh->Draw();
}


void RenderBox(const Vector& vOrigin, const QAngle& angles, const Vector& vMins, const Vector& vMaxs, Color c, bool bZBuffer, bool bInsideOut)
{
	IMaterial *pMaterial = bZBuffer ? s_pVertexColor : s_pVertexColorIgnoreZ;
	Color cActual(c.r(), c.g(), c.b(), c.a());
	RenderBox(vOrigin, angles, vMins, vMaxs, cActual, pMaterial, bInsideOut);
}
//-----------------------------------------------------------------------------
// Renders axes, red->x, green->y, blue->z
//-----------------------------------------------------------------------------
void RenderAxes(const Vector &vOrigin, float flScale, bool bZBuffer)
{
	InitializeStandardMaterials();

	CMatRenderContextPtr pRenderContext(MaterialSystemSDK);
	pRenderContext->Bind(bZBuffer ? s_pWireframe : s_pWireframeIgnoreZ);
	IMesh *pMesh = pRenderContext->GetDynamicMesh();

	CMeshBuilder meshBuilder;
	meshBuilder.Begin(pMesh, MATERIAL_LINES, 3);

	meshBuilder.Position3f(vOrigin.x, vOrigin.y, vOrigin.z);
	meshBuilder.Color4ub(255, 0, 0, 255);
	meshBuilder.AdvanceVertex();

	meshBuilder.Position3f(vOrigin.x + flScale, vOrigin.y, vOrigin.z);
	meshBuilder.Color4ub(255, 0, 0, 255);
	meshBuilder.AdvanceVertex();

	meshBuilder.Position3f(vOrigin.x, vOrigin.y, vOrigin.z);
	meshBuilder.Color4ub(0, 255, 0, 255);
	meshBuilder.AdvanceVertex();

	meshBuilder.Position3f(vOrigin.x, vOrigin.y + flScale, vOrigin.z);
	meshBuilder.Color4ub(0, 255, 0, 255);
	meshBuilder.AdvanceVertex();

	meshBuilder.Position3f(vOrigin.x, vOrigin.y, vOrigin.z);
	meshBuilder.Color4ub(0, 0, 255, 255);
	meshBuilder.AdvanceVertex();

	meshBuilder.Position3f(vOrigin.x, vOrigin.y, vOrigin.z + flScale);
	meshBuilder.Color4ub(0, 0, 255, 255);
	meshBuilder.AdvanceVertex();

	meshBuilder.End();
	pMesh->Draw();
}


void RenderAxes(const matrix3x4_t &transform, float flScale, bool bZBuffer)
{
	InitializeStandardMaterials();

	Vector xAxis, yAxis, zAxis, vOrigin, temp;
	MatrixGetColumn(transform, 0, xAxis);
	MatrixGetColumn(transform, 1, yAxis);
	MatrixGetColumn(transform, 2, zAxis);
	MatrixGetColumn(transform, 3, vOrigin);

	CMatRenderContextPtr pRenderContext(MaterialSystemSDK);
	pRenderContext->Bind(bZBuffer ? s_pWireframe : s_pWireframeIgnoreZ);
	IMesh *pMesh = pRenderContext->GetDynamicMesh();

	CMeshBuilder meshBuilder;
	meshBuilder.Begin(pMesh, MATERIAL_LINES, 3);

	meshBuilder.Position3fv(vOrigin.Base());
	meshBuilder.Color4ub(255, 0, 0, 255);
	meshBuilder.AdvanceVertex();

	VectorMA(vOrigin, flScale, xAxis, temp);
	meshBuilder.Position3fv(temp.Base());
	meshBuilder.Color4ub(255, 0, 0, 255);
	meshBuilder.AdvanceVertex();

	meshBuilder.Position3f(vOrigin.x, vOrigin.y, vOrigin.z);
	meshBuilder.Color4ub(0, 255, 0, 255);
	meshBuilder.AdvanceVertex();

	VectorMA(vOrigin, flScale, yAxis, temp);
	meshBuilder.Position3fv(temp.Base());
	meshBuilder.Color4ub(0, 255, 0, 255);
	meshBuilder.AdvanceVertex();

	meshBuilder.Position3f(vOrigin.x, vOrigin.y, vOrigin.z);
	meshBuilder.Color4ub(0, 0, 255, 255);
	meshBuilder.AdvanceVertex();

	VectorMA(vOrigin, flScale, zAxis, temp);
	meshBuilder.Position3fv(temp.Base());
	meshBuilder.Color4ub(0, 0, 255, 255);
	meshBuilder.AdvanceVertex();

	meshBuilder.End();
	pMesh->Draw();
}

//-----------------------------------------------------------------------------
// Render a line
//-----------------------------------------------------------------------------
void RenderLine(const Vector& v1, const Vector& v2, Color c, bool bZBuffer)
{
	InitializeStandardMaterials();

	CMatRenderContextPtr pRenderContext(MaterialSystemSDK);
	pRenderContext->Bind(bZBuffer ? s_pWireframe : s_pWireframeIgnoreZ);

	unsigned char chRed = c.r();
	unsigned char chGreen = c.g();
	unsigned char chBlue = c.b();
	unsigned char chAlpha = c.a();

	IMesh *pMesh = pRenderContext->GetDynamicMesh();
	CMeshBuilder meshBuilder;
	meshBuilder.Begin(pMesh, MATERIAL_LINES, 1);

	meshBuilder.Position3fv(v1.Base());
	meshBuilder.Color4ub(chRed, chGreen, chBlue, chAlpha);
	meshBuilder.AdvanceVertex();

	meshBuilder.Position3fv(v2.Base());
	meshBuilder.Color4ub(chRed, chGreen, chBlue, chAlpha);
	meshBuilder.AdvanceVertex();

	meshBuilder.End();
	pMesh->Draw();
}


//-----------------------------------------------------------------------------
// Draws a triangle
//-----------------------------------------------------------------------------
void RenderTriangle(const Vector& p1, const Vector& p2, const Vector& p3, Color c, IMaterial *pMaterial)
{
	InitializeStandardMaterials();

	CMatRenderContextPtr pRenderContext(MaterialSystemSDK);
	pRenderContext->Bind(pMaterial);

	unsigned char chRed = c.r();
	unsigned char chGreen = c.g();
	unsigned char chBlue = c.b();
	unsigned char chAlpha = c.a();

	Vector vecNormal;
	Vector vecDelta1, vecDelta2;
	VectorSubtract(p2, p1, vecDelta1);
	VectorSubtract(p3, p1, vecDelta2);
	CrossProduct(vecDelta1, vecDelta2, vecNormal);
	VectorNormalize(vecNormal);

	IMesh *pMesh = pRenderContext->GetDynamicMesh();
	CMeshBuilder meshBuilder;
	meshBuilder.Begin(pMesh, MATERIAL_TRIANGLES, 1);

	meshBuilder.Position3fv(p1.Base());
	meshBuilder.Color4ub(chRed, chGreen, chBlue, chAlpha);
	meshBuilder.Normal3fv(vecNormal.Base());
	meshBuilder.TexCoord2f(0, 0.0f, 0.0f);
	meshBuilder.AdvanceVertex();

	meshBuilder.Position3fv(p2.Base());
	meshBuilder.Color4ub(chRed, chGreen, chBlue, chAlpha);
	meshBuilder.Normal3fv(vecNormal.Base());
	meshBuilder.TexCoord2f(0, 0.0f, 1.0f);
	meshBuilder.AdvanceVertex();

	meshBuilder.Position3fv(p3.Base());
	meshBuilder.Color4ub(chRed, chGreen, chBlue, chAlpha);
	meshBuilder.Normal3fv(vecNormal.Base());
	meshBuilder.TexCoord2f(0, 1.0f, 0.0f);
	meshBuilder.AdvanceVertex();

	meshBuilder.End();
	pMesh->Draw();
}


void RenderTriangle(const Vector& p1, const Vector& p2, const Vector& p3, Color c, bool bZBuffer)
{
	IMaterial *pMaterial = bZBuffer ? s_pVertexColor : s_pVertexColorIgnoreZ;
	Color cActual(c.r(), c.g(), c.b(), c.a());
	RenderTriangle(p1, p2, p3, cActual, pMaterial);
}



//-----------------------------------------------------------------------------
// Renders an extruded box
//-----------------------------------------------------------------------------
static void DrawAxes(const Vector& origin, Vector* pts, int idx, Color c, CMeshBuilder& meshBuilder)
{
	Vector start, temp;
	VectorAdd(pts[idx], origin, start);
	meshBuilder.Position3fv(start.Base());
	meshBuilder.Color4ub(c.r(), c.g(), c.b(), c.a());
	meshBuilder.AdvanceVertex();

	int endidx = (idx & 0x1) ? idx - 1 : idx + 1;
	VectorAdd(pts[endidx], origin, temp);
	meshBuilder.Position3fv(temp.Base());
	meshBuilder.Color4ub(c.r(), c.g(), c.b(), c.a());
	meshBuilder.AdvanceVertex();

	meshBuilder.Position3fv(start.Base());
	meshBuilder.Color4ub(c.r(), c.g(), c.b(), c.a());
	meshBuilder.AdvanceVertex();

	endidx = (idx & 0x2) ? idx - 2 : idx + 2;
	VectorAdd(pts[endidx], origin, temp);
	meshBuilder.Position3fv(temp.Base());
	meshBuilder.Color4ub(c.r(), c.g(), c.b(), c.a());
	meshBuilder.AdvanceVertex();

	meshBuilder.Position3fv(start.Base());
	meshBuilder.Color4ub(c.r(), c.g(), c.b(), c.a());
	meshBuilder.AdvanceVertex();

	endidx = (idx & 0x4) ? idx - 4 : idx + 4;
	VectorAdd(pts[endidx], origin, temp);
	meshBuilder.Position3fv(temp.Base());
	meshBuilder.Color4ub(c.r(), c.g(), c.b(), c.a());
	meshBuilder.AdvanceVertex();
}

static void DrawExtrusionFace(const Vector& start, const Vector& end,
	Vector* pts, int idx1, int idx2, Color c, CMeshBuilder& meshBuilder)
{
	Vector temp;
	VectorAdd(pts[idx1], start, temp);
	meshBuilder.Position3fv(temp.Base());
	meshBuilder.Color4ub(c.r(), c.g(), c.b(), c.a());
	meshBuilder.AdvanceVertex();

	VectorAdd(pts[idx2], start, temp);
	meshBuilder.Position3fv(temp.Base());
	meshBuilder.Color4ub(c.r(), c.g(), c.b(), c.a());
	meshBuilder.AdvanceVertex();

	meshBuilder.Position3fv(temp.Base());
	meshBuilder.Color4ub(c.r(), c.g(), c.b(), c.a());
	meshBuilder.AdvanceVertex();

	VectorAdd(pts[idx2], end, temp);
	meshBuilder.Position3fv(temp.Base());
	meshBuilder.Color4ub(c.r(), c.g(), c.b(), c.a());
	meshBuilder.AdvanceVertex();

	meshBuilder.Position3fv(temp.Base());
	meshBuilder.Color4ub(c.r(), c.g(), c.b(), c.a());
	meshBuilder.AdvanceVertex();

	VectorAdd(pts[idx1], end, temp);
	meshBuilder.Position3fv(temp.Base());
	meshBuilder.Color4ub(c.r(), c.g(), c.b(), c.a());
	meshBuilder.AdvanceVertex();

	meshBuilder.Position3fv(temp.Base());
	meshBuilder.Color4ub(c.r(), c.g(), c.b(), c.a());
	meshBuilder.AdvanceVertex();

	VectorAdd(pts[idx1], start, temp);
	meshBuilder.Position3fv(temp.Base());
	meshBuilder.Color4ub(c.r(), c.g(), c.b(), c.a());
	meshBuilder.AdvanceVertex();
}

void RenderWireframeSweptBox(const Vector &vStart, const Vector &vEnd, const QAngle &angles, const Vector &vMins, const Vector &vMaxs, Color c, bool bZBuffer)
{
	CMatRenderContextPtr pRenderContext(MaterialSystemSDK);
	pRenderContext->Bind(bZBuffer ? s_pWireframe : s_pWireframeIgnoreZ);

	Color cActual(c.r(), c.g(), c.b(), c.a());

	// Build a rotation matrix from angles
	matrix3x4_t fRotateMatrix;
	AngleMatrix(angles, fRotateMatrix);

	IMesh *pMesh = pRenderContext->GetDynamicMesh();
	CMeshBuilder meshBuilder;
	meshBuilder.Begin(pMesh, MATERIAL_LINES, 30);

	Vector vDelta;
	VectorSubtract(vEnd, vStart, vDelta);

	// Compute the box points, rotated but without the origin added
	Vector temp;
	Vector pts[8];
	float dot[8];
	int minidx = 0;
	for (int i = 0; i < 8; ++i)
	{
		temp.x = (i & 0x1) ? vMaxs[0] : vMins[0];
		temp.y = (i & 0x2) ? vMaxs[1] : vMins[1];
		temp.z = (i & 0x4) ? vMaxs[2] : vMins[2];

		// Rotate the corner point
		VectorRotate(temp, fRotateMatrix, pts[i]);

		// Find the dot product with dir
		dot[i] = DotProduct(pts[i], vDelta);
		if (dot[i] < dot[minidx])
		{
			minidx = i;
		}
	}

	// Choose opposite corner
	int maxidx = minidx ^ 0x7;

	// Draw the start + end axes...
	DrawAxes(vStart, pts, minidx, cActual, meshBuilder);
	DrawAxes(vEnd, pts, maxidx, cActual, meshBuilder);

	// Draw the extrusion faces
	for (int j = 0; j < 3; ++j)
	{
		int dirflag1 = (1 << ((j + 1) % 3));
		int dirflag2 = (1 << ((j + 2) % 3));

		int idx1, idx2, idx3;
		idx1 = (minidx & dirflag1) ? minidx - dirflag1 : minidx + dirflag1;
		idx2 = (minidx & dirflag2) ? minidx - dirflag2 : minidx + dirflag2;
		idx3 = (minidx & dirflag2) ? idx1 - dirflag2 : idx1 + dirflag2;

		DrawExtrusionFace(vStart, vEnd, pts, idx1, idx3, cActual, meshBuilder);
		DrawExtrusionFace(vStart, vEnd, pts, idx2, idx3, cActual, meshBuilder);
	}
	meshBuilder.End();
	pMesh->Draw();
}


//-----------------------------------------------------------------------------
// Draws a axis-aligned quad
//-----------------------------------------------------------------------------
void RenderQuad(IMaterial *pMaterial, float x, float y, float w, float h,
	float z, float s0, float t0, float s1, float t1, const Color& clr)
{
	CMatRenderContextPtr pRenderContext(MaterialSystemSDK);
	IMesh *pMesh = pRenderContext->GetDynamicMesh(true, NULL, NULL, pMaterial);

	CMeshBuilder meshBuilder;
	meshBuilder.Begin(pMesh, MATERIAL_QUADS, 1);

	meshBuilder.Color4ub(clr.r(), clr.g(), clr.b(), clr.a());
	meshBuilder.TexCoord2f(0, s0, t0);
	meshBuilder.Position3f(x, y, z);
	meshBuilder.AdvanceVertex();

	meshBuilder.Color4ub(clr.r(), clr.g(), clr.b(), clr.a());
	meshBuilder.TexCoord2f(0, s1, t0);
	meshBuilder.Position3f(x + w, y, z);
	meshBuilder.AdvanceVertex();

	meshBuilder.Color4ub(clr.r(), clr.g(), clr.b(), clr.a());
	meshBuilder.TexCoord2f(0, s1, t1);
	meshBuilder.Position3f(x + w, y + h, z);
	meshBuilder.AdvanceVertex();

	meshBuilder.Color4ub(clr.r(), clr.g(), clr.b(), clr.a());
	meshBuilder.TexCoord2f(0, s0, t1);
	meshBuilder.Position3f(x, y + h, z);
	meshBuilder.AdvanceVertex();

	meshBuilder.End();

	pMesh->Draw();
}

//-----------------------------------------------------------------------------
// Renders a screen space quad
//-----------------------------------------------------------------------------
void DrawScreenSpaceRectangle(IMaterial *pMaterial,
	int nDestX, int nDestY, int nWidth, int nHeight,	// Rect to draw into in screen space
	float flSrcTextureX0, float flSrcTextureY0,		// which texel you want to appear at destx/y
	float flSrcTextureX1, float flSrcTextureY1,		// which texel you want to appear at destx+width-1, desty+height-1
	int nSrcTextureWidth, int nSrcTextureHeight,		// needed for fixup
	void *pClientRenderable,							// Used to pass to the bind proxies
	int nXDice, int nYDice)							// Amount to tessellate the 
{
	CMatRenderContextPtr pRenderContext(MaterialSystemSDK);

	if ((nWidth <= 0) || (nHeight <= 0))
		return;

	pRenderContext->MatrixMode(MATERIAL_VIEW);
	pRenderContext->PushMatrix();
	pRenderContext->LoadIdentity();

	pRenderContext->MatrixMode(MATERIAL_PROJECTION);
	pRenderContext->PushMatrix();
	pRenderContext->LoadIdentity();

	pRenderContext->Bind(pMaterial, pClientRenderable);

	int xSegments = max(nXDice, 1);
	int ySegments = max(nYDice, 1);

	CMeshBuilder meshBuilder;
	IMesh* pMesh = pRenderContext->GetDynamicMesh(true);
	meshBuilder.Begin(pMesh, MATERIAL_QUADS, xSegments * ySegments);

	int nScreenWidth, nScreenHeight;
	pRenderContext->GetRenderTargetDimensions(nScreenWidth, nScreenHeight);
	float flLeftX = nDestX - 0.5f;
	float flRightX = nDestX + nWidth - 0.5f;

	float flTopY = nDestY - 0.5f;
	float flBottomY = nDestY + nHeight - 0.5f;

	float flSubrectWidth = flSrcTextureX1 - flSrcTextureX0;
	float flSubrectHeight = flSrcTextureY1 - flSrcTextureY0;

	float flTexelsPerPixelX = (nWidth > 1) ? flSubrectWidth / (nWidth - 1) : 0.0f;
	float flTexelsPerPixelY = (nHeight > 1) ? flSubrectHeight / (nHeight - 1) : 0.0f;

	float flLeftU = flSrcTextureX0 + 0.5f - (0.5f * flTexelsPerPixelX);
	float flRightU = flSrcTextureX1 + 0.5f + (0.5f * flTexelsPerPixelX);
	float flTopV = flSrcTextureY0 + 0.5f - (0.5f * flTexelsPerPixelY);
	float flBottomV = flSrcTextureY1 + 0.5f + (0.5f * flTexelsPerPixelY);

	float flOOTexWidth = 1.0f / nSrcTextureWidth;
	float flOOTexHeight = 1.0f / nSrcTextureHeight;
	flLeftU *= flOOTexWidth;
	flRightU *= flOOTexWidth;
	flTopV *= flOOTexHeight;
	flBottomV *= flOOTexHeight;

	// Get the current viewport size
	int vx, vy, vw, vh;
	pRenderContext->GetViewport(vx, vy, vw, vh);

	// map from screen pixel coords to -1..1
	flRightX = FLerp(-1, 1, 0, vw, flRightX);
	flLeftX = FLerp(-1, 1, 0, vw, flLeftX);
	flTopY = FLerp(1, -1, 0, vh, flTopY);
	flBottomY = FLerp(1, -1, 0, vh, flBottomY);

	// Dice the quad up...
	if (xSegments > 1 || ySegments > 1)
	{
		// Screen height and width of a subrect
		float flWidth = (flRightX - flLeftX) / (float)xSegments;
		float flHeight = (flTopY - flBottomY) / (float)ySegments;

		// UV height and width of a subrect
		float flUWidth = (flRightU - flLeftU) / (float)xSegments;
		float flVHeight = (flBottomV - flTopV) / (float)ySegments;

		for (int x = 0; x < xSegments; x++)
		{
			for (int y = 0; y < ySegments; y++)
			{
				// Top left
				meshBuilder.Position3f(flLeftX + (float)x * flWidth, flTopY - (float)y * flHeight, 0.0f);
				meshBuilder.Normal3f(0.0f, 0.0f, 1.0f);
				meshBuilder.TexCoord2f(0, flLeftU + (float)x * flUWidth, flTopV + (float)y * flVHeight);
				meshBuilder.TangentS3f(0.0f, 1.0f, 0.0f);
				meshBuilder.TangentT3f(1.0f, 0.0f, 0.0f);
				meshBuilder.AdvanceVertex();

				// Top right (x+1)
				meshBuilder.Position3f(flLeftX + (float)(x + 1) * flWidth, flTopY - (float)y * flHeight, 0.0f);
				meshBuilder.Normal3f(0.0f, 0.0f, 1.0f);
				meshBuilder.TexCoord2f(0, flLeftU + (float)(x + 1) * flUWidth, flTopV + (float)y * flVHeight);
				meshBuilder.TangentS3f(0.0f, 1.0f, 0.0f);
				meshBuilder.TangentT3f(1.0f, 0.0f, 0.0f);
				meshBuilder.AdvanceVertex();

				// Bottom right (x+1), (y+1)
				meshBuilder.Position3f(flLeftX + (float)(x + 1) * flWidth, flTopY - (float)(y + 1) * flHeight, 0.0f);
				meshBuilder.Normal3f(0.0f, 0.0f, 1.0f);
				meshBuilder.TexCoord2f(0, flLeftU + (float)(x + 1) * flUWidth, flTopV + (float)(y + 1) * flVHeight);
				meshBuilder.TangentS3f(0.0f, 1.0f, 0.0f);
				meshBuilder.TangentT3f(1.0f, 0.0f, 0.0f);
				meshBuilder.AdvanceVertex();

				// Bottom left (y+1)
				meshBuilder.Position3f(flLeftX + (float)x * flWidth, flTopY - (float)(y + 1) * flHeight, 0.0f);
				meshBuilder.Normal3f(0.0f, 0.0f, 1.0f);
				meshBuilder.TexCoord2f(0, flLeftU + (float)x * flUWidth, flTopV + (float)(y + 1) * flVHeight);
				meshBuilder.TangentS3f(0.0f, 1.0f, 0.0f);
				meshBuilder.TangentT3f(1.0f, 0.0f, 0.0f);
				meshBuilder.AdvanceVertex();
			}
		}
	}
	else // just one quad
	{
		for (int corner = 0; corner<4; corner++)
		{
			bool bLeft = (corner == 0) || (corner == 3);
			meshBuilder.Position3f((bLeft) ? flLeftX : flRightX, (corner & 2) ? flBottomY : flTopY, 0.0f);
			meshBuilder.Normal3f(0.0f, 0.0f, 1.0f);
			meshBuilder.TexCoord2f(0, (bLeft) ? flLeftU : flRightU, (corner & 2) ? flBottomV : flTopV);
			meshBuilder.TangentS3f(0.0f, 1.0f, 0.0f);
			meshBuilder.TangentT3f(1.0f, 0.0f, 0.0f);
			meshBuilder.AdvanceVertex();
		}
	}

	meshBuilder.End();
	pMesh->Draw();

	pRenderContext->MatrixMode(MATERIAL_VIEW);
	pRenderContext->PopMatrix();

	pRenderContext->MatrixMode(MATERIAL_PROJECTION);
	pRenderContext->PopMatrix();
}

void CBeamSegDraw::Start(int nSegs, IMaterial *pMaterial)
{
	m_nSegsDrawn = 0;
	m_nTotalSegs = nSegs;

	CMatRenderContextPtr pRenderContext(MaterialSystemSDK);
	pRenderContext->Bind(pMaterial);

	IMesh* m_pMesh = pRenderContext->GetDynamicMesh(true, 0, 0, pMaterial);
	m_Mesh.Begin(m_pMesh, MATERIAL_TRIANGLE_STRIP, (nSegs - 1) * 2);
}

void  CBeamSegDraw::NextSeg(CBeamSeg *pSeg)
{
	if (m_nSegsDrawn > 0)
	{
		Vector vNormal, vAveNormal;
		ComputeNormal(m_Seg.m_vPos, pSeg->m_vPos, &vNormal);

		if (m_nSegsDrawn > 1)
		{
			// Average this with the previous normal
			VectorAdd(vNormal, m_vNormalLast, vAveNormal);
			vAveNormal *= 0.5f;
			VectorNormalize(vAveNormal);
		}
		else
		{
			vAveNormal = vNormal;
		}

		m_vNormalLast = vNormal;
		SpecifySeg(vAveNormal);
	}

	m_Seg = *pSeg;
	++m_nSegsDrawn;

	if (m_nSegsDrawn == m_nTotalSegs)
		SpecifySeg(m_vNormalLast);
}

void  CBeamSegDraw::End()
{
	m_Mesh.End(false, true);
}
void CBeamSegDraw::SpecifySeg(Vector &vNormal)
{
	C_BasePlayer* LocalPlayer = (C_BasePlayer*)EntityListSDK->GetClientEntity(EngineSDK->GetLocalPlayer());
	if (!LocalPlayer) return;

	Vector va = LocalPlayer->GetAbsOrigin();

	// SUCKY: Need to do a fair amount more work to get the tangent owing to the averaged normal
	Vector vDirToBeam, vTangentY;
	VectorSubtract(m_Seg.m_vPos, va, vDirToBeam);
	CrossProduct(vDirToBeam, vNormal, vTangentY);
	VectorNormalize(vTangentY);

	// Build the endpoints.
	Vector vPoint1, vPoint2;
	VectorMA(m_Seg.m_vPos, m_Seg.m_flWidth * 0.5f, vNormal, vPoint1);
	VectorMA(m_Seg.m_vPos, -m_Seg.m_flWidth * 0.5f, vNormal, vPoint2);

	// Specify the points.
	m_Mesh.Position3fv(vPoint1.Base());
	m_Mesh.Color4f(VectorExpand(m_Seg.m_vColor), m_Seg.m_flAlpha);
	m_Mesh.TexCoord2f(0, 0, m_Seg.m_flTexCoord);
	m_Mesh.TexCoord2f(1, 0, m_Seg.m_flTexCoord);
	m_Mesh.TangentS3fv(vNormal.Base());
	m_Mesh.TangentT3fv(vTangentY.Base());
	m_Mesh.AdvanceVertex();

	m_Mesh.Position3fv(vPoint2.Base());
	m_Mesh.Color4f(VectorExpand(m_Seg.m_vColor), m_Seg.m_flAlpha);
	m_Mesh.TexCoord2f(0, 1, m_Seg.m_flTexCoord);
	m_Mesh.TexCoord2f(1, 1, m_Seg.m_flTexCoord);
	m_Mesh.TangentS3fv(vNormal.Base());
	m_Mesh.TangentT3fv(vTangentY.Base());
	m_Mesh.AdvanceVertex();
}

void CBeamSegDraw::ComputeNormal(Vector &vStartPos, Vector &vNextPos, Vector *pNormal)
{
	// vTangentY = line vector for beam
	Vector vTangentY;
	VectorSubtract(vStartPos, vNextPos, vTangentY);

	C_BasePlayer* LocalPlayer = (C_BasePlayer*)EntityListSDK->GetClientEntity(EngineSDK->GetLocalPlayer());
	if (!LocalPlayer) return;

	Vector va = LocalPlayer->GetAbsOrigin();

	// vDirToBeam = vector from viewer origin to beam
	Vector vDirToBeam;
	VectorSubtract(vStartPos, va, vDirToBeam);

	// Get a vector that is perpendicular to us and perpendicular to the beam.
	// This is used to fatten the beam.
	CrossProduct(vTangentY, vDirToBeam, *pNormal);

	VectorNormalize(*pNormal);
}

void InterpolateAngles(const QAngle& start, const QAngle& end, QAngle& output, float frac)
{
	Quaternion src, dest;

	// Convert to quaternions
	AngleQuaternion(start, src);
	AngleQuaternion(end, dest);

	Quaternion result;

	// Slerp
	QuaternionSlerp(src, dest, frac, result);

	// Convert to euler
	QuaternionAngles(result, output);
}

void NormalizeAngles(QAngle &output)
{
	for (int i = 0; i < 3; i++)
	{
		if (output[i] > 180.f)
		{
			output[i] -= 360.f;
		}
		if (output[i] < -180.f)
		{
			output[i] += 360.f;
		}
	}
}
void KeyDown(kbutton_t *b, const char *c)
{
	int		k = -1;
	if (c && c[0])
	{
		k = atoi(c);
	}

	if (k == b->down[0] || k == b->down[1])
		return;		// repeating key

	if (!b->down[0])
		b->down[0] = k;
	else if (!b->down[1])
		b->down[1] = k;
	else
	{
		return;
	}

	if (b->state & 1)
		return;		// still down
	b->state |= 1 + 2;	// down + impulse down
}

/*
============
KeyUp
============
*/
void KeyUp(kbutton_t *b, const char *c)
{
	if (!c || !c[0])
	{
		b->down[0] = b->down[1] = 0;
		b->state = 4;	// impulse up
		return;
	}

	int k = atoi(c);

	if (b->down[0] == k)
		b->down[0] = 0;
	else if (b->down[1] == k)
		b->down[1] = 0;
	else
		return;		// key up without coresponding down (menu pass through)

	if (b->down[0] || b->down[1])
	{
		//Msg ("Keys down for button: '%c' '%c' '%c' (%d,%d,%d)!\n", b->down[0], b->down[1], c, b->down[0], b->down[1], c);
		return;		// some other key is still holding it down
	}

	if (!(b->state & 1))
		return;		// still up (this should not happen)

	b->state &= ~1;		// now up
	b->state |= 4; 		// impulse up
}

void VectorAngles2(const Vector& forward, QAngle &angles)
{
	float	tmp, yaw, pitch;

	if (forward[1] == 0.f && forward[0] == 0.f)
	{
		yaw = 0.f;
		if (forward[2] > 0.f)
			pitch = 270.f;
		else
			pitch = 90.f;
	}
	else
	{
		yaw = ((float)atan2(forward[1], forward[0]) * 180.f / M_PI_F);
		if (yaw < 0.f)
			yaw += 360.f;

		tmp = (float)sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = ((float)atan2(-forward[2], tmp) * 180.f / M_PI_F);
		if (pitch < 0.f)
			pitch += 360.0f;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0.f;
}
