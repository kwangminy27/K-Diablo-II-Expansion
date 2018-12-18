
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

#include <Object/Actor/Monster/cow.h>
#include <Object/Actor/Monster/wendigo.h>
#include <Object/Actor/Monster/fallen_shaman.h>
#include <Object/Actor/Monster/andariel.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKEditorFormView
IMPLEMENT_DYNCREATE(CKEditorFormView, CFormView)

BEGIN_MESSAGE_MAP(CKEditorFormView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_CREATE, &CKEditorFormView::OnBnClickedButtonCreate)
	ON_BN_CLICKED(IDC_BUTTON_DESTROY, &CKEditorFormView::OnBnClickedButtonDestroy)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CKEditorFormView::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CKEditorFormView::OnBnClickedButtonLoad)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CKEditorFormView::OnCbnSelchangeComboType)
	ON_CBN_SELCHANGE(IDC_COMBO_OPTION, &CKEditorFormView::OnCbnSelchangeComboOption)
	ON_CBN_SELCHANGE(IDC_COMBO_MONSTER_TYPE, &CKEditorFormView::OnCbnSelchangeComboMonsterType)
	ON_EN_CHANGE(IDC_EDIT_COUNT_X, &CKEditorFormView::OnEnChangeEditCountX)
	ON_EN_CHANGE(IDC_EDIT_COUNT_Y, &CKEditorFormView::OnEnChangeEditCountY)
	ON_EN_CHANGE(IDC_EDIT_SIZE_X, &CKEditorFormView::OnEnChangeEditSizeX)
	ON_EN_CHANGE(IDC_EDIT_SIZE_Y, &CKEditorFormView::OnEnChangeEditSizeY)
	ON_EN_CHANGE(IDC_EDIT_SCALING_X, &CKEditorFormView::OnEnChangeEditScalingX)
	ON_EN_CHANGE(IDC_EDIT_SCALING_Y, &CKEditorFormView::OnEnChangeEditScalingY)
	ON_EN_CHANGE(IDC_EDIT_SCALING_Z, &CKEditorFormView::OnEnChangeEditScalingZ)
	ON_EN_CHANGE(IDC_EDIT_ROTATION_X, &CKEditorFormView::OnEnChangeEditRotationX)
	ON_EN_CHANGE(IDC_EDIT_ROTATION_Y, &CKEditorFormView::OnEnChangeEditRotationY)
	ON_EN_CHANGE(IDC_EDIT_ROTATION_Z, &CKEditorFormView::OnEnChangeEditRotationZ)
	ON_EN_CHANGE(IDC_EDIT_TRANSLATION_X, &CKEditorFormView::OnEnChangeEditTranslationX)
	ON_EN_CHANGE(IDC_EDIT_TRANSLATION_Y, &CKEditorFormView::OnEnChangeEditTranslationY)
	ON_EN_CHANGE(IDC_EDIT_TRANSLATION_Z, &CKEditorFormView::OnEnChangeEditTranslationZ)	
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
	DDX_Control(pDX, IDC_COMBO_MONSTER_TYPE, monster_type_combo_box_);
	DDX_Text(pDX, IDC_EDIT_COUNT_X, count_x_);
	DDX_Text(pDX, IDC_EDIT_COUNT_Y, count_y_);
	DDX_Text(pDX, IDC_EDIT_SIZE_X, size_x_);
	DDX_Text(pDX, IDC_EDIT_SIZE_Y, size_y_);
	DDX_Text(pDX, IDC_EDIT_SCALING_X, scaling_x_);
	DDX_Text(pDX, IDC_EDIT_SCALING_Y, scaling_y_);
	DDX_Text(pDX, IDC_EDIT_SCALING_Z, scaling_z_);
	DDX_Text(pDX, IDC_EDIT_ROTATION_X, rotation_x_);
	DDX_Text(pDX, IDC_EDIT_ROTATION_Y, rotation_y_);
	DDX_Text(pDX, IDC_EDIT_ROTATION_Z, rotation_z_);
	DDX_Text(pDX, IDC_EDIT_TRANSLATION_X, translation_x_);
	DDX_Text(pDX, IDC_EDIT_TRANSLATION_Y, translation_y_);
	DDX_Text(pDX, IDC_EDIT_TRANSLATION_Z, translation_z_);
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

	monster_type_combo_box_.AddString(L"Cow");
	monster_type_combo_box_.AddString(L"Wendigo");
	monster_type_combo_box_.AddString(L"Fallen Shaman");
	monster_type_combo_box_.AddString(L"Andariel");
	monster_type_combo_box_.SetCurSel(0);
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

void CKEditorFormView::OnCbnSelchangeComboMonsterType()
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

void CKEditorFormView::OnEnChangeEditScalingX()
{
	UpdateData(true);
	UpdateData(false);
}

void CKEditorFormView::OnEnChangeEditScalingY()
{
	UpdateData(true);
	UpdateData(false);
}

void CKEditorFormView::OnEnChangeEditScalingZ()
{
	UpdateData(true);
	UpdateData(false);
}

