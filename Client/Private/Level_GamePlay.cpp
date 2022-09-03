#include "stdafx.h"
#include "..\Public\LEVEL_GamePlay.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "SoundMgr.h"
#include "CollisionMgr.h"
#include "KeyMgr.h"
#include "BackGroundRect.h" 
#include "BackGroundTree.h"
#include "Level_Loading.h"
#include "House.h"

CLEVEL_GamePlay::CLEVEL_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
	ZeroMemory(&Info, sizeof(CGameObject::INFO));
}

HRESULT CLEVEL_GamePlay::Initialize()
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

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Portal(TEXT("Layer_Portal"))))
		return E_FAIL;

	fSound = fSOUND;
	/*CSoundMgr::Get_Instance()->BGM_Pause();
	CSoundMgr::Get_Instance()->PlayBGM(L"Stage1_Sound.wav", fSOUND);*/
	CSoundMgr::Get_Instance()->PlayBGM(L"Boss_Sound1.wav", fSOUND);

	
	return S_OK;
}

void CLEVEL_GamePlay::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	
	
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//fSound += (pGameInstance->Get_DIMMoveState(DIMM_WHEEL)*fTimeDelta) / 100;

	if (CKeyMgr::Get_Instance()->Key_Down('O'))
		fSound += 0.01;
	if (CKeyMgr::Get_Instance()->Key_Down('P'))
		fSound -= 0.01;
	
	if (fSound > 1.f)
		fSound = 1.f;
	if (fSound < 0.f)
		fSound = 0.f;

	CSoundMgr::Get_Instance()->SetSoundVolume(SOUND_BGM, fSound);
 	
	Create_Rain(fTimeDelta);

	Safe_Release(pGameInstance);
}

void CLEVEL_GamePlay::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));

	//충돌 사용법
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	

	CGameObject* Dest;
	CGameObject* Sour;
	
	
	if (pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_Skill")) != nullptr)
	{
		if (CCollisionMgr::Collision_Sphere(pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_Skill"))->Get_Objects(), pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Monster"))->Get_Objects(), &Dest, &Sour))
		{
			
			Dest->Set_Dead();

			if(Dest->Get_Info().iMoney == 33)
			{ 
				fCollTime += fTimeDelta;
				if (fCollTime > 0.3f)
				{
					_float3 vPos = Get_CollisionPos(Dest, Sour);

					Sour->Set_Hit(Dest->Get_Info().iDmg, vPos);
					Sour->Set_Hp(Dest->Get_Info().iDmg);
					fCollTime = 0.f;
				}
			}
			else
			{
				_float3 vPos = Get_CollisionPos(Dest, Sour);

				Sour->Set_Hit(Dest->Get_Info().iDmg, vPos);
				Sour->Set_Hp(Dest->Get_Info().iDmg);
			}
			if (Sour->Get_Info().iHp <= 0)
				Sour->Set_Dead();
		}
	}
	if (CCollisionMgr::Collision_Sphere(pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player"))->Get_Objects(), pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Portal"))->Get_Objects(), &Dest, &Sour))
	{
		m_bNextLevel = true;
		pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_PlayerInfo"))->Get_Objects().front()->Set_Info(Dest->Get_Info());
	}
	if (m_bNextLevel == true)
	{
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_TOWN))))
			return;
	}
	Safe_Release(pGameInstance);
}

