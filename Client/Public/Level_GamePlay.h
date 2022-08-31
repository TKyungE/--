#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "GameObject.h"

BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
private:
	CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

public:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);

private:
	CGameObject::INFO Info;

private:
	_float3 Get_CollisionPos(CGameObject* pDest, CGameObject* pSour);


private:		// 파일입출력 라인
	void SpawnData();

	typedef struct tagIndexPos		// 백그라운드 정보 읽기
	{
		_float3  BackGroundPos;
		_uint	iIndex;
	}INDEXPOS;

	vector<INDEXPOS>	m_vecIndex;
	_float3	m_vPlayerPos;
	vector<_float3> m_vMonsterPos1;

private:
	void Create_Rain(_float fTimeDelta);

public:
	static CLevel_GamePlay* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
private:
	_float fSound = 0.f;
	_float fCollTime = 0.f;
	_float fRainTime = 0.f;
	bool m_IconRender = false;
};

END

