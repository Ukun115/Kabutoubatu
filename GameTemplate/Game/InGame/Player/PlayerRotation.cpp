/// <summary>
/// プレイヤーの回転クラス
/// </summary>
#include "stdafx.h"
#include "PlayerRotation.h"
#include "Player.h"

namespace nsKabutoubatu
{
	bool PlayerRotation::Start()
	{
		m_player = FindGO<Player>(nsStdafx::PLAYER_NAME[m_playerNum]);

		return true;
	}

	void PlayerRotation::Update()
	{
		//ショッピング中じゃないときは移動速度から回転角度を求める
		if (m_player->GetNowState() != 3)
		{
			//回転角度を求める
			m_rotAngle = atan2(m_player->GetMoveSpeed().x, m_player->GetMoveSpeed().z);
		}
		//回転を更新
		m_rot.SetRotation(Vector3::AxisY, m_rotAngle);
	}
}