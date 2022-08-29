#include "stdafx.h"
#include "..\Public\Player.h"

#include "GameInstance.h"
#include "Layer.h"
#include "KeyMgr.h"

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
	
	memcpy(&m_tInfo, pArg, sizeof(INFO));
	m_tInfo.pTarget = this;
	memcpy(pArg, &m_tInfo, sizeof(INFO));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tInfo.vPos);
	m_vTarget = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_ePreState = STATE_END;
	m_eCurState = IDLE;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.fFrameSpeed = 0.1f;

	m_tInfo.fX = 0.5f;

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
	Move_Frame(fTimeDelta);
	Key_Input(fTimeDelta);
	//Check_Front();
	
	Player_Move(fTimeDelta);
}

void CPlayer::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Motion_Change();

	Use_Skill();

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CPlayer::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	Off_SamplerState();
	if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
		return E_FAIL;

	TextureRender();

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBuffer->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;
	
	On_SamplerState();
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
	
	return pInstance;
}

HRESULT CPlayer::SetUp_Components(void)
{
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Onterrain"), LEVEL_STATIC, TEXT("Prototype_Component_Onterrain"), (CComponent**)&m_pOnTerrain)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture_IDLE_Front"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_IDLE_Front"), (CComponent**)&m_pTextureComIDLE_Front)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_IDLE_Back"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_IDLE_Back"), (CComponent**)&m_pTextureComIDLE_Back)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Move_Front"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Move_Front"), (CComponent**)&m_pTextureComMove_Front)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Move_Back"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Move_Back"), (CComponent**)&m_pTextureComMove_Back)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Skill_Front"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Skill_Front"), (CComponent**)&m_pTextureComSkill_Front)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Skill_Back"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Skill_Back"), (CComponent**)&m_pTextureComSkill_Back)))
		return E_FAIL;



	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CPlayer::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetTexture(0, nullptr);
	return S_OK;
}

HRESULT CPlayer::On_SamplerState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

HRESULT CPlayer::Off_SamplerState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	return S_OK;
}

void CPlayer::Use_Skill()
{
	CGameInstance* pInstance = CGameInstance::Get_Instance();

	if (nullptr == pInstance)
		return;

	Safe_AddRef(pInstance);

	if (CKeyMgr::Get_Instance()->Key_Down('1') && !m_bUseSkill && !m_bThunder)
	{
		CGameObject::INFO tInfo;

		tInfo.vPos = pInstance->Get_TargetPos();

		pInstance->Add_GameObject(TEXT("Prototype_GameObject_UseSkill"), LEVEL_GAMEPLAY, TEXT("Layer_UseSkill"), &tInfo);
	
		m_bUseSkill = true;
		m_bThunder = true;
	}


	if (CKeyMgr::Get_Instance()->Key_Down('2') && !m_bUseSkill && !m_bTornado)
	{
		CGameObject::INFO tInfo;

		tInfo.vPos = pInstance->Get_TargetPos();

		pInstance->Add_GameObject(TEXT("Prototype_GameObject_UseSkill"), LEVEL_GAMEPLAY, TEXT("Layer_UseSkill"), &tInfo);
		
		m_bUseSkill = true;
		m_bTornado = true;
	}
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON) && m_bUseSkill && m_bThunder)
	{
		pInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_UseSkill"))->Get_Objects().front()->Set_Dead();
		Skill_Thunder(TEXT("Layer_Skill"), pInstance->Get_TargetPos());
		m_bUseSkill = false;
		m_bThunder = false;
		m_eCurState = SKILL;
		m_tFrame.iFrameStart = 0;
	}
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON) && m_bUseSkill && m_bTornado)
	{
		pInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_UseSkill"))->Get_Objects().front()->Set_Dead();
		Skill_Tornado(TEXT("Layer_Skill"), pInstance->Get_TargetPos());
		m_bUseSkill = false;
		m_bTornado = false;
		m_eCurState = SKILL;
		m_tFrame.iFrameStart = 0;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		if (CKeyMgr::Get_Instance()->Key_Down('3'))
		{
			CGameObject::INFO tInfo;
			tInfo.vPos = pInstance->Get_TargetPos();
			pInstance->Add_GameObject(TEXT("Prototype_GameObject_Hit"), LEVEL_GAMEPLAY, TEXT("Layer_Effect"), &tInfo);	
		}
	}
	
	if (CKeyMgr::Get_Instance()->Key_Down('4'))
	{
		CGameObject::INFO tInfo;
		tInfo.pTarget = this;
		//tInfo.vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		pInstance->Add_GameObject(TEXT("Prototype_GameObject_LevelUp"), LEVEL_GAMEPLAY, TEXT("Layer_Effect"), &tInfo);
	}
	

	Safe_Release(pInstance);
}

