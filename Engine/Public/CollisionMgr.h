#pragma once
#include  "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	//리스트끼리 충돌 검사
	static bool	Collision_Sphere(CLayer::GAMEOBJECTS _Dest, CLayer::GAMEOBJECTS _Sour);
	//개별 충돌 검사
	static bool Check_Sphere(CGameObject* pDest, CGameObject* pSour);
};

END