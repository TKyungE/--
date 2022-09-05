//#pragma once
//
//#include "Component.h"
//
//BEGIN(Engine)
//
//class ENGINE_DLL CCollisionMgr final : public CComponent
//{
//public:
//	enum COLLISIONGROUP { COLLISION_PLAYER, COLLISION_MONSTER, COLLISION_BOSS, COLLISION_PLAYERSKILL, COLLISION_MONSTERSKILL, COLLISION_OBJECT, COLLISION_END };
//
//private:
//	CCollisionMgr(LPDIRECT3DDEVICE9 pGraphic_Device);
//	virtual ~CCollisionMgr() = default;
//
//public:
//	virtual HRESULT Initialize_Prototype(void) override;
//	virtual HRESULT Initialize(void* pArg) override;
//
//public:
//	HRESULT Add_ColiisionGroup(COLLISIONGROUP eCollisionGroup, class CGameObject* pGameObject);
//
//
//private:
//	list <class CGameObject*> m_GameObjects[COLLISION_END];
//	typedef list<class CGameObject*> GAMEOBJECTS;
//
//private:
//	HRESULT Collision_AABB(GAMEOBJECTS _Dest, GAMEOBJECTS _Sour);
//
//public:
//	static CCollisionMgr* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
//	virtual CComponent* Clone(void* pArg = nullptr) override;
//	virtual void Free(void) override;
//};
//
//END