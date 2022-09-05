#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollisionMgr final : public CComponent
{
private:
	CCollisionMgr(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CCollisionMgr() = default;

public:
	virtual HRESULT Initialize_Prototype(void) override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Add_ColiisionGroup(_uint iCollisionGroup, class CGameObject* pGameObject);
	_bool Collision(class CGameObject* pGameObject, _uint iCollisionGroup);

private:
	list <class CGameObject*>* m_GameObjects;
	typedef list<class CGameObject*> GAMEOBJECTS;

private:
	_bool Collision_AABB(class CGameObject* _Dest, GAMEOBJECTS _Sour);

public:
	static CCollisionMgr* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free(void) override;
};

END