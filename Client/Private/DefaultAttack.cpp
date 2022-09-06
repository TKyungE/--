#include "stdafx.h"
#include "..\Public\DefaultAttack.h"
#include "Tornado.h"
#include "GameInstance.h"

CDefaultAttack::CDefaultAttack(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CDefaultAttack::CDefaultAttack(const CDefaultAttack & rhs)
	: CGameObject(rhs)
{
}

HRESULT CDefaultAttack::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDefaultAttack::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	memcpy(&m_tInfo, pArg, sizeof(INFO));
	_float3 vScale = { 1.f,1.f,1.f };
	m_pTransformCom->Set_Scaled(vScale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tInfo.vPos);


	m_tInfo.bDead = false;
	m_tInfo.fX = 0.5f;
	m_tInfo.iMoney = 11;

	_float3		 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3		 vLook = m_tInfo.vTargetPos - vPosition;


	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * 0.5f;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	m_pColliderCom->Set_Transform(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	return S_OK;
}

void CDefaultAttack::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fDeadTime += fTimeDelta;
	if (m_fDeadTime > 0.3f)
		Set_Dead();

	
}

void CDefaultAttack::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	CGameInstance* pInstance = CGameInstance::Get_Instance();

	Safe_AddRef(pInstance);

	if (pInstance->IsInFrustum(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_pTransformCom->Get_Scale()))
	{
		if (nullptr != m_pRendererCom)
			m_pRendererCom->Add_RenderGroup_Front(CRenderer::RENDER_NONALPHABLEND, this);
	}
	Safe_Release(pInstance);
}

HRESULT CDefaultAttack::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
		return E_FAIL;

	m_pColliderCom->Render();

	return S_OK;
}






HRESULT CDefaultAttack::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;


	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Collider"), LEVEL_STATIC, TEXT("Prototype_Component_Collider"), (CComponent**)&m_pColliderCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	return S_OK;
}



CDefaultAttack * CDefaultAttack::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDefaultAttack*	pInstance = new CDefaultAttack(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CDefaultAttack"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDefaultAttack::Clone(void* pArg)
{
	CDefaultAttack*	pInstance = new CDefaultAttack(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CDefaultAttack"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

_float4x4 CDefaultAttack::Get_World(void)
{
	return m_pTransformCom->Get_WorldMatrix();
}

void CDefaultAttack::Free()
{
	__super::Free();
	
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
