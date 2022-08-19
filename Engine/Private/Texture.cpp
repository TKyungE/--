#include "..\Public\Texture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs)
	, m_Textures(rhs.m_Textures)
	, m_iNumTextures(rhs.m_iNumTextures)
{
	/*for (auto& pTexture : m_Textures)
		Safe_AddRef(pTexture);*/
}

//Texture.png
//Texture%d.png

HRESULT CTexture::Initialize_Prototype(TYPE eType, const _tchar* pPrototypeTag,const _tchar * pTextureFilePath, _uint iNumTexture)
{
	m_iNumTextures = iNumTexture;

	_tchar			szFullPath[MAX_PATH] = TEXT("");
	vector<IDirect3DBaseTexture9*> *vecImg = new vector<IDirect3DBaseTexture9*>;
	for (_uint i = 0; i < m_iNumTextures; ++i)
	{
		IDirect3DBaseTexture9*			pTexture = nullptr;
		
		wsprintf(szFullPath, pTextureFilePath, i);
		
		HRESULT hr = eType == TYPE_DEFAULT ? D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture) : D3DXCreateCubeTextureFromFile(m_pGraphic_Device, szFullPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);

		if (FAILED(hr))
			return E_FAIL;
		vecImg->push_back(pTexture);
	}
	m_Textures.emplace(pPrototypeTag, vecImg);
	return S_OK;
}

HRESULT CTexture::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CTexture::LoadTexture(TYPE eType, const _tchar* pPrototypeTag, const _tchar * pTextureFilePath, _uint iNumTexture)
{
	m_iNumTextures = iNumTexture;

	_tchar			szFullPath[MAX_PATH] = TEXT("");
	vector<IDirect3DBaseTexture9*> *vecImg = new vector<IDirect3DBaseTexture9*>;
	for (_uint i = 0; i < m_iNumTextures; ++i)
	{
		IDirect3DBaseTexture9*			pTexture = nullptr;
		
		wsprintf(szFullPath, pTextureFilePath, i);

		HRESULT hr = eType == TYPE_DEFAULT ? D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture) : D3DXCreateCubeTextureFromFile(m_pGraphic_Device, szFullPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);
		
		if (FAILED(hr))
			return E_FAIL;
		vecImg->push_back(pTexture);
	}
	m_Textures.emplace(pPrototypeTag, vecImg);
	return S_OK;
}

HRESULT CTexture::Bind_OnGraphicDev(const _tchar* pPrototypeTag,_uint iIndex)
{
	if (iIndex >= m_iNumTextures)
		return E_FAIL;

	auto&	iter = find_if(m_Textures.begin(), m_Textures.end(), CTag_Finder(pPrototypeTag));

	if (iter == m_Textures.end())
		return E_FAIL;
	
	return m_pGraphic_Device->SetTexture(0, (*iter->second)[iIndex]);
}

_int CTexture::MoveFrame(_float fTimeDelta, _float MoveSpeed, _int MaxFrame)
{
	m_fFrameTime += fTimeDelta;
	if (m_fFrameTime > MoveSpeed)
		++m_iFrame;
	if (m_iFrame > MaxFrame)
		m_iFrame = 0;
	return m_iFrame;
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eType, const _tchar* pPrototypeTag,const _tchar * pTextureFilePath, _uint iNumTexture)
{
	CTexture*	pInstance = new CTexture(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eType, pPrototypeTag,pTextureFilePath, iNumTexture)))
	{
		ERR_MSG(TEXT("Failed to Created : CTexture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTexture::Clone(void* pArg)
{
	CTexture*	pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CTexture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& Pair : m_Textures)
	{
		for (auto& vec : (*Pair.second))
		{
			Safe_Release(vec);
		}
		(*Pair.second).clear();
	//	Safe_Release(Pair.second);
	}
	m_Textures.clear();
}
