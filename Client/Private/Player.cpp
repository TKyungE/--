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
	*(CGameObject**)pArg = this;


	m_vTarget = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_ePreState = STATE_END;
	m_eCurState = IDLE;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.fFrameSpeed = 0.1f;

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

	BodyRender();

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferBody->Render();
	
	Set_HeadPos();
	if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
		return E_FAIL;
	HeadRender();
	m_pVIBufferHead->Render();
	_float3 vScale = { 1.f,1.f ,1.f };
	m_pTransformCom->Set_Scaled(vScale);
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

	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer_Body"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferBody)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer_Head"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferHead)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Onterrain"), LEVEL_STATIC, TEXT("Prototype_Component_Onterrain"), (CComponent**)&m_pOnTerrain)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Body_IDLE_Front"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Body_IDLE_Front"), (CComponent**)&m_pTextureComBodyIDLE_Front)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Body_IDLE_Back"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Body_IDLE_Back"), (CComponent**)&m_pTextureComBodyIDLE_Back)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Body_Move_Front"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Body_Move_Front"), (CComponent**)&m_pTextureComBodyMove_Front)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Body_Move_Back"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Body_Move_Back"), (CComponent**)&m_pTextureComBodyMove_Back)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Body_Skill_Front"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Body_Skill_Front"), (CComponent**)&m_pTextureComBodySkill_Front)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Body_Skill_Back"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Body_Skill_Back"), (CComponent**)&m_pTextureComBodySkill_Back)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Head_IDLE_Front"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Head_IDLE_Front"), (CComponent**)&m_pTextureComHeadIDLE_Front)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Head_IDLE_Back"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Head_IDLE_Back"), (CComponent**)&m_pTextureComHeadIDLE_Back)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Head_Skill_Front"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Head_Skill_Front"), (CComponent**)&m_pTextureComHeadSkill_Front)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture_Head_Skill_Back"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Head_Skill_Back"), (CComponent**)&m_pTextureComHeadSkill_Back)))
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
	}
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON) && m_bUseSkill && m_bTornado)
	{
		pInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_UseSkill"))->Get_Objects().front()->Set_Dead();
		Skill_Tornado(TEXT("Layer_Skill"), pInstance->Get_TargetPos());
		m_bUseSkill = false;
		m_bTornado = false;
		m_eCurState = SKILL;
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

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RBUTTON))
	{
		m_vTarget = pInstance->Get_TargetPos();
		m_eCurState = MOVE;
	}
	

	//if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
	//	m_pTransformCom->Go_Straight(fTimeDelta);

	//if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
	//	m_pTransformCom->Go_Backward(fTimeDelta);

	//if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
	//	m_pTransformCom->Go_Left(fTimeDelta);

	//if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
	//	m_pTransformCom->Go_Right(fTimeDelta);

	//if (CKeyMgr::Get_Instance()->Key_Pressing('Q'))
	//	m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * -1.f);

	//if (CKeyMgr::Get_Instance()->Key_Pressing('E'))
	//	m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * 1.f);

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
	Safe_Release(m_pVIBufferHead);
	Safe_Release(m_pVIBufferBody);
	Safe_Release(m_pTextureComBodyIDLE_Front);
	Safe_Release(m_pTextureComBodyIDLE_Back);
	Safe_Release(m_pTextureComBodyMove_Front);
	Safe_Release(m_pTextureComBodyMove_Back);
	Safe_Release(m_pTextureComBodySkill_Front);
	Safe_Release(m_pTextureComBodySkill_Back);
	Safe_Release(m_pTextureComHeadIDLE_Front);
	Safe_Release(m_pTextureComHeadIDLE_Back);
	Safe_Release(m_pTextureComHeadSkill_Front);
	Safe_Release(m_pTextureComHeadSkill_Back);
	
}

_float3 CPlayer::Get_Pos()
{
	return m_pTransformCom->Get_State(CTransform::STATE_POSITION);
}

