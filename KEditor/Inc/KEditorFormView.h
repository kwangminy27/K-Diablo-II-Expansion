
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
public:
	afx_msg void OnBnClickedButtonCreate();
	afx_msg void OnCbnSelchangeComboType();
	afx_msg void OnCbnSelchangeComboOption();
	afx_msg void OnCbnSelchangeComboActorType();
	afx_msg void OnEnChangeEditCountX();
	afx_msg void OnEnChangeEditCountY();
	afx_msg void OnEnChangeEditSizeX();
	afx_msg void OnEnChangeEditSizeY();
	afx_msg void OnEnChangeEditScalingX();
	afx_msg void OnEnChangeEditScalingY();
	afx_msg void OnEnChangeEditScalingZ();
	afx_msg void OnEnChangeEditRotationX();
	afx_msg void OnEnChangeEditRotationY();
	afx_msg void OnEnChangeEditRotationZ();
	afx_msg void OnEnChangeEditTranslationX();
	afx_msg void OnEnChangeEditTranslationY();
	afx_msg void OnEnChangeEditTranslationZ();
	afx_msg void OnBnClickedButtonDestroy();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();

	K::TILE_OPTION GetTileOption() const;
	std::string GetActorType() const;

	K::Vector3 GetScaling() const;
	K::Vector3 GetRotation() const;
	K::Vector3 GetTranslation() const;

	void SaveLevel(CString const& _path);
	void LoadLevel(CString const& _path);

	CComboBox& option_combo_box();
	std::list<K::APTR>& actor_list();

	int count_x() const;
	int count_y() const;

	CComboBox type_combo_box_;
	CComboBox option_combo_box_;
	CComboBox actor_type_combo_box_;
	int count_x_{};
	int count_y_{};
	float size_x_{};
	float size_y_{};
	float scaling_x_{};
	float scaling_y_{};
	float scaling_z_{};
	float rotation_x_{};
	float rotation_y_{};
	float rotation_z_{};
	float translation_x_{};
	float translation_y_{};
	float translation_z_{};

	std::list<K::APTR> actor_list_{};
};

#ifndef _DEBUG  // KEditorFormView.cpp의 디버그 버전
inline CKEditorDoc* CKEditorFormView::GetDocument() const
   { return reinterpret_cast<CKEditorDoc*>(m_pDocument); }
#endif

