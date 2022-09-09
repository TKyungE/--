#include "stdafx.h"
#include "..\Public\HuntQuest.h"

CHuntQuest::CHuntQuest(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CQuest(pGraphic_Device)
{
}

CHuntQuest::CHuntQuest(const CHuntQuest & rhs)
	: CQuest(rhs)
{
}

HRESULT CHuntQuest::Initialize_Prototype(void)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CHuntQuest::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	


	return S_OK;
}

void CHuntQuest::Tick(void)
{
}

CHuntQuest * CHuntQuest::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	return nullptr;
}

CHuntQuest * CHuntQuest::Clone(void * pArg)
{
	return nullptr;
}

void CHuntQuest::Free(void)
{
}
