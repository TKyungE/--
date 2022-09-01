#include "stdafx.h"
#include "..\Public\BackGroundTree.h"

CBackGroundTree::CBackGroundTree(LPDIRECT3DDEVICE9 _pGraphic_Device)
	:CGameObject(_pGraphic_Device)
{
}

CBackGroundTree::CBackGroundTree(const CBackGroundTree & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBackGroundTree::Initialize_Prototype(void)
{
	return E_NOTIMPL;
}

HRESULT CBackGroundTree::Initialize(void * pArg)
{
	return E_NOTIMPL;
}

void CBackGroundTree::Tick(_float fTimeDelta)
{
}

void CBackGroundTree::Late_Tick(_float fTimeDelta)
{
}

HRESULT CBackGroundTree::Render(void)
{
	return E_NOTIMPL;
}

HRESULT CBackGroundTree::SetUp_Components(void)
{
	return E_NOTIMPL;
}

HRESULT CBackGroundTree::SetUp_RenderState(void)
{
	return E_NOTIMPL;
}

HRESULT CBackGroundTree::Release_RenderState(void)
{
	return E_NOTIMPL;
}

void CBackGroundTree::OnTerrain(void)
{
}

void CBackGroundTree::OnBillBoard(void)
{
}

CBackGroundTree * CBackGroundTree::Create(LPDIRECT3DDEVICE9 _pGraphic_Device)
{
	return nullptr;
}

CGameObject * CBackGroundTree::Clone(void * pArg)
{
	return nullptr;
}

_float4x4 CBackGroundTree::Get_World(void)
{
	return _float4x4();
}

void CBackGroundTree::Free(void)
{
}
