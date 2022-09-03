#include "..\Public\CollisionMgr.h"

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}


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


