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
	HRESULT Ready_ObjectsArray(_uint iNumObjects);
	HRESULT Add_ColiisionGroup(_uint iCollisionGroup, class CGameObject* pGameObject);
	_bool Collision(class CGameObject* pGameObject, _uint iCollisionGroup);
	void Release_Objects(void);

public:
	_float3 Get_Collision(void) { return m_vCollision; }

private:
	_uint m_iNumObjects = 0;
	_float3 m_vCollision = _float3(0.f, 0.f, 0.f);
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