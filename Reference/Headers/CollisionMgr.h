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
	static bool	Collision_Sphere(CLayer::GAMEOBJECTS _Dest, CLayer::GAMEOBJECTS _Sour);
	//���� �浹 �˻�
	static bool Check_Sphere(CGameObject* pDest, CGameObject* pSour);
};

END