void CKEditorFormView::OnEnChangeEditRotationX()
{
	UpdateData(true);
	UpdateData(false);
}

void CKEditorFormView::OnEnChangeEditRotationY()
{
	UpdateData(true);
	UpdateData(false);
}

void CKEditorFormView::OnEnChangeEditRotationZ()
{
	UpdateData(true);
	UpdateData(false);
}

void CKEditorFormView::OnEnChangeEditTranslationX()
{
	UpdateData(true);
	UpdateData(false);
}

void CKEditorFormView::OnEnChangeEditTranslationY()
{
	UpdateData(true);
	UpdateData(false);
}

void CKEditorFormView::OnEnChangeEditTranslationZ()
{
	UpdateData(true);
	UpdateData(false);
}

void CKEditorFormView::OnBnClickedButtonDestroy()
{
	auto const& tile_map = std::static_pointer_cast<K::TileMapActor>(K::WorldManager::singleton()->FindActor(K::TAG{ K::TILE_MAP, 0 }));

	tile_map->DestroyMap();

	auto const& layer = K::WorldManager::singleton()->FindLayer(K::TAG{ "DefaultLayer", 0 });

	for (auto& monster : monster_list_)
		layer->RemoveActor(monster);

	monster_list_.clear();
}

K::TILE_OPTION CKEditorFormView::GetTileOption() const
{
	return static_cast<K::TILE_OPTION>(option_combo_box_.GetCurSel());
}

std::string CKEditorFormView::GetMonsterType() const
{
	std::string monster_type{};

	CString string{};
	GetDlgItemText(IDC_COMBO_MONSTER_TYPE, string);

	monster_type = CT2CA(string);

	return monster_type;
}

K::Vector3 CKEditorFormView::GetScaling() const
{
	return K::Vector3{ scaling_x_, scaling_y_, scaling_z_ };
}

K::Vector3 CKEditorFormView::GetRotation() const
{
	return K::Vector3{ rotation_x_, rotation_y_, rotation_z_ };
}

K::Vector3 CKEditorFormView::GetTranslation() const
{
	return K::Vector3{ translation_x_, translation_y_, translation_z_ };
}

void CKEditorFormView::SaveLevel(CString const& _path)
{
	std::wstring file_name = CT2CW(_path);

	std::fstream file{ file_name, std::ios::out | std::ios::binary };

	auto const& tile_map = K::WorldManager::singleton()->FindActor(K::TAG{ K::TILE_MAP, 0 });

	K::OutputMemoryStream omstream{};
	tile_map->Serialize(omstream);

	auto monster_size = monster_list_.size();
	omstream.Serialize(monster_size);

	for (auto& monster : monster_list_)
	{
		K::TAG tag = monster->tag();

		omstream.Serialize(tag.first);
		omstream.Serialize(tag.second);

		monster->Serialize(omstream);
	}

	file.write(reinterpret_cast<char const*>(omstream.buffer()->data()), omstream.head());
}

void CKEditorFormView::LoadLevel(CString const& _path)
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

	size_t monster_size{};
	imstream.Serialize(monster_size);

	auto const& object_manager = K::ObjectManager::singleton();

	for (size_t i = 0; i < monster_size; ++i)
	{
		K::TAG tag{};

		imstream.Serialize(tag.first);
		imstream.Serialize(tag.second);

		K::APTR monster{};

		if (tag.first == "Cow")
			monster = object_manager->CreateActor<K::Cow>(tag);
		else if (tag.first == "Wendigo")
			monster = object_manager->CreateActor<K::Wendigo>(tag);
		else if (tag.first == "Fallen Shaman")
			monster = object_manager->CreateActor<K::FallenShaman>(tag);
		else if (tag.first == "Andariel")
			monster = object_manager->CreateActor<K::Andariel>(tag);

		monster->Serialize(imstream);

		auto const& default_layer = K::WorldManager::singleton()->FindLayer(K::TAG{ "DefaultLayer", 0 });
		default_layer->AddActor(monster);

		monster_list_.push_back(monster);
	}
}

CComboBox& CKEditorFormView::option_combo_box()
{
	return option_combo_box_;
}

std::list<K::APTR>& CKEditorFormView::monster_list()
{
	return monster_list_;
}

int CKEditorFormView::count_x() const
{
	return count_x_;
}

int CKEditorFormView::count_y() const
{
	return count_y_;
}

void CKEditorFormView::OnBnClickedButtonSave()
{
	CFileDialog	file_dialog(false, L".lv", nullptr, OFN_OVERWRITEPROMPT, L"Level Files (*.lv)|*.lv|All Files (*.*)|*.*||");

	if (file_dialog.DoModal() == IDOK)
	{
		CString path = file_dialog.GetPathName();

		SaveLevel(path);
	}
}

void CKEditorFormView::OnBnClickedButtonLoad()
{
	CFileDialog	file_dialog(true, L".lv", nullptr, OFN_HIDEREADONLY, L"Level Files (*.lv)|*.lv|All Files (*.*)|*.*||");

	if (file_dialog.DoModal() == IDOK)
	{
		CString path = file_dialog.GetPathName();

		LoadLevel(path);
	}
}
