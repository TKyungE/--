#include "..\Public\CollisionMgr.h"


bool CCollisionMgr::Collision_Sphere(CLayer::GAMEOBJECTS _Dest, CLayer::GAMEOBJECTS _Sour, CGameObject** pDest, CGameObject** pSour)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Sour : _Sour)
		{
			if (Check_Sphere(Dest, Sour))
			{
				*pDest = Dest;
				*pSour = Sour;
				return true;
			}
		}
	}
	return false;
}

bool CCollisionMgr::Check_Sphere(CGameObject * pDest, CGameObject * pSour)
{
	_float fDist = D3DXVec3Length(&(*(_float3*)&pDest->Get_World().m[3][0] - *(_float3*)&pSour->Get_World().m[3][0]));
	_float	fRadius = pDest->Get_Info().fX + pSour->Get_Info().fX;


	return fRadius >= fDist;	// 충돌을 한 경우
}

//HRESULT CCollisionMgr::Initialize_Prototype(void)
//{
//	return E_NOTIMPL;
//}
//
//HRESULT CCollisionMgr::Initialize(void * pArg)
//{
//	return E_NOTIMPL;
//}
//
//HRESULT CCollisionMgr::Add_ColiisionGroup(COLLISIONGROUP eCollisionGroup, CGameObject * pGameObject)
//{
//	return E_NOTIMPL;
//}
//
//HRESULT CCollisionMgr::Collision_AABB(GAMEOBJECTS _Dest, GAMEOBJECTS _Sour)
//{
//	return E_NOTIMPL;
//}
//
//CCollisionMgr * CCollisionMgr::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
//{
//	return nullptr;
//}
//
//CComponent * CCollisionMgr::Clone(void * pArg)
//{
//	return nullptr;
//}
//
//void CCollisionMgr::Free(void)
//{
//}
