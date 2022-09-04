#include "..\Public\Collider.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CCollider::CCollider(const CCollider & rhs)
	: CCollider(rhs)
{
	memcpy(&m_matTarget, &rhs.m_matTarget, sizeof(_float4x4));
}

HRESULT CCollider::Initialize_Prototype(void)
{
	D3DXMatrixIdentity(&m_matTarget);

	return S_OK;
}

HRESULT CCollider::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
	{
		ERR_MSG(TEXT("Failed to Set Components : CCollider"));
		return E_FAIL;
	}

	return S_OK;
}

void CCollider::Tick(_float fTimeDelta)
{
}

void CCollider::Late_Tick(_float fTimeDelta)
{
}

HRESULT CCollider::Render(void)
{
	return E_NOTIMPL;
}

HRESULT CCollider::SetUp_Components(void)
{
	return E_NOTIMPL;
}

HRESULT CCollider::SetUp_RenderState(void)
{
	return E_NOTIMPL;
}

HRESULT CCollider::Release_RenderState(void)
{
	return E_NOTIMPL;
}

CCollider * CCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	return nullptr;
}

CComponent * CCollider::Clone(void * pArg)
{
	return nullptr;
}

void CCollider::Free(void)
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}
