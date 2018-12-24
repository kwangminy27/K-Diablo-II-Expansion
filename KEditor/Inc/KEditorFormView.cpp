
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
#include <Object/Actor/NPC/akara.h>

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
	ON_CBN_SELCHANGE(IDC_COMBO_MONSTER_TYPE, &CKEditorFormView::OnCbnSelchangeComboActorType)
	ON_EN_CHANGE(IDC_EDIT_COUNT_X, &CKEditorFormView::OnEnChangeEditCountX)
	ON_EN_CHANGE(IDC_EDIT_COUNT_Y, &CKEditorFormView::OnEnChangeEditCountY)
	ON_EN_CHANGE(IDC_EDIT_SIZE_X, &CKEditorFormView::OnEnChangeEditSizeX)
	ON_EN_CHANGE(IDC_EDIT_SIZE_Y, &CKEditorFormView::OnEnChangeEditSizeY)
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
	DDX_Control(pDX, IDC_COMBO_MONSTER_TYPE, actor_type_combo_box_);
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

	actor_type_combo_box_.AddString(L"1. Cow");
	actor_type_combo_box_.AddString(L"2. Wendigo");
	actor_type_combo_box_.AddString(L"3. Fallen Shaman");
	actor_type_combo_box_.AddString(L"4. Andariel");
	actor_type_combo_box_.AddString(L"5. Akara");
	actor_type_combo_box_.SetCurSel(0);
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

void CKEditorFormView::OnCbnSelchangeComboActorType()
{
	CImage image{};

	auto actor_type = GetActorType();

	auto path = K::PathManager::singleton()->FindPath(K::TEXTURE_PATH);

	if (path.empty())
		return;

	if ("1. Cow" == actor_type)
		path /= "Main/cow/cow.png";
	else if ("2. Wendigo" == actor_type)
		path /= "Main/wendigo/wendigo.png";
	else if ("3. Fallen Shaman" == actor_type)
		path /= "Main/fallen shaman/fallen_shaman.png";
	else if ("4. Andariel" == actor_type)
		path /= "Main/andariel/andariel.png";
	else if ("5. Akara" == actor_type)
		path /= "Main/akara/akara.png";

	image.Load(path.wstring().c_str());
	image.Draw(GetDC()->m_hDC, 230, 360, 126, 168);
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

	auto const& tile_layer = K::WorldManager::singleton()->FindLayer(K::TAG{ "TileLayer", 0 });
	auto const& layer = K::WorldManager::singleton()->FindLayer(K::TAG{ "DefaultLayer", 1 });

	for (auto& actor : actor_list_)
	{
		tile_layer->RemoveActor(actor);
		layer->RemoveActor(actor);
	}

	actor_list_.clear();
}

K::TILE_OPTION CKEditorFormView::GetTileOption() const
{
	return static_cast<K::TILE_OPTION>(option_combo_box_.GetCurSel());
}

std::string CKEditorFormView::GetActorType() const
{
	std::string actor_type{};

	CString string{};
	actor_type_combo_box_.GetLBText(actor_type_combo_box_.GetCurSel(), string);

	actor_type = CT2CA(string);

	return actor_type;
}

void CKEditorFormView::SaveLevel(CString const& _path)
{
	std::wstring file_name = CT2CW(_path);

	std::fstream file{ file_name, std::ios::out | std::ios::binary };

	auto const& tile_map = K::WorldManager::singleton()->FindActor(K::TAG{ K::TILE_MAP, 0 });

	K::OutputMemoryStream omstream{};
	tile_map->Serialize(omstream);

	auto actor_size = actor_list_.size();
	omstream.Serialize(actor_size);

	for (auto& actor : actor_list_)
	{
		K::TAG tag = actor->tag();

		omstream.Serialize(tag.first);
		omstream.Serialize(tag.second);

		actor->Serialize(omstream);
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

	size_t actor_size{};
	imstream.Serialize(actor_size);

	auto const& object_manager = K::ObjectManager::singleton();

	for (size_t i = 0; i < actor_size; ++i)
	{
		K::TAG tag{};

		imstream.Serialize(tag.first);
		imstream.Serialize(tag.second);

		K::APTR actor{};

		if (tag.first == "Cow")
			actor = object_manager->CreateActor<K::Cow>(tag);
		else if (tag.first == "Wendigo")
			actor = object_manager->CreateActor<K::Wendigo>(tag);
		else if (tag.first == "FallenShaman")
			actor = object_manager->CreateActor<K::FallenShaman>(tag);
		else if (tag.first == "Andariel")
			actor = object_manager->CreateActor<K::Andariel>(tag);
		else if (tag.first == "Akara")
			actor = object_manager->CreateActor<K::Akara>(tag);

		actor->Serialize(imstream);

		auto const& default_layer = K::WorldManager::singleton()->FindLayer(K::TAG{ "DefaultLayer", 1 });
		default_layer->AddActor(actor);

		actor_list_.push_back(actor);
	}
}

CComboBox& CKEditorFormView::option_combo_box()
{
	return option_combo_box_;
}

std::list<K::APTR>& CKEditorFormView::actor_list()
{
	return actor_list_;
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

void CKEditorFormView::OnDraw(CDC* pDC)
{
	CImage image{};

	auto actor_type = GetActorType();

	auto path = K::PathManager::singleton()->FindPath(K::TEXTURE_PATH);

	if (path.empty())
		return;

	if ("1. Cow" == actor_type)
		path /= "Main/cow/cow.png";
	else if ("2. Wendigo" == actor_type)
		path /= "Main/wendigo/wendigo.png";
	else if ("3. Fallen Shaman" == actor_type)
		path /= "Main/fallen shaman/fallen_shaman.png";
	else if ("4. Andariel" == actor_type)
		path /= "Main/andariel/andariel.png";
	else if ("5. Akara" == actor_type)
		path /= "Main/akara/akara.png";

	image.Load(path.wstring().c_str());
	image.Draw(pDC->m_hDC, 230, 360, 126, 168);
}
