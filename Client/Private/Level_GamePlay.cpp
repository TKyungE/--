#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "SoundMgr.h"
#include "CollisionMgr.h"

CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
	ZeroMemory(&Info, sizeof(CGameObject::INFO));
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

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

	/*fSound = fSOUND;
	CSoundMgr::Get_Instance()->BGM_Pause();
	CSoundMgr::Get_Instance()->PlayBGM(L"Stage1_Sound.wav", fSOUND);*/

	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	
	
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	fSound += (pGameInstance->Get_DIMMoveState(DIMM_WHEEL)*fTimeDelta) / 100;

	if (fSound > 1.f)
		fSound = 1.f;
	if (fSound < 0.f)
		fSound = 0.f;

	CSoundMgr::Get_Instance()->SetSoundVolume(SOUND_BGM, fSound);
 	
	Safe_Release(pGameInstance);
}

void CLevel_GamePlay::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));

	//충돌 사용법
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	

	CGameObject* Dest;
	CGameObject* Sour;
	_float3* vPos;
	if (CCollisionMgr::Collision_Sphere(pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player"))->Get_Objects(), pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Monster"))->Get_Objects(), &Dest, &Sour,&vPos))
	{
		//Dest->Set_Dead();
		//Sour->Set_Dead();
	}
	if (pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Skill")) != nullptr)
	{
		if (CCollisionMgr::Collision_Sphere(pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Skill"))->Get_Objects(), pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Monster"))->Get_Objects(), &Dest, &Sour,&vPos))
		{
			Dest->Set_Dead();
			
			CGameObject::INFO tInfo;
			tInfo.vPos = *(_float3*)&Sour->Get_World().m[3][0];
			pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Hit"), LEVEL_GAMEPLAY, TEXT("Layer_Effect"), &tInfo);
			Sour->Set_Hit(Dest->Get_Info().iDmg);
			Sour->Set_Hp(Dest->Get_Info().iDmg);
			if(Sour->Get_Info().iHp <= 0)
				Sour->Set_Dead();
		}
	}

	Safe_Release(pGameInstance);
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Sky"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Terrain"), LEVEL_GAMEPLAY, pLayerTag, (CGameObject**)&Info.pTerrain)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_GAMEPLAY, pLayerTag, (CGameObject**)&Info.pTarget)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Monster"), LEVEL_GAMEPLAY, pLayerTag, &Info)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
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

	memcpy(&CameraDesc.CameraDesc.Info, &Info, sizeof(CGameObject::INFO));

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_GAMEPLAY, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_HpBar"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ExpBar"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_IconBar"), LEVEL_GAMEPLAY, pLayerTag, &m_IconRender)))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_InventoryIcon"), LEVEL_GAMEPLAY, pLayerTag, &m_IconRender)))
		return E_FAIL;
	Safe_Release(pGameInstance);

	return S_OK;
}


CLevel_GamePlay * CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();


}
