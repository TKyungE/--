#include "stdafx.h"
#include "..\Public\Alligator.h"
#include "..\Public\Player.h"
#include "GameInstance.h"
#include "SoundMgr.h"

CAlligator::CAlligator(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CGameObject(_pGraphic_Device)
{
}

CAlligator::CAlligator(const CAlligator& rhs)
	: CGameObject(rhs)
{
}
HRESULT CAlligator::Initialize_Prototype(void)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAlligator::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	memcpy(&m_tInfo, pArg, sizeof(INFO));
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	
	//m_tInfo.vPos.y += 0.f;
	_float3 vScale = { 2.f,2.f,1.f };
	m_pTransformCom->Set_Scaled(vScale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tInfo.vPos);

	m_ePreState = STATE_END;
	m_eCurState = IDLE;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.fFrameSpeed = 0.1f;
	m_tInfo.bDead = false;
	m_tInfo.iDmg = 66;
	m_tInfo.fX = 0.5f;
	m_tInfo.iMaxHp = 999999;
	m_tInfo.iHp = m_tInfo.iMaxHp;

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;
	Safe_AddRef(pGameInstance);
	CGameObject::INFO tInfo;
	tInfo.pTarget = this;
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_WorldHpBar"), LEVEL_GAMEPLAY, TEXT("Layer_Status"), &tInfo);
	tInfo.vPos = { 2.f,2.f,1.f };

	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Shadow"), LEVEL_GAMEPLAY, TEXT("Layer_Effect"), &tInfo);

	Safe_Release(pGameInstance);


	return S_OK;
}

void CAlligator::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	OnTerrain();
	Check_Front();
	if (m_eCurState == DEAD)
	{
		if (m_tFrame.iFrameStart == 3)
		{
			m_bDead = true;
			m_fDeadTime += fTimeDelta;
			if (m_fDeadTime > 5.f)
			{
				m_tInfo.bDead = true;
				return;
			}
		}
	}
	if (!m_bDead)
	{
		Move_Frame(fTimeDelta);
		Use_Skill(fTimeDelta);
	}

	if (nullptr != m_tInfo.pTarget)
		Chase(fTimeDelta);

	m_tInfo.bDead = false;
}

void CAlligator::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
	if (!m_bDead)
	{
		Check_Hit();
		Motion_Change();
	}
	OnBillboard();
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CAlligator::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	Off_SamplerState();

	if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
		return E_FAIL;
	TextureRender();

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;
	On_SamplerState();
	return S_OK;
}
HRESULT CAlligator::SetUp_Components(void)
{
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture_IDLE_Front"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Alligator_IDLE_Front"), (CComponent**)&m_pTextureComIDLE_Front)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_IDLE_Back"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Alligator_IDLE_Back"), (CComponent**)&m_pTextureComIDLE_Back)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Move_Front"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Alligator_Move_Front"), (CComponent**)&m_pTextureComMove_Front)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Move_Back"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Alligator_Move_Back"), (CComponent**)&m_pTextureComMove_Back)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Attack_Front"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Alligator_Attack_Front"), (CComponent**)&m_pTextureComAttack_Front)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Attack_Back"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Alligator_Attack_Back"), (CComponent**)&m_pTextureComAttack_Back)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Dead_Front"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Alligator_Dead_Front"), (CComponent**)&m_pTextureComDead_Front)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Dead_Back"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Alligator_Dead_Back"), (CComponent**)&m_pTextureComDead_Back)))
		return E_FAIL;
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 2.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CAlligator::SetUp_RenderState(void)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CAlligator::Release_RenderState(void)
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetTexture(0, nullptr);
	return S_OK;
}

void CAlligator::Check_Hit()
{
	if (m_tInfo.bHit)
	{
		CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		CGameObject::INFO tInfo;
		tInfo.pTarget = this;
		tInfo.vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);;
		tInfo.iTargetDmg = m_tInfo.iTargetDmg;
		pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_DmgFont"), LEVEL_GAMEPLAY, TEXT("Layer_DmgFont"), &tInfo);
		tInfo.vPos = m_tInfo.vTargetPos;
		pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Hit"), LEVEL_GAMEPLAY, TEXT("Layer_Effect"), &tInfo);
		CSoundMgr::Get_Instance()->PlayEffect(L"Hit_Sound.wav", fSOUND);
		m_tInfo.bHit = false;
		Safe_Release(pGameInstance);
	}
}

