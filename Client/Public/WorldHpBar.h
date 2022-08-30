#pragma once

#include"GameObject.h"
#include"Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CWorldHpBar : public CGameObject
{
private:
	CWorldHpBar(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWorldHpBar(const CWorldHpBar& rhs);
	virtual ~CWorldHpBar() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void	Tick(_float fTimeDelta)override;
	virtual void	Late_Tick(_float fTimeDelta)override;
	virtual HRESULT Render() override;

private: /* For.Components */
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

private:
	typedef struct HpPos
	{
		_float3 vPos;
		_int	iHp;
	}HpPos;

	_float4x4				m_ProjMatrix;
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	bool*					m_Click;
	_ushort					m_Check = 0;

	short					m_Temp = 0;
	HpPos					m_Recive;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CWorldHpBar* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual _float4x4 Get_World(void) override;
	virtual void Free() override;
};

END


