
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "KEditor.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.style = WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;

	RECT rc{ 0, 0, static_cast<int>(K::RESOLUTION::WIDTH) , static_cast<int>(K::RESOLUTION::HEIGHT) };
	AdjustWindowRectEx(&rc, cs.style, true, cs.dwExStyle);

	cs.cx = rc.right - rc.left;
	cs.cy = rc.bottom - rc.top;

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	auto result = splitter_.CreateStatic(this, 1, 2);

	splitter_.CreateView(0, 0, RUNTIME_CLASS(CKEditorFormView), CSize(400, 900), pContext);
	splitter_.CreateView(0, 1, RUNTIME_CLASS(CKEditorView), CSize(1200, 900), pContext);

	return result;
}


K::Splitter const& CMainFrame::splitter()
{
	// TODO: 여기에 구현 코드 추가.
	return splitter_;
}

int K::Splitter::HitTest(CPoint pt) const
{
	return HTNOWHERE;
}