void CPlayer::Set_HeadPos()
{
	_float3 HeadPos = { -0.01f,0.32f,0.f };
	_float3 vScale = { 0.45f,0.45f ,1.f };
	m_pTransformCom->Set_Scaled(vScale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION)+ HeadPos);
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
			}
		}
		else
			m_eCurState = IDLE;
		
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
			m_tFrame.iFrameStart = m_pTextureComBodyIDLE_Front->MoveFrame(fTimeDelta, m_tFrame.fFrameSpeed, m_tFrame.iFrameEnd);
		else
			m_tFrame.iFrameStart = m_pTextureComBodyIDLE_Back->MoveFrame(fTimeDelta, m_tFrame.fFrameSpeed, m_tFrame.iFrameEnd);
		break;
	case MOVE:
		if (m_bFront)
			m_tFrame.iFrameStart = m_pTextureComBodyMove_Front->MoveFrame(fTimeDelta, m_tFrame.fFrameSpeed, m_tFrame.iFrameEnd);
		else
			m_tFrame.iFrameStart = m_pTextureComBodyMove_Back->MoveFrame(fTimeDelta, m_tFrame.fFrameSpeed, m_tFrame.iFrameEnd);
		break;
	case SKILL:
		if (m_bFront)
			m_tFrame.iFrameStart = m_pTextureComBodySkill_Front->MoveFrame(fTimeDelta, m_tFrame.fFrameSpeed, m_tFrame.iFrameEnd);
		else
			m_tFrame.iFrameStart = m_pTextureComBodySkill_Back->MoveFrame(fTimeDelta, m_tFrame.fFrameSpeed, m_tFrame.iFrameEnd);
		break;
	default:
		break;
	}
}

HRESULT CPlayer::BodyRender()
{
	switch (m_eCurState)
	{
	case IDLE:
		if (m_bFront)
		{
			if (FAILED(m_pTextureComBodyIDLE_Front->Bind_OnGraphicDev(m_tFrame.iFrameStart)))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pTextureComBodyIDLE_Back->Bind_OnGraphicDev(m_tFrame.iFrameStart)))
				return E_FAIL;
		}
		break;
	case MOVE:
		if (m_bFront)
		{
			if (FAILED(m_pTextureComBodyMove_Front->Bind_OnGraphicDev(m_tFrame.iFrameStart)))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pTextureComBodyMove_Back->Bind_OnGraphicDev(m_tFrame.iFrameStart)))
				return E_FAIL;
		}
		break;
	case SKILL:
		if (m_bFront)
		{
			if (FAILED(m_pTextureComBodySkill_Front->Bind_OnGraphicDev(m_tFrame.iFrameStart)))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pTextureComBodySkill_Back->Bind_OnGraphicDev(m_tFrame.iFrameStart)))
				return E_FAIL;
		}
		break;
	default:
		break;
	}
	return S_OK;
}
HRESULT CPlayer::HeadRender()
{
	switch (m_eCurState)
	{
	case IDLE:
		if (m_bFront)
		{
			if (FAILED(m_pTextureComHeadIDLE_Front->Bind_OnGraphicDev(0)))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pTextureComHeadIDLE_Back->Bind_OnGraphicDev(0)))
				return E_FAIL;
		}
		break;
	case MOVE:
		if (m_bFront)
		{
			if (FAILED(m_pTextureComHeadIDLE_Front->Bind_OnGraphicDev(0)))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pTextureComHeadIDLE_Back->Bind_OnGraphicDev(0)))
				return E_FAIL;
		}
		break;
	case SKILL:
		if (m_bFront)
		{
			if (FAILED(m_pTextureComHeadSkill_Front->Bind_OnGraphicDev(0)))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pTextureComHeadSkill_Back->Bind_OnGraphicDev(0)))
				return E_FAIL;
		}
		break;
	default:
		break;
	}
	return S_OK;
}
