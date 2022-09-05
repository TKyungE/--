#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollisionMgr;
END

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
private:		// 파일입출력 라인
	void LoadData();

	typedef struct tagIndexPos		// 백그라운드 정보 읽기
	{
		_float3  BackGroundPos;
		_float3 vScale;
		_uint	iIndex;
	}INDEXPOS;

	vector<INDEXPOS>	m_vecTree;
	vector<INDEXPOS>	m_vecIndex;
	vector<INDEXPOS>	m_vecHouse;
	_float3	m_vPlayerPos;
	vector<_float3> m_vMonsterPos1;

private:
	bool m_bNextLevel = false;

public:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_NPC(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Portal(const _tchar* pLayerTag);

private:
	HRESULT SetUp_Components(void);

private:
	CCollisionMgr* m_pCollisionMgr = nullptr;

public:
	static CTown* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END