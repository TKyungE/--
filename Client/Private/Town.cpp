#include "stdafx.h"
#include "..\Public\Town.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "SoundMgr.h"
#include "BackGroundRect.h" 
#include "Camera_Dynamic.h"
#include "KeyMgr.h"
#include "House.h"
#include "Layer.h"

CTown::CTown(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
	ZeroMemory(&Info, sizeof(CGameObject::INFO));
}

HRESULT CTown::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	LoadData();

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_NPC(TEXT("Layer_NPC"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Portal(TEXT("Layer_Portal"))))
		return E_FAIL;

	return S_OK;
}

void CTown::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	
	
	/*CGameObject* Dest;
	CGameObject* Sour;*/
	
	/*if (CCollisionMgr::Collision_Sphere(pGameInstance->Find_Layer(LEVEL_TOWN, TEXT("Layer_Player"))->Get_Objects(), pGameInstance->Find_Layer(LEVEL_TOWN, TEXT("Layer_Portal"))->Get_Objects(), &Dest, &Sour))
	{
		m_bNextLevel = true;
		pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_PlayerInfo"))->Get_Objects().front()->Set_Info(Dest->Get_Info());
	}*/
	if (GetKeyState('Y') < 0)
	{
		if (!g_bCollider)
			g_bCollider = true;
	}
	if (GetKeyState('U') < 0)
	{
		if (g_bCollider)
		{
			g_bCollider = false;
		}
	}

	if (m_bNextLevel == true)
	{
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_GAMEPLAY))))
			return;
	}
	Safe_Release(pGameInstance);
}

void CTown::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));
}

HRESULT CTown::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Terrain"), LEVEL_TOWN, pLayerTag, (CGameObject**)&Info.pTerrain)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Sky"), LEVEL_TOWN, pLayerTag)))
		return E_FAIL;
	


	for (auto& iter : m_vecHouse)
	{
		CHouse::INDEXPOS indexpos;
		ZeroMemory(&indexpos, sizeof(CHouse::INDEXPOS));
		indexpos.iIndex = iter.iIndex;
		indexpos.vScale = iter.vScale;
		indexpos.vPos = iter.BackGroundPos;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_House"), LEVEL_TOWN, pLayerTag, &indexpos)))
			return E_FAIL;
	}
	

	Safe_Release(pGameInstance);

	return S_OK;
}
HRESULT CTown::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_PlayerInfo")) != nullptr)
	{
		CGameObject::INFO tInfo = pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_PlayerInfo"))->Get_Objects().front()->Get_Info();
		memcpy(&Info, &tInfo, sizeof(CGameObject::INFO));
		Info.iLevelIndex = LEVEL_TOWN;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_TOWN, pLayerTag, &Info)))
			return E_FAIL;
	}
	else
	{
		CGameObject::INFO tInfo;
		Info.vPos = m_vPlayerPos;
		Info.iLevelIndex = LEVEL_TOWN;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_TOWN, pLayerTag, &Info)))
			return E_FAIL;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PlayerInfo"), LEVEL_STATIC, TEXT("Layer_PlayerInfo"), &tInfo)))
			return E_FAIL;
	}
	Safe_Release(pGameInstance);

	return S_OK;
}




HRESULT CTown::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CCamera_Dynamic::CAMERADESC_DERIVED				CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera_Dynamic::CAMERADESC_DERIVED));

	CameraDesc.iTest = 10;

	CameraDesc.CameraDesc.vEye = _float3(0.f, 10.f, -10.f);
	CameraDesc.CameraDesc.vAt = _float3(0.f, 0.f, 0.f);

	CameraDesc.CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc.CameraDesc.fNear = 0.2f;
	CameraDesc.CameraDesc.fFar = 500.f;

	CameraDesc.CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	CameraDesc.CameraDesc.Info.pTarget = Info.pTarget;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_TOWN, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CTown::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CGameObject::INFO tInfo;

	tInfo.iLevelIndex = LEVEL_TOWN;
	tInfo.bHit = false;
	tInfo.bDead = false;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI"), LEVEL_TOWN, pLayerTag, &tInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ExpBlank"), LEVEL_TOWN, pLayerTag, &tInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ExpLogo"), LEVEL_TOWN, pLayerTag, &tInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_TOWN, pLayerTag, &tInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_HpPotion"), LEVEL_TOWN, pLayerTag, &tInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_IconBar"), LEVEL_TOWN, pLayerTag, &tInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_InventoryIcon"), LEVEL_TOWN, pLayerTag, &tInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_EquipIcon"), LEVEL_TOWN, pLayerTag, &tInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SkillIcon"), LEVEL_TOWN, pLayerTag, &tInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StatsIcon"), LEVEL_TOWN, pLayerTag, &tInfo)))
		return E_FAIL;

	

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CTown::Ready_Layer_NPC(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CGameObject::INFO tInfo;
	tInfo.iLevelIndex = LEVEL_TOWN;
	tInfo.vPos = { 2.f,0.f,2.f };
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Default_NPC"), LEVEL_TOWN, pLayerTag, &tInfo)))
		return E_FAIL;
	tInfo.vPos = { 6.f,0.f,6.f };
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Village_Chief"), LEVEL_TOWN, pLayerTag, &tInfo)))
		return E_FAIL;
	tInfo.vPos = { 8.f,0.f,6.f };
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Village_Quest1"), LEVEL_TOWN, pLayerTag, &tInfo)))
		return E_FAIL;
	tInfo.vPos = { 10.f,0.f,14.f };
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Village_Quest2"), LEVEL_TOWN, pLayerTag, &tInfo)))
		return E_FAIL;
	tInfo.vPos = { 10.f,0.f,16.f };
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Engineer"), LEVEL_TOWN, pLayerTag, &tInfo)))
		return E_FAIL;



	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CTown::Ready_Layer_Portal(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CGameObject::INFO tInfo;
	tInfo.iLevelIndex = LEVEL_TOWN;
	tInfo.vPos = { 2.f,0.01f,20.f };
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_TOWN, pLayerTag, &tInfo)))
		return E_FAIL;
	Safe_Release(pGameInstance);
	return S_OK;
}

