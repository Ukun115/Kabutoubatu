/// <summary>
/// プレイヤーのステータスクラス
/// </summary>
#include "stdafx.h"
#include "PlayerStatus.h"
#include "PlayerSound.h"

namespace nsKabutoubatu
{
	bool PlayerStatus::Start()
	{
		m_playerSound = FindGO< PlayerSound >(nsStdafx::PLAYER_SOUND_NAME[m_playerNum]);

		return true;
	}

	PlayerStatus::~PlayerStatus()
	{

	}

	void PlayerStatus::Update()
	{
		//ヒットポイントの補正
		m_hitPoint = min(m_maxHitPoint, m_hitPoint);	//ヒットポイントが最大体力以上にならないようにする
		m_hitPoint = max(0, m_hitPoint);		//ヒットポイントが0以下にならないようにする
	}

	//プレイヤーのコインを増やすメソッド
	void PlayerStatus::AddCoin(const int coinValue)
	{
		//所持コイン数を増やす
		m_possessionCoin += int(coinValue * m_addMoneyMagnification);
		//総取得コイン数を増やす
		m_totalGetCoin += int(coinValue * m_addMoneyMagnification);

		//取得音
		m_playerSound->PlayAddCoinSound();
	};

	//プレイヤーを回復させるメソッド
	void PlayerStatus::AddHitPoint(const int addValue)
	{
		//回復力分回復する
		m_hitPoint += addValue;
		//自身の最大ヒットポイント数を超えてしまわないために補正をかける
		if (m_hitPoint > m_maxHitPoint)
		{
			m_hitPoint = m_maxHitPoint;
		}
	};
}