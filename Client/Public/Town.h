#pragma once
#include "Client_Defines.h"
#include "Level.h"
#include "GameObject.h"
BEGIN(Client)

class CTown final : public CLevel
{
private:
	CTown(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTown() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
private:
	CGameObject::INFO Info;
private:		// ��������� ����
	void SpawnData();

	typedef struct tagIndexPos		// ��׶��� ���� �б�
	{
		_float3  BackGroundPos;
		_uint	iIndex;
	}INDEXPOS;

	vector<INDEXPOS>	m_vecIndex;
	_float3	m_vPlayerPos;
	vector<_float3> m_vMonsterPos1;

private:
	bool m_bNextLevel = false;
	bool m_IconRender = false;
public:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);
public:
	static CTown* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END