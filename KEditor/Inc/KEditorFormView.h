
// KEditorFormView.h: CKEditorFormView 클래스의 인터페이스
//

#pragma once

#include "KEditorDoc.h"

class CKEditorFormView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CKEditorFormView() noexcept;
	DECLARE_DYNCREATE(CKEditorFormView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_KEDITOR_FORM };
#endif

// 특성입니다.
public:
	CKEditorDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CKEditorFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // KEditorFormView.cpp의 디버그 버전
inline CKEditorDoc* CKEditorFormView::GetDocument() const
   { return reinterpret_cast<CKEditorDoc*>(m_pDocument); }
#endif

