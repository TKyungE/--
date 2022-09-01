#include "stdafx.h"
#include "..\Public\QuickSlot.h"
#include"GameInstance.h"


CQuickSlot::CQuickSlot(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CQuickSlot::CQuickSlot(const CQuickSlot & rhs)
	: CGameObject(rhs)
{
}

HRESULT CQuickSlot::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CQuickSlot::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	D3DXMatrixOrthoLH(&m_ProjMatrix, (float)g_iWinSizeX, (float)g_iWinSizeY, 0.f, 1.f);

	m_fSizeX = 400.0f;
	m_fSizeY = 50.0f;
	m_fX = 600.f;
	m_fY = 25.f;


	m_Pass.fPosX = m_fX + 188;//반지름에서 12(x박스크기의 반값)만큼 추가로 빼야함
	m_Pass.fPosY = m_fY - 13;//반지름에서 12(x박스크기의 반값)만큼 추가로 빼야함
	m_Pass.bNext = false;
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();

	Safe_AddRef(pGameInstance);

	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_XBox"), LEVEL_STATIC, TEXT("Layer_UI"), &m_Pass);
	for (int i = 0; i < 9; ++i)
	{
		POINT ptPos;

		ptPos.x = 400 + i * 100;
		ptPos.y = (long)m_fY;
		pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_STATIC, TEXT("Layer_UI"), &ptPos);
		pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Slot"), LEVEL_STATIC, TEXT("Layer_UI"), &ptPos);

	}
	Safe_Release(pGameInstance);


	if (FAILED(SetUp_Components()))
		return E_FAIL;


	return S_OK;
}

void CQuickSlot::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);
	if (m_Pass.bNext == true)
	{
		Set_Dead();
	}
	RECT		rcRect;
	SetRect(&rcRect, (int)(m_fX - m_fSizeX * 0.5f),(int)( m_fY - m_fSizeY * 0.5f),(int)( m_fX + m_fSizeX * 0.5f),(int)( m_fY - m_fSizeY * 0.35f));

	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&rcRect, ptMouse))
	{

	}

}
void CQuickSlot::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup_Front(CRenderer::RENDER_UI, this);

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));//여기가중점
}

HRESULT CQuickSlot::Render()
{

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
		return E_FAIL;

	_float4x4		ViewMatrix;
	D3DXMatrixIdentity(&ViewMatrix);

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	if (FAILED(m_pTextureCom->Bind_OnGraphicDev(0)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;




	return S_OK;
}

HRESULT CQuickSlot::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_QuickSlot"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CQuickSlot::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return S_OK;
}
HRESULT CQuickSlot::Release_RenderState()
{
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetTexture(0, nullptr);
	return S_OK;
}

CQuickSlot * CQuickSlot::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CQuickSlot*	pInstance = new CQuickSlot(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CQuickSlot"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CQuickSlot::Clone(void * pArg)
{
	CQuickSlot*	pInstance = new CQuickSlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CQuickSlot"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

_float4x4 CQuickSlot::Get_World(void)
{
	return _float4x4();
}

void CQuickSlot::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
}
