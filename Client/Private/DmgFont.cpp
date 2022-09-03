#include "stdafx.h"
#include "..\Public\DmgFont.h"
#include "GameInstance.h"

CDmgFont::CDmgFont(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CDmgFont::CDmgFont(const CDmgFont & rhs)
	: CGameObject(rhs)
{
}

HRESULT CDmgFont::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDmgFont::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	memcpy(&m_tInfo, pArg, sizeof(INFO));
	m_tInfo.vPos.y += 0.85f;
	_float3 vScale = { 0.3f,0.3f,1.f };
	m_pTransformCom->Set_Scaled(vScale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tInfo.vPos);
	m_vPos = m_tInfo.vPos;
	m_eCurState = STATE_END;
	
	m_tInfo.bDead = false;

	return S_OK;
}

void CDmgFont::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	Set_State();
	Jump();
	

	m_fDeadTime += fTimeDelta;
	if (m_fDeadTime > 0.25f)
		Set_Dead();
}

void CDmgFont::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	OnBillboard();

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup_Front(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CDmgFont::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	Off_SamplerState();

	if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
		return E_FAIL;


	//상태에 따라 바인드하는 함수
	

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	TextureRender();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	On_SamplerState();
	return S_OK;
}



void CDmgFont::OnBillboard()
{
	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	_float3 vScale = { 0.3f,0.3f,1.f };
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0] * vScale.x);
	//m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);
}



HRESULT CDmgFont::On_SamplerState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

HRESULT CDmgFont::Off_SamplerState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	return S_OK;
}

void CDmgFont::Set_State()
{
	int iScore = m_tInfo.iTargetDmg;
	for (int i = 3; i > -1; --i)
	{
		m_iDraw[i] = iScore % 10;
		iScore /= 10;
	/*	if (iScore < 100)
			m_iDraw[0] = iScore % 10;*/
	}
	//m_iDraw[0] = iScore / 1000;
	//m_iDraw[1] = (iScore % 1000) / 100;
	//m_iDraw[2] = (iScore % 1000) / 10;
	//m_iDraw[3] = iScore % 10;
	if (m_iDraw[3] != 0)
		m_eCurState = ONE;
	if (m_iDraw[2] != 0)
		m_eCurState = TWO;
	if (m_iDraw[1] != 0)
		m_eCurState = THREE;
	if (m_iDraw[0] != 0)
		m_eCurState = FOUR;
}

void CDmgFont::Jump()
{
	m_fMoveX += 0.015f;
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPos.y += 2.5 * m_fTime - (7.8f * m_fTime * m_fTime * 0.5f);
	vPos.x += m_fMoveX;
	m_fTime += 0.025f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	
	
}

HRESULT CDmgFont::TextureRender()
{
	_float3 vPos;
	switch (m_eCurState)
	{
	case ONE:
		if (FAILED(m_pTextureCom->Bind_OnGraphicDev(m_iDraw[3])))
			return E_FAIL;
		m_pVIBufferCom1->Render();
		break;
	case TWO:
		if (FAILED(m_pTextureCom->Bind_OnGraphicDev(m_iDraw[3])))
			return E_FAIL;
		m_pVIBufferCom1->Render();
		vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos.x -= 0.15f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_OnGraphicDev(m_iDraw[2])))
			return E_FAIL;
		m_pVIBufferCom2->Render();
		break;
	case THREE:
		if (FAILED(m_pTextureCom->Bind_OnGraphicDev(m_iDraw[3])))
			return E_FAIL;
		m_pVIBufferCom1->Render();
		vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos.x -= 0.15f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_OnGraphicDev(m_iDraw[2])))
			return E_FAIL;
		m_pVIBufferCom2->Render();
		vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos.x -= 0.15f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_OnGraphicDev(m_iDraw[1])))
			return E_FAIL;
		m_pVIBufferCom3->Render();
		break;
	case FOUR:
		if (FAILED(m_pTextureCom->Bind_OnGraphicDev(m_iDraw[3])))
			return E_FAIL;
		m_pVIBufferCom1->Render();
		vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos.x -= 0.15f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_OnGraphicDev(m_iDraw[2])))
			return E_FAIL;
		m_pVIBufferCom2->Render();
		vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos.x -= 0.15f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_OnGraphicDev(m_iDraw[1])))
			return E_FAIL;
		m_pVIBufferCom3->Render();
		vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos.x -= 0.15f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_OnGraphicDev(m_iDraw[0])))
			return E_FAIL;
		m_pVIBufferCom4->Render();
		break;
	default:
		break;
	}
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos);
	return S_OK;
}

HRESULT CDmgFont::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_DmgFont"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer1"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom1)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer2"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom2)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer3"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom3)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer4"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom4)))
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

HRESULT CDmgFont::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CDmgFont::Release_RenderState()
{
	// m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetTexture(0, nullptr);
	return S_OK;
}

CDmgFont * CDmgFont::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDmgFont*	pInstance = new CDmgFont(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CDmgFont"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDmgFont::Clone(void* pArg)
{
	CDmgFont*	pInstance = new CDmgFont(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CDmgFont"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

_float4x4 CDmgFont::Get_World(void)
{
	return _float4x4();
}

void CDmgFont::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom1);
	Safe_Release(m_pVIBufferCom2);
	Safe_Release(m_pVIBufferCom3);
	Safe_Release(m_pVIBufferCom4);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
}
