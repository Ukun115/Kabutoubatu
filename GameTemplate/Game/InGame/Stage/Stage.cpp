/// <summary>
/// ステージクラス
/// </summary>
#include "stdafx.h"
#include "Stage.h"
#include "StageBuilding.h"
#include "../Enemy/WeakEnemy/Mole.h"
#include "../Enemy/WeakEnemy/Slime.h"
#include "../Enemy/WeakEnemy/NeedleSlime.h"
#include "../Player/Player.h"

namespace nsKabutoubatu
{
	namespace nsStage
	{
		const Vector3 BUILDING_POS[3] = { { -300.0f, 0.0f, -1400.0f },{ -1800.0f, 0.0f, -7500.0f },{ -5850.0f, 0.0f, -7700.0f } };

		const Vector3 WEAK_ENEMY_FIRST_RESPOWN_POS[12] =
		{
			{ -1950.0f,0.0f,-740.0f },{ -1795.0f,0.0f,-740.0f },
			{ -1950.0f,0.0f,-1400.0f },{ -1950.0f,0.0f,-2850.0f },
			{ -2500.0f,0.0f,-6230.0f },{ -930.0f,100.0f,-60.0f },
			{ -3000.0f,100.0f,-6230.0f },{ -1700.0f,100.0f,-3000.0f },
			{ -3000.0f,0.0f,-5500.0f },{ -1200.0f,100.0f,-60.0f },
			{ -1400.0f,0.0f,-60.0f },{ -1800.0f,100.0f,-4060.0f }
		};
	}