HRESULT CLEVEL_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Terrain"), LEVEL_GAMEPLAY, pLayerTag, (CGameObject**)&Info.pTerrain)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Sky"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	

	for (auto& iter : m_vecIndex)
	{
		CBackGroundRect::INDEXPOS indexpos;
		ZeroMemory(&indexpos, sizeof(CBackGroundRect::INDEXPOS));
		indexpos.iIndex = iter.iIndex;
		indexpos.vScale = iter.vScale;
		indexpos.vPos = iter.BackGroundPos;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BackGroundRect"), LEVEL_GAMEPLAY, pLayerTag, &indexpos)))
			return E_FAIL;
		
	}
	for (auto& iter : m_vecHouse)
	{
		CHouse::INDEXPOS indexpos;
		ZeroMemory(&indexpos, sizeof(CHouse::INDEXPOS));
		indexpos.iIndex = iter.iIndex;
		indexpos.vScale = iter.vScale;
		indexpos.vPos = iter.BackGroundPos;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_House"), LEVEL_GAMEPLAY, pLayerTag, &indexpos)))
			return E_FAIL;
	}

	for (auto& iter : m_vecTree)
	{
		CBackGroundTree::INDEXPOS indexpos;
		ZeroMemory(&indexpos, sizeof(CBackGroundTree::INDEXPOS));
		indexpos.iIndex = iter.iIndex;
		indexpos.vScale = iter.vScale;
		indexpos.vPos = iter.BackGroundPos;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BackGroundTree"), LEVEL_GAMEPLAY, pLayerTag, &indexpos)))
			return E_FAIL;
	}
	CGameObject::INFO tInfo;
	for (int i = 0; i < 100; ++i)
	{
		_float iSour = rand() % 90000 * 0.001f;
		_float iTemp = rand() % 90000 * 0.001f;

		_float3 vPos = { 0.f,0.f,0.f };
		tInfo.vPos.x = vPos.x + iSour;
		tInfo.vPos.y = vPos.y;
		tInfo.vPos.z = vPos.z + iTemp;
		tInfo.iLevelIndex = LEVEL_GAMEPLAY;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Stone"), LEVEL_GAMEPLAY, pLayerTag, &tInfo)))
			return E_FAIL;
	}
	


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLEVEL_GamePlay::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CGameObject::INFO tInfo = pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_PlayerInfo"))->Get_Objects().front()->Get_Info();

	memcpy(&Info, &tInfo, sizeof(CGameObject::INFO));
	Info.iLevelIndex = LEVEL_GAMEPLAY;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_GAMEPLAY, pLayerTag, &Info)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLEVEL_GamePlay::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	
	for (auto& iter : m_vMonsterPos1)
	{
		Info.vPos = iter;
	}
	
	
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Monster"), LEVEL_GAMEPLAY, pLayerTag, &Info)))
		return E_FAIL;


	Info.vPos = { 15.f,0.f,15.f };
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FireDragon"), LEVEL_GAMEPLAY, pLayerTag, &Info)))
		return E_FAIL;
	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CLEVEL_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
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

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_GAMEPLAY, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLEVEL_GamePlay::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CGameObject::INFO tInfo;

	tInfo.iLevelIndex = LEVEL_GAMEPLAY;
	tInfo.bHit = false;
	tInfo.bDead = false;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI"), LEVEL_GAMEPLAY, pLayerTag, &tInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ExpBlank"), LEVEL_GAMEPLAY, pLayerTag, &tInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ExpLogo"), LEVEL_GAMEPLAY, pLayerTag, &tInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_QuickSlot"), LEVEL_GAMEPLAY, pLayerTag, &tInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_IconBar"), LEVEL_GAMEPLAY, pLayerTag, &tInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_InventoryIcon"), LEVEL_GAMEPLAY, pLayerTag, &tInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_EquipIcon"), LEVEL_GAMEPLAY, pLayerTag, &tInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SkillIcon"), LEVEL_GAMEPLAY, pLayerTag, &tInfo)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_StatsIcon"), LEVEL_GAMEPLAY, pLayerTag, &tInfo)))
		return E_FAIL;



	Safe_Release(pGameInstance);

	return S_OK;
}
HRESULT CLEVEL_GamePlay::Ready_Layer_Portal(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CGameObject::INFO tInfo;
	tInfo.iLevelIndex = LEVEL_GAMEPLAY;
	tInfo.vPos = { 20.f,0.f,2.f };
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_GAMEPLAY, pLayerTag, &tInfo)))
		return E_FAIL;
	Safe_Release(pGameInstance);
	return S_OK;
}
_float3 CLEVEL_GamePlay::Get_CollisionPos(CGameObject * pDest, CGameObject * pSour)
{
	_float3 vLook = *(_float3*)&pDest->Get_World().m[3][0] - *(_float3*)&pSour->Get_World().m[3][0];
	D3DXVec3Normalize(&vLook, &vLook);

	vLook = vLook * 0.5f;

	_float Angle = D3DXVec3Dot(&vLook, (_float3*)&pSour->Get_World().m[1][0]);
	_float3 SourUp = *(_float3*)&pSour->Get_World().m[1][0];
	_float3 Proj = (Angle / D3DXVec3Length(&SourUp) * D3DXVec3Length(&SourUp)) * *(_float3*)&pSour->Get_World().m[1][0];
	
	_float3 CollisionPos = *(_float3*)&pSour->Get_World().m[3][0] + Proj;
	CollisionPos.y -= 0.5;
	CollisionPos.x = pDest->Get_World().m[3][0];

	return CollisionPos;
}



void CLEVEL_GamePlay::LoadData()
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

void CLEVEL_GamePlay::Create_Rain(_float fTimeDelta)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CGameObject::INFO tInfo;
	fRainTime += fTimeDelta;
	if (fRainTime > 0.3f)
	{
		fRainTime = 0.f;
		for (int i = 0; i < 30; ++i)
		{
			_float iSour = rand() % 60000 * 0.001f;
			_float iTemp = rand() % 40000 * 0.001f;

			_float3 vPos = { 0.f,0.f,0.f };
			tInfo.vPos.x = vPos.x + iSour;
			tInfo.vPos.y = vPos.y;
			tInfo.vPos.z = vPos.z + iTemp;

			pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rain"), LEVEL_GAMEPLAY, TEXT("Layer_Effect"), &tInfo);
				
		}
	}
	Safe_Release(pGameInstance);

}


CLEVEL_GamePlay * CLEVEL_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLEVEL_GamePlay*	pInstance = new CLEVEL_GamePlay(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLEVEL_GAMEPLAY"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLEVEL_GamePlay::Free()
{
	__super::Free();

	

}
