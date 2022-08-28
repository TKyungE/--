#pragma once
#include  "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	//����Ʈ���� �浹 �˻�
	static bool	Collision_Sphere(CLayer::GAMEOBJECTS _Dest, CLayer::GAMEOBJECTS _Sour, CGameObject** pDest, CGameObject** pSour,_float3** vPos);
	//���� �浹 �˻�
	static bool Check_Sphere(CGameObject* pDest, CGameObject* pSour, _float3** vPos);
};

END