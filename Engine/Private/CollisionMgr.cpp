#include "..\Public\CollisionMgr.h"
#include "GameObject.h"

CCollisionMgr::CCollisionMgr(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

HRESULT CCollisionMgr::Initialize_Prototype(void)
{
	return S_OK;
}

HRESULT CCollisionMgr::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CCollisionMgr::Add_ColiisionGroup(COLLISIONGROUP eCollisionGroup, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;
	
	m_GameObjects[eCollisionGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

_bool CCollisionMgr::Collision_AABB(GAMEOBJECTS _Dest, GAMEOBJECTS _Sour)
{
	/*for (auto& Dest : _Dest)
	{
		for (auto& Sour : _Sour)
		{

		}
	}*/


	return true;
}

CCollisionMgr * CCollisionMgr::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollisionMgr* pInstance = new CCollisionMgr(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CColiisionMgr"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollisionMgr::Clone(void * pArg)
{
	AddRef();

	return this;
}

void CCollisionMgr::Free(void)
{
	__super::Free();
}


//bool CCollisionMgr::Collision_Sphere(CLayer::GAMEOBJECTS _Dest, CLayer::GAMEOBJECTS _Sour, CGameObject** pDest, CGameObject** pSour)
//{
//	for (auto& Dest : _Dest)
//	{
//		for (auto& Sour : _Sour)
//		{
//			if (Check_Sphere(Dest, Sour))
//			{
//				*pDest = Dest;
//				*pSour = Sour;
//				return true;
//			}
//		}
//	}
//	return false;
//}
//
//bool CCollisionMgr::Check_Sphere(CGameObject * pDest, CGameObject * pSour)
//{
//	_float fDist = D3DXVec3Length(&(*(_float3*)&pDest->Get_World().m[3][0] - *(_float3*)&pSour->Get_World().m[3][0]));
//	_float	fRadius = pDest->Get_Info().fX + pSour->Get_Info().fX;
//
//
//	return fRadius >= fDist;	// 충돌을 한 경우
//}

