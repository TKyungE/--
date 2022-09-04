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

HRESULT CCollider::Render(void)
{
	if (FAILED())
	{

	}

	return S_OK;
}

HRESULT CCollider::SetUp_Components(void)
{
	if (true)
	{

	}

	return E_NOTIMPL;
}

HRESULT CCollider::SetUp_RenderState(void)
{
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	
	return S_OK;
}

HRESULT CCollider::Release_RenderState(void)
{
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}

CCollider * CCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollider* pInstance = new CCollider(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCollider"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider::Clone(void * pArg)
{
	CCollider* pInstance = new CCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCollider"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free(void)
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}
