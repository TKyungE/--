#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTexture;
class CTransform;
END

class CTerrainRect final : public CGameObject
{
public:
	typedef struct tagRectInfo
	{
		_float3 vPos;
		_uint iTex;
		vector<_float3> vecPointPos;
		vector<_float2> vecPointTex;
	}RECTINFO;

private:
	CTerrainRect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTerrainRect(const CTerrainRect& rhs);
	virtual ~CTerrainRect() = default;

public:
	virtual HRESULT Initialize_Prototype(void) override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta)override;
	virtual void Late_Tick(_float fTimeDelta)override;
	virtual HRESULT Render(void) override;

public:
	RECTINFO Get_RectInfo(void) { return m_tInfo; }
	void Set_RectTex(_uint iTex) { m_tInfo.iTex = iTex; }

	LPDIRECT3DVERTEXBUFFER9 Get_VB(void) { return m_pVBuffer; }

private:
	CRenderer* m_pRendererCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CTransform* m_pTransformCom = nullptr;

private:
	HRESULT SetUp_Components(void);

private:
	RECTINFO m_tInfo;
	_uint m_iIndex = 0;
	_uint m_iIndices[4];

	LPDIRECT3DVERTEXBUFFER9 m_pVBuffer;
	LPDIRECT3DINDEXBUFFER9 m_pIBuffer;

public:
	static CTerrainRect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual _float4x4 Get_World(void) override;
	virtual void Free(void) override;
};
