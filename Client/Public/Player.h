#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class COnterrain;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{
private:
	CPlayer(LPDIRECT3DDEVICE9 _pGraphic_Device);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype(void) override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render(void) override;

private:
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;
	COnterrain* m_pOnTerrain = nullptr;

private:
	HRESULT SetUp_Components(void);
	void Use_Skill();
	void Key_Input(_float fTimeDelta);
	HRESULT Skill_Thunder(const _tchar * pLayerTag, _float3 _vPos);
	HRESULT Skill_Tornado(const _tchar * pLayerTag,_float3 _vPos);
public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 _pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual _float4x4 Get_World(void) override;
	virtual void Free(void) override;

private:
	_bool	m_bUseSkill = false;
	_bool	m_bThunder = false;
	_bool	m_bTornado = false;
	
};

END