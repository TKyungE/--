#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class COnterrain;
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{
public:
	enum STATE { IDLE,MOVE,SKILL, STATE_END };
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
	CVIBuffer_Rect* m_pVIBuffer = nullptr;
	COnterrain* m_pOnTerrain = nullptr;

	CTexture* m_pTextureComIDLE_Front = nullptr;
	CTexture* m_pTextureComIDLE_Back = nullptr;
	CTexture* m_pTextureComMove_Front = nullptr;
	CTexture* m_pTextureComMove_Back = nullptr;
	CTexture* m_pTextureComSkill_Front = nullptr;
	CTexture* m_pTextureComSkill_Back = nullptr;

private:
	HRESULT SetUp_Components(void);
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
	HRESULT On_SamplerState();
	HRESULT Off_SamplerState();
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
	_float3 m_vTargetLook;
	_float3 m_vTarget;
	_bool	m_bUseSkill = false;
	_bool	m_bThunder = false;
	_bool	m_bTornado = false;
	_bool	m_bFront = false;
	_bool	m_bCamera = false;
	STATE				m_ePreState;
	STATE				m_eCurState;
public:
	_float3 Get_Pos();
	_bool Get_Front() { return m_bFront; }
	void Set_Front(_bool bfront) { m_bFront = bfront; }
	void Set_Camera(_bool bCamera) { m_bCamera = bCamera; }
private:
	void Player_Move(_float fTimeDelta);
	void Motion_Change();
	void Move_Frame(_float fTimeDelta);
	void Check_Front();
	HRESULT TextureRender();
	
};

END