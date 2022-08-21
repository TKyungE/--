#include "stdafx.h"
#include "..\Public\Camera_Dynamic.h"
#include "GameInstance.h"

CCamera_Dynamic::CCamera_Dynamic(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
	ZeroMemory(&m_vecCameraNormal, sizeof(_float3));

	D3DXMatrixIdentity(&m_matRotY);
}

CCamera_Dynamic::CCamera_Dynamic(const CCamera_Dynamic & rhs)
	: CCamera(rhs)
	, m_vecCameraNormal(rhs.m_vecCameraNormal)
	, m_YfAngle(rhs.m_YfAngle)
	, m_matRotY(rhs.m_matRotY)
{
}

HRESULT CCamera_Dynamic::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Dynamic::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&((CAMERADESC_DERIVED*)pArg)->CameraDesc)))
		return E_FAIL;
	
	m_vecCameraNormal = *(_float3*)&m_CameraDesc.Info.pTarget->Get_World().m[2][0] * -1.f;
	D3DXVec3Normalize(&m_vecCameraNormal, &m_vecCameraNormal);

	return S_OK;
}

void CCamera_Dynamic::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	_long MouseMove = 0;

	if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_X))
		CameraRotationX(fTimeDelta, MouseMove);

	if ((GetKeyState(VK_LSHIFT)& 8000) &&  (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_Y)))
		CameraRotationY(fTimeDelta, MouseMove);

	if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_WHEEL))
		m_CameraDesc.fFovy += D3DXToRadian(fTimeDelta * MouseMove * -1.f);

	_float4x4 CameraRotationMatrix;
	D3DXMatrixRotationAxis(&CameraRotationMatrix, &m_pTransform->Get_State(CTransform::STATE_RIGHT), D3DXToRadian(15.f));

	_float3 Camera;
	CameraRotationMatrix *= m_matRotY;
	D3DXVec3TransformNormal(&Camera, &m_vecCameraNormal, &CameraRotationMatrix);

	m_pTransform->Set_State(CTransform::STATE_POSITION, (Camera * 5.f) + *(_float3*)&m_CameraDesc.Info.pTarget->Get_World().m[3][0]);

	Safe_Release(pGameInstance);

	m_pTransform->LookAt(*(_float3*)&m_CameraDesc.Info.pTarget->Get_World().m[3][0]);

	if (FAILED(Bind_OnGraphicDev()))
		return;
}

void CCamera_Dynamic::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

}

HRESULT CCamera_Dynamic::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CCamera_Dynamic::CameraRotationX(_float fTimeDelta, _long MouseMove)
{
	_float4x4 matXRot;
	D3DXMatrixRotationAxis(&matXRot, &_float3(0.f, 1.f, 0.f), fTimeDelta * MouseMove * m_CameraDesc.TransformDesc.fRotationPerSec * 0.1f);

	D3DXVec3TransformNormal(&m_vecCameraNormal, &m_vecCameraNormal, &matXRot);
}

void CCamera_Dynamic::CameraRotationY(_float fTimeDelta, _long MouseMove)
{
	m_YfAngle += fTimeDelta * -MouseMove * m_CameraDesc.TransformDesc.fRotationPerSec * 0.05f;

	if (m_YfAngle > D3DXToRadian(-5.f) && m_YfAngle < D3DXToRadian(25.f))
		D3DXMatrixRotationAxis(&m_matRotY, &m_pTransform->Get_State(CTransform::STATE_RIGHT), m_YfAngle);
}

CCamera_Dynamic * CCamera_Dynamic::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Dynamic*	pInstance = new CCamera_Dynamic(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCamera_Dynamic"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Dynamic::Clone(void* pArg)
{
	CCamera_Dynamic*	pInstance = new CCamera_Dynamic(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCamera_Dynamic"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

_float4x4 CCamera_Dynamic::Get_World(void)
{
	return _float4x4();
}

void CCamera_Dynamic::Free()
{
	__super::Free();
}
