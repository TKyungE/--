#include "..\Public\CollisionMgr.h"

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}


bool CCollisionMgr::Collision_Sphere(CLayer::GAMEOBJECTS _Dest, CLayer::GAMEOBJECTS _Sour)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Sour : _Sour)
		{
			if (Check_Sphere(Dest, Sour))
			{
				return true;
			}
		}
	}
	return false;
}

bool CCollisionMgr::Check_Sphere(CGameObject * pDest, CGameObject * pSour)
{
	// abs : ���밪�� ������ִ� �Լ�
	float	fWidth = fabs(pDest->Get_Info().vPos.x - pSour->Get_Info().vPos.x);
	float	fHeight = fabs(pDest->Get_Info().vPos.y - pSour->Get_Info().vPos.y);
	float   fDeep	= fabs(pDest->Get_Info().vPos.z - pSour->Get_Info().vPos.z);
	// sqrt : ��Ʈ�� �����ִ� �Լ�
	float	fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight + fDeep * fDeep);

	float	fRadius = (pDest->Get_Info().fX + pSour->Get_Info().fX) * 0.5f;

	return fRadius >= fDiagonal;	// �浹�� �� ���
}


