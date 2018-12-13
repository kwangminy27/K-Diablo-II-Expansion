
// KEditorFormView.cpp: CKEditorFormView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "KEditor.h"
#endif

#include "KEditorDoc.h"
#include "KEditorFormView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKEditorFormView

IMPLEMENT_DYNCREATE(CKEditorFormView, CFormView)

BEGIN_MESSAGE_MAP(CKEditorFormView, CFormView)
END_MESSAGE_MAP()

// CKEditorFormView 생성/소멸

CKEditorFormView::CKEditorFormView() noexcept
	: CFormView(IDD_KEDITOR_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CKEditorFormView::~CKEditorFormView()
{
}

void CKEditorFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CKEditorFormView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CKEditorFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CKEditorFormView 진단

#ifdef _DEBUG
void CKEditorFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CKEditorFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CKEditorDoc* CKEditorFormView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKEditorDoc)));
	return (CKEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CKEditorFormView 메시지 처리기