void CAlligator::Chase(_float fTimeDelta)
{
	_float Distance = D3DXVec3Length(&(*(_float3*)&m_tInfo.pTarget->Get_World().m[3][0] - m_pTransformCom->Get_State(CTransform::STATE_POSITION)));

	if (0.25f < Distance && m_eCurState != DEAD)
	{
		_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vTargetPos = *(_float3*)&m_tInfo.pTarget->Get_World().m[3][0];

		vPosition += *D3DXVec3Normalize(&vTargetPos, &(vTargetPos - vPosition)) * m_pTransformCom->Get_TransformDesc().fSpeedPerSec * fTimeDelta;
	//	vPosition.y += 2.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	}
	else
	{
		_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vTargetPos = *(_float3*)&m_tInfo.pTarget->Get_World().m[3][0];
	//	vPosition.y = vTargetPos.y += 2.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	}
}
void CAlligator::OnTerrain()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return;
	Safe_AddRef(pGameInstance);
	CVIBuffer_Terrain*		pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer"), 0);
	if (nullptr == pVIBuffer_Terrain)
		return;

	CTransform*		pTransform_Terrain = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_Transform"), 0);
	if (nullptr == pTransform_Terrain)
		return;

	_float3			vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition.y = pVIBuffer_Terrain->Compute_Height(vPosition, pTransform_Terrain->Get_WorldMatrix(), 0.5f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	Safe_Release(pGameInstance);
}

CAlligator * CAlligator::Create(LPDIRECT3DDEVICE9 _pGraphic_Device)
{
	CAlligator* pInstance = new CAlligator(_pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CAlligator"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAlligator::Clone(void * pArg)
{
	CAlligator* pInstance = new CAlligator(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CAlligator"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

_float4x4 CAlligator::Get_World(void)
{
	return m_pTransformCom->Get_WorldMatrix();
}

void CAlligator::Free(void)
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureComIDLE_Front);
	Safe_Release(m_pTextureComIDLE_Back);
	Safe_Release(m_pTextureComMove_Front);
	Safe_Release(m_pTextureComMove_Back);
	Safe_Release(m_pTextureComAttack_Front);
	Safe_Release(m_pTextureComAttack_Back);
	Safe_Release(m_pTextureComDead_Front);
	Safe_Release(m_pTextureComDead_Back);
}



HRESULT CAlligator::On_SamplerState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

HRESULT CAlligator::Off_SamplerState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	return S_OK;
}

HRESULT CAlligator::Skill_PoisonArrow(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CGameObject::INFO tInfo;
	tInfo.vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	tInfo.iLevelIndex = LEVEL_GAMEPLAY;
	tInfo.vTargetPos = *(_float3*)&m_tInfo.pTarget->Get_World().m[3][0];
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PoisonArrow"), LEVEL_GAMEPLAY, pLayerTag, &tInfo)))
			return E_FAIL;
	
	Safe_Release(pGameInstance);

	return S_OK;
}
void CAlligator::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.fFrameSpeed = 0.1f;
			break;
		case MOVE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.fFrameSpeed = 0.1f;
			break;
		case DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.fFrameSpeed = 0.3f;
			break;
		case SKILL:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.fFrameSpeed = 0.1f;
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CAlligator::Move_Frame(_float fTimeDelta)
{
	switch (m_eCurState)
	{
	case IDLE:
		if (m_bFront)
			m_tFrame.iFrameStart = m_pTextureComIDLE_Front->MoveFrame(fTimeDelta, m_tFrame.fFrameSpeed, m_tFrame.iFrameEnd);
		else
			m_tFrame.iFrameStart = m_pTextureComIDLE_Back->MoveFrame(fTimeDelta, m_tFrame.fFrameSpeed, m_tFrame.iFrameEnd);
		break;
	case MOVE:
		if (m_bFront)
			m_tFrame.iFrameStart = m_pTextureComMove_Front->MoveFrame(fTimeDelta, m_tFrame.fFrameSpeed, m_tFrame.iFrameEnd);
		else
			m_tFrame.iFrameStart = m_pTextureComMove_Back->MoveFrame(fTimeDelta, m_tFrame.fFrameSpeed, m_tFrame.iFrameEnd);
		break;
	case DEAD:
		if (m_bFront)
			m_tFrame.iFrameStart = m_pTextureComDead_Front->MoveFrame(fTimeDelta, m_tFrame.fFrameSpeed, m_tFrame.iFrameEnd);
		else
			m_tFrame.iFrameStart = m_pTextureComDead_Back->MoveFrame(fTimeDelta, m_tFrame.fFrameSpeed, m_tFrame.iFrameEnd);
		break;
	case SKILL:
		if (m_bFront)
			m_tFrame.iFrameStart = m_pTextureComAttack_Front->MoveFrame(fTimeDelta, m_tFrame.fFrameSpeed, m_tFrame.iFrameEnd);
		else
			m_tFrame.iFrameStart = m_pTextureComAttack_Back->MoveFrame(fTimeDelta, m_tFrame.fFrameSpeed, m_tFrame.iFrameEnd);
		break;
	default:
		break;
	}
}
void CAlligator::Check_Front()
{
	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vTargetPos = *(_float3*)&m_tInfo.pTarget->Get_World().m[3][0];

	if (vTargetPos.z > vPos.z)
		m_bFront = false;
	if (vTargetPos.z <= vPos.z)
		m_bFront = true;

	
	if (m_tInfo.bDead && m_eCurState != DEAD)
	{
		m_eCurState = DEAD;
		m_tFrame.iFrameStart = 0;
	}

}
void CAlligator::Use_Skill(_float fTimeDelta)
{
	m_fSkillCool += fTimeDelta;

	if (m_fSkillCool >	3.f)
	{
		m_fSkillCool = 0.f;
		m_eCurState = SKILL;
		m_tFrame.iFrameStart = 0;
	}
	if(	m_eCurState == SKILL && m_tFrame.iFrameStart == 4)
		Skill_PoisonArrow(TEXT("Layer_MonsterSkill"));
	if (m_eCurState == SKILL&& m_tFrame.iFrameStart == 5)
	{
		m_eCurState = IDLE;
		m_tFrame.iFrameStart = 0;
	}
}
HRESULT CAlligator::TextureRender()
{
	switch (m_eCurState)
	{
	case IDLE:
		if (m_bFront)
		{
			if (FAILED(m_pTextureComIDLE_Front->Bind_OnGraphicDev(m_tFrame.iFrameStart)))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pTextureComIDLE_Back->Bind_OnGraphicDev(m_tFrame.iFrameStart)))
				return E_FAIL;
		}
		break;
	case MOVE:
		if (m_bFront)
		{
			if (FAILED(m_pTextureComMove_Front->Bind_OnGraphicDev(m_tFrame.iFrameStart)))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pTextureComMove_Back->Bind_OnGraphicDev(m_tFrame.iFrameStart)))
				return E_FAIL;
		}
		break;
	case DEAD:
		if (m_bFront)
		{
			if (FAILED(m_pTextureComDead_Front->Bind_OnGraphicDev(m_tFrame.iFrameStart)))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pTextureComDead_Back->Bind_OnGraphicDev(m_tFrame.iFrameStart)))
				return E_FAIL;
		}
		break;
	case SKILL:
		if (m_bFront)
		{
			if (FAILED(m_pTextureComAttack_Front->Bind_OnGraphicDev(m_tFrame.iFrameStart)))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pTextureComAttack_Back->Bind_OnGraphicDev(m_tFrame.iFrameStart)))
				return E_FAIL;
		}
		break;
	default:
		break;
	}
	return S_OK;
}
void CAlligator::OnBillboard()
{
	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	_float3 vScale = { 2.f,2.f,1.f };
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0] * vScale.x);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&ViewMatrix.m[1][0] * vScale.x);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);
}