void CTown::LoadData()
{
	HANDLE hFile = CreateFile(TEXT("../../Data/Pos.dat"), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte = 0;

	_float3 vPos1;
	_uint iMSize, iIndexSize, iTreeSize, iHouseSize;
	_tchar str1[MAX_PATH];
	_tchar str2[MAX_PATH];
	_tchar str3[MAX_PATH];
	_tchar str4[MAX_PATH];

	ReadFile(hFile, vPos1, sizeof(_float3), &dwByte, nullptr);
	m_vPlayerPos = vPos1;

	ReadFile(hFile, str1, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr);
	ReadFile(hFile, str2, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr);
	ReadFile(hFile, str3, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr);
	ReadFile(hFile, str4, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr);

	iMSize = stoi(str1);
	iIndexSize = stoi(str2);
	iTreeSize = stoi(str3);
	iHouseSize = stoi(str4);



	while (true)
	{
		for (_uint i = 0; i < iMSize; ++i)
		{
			if (0 == dwByte)
				break;

			_float3 Pos;

			ReadFile(hFile, &Pos, sizeof(_float3), &dwByte, nullptr);

			_float3 vPos;

			vPos = Pos;

			m_vMonsterPos1.push_back(vPos);


		}

		for (_uint i = 0; i < iIndexSize; ++i)
		{
			if (0 == dwByte)
				break;

			_float3 BackPos, Scale;
			_tchar str3[MAX_PATH];
			_uint Index;

			ReadFile(hFile, &BackPos, sizeof(_float3), &dwByte, nullptr);
			ReadFile(hFile, &Scale, sizeof(_float3), &dwByte, nullptr);
			ReadFile(hFile, str3, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr);

			Index = stoi(str3);

			INDEXPOS IndexPos;

			IndexPos.BackGroundPos = BackPos;
			IndexPos.vScale = Scale;
			IndexPos.iIndex = Index;

			m_vecIndex.push_back(IndexPos);
		}

		for (_uint i = 0; i < iTreeSize; ++i)
		{
			if (0 == dwByte)
				break;

			_float3 BackPos, Scale;
			_tchar str3[MAX_PATH];
			_uint Index;

			ReadFile(hFile, &BackPos, sizeof(_float3), &dwByte, nullptr);
			ReadFile(hFile, &Scale, sizeof(_float3), &dwByte, nullptr);
			ReadFile(hFile, str3, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr);

			Index = stoi(str3);

			INDEXPOS TreePos;

			TreePos.BackGroundPos = BackPos;
			TreePos.vScale = Scale;
			TreePos.iIndex = Index;

			m_vecTree.push_back(TreePos);
		}

		for (_uint i = 0; i < iHouseSize; ++i)
		{
			if (0 == dwByte)
				break;

			_float3 BackPos, Scale;
			_tchar str3[MAX_PATH];
			_uint Index;

			ReadFile(hFile, &BackPos, sizeof(_float3), &dwByte, nullptr);
			ReadFile(hFile, &Scale, sizeof(_float3), &dwByte, nullptr);
			ReadFile(hFile, str3, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr);

			Index = stoi(str3);

			INDEXPOS HousePos;

			HousePos.BackGroundPos = BackPos;
			HousePos.vScale = Scale;
			HousePos.iIndex = Index;

			m_vecHouse.push_back(HousePos);
		}

		if (0 == dwByte)
			break;
	}

	CloseHandle(hFile);
}
CTown * CTown::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTown*	pInstance = new CTown(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CTown"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTown::Free()
{
	__super::Free();
}
