#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "Monster.h"
#include "Player.h"
#include "Camera_Dynamic.h"
#include "BackGround.h"
#include "Terrain.h"
#include "Sky.h"
#include "UI.h"
#include "ThunderCloud.h"
#include "ThunderSword.h"
#include "Tornado.h"
#include "Hit.h"
#include "UseSkill.h"
#include "HpBar.h"
#include "MpBar.h"
#include "IconBar.h"
#include "InventoryIcon.h"
#include "Inventory.h"
#include "XBox.h"
#include "LevelUp.h"
#include "DmgFont.h"
#include "FireSpear.h"
#include "Fire.h"
#include "Meteor.h"
#include "MeteorEff.h"
#include "Rain.h"
#include"EquipIcon.h"
#include"SkillIcon.h"
#include"StatsIcon.h"
#include"QuickSlot.h"
#include"SkillWnd.h"
#include"StatsWnd.h"
#include"PlusBox.h"
#include"WorldHpBar.h"
#include"PlayerMpBar.h"
#include "FireDragon.h"
#include "Wind.h"
#include "Shadow.h"
#include"ExpBar.h"
#include"ExpLogo.h"
#include"ExpBlank.h"
#include "BackGroundRect.h"
#include "Poring.h"
#include "BackGroundTree.h"
#include "House.h"
#include "Stone.h"
#include "Portal.h"
#include "PlayerInfo.h"
#include "Wing.h"
#include "House2.h"
#include"SkillSlot.h"
#include"HpPotion.h"
#include "Default_NPC.h"
#include "Village_Chief.h"
#include "Village_Quest1.h"
#include "Village_Quest2.h"
#include "Engineer.h"
#include "PoisonArrow.h"
#include "Alligator.h"
#include "TerrainRect.h"

CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

unsigned int APIENTRY Thread_Main(void* pArg)
{
	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CriticalSection());

	switch (pLoader->Get_NextLevelID())
	{
	case LEVEL_LOGO:
		pLoader->Loading_ForLogoLevel();
		break;
	case LEVEL_GAMEPLAY:
		pLoader->Loading_ForGamePlayLevel();
		break;
	case LEVEL_TOWN:
		pLoader->Loading_ForTownLevel();
		break;
	}

	LeaveCriticalSection(&pLoader->Get_CriticalSection());

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevel)
{
	m_eNextLevel = eNextLevel;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;
		
	return S_OK;
}

HRESULT CLoader::Loading_ForLogoLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	
	Loading_Prototype();
	Loading_Static(LEVEL_STATIC);

	lstrcpy(m_szLoadingText, TEXT("?????? ??????????????."));

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	/* ?????? ???? ??. */
	lstrcpy(m_szLoadingText, TEXT("?????? ???? ??."));

	

	/* ???? ???? ??. */
	lstrcpy(m_szLoadingText, TEXT("???? ???? ??."));


	/* ?????? ???? ??. */
	lstrcpy(m_szLoadingText, TEXT("?????? ???? ??."));
	

	/* ???? ???? ??. */
	lstrcpy(m_szLoadingText, TEXT("???? ???? ??."));



	lstrcpy(m_szLoadingText, TEXT("?????? ??????????????."));

	Safe_Release(pGameInstance);

	m_isFinished = true;


	return S_OK;
}

