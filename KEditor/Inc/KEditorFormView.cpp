
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
	ON_BN_CLICKED(IDC_BUTTON_CREATE, &CKEditorFormView::OnBnClickedButtonCreate)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CKEditorFormView::OnCbnSelchangeComboType)
	ON_EN_CHANGE(IDC_EDIT_COUNT_X, &CKEditorFormView::OnEnChangeEditCountX)
	ON_EN_CHANGE(IDC_EDIT_COUNT_Y, &CKEditorFormView::OnEnChangeEditCountY)
	ON_EN_CHANGE(IDC_EDIT_SIZE_X, &CKEditorFormView::OnEnChangeEditSizeX)
	ON_EN_CHANGE(IDC_EDIT_SIZE_Y, &CKEditorFormView::OnEnChangeEditSizeY)
	ON_BN_CLICKED(IDC_BUTTON_DESTROY, &CKEditorFormView::OnBnClickedButtonDestroy)
	ON_CBN_SELCHANGE(IDC_COMBO_OPTION, &CKEditorFormView::OnCbnSelchangeComboOption)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CKEditorFormView::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CKEditorFormView::OnBnClickedButtonLoad)
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
	DDX_Control(pDX, IDC_COMBO_TYPE, type_combo_box_);
	DDX_Control(pDX, IDC_COMBO_OPTION, option_combo_box_);
	DDX_Text(pDX, IDC_EDIT_COUNT_X, count_x_);
	DDX_Text(pDX, IDC_EDIT_COUNT_Y, count_y_);
	DDX_Text(pDX, IDC_EDIT_SIZE_X, size_x_);
	DDX_Text(pDX, IDC_EDIT_SIZE_Y, size_y_);
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

	type_combo_box_.AddString(L"Isometric");
	type_combo_box_.AddString(L"Orthographic");
	type_combo_box_.SetCurSel(0);

	option_combo_box_.AddString(L"1. Normal");
	option_combo_box_.AddString(L"2. Blocked");
	option_combo_box_.SetCurSel(0);
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


void CKEditorFormView::OnBnClickedButtonCreate()
{
	UpdateData(true);

	auto const& tile_map = std::static_pointer_cast<K::TileMapActor>(K::WorldManager::singleton()->FindActor(K::TAG{ K::TILE_MAP, 0 }));

	tile_map->CreateMap(static_cast<K::TILE_TYPE>(type_combo_box_.GetCurSel()), count_x_, count_y_, K::Vector2{ size_x_, size_y_ });

	UpdateData(false);
}


void CKEditorFormView::OnCbnSelchangeComboType()
{
}


void CKEditorFormView::OnCbnSelchangeComboOption()
{
}


void CKEditorFormView::OnEnChangeEditCountX()
{
}


void CKEditorFormView::OnEnChangeEditCountY()
{
}


void CKEditorFormView::OnEnChangeEditSizeX()
{
}


void CKEditorFormView::OnEnChangeEditSizeY()
{
}


void CKEditorFormView::OnBnClickedButtonDestroy()
{
	auto const& tile_map = std::static_pointer_cast<K::TileMapActor>(K::WorldManager::singleton()->FindActor(K::TAG{ K::TILE_MAP, 0 }));

	tile_map->DestroyMap();
}

K::TILE_OPTION CKEditorFormView::GetTileOption() const
{
	return static_cast<K::TILE_OPTION>(option_combo_box_.GetCurSel());
}

void CKEditorFormView::SaveTileMap(CString const& _path)
{
	std::wstring file_name = CT2CW(_path);

	std::fstream file{ file_name, std::ios::out | std::ios::binary };

	auto const& tile_map = K::WorldManager::singleton()->FindActor(K::TAG{ K::TILE_MAP, 0 });
	
	K::OutputMemoryStream omstream{};
	tile_map->Serialize(omstream);

	file.write(reinterpret_cast<char const*>(omstream.buffer()->data()), omstream.head());
}

void CKEditorFormView::LoadTileMap(CString const& _path)
{
	std::wstring file_name = CT2CW(_path);

	std::fstream file{ file_name, std::ios::in | std::ios::binary | std::ios::ate };

	auto const& tile_map = K::WorldManager::singleton()->FindActor(K::TAG{ K::TILE_MAP, 0 });

	auto size = file.tellg();
	file.seekg(std::ios::beg);

	K::InputMemoryStream imstream{};
	imstream.Resize(size);
	file.read(reinterpret_cast<char*>(imstream.buffer()->data()), size);

	tile_map->Serialize(imstream);
}

CComboBox& CKEditorFormView::option_combo_box()
{
	return option_combo_box_;
}



void CKEditorFormView::OnBnClickedButtonSave()
{
	CFileDialog	file_dialog(false, L".tm", nullptr, OFN_OVERWRITEPROMPT, L"TileMap Files (*.tm)|*.tm|All Files (*.*)|*.*||");

	if (file_dialog.DoModal() == IDOK)
	{
		CString path = file_dialog.GetPathName();

		SaveTileMap(path);
	}
}


void CKEditorFormView::OnBnClickedButtonLoad()
{
	CFileDialog	file_dialog(true, L".tm", nullptr, OFN_HIDEREADONLY, L"TileMap Files (*.tm)|*.tm|All Files (*.*)|*.*||");

	if (file_dialog.DoModal() == IDOK)
	{
		CString path = file_dialog.GetPathName();

		LoadTileMap(path);
	}
}
