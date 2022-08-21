#include "stdafx.h"
#include "..\Public\Player.h"

#include "GameInstance.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CGameObject(_pGraphic_Device)
{
}

CPlayer::CPlayer(const CPlayer& rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer::Initialize_Prototype(void)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

void CPlayer::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_float3 Position = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float a;
	if (FAILED(m_pOnTerrain->Get_OnTerrainY(Position, &a)))
	{
		ERR_MSG(TEXT("Failed to OnTerrain"));
		return;
	}

	Position.y = a + (D3DXVec3Length(&m_pTransformCom->Get_State(CTransform::STATE_UP)) * 0.5f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Position);

	CGameInstance* pInstance = CGameInstance::Get_Instance();

	if (nullptr == pInstance)
		return;

	Safe_AddRef(pInstance);
	

	if (pInstance->Get_DIKState(DIK_W) < 0)
		m_pTransformCom->Go_Straight(fTimeDelta);

	if (pInstance->Get_DIKState(DIK_S) < 0)
		m_pTransformCom->Go_Backward(fTimeDelta);

	if (pInstance->Get_DIKState(DIK_A) < 0)
		m_pTransformCom->Go_Left(fTimeDelta);

	if (pInstance->Get_DIKState(DIK_D) < 0)
		m_pTransformCom->Go_Right(fTimeDelta);

	if (pInstance->Get_DIKState(DIK_Q) < 0)
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * -1.f);

	if (pInstance->Get_DIKState(DIK_E) < 0)
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * 1.f);

	if (pInstance->Get_DIMKeyState(DIMK_LBUTTON) < 0)
	{
		if (pInstance->Get_DIKState(DIK_1) < 0)
			Skill_Thunder(TEXT("Layer_Skill"), pInstance->Get_TargetPos());
		if (pInstance->Get_DIKState(DIK_2) < 0)
			Skill_Tornado(TEXT("Layer_Skill"), pInstance->Get_TargetPos());
	}

	/*if (pInstance->Get_DIKState(DIK_TAB) < 0)
	{
		if (30 == g_iFrame)
		{
			g_iFrame = 60;
		}
		else if (60 == g_iFrame)
		{
			g_iFrame = 30;
		}
	}*/

	Safe_Release(pInstance);
}

void CPlayer::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
}

HRESULT CPlayer::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 _pGraphic_Device)
{
	CPlayer* pInstance = new CPlayer(_pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer::Clone(void * pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CPlayer"));
		Safe_Release(pInstance);
	}

	memcpy(&m_tInfo, pArg, sizeof(INFO));
	m_tInfo.pTarget = pInstance;
	memcpy(pArg, &m_tInfo, sizeof(INFO));

	return pInstance;
}

HRESULT CPlayer::SetUp_Components(void)
{
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Onterrain"), LEVEL_STATIC, TEXT("Prototype_Component_Onterrain"), (CComponent**)&m_pOnTerrain)))
		return E_FAIL;

	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Skill_Thunder(const _tchar * pLayerTag, _float3 _vPos)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CGameObject::INFO tInfo;

	tInfo.vPos = _vPos;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ThunderCloud"), LEVEL_GAMEPLAY, pLayerTag, &tInfo)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CPlayer::Skill_Tornado(const _tchar * pLayerTag, _float3 _vPos)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CGameObject::INFO tInfo;

	tInfo.vPos = _vPos;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Tornado"), LEVEL_GAMEPLAY, pLayerTag, &tInfo)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

_float4x4 CPlayer::Get_World(void)
{
	return m_pTransformCom->Get_WorldMatrix();
}

void CPlayer::Free(void)
{
	__super::Free();

	Safe_Release(m_pOnTerrain);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