HRESULT CLoader::Loading_ForTownLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	
	
	
	

	lstrcpy(m_szLoadingText, TEXT("???? ?????? ??????????????."));

	m_isFinished = true;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_Static(LEVEL Level)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);
	//NPC
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Default_NPC"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/NPC/Default/%d.png"),3))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Village_Chief"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/NPC/Village_Chief/%d.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Village_Quest1"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/NPC/Village_Quest1/%d.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Village_Quest2"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/NPC/Village_Quest2/%d.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Engineer"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/NPC/Engineer/%d.png"), 1))))
		return E_FAIL;
	//Pet ??????
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Poring_IDLE_Back"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Poring/IDLE_Back/%d.png"), 4))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Poring_IDLE_Front"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Poring/IDLE_Front/%d.png"), 4))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Poring_Move_Front"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Poring/Move_Front/%d.png"), 8))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Poring_Move_Back"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Poring/Move_Back/%d.png"), 8))))
		return E_FAIL;

	//UI ??????
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_UI"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/UI/INTERFACE/Info%d.png"), 2))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Temp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/UI/QuickSlot/QuickSlot%d.png"), 9))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_StatsWnd"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/UI/StatWnd/StatWnd%d.png"), 3))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_PlusBox"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/UI/StatWnd/StatWnd%d.png"), 3))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_XBox"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/UI/XBox/XBox%d.png"), 2))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Item"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/UI/StatWnd/StatWnd%d.png"), 3))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_HpBar"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/UI/HpBar/HpBar%d.png"), 2))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_ExpBar"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/UI/ExpBar/ExpBar%d.png"), 2))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_ExpLogo"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/UI/ExpBar/ExpLogo%d.png"), 2))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_ExpBlank"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/UI/ExpBar/ExpBar%d.png"), 2))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_TePrototype_Component_Texture_QuickSlotxture_PlayerMpBar"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/UI/HpBar/HpBar%d.png"), 2))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_WorldHpBar"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/UI/HpBar/HpBar%d.png"), 3))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_MpBar"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/UI/HpBar/HpBar%d.png"), 2))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Inventory"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/UI/Inventory/Inventory%d.jpg"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_QuickSlot"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/UI/QuickSlot/QuickSlot%d.jpg"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_SkillWnd"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/UI/SkillWnd/SkillWnd%d.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_IconBar"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/UI/IconBar%d.jpg"), 2))))
		return E_FAIL;

	//Icon ??????
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_EquipIcon"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/UI/UIWnd/UIWnd%d.png"), 2))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_InventoryIcon"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/UI/UIWnd/UIWnd%d.png"), 4))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_SkillIcon"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/UI/UIWnd/UIWnd%d.png"), 6))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_StatsIcon"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/UI/UIWnd/UIWnd%d.png"), 8))))
		return E_FAIL;

	// Terrain ??????
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.tga"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_TerrainRect"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/MAP/LookMap/Map%d.png"), 1070))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Portal"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/BackGround/Portal/Portal%d.png"), 24))))
		return E_FAIL;
	// sky ??????
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
		return E_FAIL;

	//Effect ??????
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Wind"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Wind/%d.bmp"), 5))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Hit"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Skill/Hit/Effect%d.png"), 3))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_ThunderEff"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/ThunderEff/Effec%d.bmp"), 15))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_DmgFont"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/DmgFont/%d.png"), 10))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_UseSkill"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/UseSkill/Effect%d.bmp"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_LevelUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Level/ItemLevelUp_%d.bmp"), 22))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_MeteorEff"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/MeteorEff/Effect%d.bmp"), 20))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Rain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/snow/snow.png"), 1))))
		return E_FAIL;

	//Skill ??????
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_ThunderCloud"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Skill/Thunder/ThunderCloud/Cloud%d.bmp"), 18))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_ThunderSword"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Skill/Thunder/ThunderSword/%d.bmp"), 17))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Tornado"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Skill/Tornado/Effect%d.bmp"), 25))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_FireSpear"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Boss_Skill/FireSpear/%d.bmp"), 11))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Fire"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Fire/%d.bmp"), 4))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Meteor"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Boss_Skill/Meteor/%d.png"), 12))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_PoisonArrow"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/PoisonArrow/Effect%d.bmp"), 11))))
		return E_FAIL;

	//Player ??????
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_IDLE_Back"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Player/IDLE_Back/%d.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_IDLE_Front"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Player/IDLE_Front/%d.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Move_Front"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Player/Move_Front/%d.png"), 8))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Move_Back"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Player/Move_Back/%d.png"), 8))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Skill_Back"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Player/Skill_Back/%d.png"), 5))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Skill_Front"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Player/Skill_Front/%d.png"), 5))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Ride_IDLE_Back"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Player/Ride_IDLE_Back/%d.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Ride_IDLE_Front"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Player/Ride_IDLE_Front/%d.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Ride_Move_Front"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Player/Ride_Move_Front/%d.png"), 5))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Ride_Move_Back"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Player/Ride_Move_Back/%d.png"), 5))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Shadow"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Shadow/%d.bmp"), 1))))
		return E_FAIL;

	//Wing
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Wing"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Player/Wing/%d.png"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_House_Body"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/House/Body/%d.dds"), 2))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_House_Head"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/House/Head/%d.png"), 1))))
		return E_FAIL;
	
	//Boss
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_FireDragon_IDLE_Front"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/BOSS/FireDragon/IDLE_Front/%d.png"), 7))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_FireDragon_IDLE_Back"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/BOSS/FireDragon/IDLE_Back/%d.png"), 7))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_FireDragon_Dead"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/BOSS/FireDragon/Dead/%d.png"), 4))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_FireDragon_Attack"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/BOSS/FireDragon/Attack/%d.png"), 9))))
		return E_FAIL;

	//Monster
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Monster"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/MONSTER/Test/Alpha/agav00%d.png"), 10))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Alligator_IDLE_Front"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/MONSTER/Alligator/IDLE_Front/%d.png"), 5))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Alligator_IDLE_Back"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/MONSTER/Alligator/IDLE_Back/%d.png"), 5))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Alligator_Move_Front"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/MONSTER/Alligator/Move_Front/%d.png"), 5))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Alligator_Move_Back"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/MONSTER/Alligator/Move_Back/%d.png"), 5))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Alligator_Attack_Front"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/MONSTER/Alligator/Attack_Front/%d.png"), 6))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Alligator_Attack_Back"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/MONSTER/Alligator/Attack_Back/%d.png"), 6))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Alligator_Dead_Front"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/MONSTER/Alligator/Dead_Front/%d.png"), 4))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Alligator_Dead_Back"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/MONSTER/Alligator/Dead_Back/%d.png"), 4))))
		return E_FAIL;
	//for GamePlay
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Tree"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Terrain/grass_%d.png"), 2))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_Stone"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/Stone/%d.dds"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_BackGroundTree"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/BackGround/Tree/%d.dds"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_BackGroundTreeRect"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/BackGround/Tree/%d.png"), 2))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_BackGroundRect"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/BackGround/etc/%d.png"), 7))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_HouseRect"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/BackGround/House/%d.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(Level, TEXT("Prototype_Component_Texture_HouseCube"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/BackGround/House/%d.dds"), 1))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_Prototype()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);
	//NPC
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Default_NPC"),
		CDefault_NPC::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Village_Chief"),
		CVillage_Chief::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Village_Quest1"),
		CVillage_Quest1::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Village_Quest2"),
		CVillage_Quest2::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Engineer"),
		CEngineer::Create(m_pGraphic_Device))))
		return E_FAIL;
	//UI ????
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI"),
		CUI::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StatsWnd"),
		CStatsWnd::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HpPotion"),
		CHpPotion::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SkillSlot"),
		CSkillSlot::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WorldHpBar"),
		CWorldHpBar::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PlayerMpBar"),
		CPlayerMpBar::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PlusBox"),
		CPlusBox::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Inventory"),
		CInventory::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HpBar"),
		CHpBar::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ExpBar"),
		CExpBar::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ExpLogo"),
		CExpLogo::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ExpBlank"),
		CExpBlank::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MpBar"),
		CMpBar::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_IconBar"),
		CIconBar::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_InventoryIcon"),
		CInventoryIcon::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EquipIcon"),
		CEquipIcon::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_XBox"),
		CXBox::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SkillIcon"),
		CSkillIcon::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StatsIcon"),
		CStatsIcon::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_QuickSlot"),
		CQuickSlot::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SkillWnd"),
		CSkillWnd::Create(m_pGraphic_Device))))
		return E_FAIL;

	//Player ????
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PlayerInfo"),
		CPlayerInfo::Create(m_pGraphic_Device))))
		return E_FAIL;
	//Pet ????
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Poring"),
		CPoring::Create(m_pGraphic_Device))))
		return E_FAIL;
	//Wing ????
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wing"),
		CWing::Create(m_pGraphic_Device))))
		return E_FAIL;


	//Terrain ????
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TerrainRect"),
		CTerrainRect::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGroundTree"),
		CBackGroundTree::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGroundRect"),
		CBackGroundRect::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Portal"),
		CPortal::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_House2"),
		CHouse2::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	//sky ????
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	//camera ????
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Dynamic"),
		CCamera_Dynamic::Create(m_pGraphic_Device))))
		return E_FAIL;

	//Skill ????
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ThunderCloud"),
		CThunderCloud::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ThunderSword"),
		CThunderSword::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tornado"),
		CTornado::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FireSpear"),
		CFireSpear::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Fire"),
		CFire::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Meteor"),
		CMeteor::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PoisonArrow"),
		CPoisonArrow::Create(m_pGraphic_Device))))
		return E_FAIL;
	//Effect ????
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wind"),
		CWind::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Hit"),
		CHit::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_DmgFont"),
		CDmgFont::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UseSkill"),
		CUseSkill::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LevelUp"),
		CLevelUp::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MeteorEff"),
		CMeteorEff::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Rain"),
		CRain::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Shadow"),
		CShadow::Create(m_pGraphic_Device))))
		return E_FAIL;
	

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_House"),
		CHouse::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Stone"),
		CStone::Create(m_pGraphic_Device))))
		return E_FAIL;
	//Monster ????
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"),
		CMonster::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FireDragon"),
		CFireDragon::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Alligator"),
		CAlligator::Create(m_pGraphic_Device))))
		return E_FAIL;

	

	Safe_Release(pGameInstance);

	return S_OK;
}



CLoader * CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel)
{
	CLoader*	pInstance = new CLoader(pGraphic_Device);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		ERR_MSG(TEXT("Failed to Created : CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pGraphic_Device);
}