void CPlayer::Key_Input(_float fTimeDelta)
{
	CGameInstance* pInstance = CGameInstance::Get_Instance();

	if (nullptr == pInstance)
		return;

	Safe_AddRef(pInstance);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RBUTTON) && m_eCurState != SKILL)
	{
		m_vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		m_vTarget = pInstance->Get_TargetPos();
		m_eCurState = MOVE;
		m_tFrame.iFrameStart = 0;
		Check_Front();
	}
	

	Safe_Release(pInstance);
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
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTextureComIDLE_Front);
	Safe_Release(m_pTextureComIDLE_Back);
	Safe_Release(m_pTextureComMove_Front);
	Safe_Release(m_pTextureComMove_Back);
	Safe_Release(m_pTextureComSkill_Front);
	Safe_Release(m_pTextureComSkill_Back);
	
}

_float3 CPlayer::Get_Pos()
{
	return m_pTransformCom->Get_State(CTransform::STATE_POSITION);
}


void CPlayer::Player_Move(_float fTimeDelta)
{
	
	_float3 vLook;
	m_vTargetLook = m_vTarget - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	D3DXVec3Normalize(&vLook, &m_vTargetLook);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);
	if (D3DXVec3Length(&m_vTargetLook) < 0.55f)
	{
		if (m_eCurState == SKILL)
		{
			if (m_tFrame.iFrameStart == 4)
			{
				m_eCurState = IDLE;
				m_tFrame.iFrameStart = 0;
			}
		}
		else
		{
			m_eCurState = IDLE;
			m_tFrame.iFrameStart = 0;
		}
		
	}

	m_pTransformCom->Go_Straight(fTimeDelta);
}

void CPlayer::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.fFrameSpeed = 0.1f;
			break;
		case MOVE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.fFrameSpeed = 0.1f;
			break;
		case SKILL:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.fFrameSpeed = 0.1f;
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CPlayer::Move_Frame(_float fTimeDelta)
{
	switch (m_eCurState)
	{
	case IDLE:
		if(m_bFront)
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
	case SKILL:
		if (m_bFront)
			m_tFrame.iFrameStart = m_pTextureComSkill_Front->MoveFrame(fTimeDelta, m_tFrame.fFrameSpeed, m_tFrame.iFrameEnd);
		else
			m_tFrame.iFrameStart = m_pTextureComSkill_Back->MoveFrame(fTimeDelta, m_tFrame.fFrameSpeed, m_tFrame.iFrameEnd);
		break;
	default:
		break;
	}
}

void CPlayer::Check_Front()
{
	
	_float3 vLook;
	m_vTargetLook = m_vTarget - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	D3DXVec3Normalize(&vLook, &m_vTargetLook);
	_float cos = D3DXVec3Dot(&vLook, &m_vLook);
	_float Angle = 0.f;
	Angle = acosf(Angle);
	if (Angle < D3DXToRadian(90.f) && Angle > D3DXToRadian(-90.f))
		m_bFront = false;
	else if (Angle < D3DXToRadian(-90.f))
		m_bFront = true;
}

HRESULT CPlayer::TextureRender()
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
	case SKILL:
		if (m_bFront)
		{
			if (FAILED(m_pTextureComSkill_Front->Bind_OnGraphicDev(m_tFrame.iFrameStart)))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pTextureComSkill_Back->Bind_OnGraphicDev(m_tFrame.iFrameStart)))
				return E_FAIL;
		}
		break;
	default:
		break;
	}
	return S_OK;
}

