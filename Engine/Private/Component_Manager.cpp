#include "..\Public\Component_Manager.h"

IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
{
	
}

HRESULT CComponent_Manager::Reserve_Container(_uint iNumLevels)
{
	if (nullptr != m_pPrototypes)
		return E_FAIL;

	m_pPrototypes = new PROTOTYPES[iNumLevels];

	m_iNumLevels = iNumLevels;

	return S_OK;
}

HRESULT CComponent_Manager::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pPrototypes ||
		iLevelIndex >= m_iNumLevels)
		return E_FAIL;

	if (nullptr != Find_Component(iLevelIndex, pPrototypeTag))
		return E_FAIL;

	m_pPrototypes[iLevelIndex].emplace(pPrototypeTag, pPrototype);

	if (!lstrcmp(pPrototypeTag, TEXT("Prototype_Component_Texture_Terrain_CollTile")))
	{
		dynamic_cast<CTexture*>(pPrototype)->LoadTexture(CTexture::TYPE_DEFAULT, TEXT("Prototype_Component_Texture_Terrain_LookMap"), TEXT("../Bin/Resources/Textures/OBJ/OBJ/MAP/LookMap/Map%d.png"), 1111);
	}


	//*if (!lstrcmp(pPrototypeTag, TEXT("Prototype_Component_Texture_Player_IDLE")))
	//{
	//	dynamic_cast<CTexture*>(pPrototype)->LoadTexture(CTexture::TYPE_DEFAULT, TEXT("Prototype_Component_Texture_Player_Front"), TEXT("../Bin/Resources/Textures/Player/Attack/%d.jpg"), 5);
	//	dynamic_cast<CTexture*>(pPrototype)->LoadTexture(CTexture::TYPE_DEFAULT, TEXT("Prototype_Component_Texture_Player_Front"), TEXT("../Bin/Resources/Textures/Player/Front/%d.jpg"), 5);

	//}
	//else if (!lstrcmp(pPrototypeTag, TEXT("Prototype_Component_Texture_Monster_IDLE")))
	//{
	//	dynamic_cast<CTexture*>(pPrototype)->LoadTexture(CTexture::TYPE_DEFAULT, TEXT("Prototype_Component_Texture_Player_Front"), TEXT("../Bin/Resources/Textures/Player/Attack/%d.jpg"), 5);
	//	dynamic_cast<CTexture*>(pPrototype)->LoadTexture(CTexture::TYPE_DEFAULT, TEXT("Prototype_Component_Texture_Player_Front"), TEXT("../Bin/Resources/Textures/Player/Front/%d.jpg"), 5);

	//}*/
	return S_OK;
}

CComponent * CComponent_Manager::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pPrototypes ||
		iLevelIndex >= m_iNumLevels)
		return nullptr;

	CComponent*		pPrototype = Find_Component(iLevelIndex, pPrototypeTag);
	if (nullptr == pPrototype)
		return nullptr;

	CComponent*		pComponent = pPrototype->Clone(pArg);
	if (nullptr == pComponent)
		return nullptr;

	return pComponent;
}

void CComponent_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

	for (auto& Pair : m_pPrototypes[iLevelIndex])	
		Safe_Release(Pair.second);	

	m_pPrototypes[iLevelIndex].clear();
}

CComponent * CComponent_Manager::Find_Component(_uint iLevelIndex, const _tchar * pPrototypeTag)
{
	auto	iter = find_if(m_pPrototypes[iLevelIndex].begin(), m_pPrototypes[iLevelIndex].end(), CTag_Finder(pPrototypeTag));

	if (iter == m_pPrototypes[iLevelIndex].end())
		return nullptr;

	return iter->second;	
}

void CComponent_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pPrototypes[i])
		{
			Safe_Release(Pair.second);
		}
		m_pPrototypes[i].clear();
	}
	Safe_Delete_Array(m_pPrototypes);
}
