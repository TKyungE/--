#include "..\Public\CollisionMgr.h"




CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}


bool CCollisionMgr::Collision_Sphere(CLayer::GAMEOBJECTS _Dest, CLayer::GAMEOBJECTS _Sour, CGameObject** pDest, CGameObject** pSour, _float3** vPos)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Sour : _Sour)
		{
			if (Check_Sphere(Dest, Sour, vPos))
			{
				*pDest = Dest;
				*pSour = Sour;
				return true;
			}
		}
	}
	return false;
}

bool CCollisionMgr::Check_Sphere(CGameObject * pDest, CGameObject * pSour, _float3** vPos)
{
	_float fDist = D3DXVec3Length(&(*(_float3*)&pDest->Get_World().m[3][0] - *(_float3*)&pSour->Get_World().m[3][0]));
	_float	fRadius = pDest->Get_Info().fX + pSour->Get_Info().fX;

	if (fRadius >= fDist)
	{
		/*_float3 vLook = *(_float3*)&pDest->Get_World().m[3][0] - *(_float3*)&pSour->Get_World().m[3][0];
		D3DXVec3Normalize(&vLook, &vLook);
		vLook = vLook * 0.5f;
		_float Angle = D3DXVec3Dot(&vLook, (_float3*)&pSour->Get_World().m[1][0]);
		_float3 Proj = (Angle / D3DXVec3Length((_float3*)&pSour->Get_World().m[1][0]) * D3DXVec3Length((_float3*)&pSour->Get_World().m[1][0])) * *(_float3*)&pSour->Get_World().m[1][0];
		_float3* pProj = &(*(_float3*)&pSour->Get_World().m[3][0] + Proj);

		*vPos = pProj;*/
	}
	
	return fRadius >= fDist;	// 충돌을 한 경우
}


