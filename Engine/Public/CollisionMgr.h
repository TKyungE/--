#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollisionMgr final : public CComponent
{
public:
	enum COLLISIONGROUP { COLLISION_PLAYER, COLLISION_MONSTER, COLLISION_MONSTER, COLLISION_PLAYERSKILL, COLLISION_MONSTERSKILL, COLLISION_OBJECT, COLLISION_END };

private:
	CCollisionMgr(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CCollisionMgr() = default;

public:
	virtual HRESULT Initialize_Prototype(void) override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Add_ColiisionGroup(COLLISIONGROUP eCollisionGroup, class CGameObject* pGameObject);

public:
	//리스트끼리 충돌 검사
	static bool	Collision_Sphere(CLayer::GAMEOBJECTS _Dest, CLayer::GAMEOBJECTS _Sour, CGameObject** pDest, CGameObject** pSour);
	//개별 충돌 검사
	static bool Check_Sphere(CGameObject* pDest, CGameObject* pSour);

private:
	list <class CGameObject*> m_GameObjects[COLLISION_END];
	typedef list<class CGameObject*> GAMEOBJECTS;

private:
	HRESULT Collision_AABB(GAMEOBJECTS _Dest, GAMEOBJECTS _Sour);

public:
	static CCollisionMgr* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free(void) override;
};

END