	bool Stage::Start()
	{
		for (int playerNum = 0; playerNum < m_playerNum; playerNum++)
		{
			m_player[playerNum] = FindGO<Player>(nsStdafx::PLAYER_NAME[playerNum]);
		}

		//海モデルの初期化
		m_seaModel = NewGO<SkinModelRender>();
		m_seaModel->Init("sea");

		//足場モデルの初期化
		m_floorModel = NewGO<SkinModelRender>();
		m_floorModel->Init("FirstStage_map1_floor");
		//当たり判定を適応
		m_physicsStaticObject[0].CreateFromModel(*m_floorModel->GetModel(), m_floorModel->GetModel()->GetWorldMatrix());

		//ステージ上の当たり判定がある装飾モデルの初期化
		m_hitStageAcceModel = NewGO<SkinModelRender>();
		m_hitStageAcceModel->SetShadowCaster(true);	//シャドウは落とす
		m_hitStageAcceModel->SetOutline(true);		//輪郭線をつける
		m_hitStageAcceModel->SetSilhouette(false);
		m_hitStageAcceModel->Init("FirstStage_map1_hitAcce");
		//当たり判定を適応
		m_physicsStaticObject[1].CreateFromModel(*m_hitStageAcceModel->GetModel(), m_hitStageAcceModel->GetModel()->GetWorldMatrix());

		//ステージの当たり判定のない装飾モデルの初期化
		m_dontHitStageAcceModel = NewGO<SkinModelRender>();
		m_dontHitStageAcceModel->SetShadowCaster(true);	//シャドウは落とす
		m_dontHitStageAcceModel->SetOutline(true);		//輪郭線をつける
		m_dontHitStageAcceModel->SetSilhouette(false);
		m_dontHitStageAcceModel->Init("FirstStage_map1_dontHitAcce");

		//ショップの建物をステージ上に生成
		for (int shopNum = 0; shopNum < 3; shopNum++)
		{
			m_shopBuiding[shopNum] = NewGO<StageBuilding>();
			m_shopBuiding[shopNum]->SetPlayerNumber(m_playerNum);
			//アイテムショップか宿屋か最初のボスの家かを設定
			m_shopBuiding[shopNum]->SetShopType(shopNum);
			//位置をセット
			switch (shopNum)
			{
			case 0:
				m_shopBuiding[shopNum]->SetPosition(nsStage::BUILDING_POS[0]);
				break;
			case 1:
				m_shopBuiding[shopNum]->SetPosition(nsStage::BUILDING_POS[1]);
				break;
			case 2:
				m_shopBuiding[shopNum]->SetPosition(nsStage::BUILDING_POS[2]);
			}
		}

		//ザコ敵を生成
		for (int weakEnemyNum = 0; weakEnemyNum < 4; weakEnemyNum++)
		{
			m_mole[weakEnemyNum] = NewGO<Mole>();
			m_mole[weakEnemyNum]->SetPlayerNum(m_playerNum);
			m_slime[weakEnemyNum] = NewGO<Slime>();
			m_slime[weakEnemyNum]->SetPlayerNum(m_playerNum);
			m_needleSlime[weakEnemyNum] = NewGO<NeedleSlime>(nsStdafx::PRIORITY_0,nsStdafx::NEEDLE_SLIME_NAME);
			m_needleSlime[weakEnemyNum]->SetPlayerNum(m_playerNum);
		}
		//もぐらを初期リスポーン位置にセット
		m_mole[0]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[0]);
		m_mole[1]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[1]);
		m_mole[2]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[2]);
		m_mole[3]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[3]);
		//スライムを初期リスポーン位置にセット

		m_slime[0]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[4]);
		m_slime[1]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[5]);
		m_slime[2]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[6]);
		m_slime[3]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[7]);
		//とげスライムを初期リスポーン位置にセット
		m_needleSlime[0]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[8]);
		m_needleSlime[1]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[9]);
		m_needleSlime[2]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[10]);
		m_needleSlime[3]->SetInitPosition(nsStage::WEAK_ENEMY_FIRST_RESPOWN_POS[11]);

		return true;
	}

	Stage::~Stage()
	{
		//海モデルの削除
		DeleteGO(m_seaModel);
		//足場モデルを削除
		DeleteGO(m_floorModel);
		//ステージ上の当たり判定がある装飾モデルを削除
		DeleteGO(m_hitStageAcceModel);
		//ステージの当たり判定のない装飾モデルを削除
		DeleteGO(m_dontHitStageAcceModel);
		//ショップの建物を削除
		for (int shopNum = 0; shopNum < 3; shopNum++)
		{
			//ショップ建物を削除
			DeleteGO(m_shopBuiding[shopNum]);
		}
		//弱敵を削除
		for (int weakEnemyNum = 0; weakEnemyNum < 4; weakEnemyNum++)
		{
			DeleteGO(m_mole[weakEnemyNum]);
			DeleteGO(m_slime[weakEnemyNum]);
			DeleteGO(m_needleSlime[weakEnemyNum]);
		}
	}

	void Stage::Update()
	{
		//ザコ敵のリスポーン処理
		WeakEnemyResporn();

		PlayerAttackOk();
	}

	//ザコ敵のリスポーン処理メソッド
	void Stage::WeakEnemyResporn()
	{
		for (int weakEnemyNum = 0; weakEnemyNum < 4; weakEnemyNum++)
		{
			//もぐらが死んだとき、
			if (m_mole[weakEnemyNum]->IsDead() || m_mole[weakEnemyNum]->GetFall())
			{
				DeleteGO(m_mole[weakEnemyNum]);
				//新しいもぐらを生成させる。
				m_mole[weakEnemyNum] = NewGO<Mole>();
				//ゲームに参加しているプレイヤーの人数を渡しておく。
				m_mole[weakEnemyNum]->SetPlayerNum(m_playerNum);
				//スポーン位置を設定する。
				switch (weakEnemyNum)
				{
				case 0:
					m_mole[0]->SetInitPosition(WeakEnemyRandomRespownPos(-2100, -177, -1537, -6828));
					break;
				case 1:
					m_mole[1]->SetInitPosition(WeakEnemyRandomRespownPos(-2100, -177, -1537, -6828));
					break;
				case 2:
					m_mole[2]->SetInitPosition(WeakEnemyRandomRespownPos(-2100, -177, -1537, -6828));
					break;
				case 3:
					m_mole[3]->SetInitPosition(WeakEnemyRandomRespownPos(-2100, -177, -1537, -6828));
					break;
				}
			}
			//スライムが死んだとき、
			if (m_slime[weakEnemyNum]->IsDead() || m_slime[weakEnemyNum]->GetFall())
			{
				DeleteGO(m_slime[weakEnemyNum]);
				//新しいスライムを生成させる。
				m_slime[weakEnemyNum] = NewGO<Slime>();
				//ゲームに参加しているプレイヤーの人数を渡しておく。
				m_slime[weakEnemyNum]->SetPlayerNum(m_playerNum);
				//スポーン位置を設定する。
				switch (weakEnemyNum)
				{
				case 0:
					m_slime[0]->SetInitPosition(WeakEnemyRandomRespownPos(-798, -567, -2100, 177));
					break;
				case 1:
					m_slime[1]->SetInitPosition(WeakEnemyRandomRespownPos(-798, -567, -2100, 177));
					break;
				case 2:
					m_slime[2]->SetInitPosition(WeakEnemyRandomRespownPos(-798, -567, -2100, 177));
					break;
				case 3:
					m_slime[3]->SetInitPosition(WeakEnemyRandomRespownPos(-798, -567, -2100, 177));
					break;
				}
			}
			//とげスライムが死んだとき、
			if (m_needleSlime[weakEnemyNum]->IsDead()|| m_needleSlime[weakEnemyNum]->GetFall())
			{
				DeleteGO(m_needleSlime[weakEnemyNum]);
				//新しいとげスライムを生成させる。
				m_needleSlime[weakEnemyNum] = NewGO<NeedleSlime>();
				//ゲームに参加しているプレイヤーの人数を渡しておく。
				m_needleSlime[weakEnemyNum]->SetPlayerNum(m_playerNum);
				//スポーン位置を設定する。
				switch (weakEnemyNum)
				{
				case 0:
					m_needleSlime[0]->SetInitPosition(WeakEnemyRandomRespownPos(-798, -567, -2100, 177));
					break;
				case 1:
					m_needleSlime[1]->SetInitPosition(WeakEnemyRandomRespownPos(-798, -567, -2100, 177));
					break;
				case 2:
					m_needleSlime[2]->SetInitPosition(WeakEnemyRandomRespownPos(-798, -567, -2100, 177));
					break;
				case 3:
					m_needleSlime[3]->SetInitPosition(WeakEnemyRandomRespownPos(-798, -567, -2100, 177));
					break;
				}
			}
		}
	}

	void Stage::PlayerAttackOk()
	{
		for (int playerNum = 0; playerNum < m_playerNum; playerNum++)
		{
			for (int shopNum = 0; shopNum < 3; shopNum++)
			{
				if (m_shopBuiding[shopNum]->GetDoorTatchFlg(playerNum))
				{
					return;
				}
				if (shopNum == 2)
				{
					m_player[playerNum]->SetNowDoorTatch(false);
				}
			}
		}
	}

	Vector3 Stage::WeakEnemyRandomRespownPos(const int x1, const int z1,const int x2,const int z2)
	{
		m_weakEnemyRandomRespownPos.y = 500.0f;
		m_weakEnemyRandomRespownPos.x = x1 + (x2 - x1) * ((rand()%100) / 100.0f);
		m_weakEnemyRandomRespownPos.z = z1 + (z2 - z1) * ((rand()%100) / 100.0f);
		return m_weakEnemyRandomRespownPos;

		//四角形の角の位置
		//(-798,-567)→(-2100,177)→(-1537,-6828)→(-6810,-6095)
	}
}