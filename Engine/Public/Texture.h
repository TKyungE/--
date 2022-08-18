#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
public:
	enum TYPE { TYPE_DEFAULT, TYPE_CUBEMAP, TYPE_END };
private:
	CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	HRESULT Initialize_Prototype(TYPE eType, const _tchar* pTextureFilePath, _uint iNumTexture);
	HRESULT Initialize(void* pArg) override;

public:
	HRESULT Bind_OnGraphicDev(_uint iIndex = 0);

private:
	_uint										m_iNumTextures = 0;
	vector<IDirect3DBaseTexture9*>				m_Textures;
	//LPDIRECT3DCUBETEXTURE9
	//LPDIRECT3DTEXTURE9

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eType, const _tchar* pTextureFilePath, _uint iNumTexture = 1);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END