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
	
	Key_Input(fTimeDelta);
	Use_Skill();
}

void CPlayer::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
}

HRESULT CPlayer::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
		return E_FAIL;

	m_pVIBufferCom->Render();
	m_pGraphic_Device->SetTexture(0, nullptr);
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

	memcpy(&m_tInfo, pArg, sizeof(INFO));
	m_tInfo.pTarget = pInstance;
	memcpy(pArg, &m_tInfo, sizeof(INFO));

	return pInstance;
}

HRESULT CPlayer::SetUp_Components(void)
{
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Onterrain"), LEVEL_STATIC, TEXT("Prototype_Component_Onterrain"), (CComponent**)&m_pOnTerrain)))
		return E_FAIL;

	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

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
	}
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON) && m_bUseSkill && m_bTornado)
	{
		pInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_UseSkill"))->Get_Objects().front()->Set_Dead();
		Skill_Tornado(TEXT("Layer_Skill"), pInstance->Get_TargetPos());
		m_bUseSkill = false;
		m_bTornado = false;
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


	if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
		m_pTransformCom->Go_Straight(fTimeDelta);

	if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
		m_pTransformCom->Go_Backward(fTimeDelta);

	if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
		m_pTransformCom->Go_Left(fTimeDelta);

	if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
		m_pTransformCom->Go_Right(fTimeDelta);

	if (CKeyMgr::Get_Instance()->Key_Pressing('Q'))
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * -1.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing('E'))
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * 1.f);

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
	Safe_Release(m_pVIBufferCom);
}

_float3 CPlayer::Get_Pos()
{
	return m_pTransformCom->Get_State(CTransform::STATE_POSITION);
}
