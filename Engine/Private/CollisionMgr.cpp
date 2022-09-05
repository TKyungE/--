#include "..\Public\CollisionMgr.h"
#include "GameInstance.h"
#include "GameObject.h"

CCollisionMgr::CCollisionMgr(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

HRESULT CCollisionMgr::Initialize_Prototype(void)
{
	return S_OK;
}

HRESULT CCollisionMgr::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CCollisionMgr::Ready_ObjectsArray(_uint iNumObjects)
{
	m_iNumObjects = iNumObjects;

	m_GameObjects = new GAMEOBJECTS[m_iNumObjects];

	return S_OK;
}

HRESULT CCollisionMgr::Add_ColiisionGroup(_uint iCollisionGroup, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects[iCollisionGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

_bool CCollisionMgr::Collision(CGameObject * pGameObject, _uint iCollisionGroup)
{
	if (nullptr == pGameObject)
	{
		ERR_MSG(TEXT("Failed to Collision : pGameObject"));
		return false;
	}

	if (!Collision_AABB(pGameObject, m_GameObjects[iCollisionGroup]))
		return false;

	else
		return true;
}

void CCollisionMgr::Release_Objects(void)
{
	if (nullptr != m_GameObjects)
	{
		for (_uint i = 0; i < m_iNumObjects; ++i)
		{
			for (auto& iter : m_GameObjects[i])
				Safe_Release(iter);

			m_GameObjects[i].clear();
		}
	}
}


_bool CCollisionMgr::Collision_AABB(class CGameObject* _Dest, GAMEOBJECTS _Sour)
{
	CCollider* DestCollider = (CCollider*)_Dest->Find_Component(TEXT("Com_Collider"));
	if (nullptr == DestCollider)
	{
		ERR_MSG(TEXT("Failed to Check AABB : Dest"));
		return true;
	}

	_float3 DestMin = DestCollider->Find_MinPoint();
	_float3 DestMax = DestCollider->Find_MaxPoint();

	for (auto& Sour : _Sour)
	{
		CCollider* SourCollider = (CCollider*)Sour->Find_Component(TEXT("Com_Collider"));
		if (nullptr == SourCollider)
		{
			ERR_MSG(TEXT("Failed to Check AABB : Sour"));
			return true;
		}

		_float3 SourMin = SourCollider->Find_MinPoint();
		_float3 SourMax = SourCollider->Find_MaxPoint();

		if (_Dest->Get_World().m[3][0] < Sour->Get_World().m[3][0])
		{
			if (DestMax.x < SourMin.x)
				return false;
			else
				m_vCollision.x = DestMax.x - SourMin.x;
		}
		else
		{
			if (DestMin.x > SourMax.x)
				return false;
			else
				m_vCollision.x = SourMax.x - DestMin.x;
		}
		if (_Dest->Get_World().m[3][2] < Sour->Get_World().m[3][2])
		{
			if (DestMax.z < SourMin.z)
				return false;

			else
				m_vCollision.z = DestMax.z - SourMin.z;
		}
		else
		{
			if (DestMin.z > SourMax.z)
				return false;

			else
				m_vCollision.z = SourMax.z - DestMin.z;
		}
		if (_Dest->Get_World().m[3][1] < Sour->Get_World().m[3][1])
		{
			if (DestMax.y < SourMin.y)
				return false;

			else
				m_vCollision.y = DestMax.y - SourMin.y;
		}
		else
		{
			if (DestMin.y > SourMax.y)
				return false;

			else
				m_vCollision.y = SourMax.y - DestMin.y;
		}
	}

	return true;
}

CCollisionMgr * CCollisionMgr::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollisionMgr* pInstance = new CCollisionMgr(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CColiisionMgr"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollisionMgr::Clone(void * pArg)
{
	AddRef();

	return this;
}

void CCollisionMgr::Free(void)
{
	for (_uint i = 0; i < m_iNumObjects; ++i)
	{
		for (auto& iter : m_GameObjects[i])
			Safe_Release(iter);

		m_GameObjects[i].clear();
	}

	Safe_Delete_Array(m_GameObjects);

	__super::Free();
}
