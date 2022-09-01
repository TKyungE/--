#include "stdafx.h"
#include "..\Public\Level_Logo.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "SoundMgr.h"
#include "CollisionMgr.h"

CLevel_Logo::CLevel_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;
	//배경음 사용법
	CSoundMgr::Get_Instance()->PlayBGM(L"Boss_Sound1.wav", fSOUND);
	//효과음 사용법
	CSoundMgr::Get_Instance()->PlayEffect(L"KingkongGun.wav", fSOUND);
	return S_OK;
}

void CLevel_Logo::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	
	
	if (m_bNextLevel == true)
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_GAMEPLAY))))
			return;

		Safe_Release(pGameInstance);
	}
	//충돌 사용법
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	//Layer_Player,Layer_Monster 가 있다는 가정하에 적은 예외코드;
	/*if (CCollisionMgr::Collision_Sphere(pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_Player"))->Get_Objects(), pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_Monster"))->Get_Objects()))
	{
		Dead();
	}*/

	Safe_Release(pGameInstance);
}

void CLevel_Logo::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));
}

void CLevel_Logo::Dead()
{
	
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BackGround"), LEVEL_LOGO, pLayerTag, nullptr)))
		return E_FAIL;*/
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Logo"), LEVEL_LOGO, pLayerTag, nullptr)))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NextButton"), LEVEL_LOGO, pLayerTag, &m_bNextLevel)))
		return E_FAIL;
	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_Logo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();


}
