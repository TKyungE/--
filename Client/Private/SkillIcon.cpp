#include "stdafx.h"
#include "..\Public\SkillIcon.h"
#include "GameInstance.h"

CSkillIcon::CSkillIcon(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CSkillIcon::CSkillIcon(const CSkillIcon & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSkillIcon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkillIcon::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	memcpy(&m_tInfo, pArg, sizeof(INFO));
	D3DXMatrixOrthoLH(&m_ProjMatrix, (float)g_iWinSizeX, (float)g_iWinSizeY, 0.f, 1.f);

	m_fSizeX = 30.0f;
	m_fSizeY = 30.0f;
	m_fX = 150.f;
	m_fY = 210.f;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	return S_OK;
}

void CSkillIcon::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	if (m_tInfo.pTarget->Get_Info().bHit)
	{
		RECT		rcRect;
		SetRect(&rcRect, (int)(m_fX - m_fSizeX * 0.5f), (int)(m_fY - m_fSizeY * 0.5f), (int)(m_fX + m_fSizeX * 0.5f), (int)(m_fY + m_fSizeY * 0.5f));

		POINT		ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		if (PtInRect(&rcRect, ptMouse))
		{
			m_Check = 4;
			if (GetKeyState(VK_LBUTTON) & 0x8000)
			{
				CGameInstance*			pGameInstance = CGameInstance::Get_Instance();

				Safe_AddRef(pGameInstance);
				pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SkillWnd"), m_tInfo.iLevelIndex, TEXT("Layer_UI"),&m_tInfo);
				Safe_Release(pGameInstance);
			}
		}
		else
			m_Check = 5;
	}
}

void CSkillIcon::Late_Tick(_float fTimeDelta)
{
	if (m_tInfo.pTarget->Get_Info().bHit)
	{
		__super::Late_Tick(fTimeDelta);

		if (nullptr != m_pRendererCom)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CSkillIcon::Render()
{
	if (m_tInfo.pTarget->Get_Info().bHit)
	{
		if (FAILED(__super::Render()))
			return E_FAIL;

		if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
			return E_FAIL;

		_float4x4		ViewMatrix;
		D3DXMatrixIdentity(&ViewMatrix);

		m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

		if (FAILED(m_pTextureCom->Bind_OnGraphicDev(m_Check)))
			return E_FAIL;

		if (FAILED(SetUp_RenderState()))
			return E_FAIL;

		m_pVIBufferCom->Render();

		if (FAILED(Release_RenderState()))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CSkillIcon::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), m_tInfo.iLevelIndex, TEXT("Prototype_Component_Texture_SkillIcon"), (CComponent**)&m_pTextureCom)))
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

HRESULT CSkillIcon::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return S_OK;
}

HRESULT CSkillIcon::Release_RenderState()
{
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}

CSkillIcon * CSkillIcon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSkillIcon*	pInstance = new CSkillIcon(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CInventoryIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkillIcon::Clone(void * pArg)
{
	CSkillIcon*	pInstance = new CSkillIcon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CInventoryIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

_float4x4 CSkillIcon::Get_World(void)
{
	return _float4x4();
}

void CSkillIcon::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
}
