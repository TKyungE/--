#include "stdafx.h"
#include "..\Public\HpPotion.h"
#include"GameInstance.h"
#include"Layer.h"
CHpPotion::CHpPotion(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CHpPotion::CHpPotion(const CHpPotion & rhs)
	: CGameObject(rhs)
{
}
HRESULT CHpPotion::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHpPotion::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	memcpy(&m_tInfo, pArg, sizeof(INFO));
	D3DXMatrixOrthoLH(&m_ProjMatrix, (float)g_iWinSizeX, (float)g_iWinSizeY, 0.f, 1.f);
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = 30.0f;
	m_fSizeY = 30.0f;
	
	
	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	

	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();

	Safe_AddRef(pGameInstance);

	pTarget=pGameInstance->Find_Layer(m_tInfo.iLevelIndex, TEXT("Layer_Player"))->Get_Objects().back();

	Safe_Release(pGameInstance);

	return S_OK;
}

void CHpPotion::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_tInfo.iMp == 5)
	{
		RECT		rcRect;
		SetRect(&rcRect, (int)(m_tInfo.vPos.x - m_fSizeX * 0.5f), (int)(m_tInfo.vPos.y - m_fSizeY * 0.5f), (int)(m_tInfo.vPos.x + m_fSizeX * 0.5f), (int)(m_tInfo.vPos.y + m_fSizeY * 0.5f));

		POINT		ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		if (PtInRect(&rcRect, ptMouse))
		{

			if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
			{
				m_tInfo.vPos.x = ptMouse.x;
				m_tInfo.vPos.y = ptMouse.y;
			}

		}

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_tInfo.vPos.x - g_iWinSizeX * 0.5f, -m_tInfo.vPos.y + g_iWinSizeY * 0.5f, 0.f));
	}

	RECT		rcRect;
	SetRect(&rcRect, (int)(m_tInfo.vPos.x - m_fSizeX * 0.5f), (int)(m_tInfo.vPos.y - m_fSizeY * 0.5f), (int)(m_tInfo.vPos.x + m_fSizeX * 0.5f), (int)(m_tInfo.vPos.y + m_fSizeY * 0.5f));

	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&rcRect, ptMouse))
	{
		if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
		{
			m_tInfo.iExp = 2;
		}
	}

	if (m_tInfo.iExp == 2)
	{
		Use();
	}

}
void CHpPotion::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

}

HRESULT CHpPotion::Render()

{
	if (m_tInfo.iMp == 5)
	{
		if (FAILED(__super::Render()))
			return E_FAIL;

		if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
			return E_FAIL;

		_float4x4		ViewMatrix;
		D3DXMatrixIdentity(&ViewMatrix);

		m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

		if (FAILED(m_pTextureCom->Bind_OnGraphicDev(1)))
			return E_FAIL;

		if (FAILED(SetUp_RenderState()))
			return E_FAIL;

		m_pVIBufferCom->Render();

		if (FAILED(Release_RenderState()))
			return E_FAIL;
	}
	return S_OK;
	
}

void CHpPotion::Use(void)
{
	if (pTarget->Get_Info().iHp != pTarget->Get_Info().iMaxHp)
	{
		pTarget->Set_Hp(-(pTarget->Get_Info().iMaxHp*0.3));
		Set_Dead();
	}
}

HRESULT CHpPotion::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Temp"), (CComponent**)&m_pTextureCom)))
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

HRESULT CHpPotion::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return S_OK;
}

HRESULT CHpPotion::Release_RenderState()
{
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}

CHpPotion * CHpPotion::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHpPotion*	pInstance = new CHpPotion(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CHpPotion"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHpPotion::Clone(void * pArg)
{
	CHpPotion*	pInstance = new CHpPotion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CHpPotion"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

_float4x4 CHpPotion::Get_World(void)
{
	return _float4x4();
}

void CHpPotion::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
}


