#include "..\Public\Picking.h"

IMPLEMENT_SINGLETON(CPicking)

CPicking::CPicking()
{
	ZeroMemory(&m_GraphicDesc, sizeof(GRAPHIC_DESC));
	ZeroMemory(&m_MouseRayPos, sizeof(_float3));
	ZeroMemory(&m_MouseRayDir, sizeof(_float3));
	ZeroMemory(&m_TargetPos, sizeof(_float3));
}

HRESULT CPicking::Initialize(const GRAPHIC_DESC & GraphicDesc, LPDIRECT3DDEVICE9 * ppOut)
{
	memcpy(&m_GraphicDesc, &GraphicDesc, sizeof(GRAPHIC_DESC));

	m_pGraphic_Device = *ppOut;
	Safe_AddRef(m_pGraphic_Device);

	return S_OK;
}

void CPicking::Tick(void)
{
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(m_GraphicDesc.hWnd, &ptMouse);

	_float4x4 View, Proj, InvView, InvProj;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &View);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &Proj);

	m_MouseRayDir.x = (((_float)ptMouse.x / m_GraphicDesc.iWinSizeX) * 2.f) - 1.f;
	m_MouseRayDir.y = (((_float)ptMouse.y / m_GraphicDesc.iWinSizeY) * -2.f) + 1.f;
	m_MouseRayDir.z = 0.f;

	D3DXMatrixInverse(&InvProj, nullptr, &Proj);
	D3DXVec3TransformCoord(&m_MouseRayDir, &m_MouseRayDir, &InvProj);

	m_MouseRayPos = { 0.f, 0.f, 0.f };

	D3DXMatrixInverse(&InvView, nullptr, &View);

	D3DXVec3TransformCoord(&m_MouseRayPos, &m_MouseRayPos, &InvView);
	D3DXVec3TransformNormal(&m_MouseRayDir, &m_MouseRayDir, &InvView);
}

HRESULT CPicking::Intersect(_float4x4 InvWorld, _float3* LU, _float3* RU, _float3* RD)
{
	D3DXVec3TransformCoord(&m_MouseRayPos, &m_MouseRayPos, &InvWorld);
	D3DXVec3TransformNormal(&m_MouseRayDir, &m_MouseRayDir, &InvWorld);

	D3DXVec3Normalize(&m_MouseRayDir, &m_MouseRayDir);

	_float Dist = 0.f;

	if (D3DXIntersectTri(LU, RU, RD, &m_MouseRayPos, &m_MouseRayDir, nullptr, nullptr, &Dist))
		m_TargetPos = m_MouseRayPos + m_MouseRayDir * Dist;
	else
		return E_FAIL;

	return S_OK;
}

void CPicking::Free(void)
{
	Safe_Release(m_pGraphic_Device);
}
