// KEditorView.cpp: 구현 파일
//

#include "stdafx.h"
#include "KEditor.h"
#include "KEditorView.h"


// KEditorView

IMPLEMENT_DYNCREATE(CKEditorView, CView)

CKEditorView::CKEditorView()
{

}

CKEditorView::~CKEditorView()
{
}

BEGIN_MESSAGE_MAP(CKEditorView, CView)
END_MESSAGE_MAP()


// KEditorView 그리기

void CKEditorView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// KEditorView 진단

#ifdef _DEBUG
void CKEditorView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CKEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// KEditorView 메시지 처리기
