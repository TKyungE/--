#include "stdafx.h"
#include "..\Public\ThunderSword.h"
#include "GameInstance.h"

CThunderSword::CThunderSword(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CThunderSword::CThunderSword(const CThunderSword & rhs)
	: CGameObject(rhs)
{
}

HRESULT CThunderSword::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CThunderSword::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	memcpy(&m_tInfo, pArg, sizeof(INFO));
	_float3 vScale = { 2.f,2.f,1.f };
	m_pTransformCom->Set_Scaled(vScale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tInfo.vPos);

	m_ePreState = STATE_END;
	m_eCurState = IDLE;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 17;
	m_tFrame.fFrameSpeed = 0.06f;
	m_tInfo.bDead = false;
	
	return S_OK;
}

void CThunderSword::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Move_Frame(fTimeDelta);
	m_pTransformCom->Go_Down(fTimeDelta);
	if(m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < 0.f)
		Set_Dead();
}

void CThunderSword::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


	Motion_Change();

	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	//m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CThunderSword::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	Off_SamplerState();

	if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
		return E_FAIL;


	//상태에 따라 바인드하는 함수
	TextureRender();

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	On_SamplerState();

	return S_OK;
}


void CThunderSword::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 17;
			m_tFrame.fFrameSpeed = 0.06f;
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CThunderSword::Move_Frame(_float fTimeDelta)
{
	switch (m_eCurState)
	{
	case IDLE:
		m_tFrame.iFrameStart = m_pTextureCom->MoveFrame(fTimeDelta, m_tFrame.fFrameSpeed, m_tFrame.iFrameEnd);
		break;
	default:
		break;
	}

}

HRESULT CThunderSword::TextureRender()
{
	switch (m_eCurState)
	{
	case IDLE:
		if (FAILED(m_pTextureCom->Bind_OnGraphicDev(m_tFrame.iFrameStart)))
			return E_FAIL;
		break;
	default:
		break;
	}
	return S_OK;
}

HRESULT CThunderSword::On_SamplerState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

HRESULT CThunderSword::Off_SamplerState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	return S_OK;
}
HRESULT CThunderSword::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ThunderSword"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 3.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CThunderSword::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CThunderSword::Release_RenderState()
{
	// m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetTexture(0, nullptr);
	return S_OK;
}

CThunderSword * CThunderSword::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CThunderSword*	pInstance = new CThunderSword(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CThunderSword"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CThunderSword::Clone(void* pArg)
{
	CThunderSword*	pInstance = new CThunderSword(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CThunderSword"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

_float4x4 CThunderSword::Get_World(void)
{
	return _float4x4();
}

void CThunderSword::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
}
