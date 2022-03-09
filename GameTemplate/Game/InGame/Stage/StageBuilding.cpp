/// <summary>
/// ステージ上の建物クラス
/// </summary>
#include "stdafx.h"
#include "StageBuilding.h"
#include "../Player/Player.h"
#include "../Player/PlayerRotation.h"
#include "../Player/PlayerAnimation.h"
#include "../Shop/Hotel/HotelScene.h"
#include "../Shop/ItemShop/ItemShopScene.h"
#include "FirstBossStage.h"
#include "../AccompanyAI/AccompanyAI.h"
#include "../../Graphics/Fade.h"
#include "../Camera/PlayerCamera.h"
#include "../Online/OnlineUpdateSpeed.h"

namespace nsKabutoubatu
{
	bool StageBuilding::Start()
	{
		m_onlineUpdateSpeed = FindGO<OnlineUpdateSpeed>(nsStdafx::ONLINEUPDATESPEED_NAME);

		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//プレイヤーのインスタンスを検索
			m_player[playerNum] = FindGO<Player>(nsStdafx::PLAYER_NAME[playerNum]);
			m_playerRotation[playerNum] = FindGO<PlayerRotation>(nsStdafx::PLAYER_ROTATION_NAME[playerNum]);
			m_playerAnimation[playerNum] = FindGO<PlayerAnimation>(nsStdafx::PLAYER_ANIMATION_NAME[playerNum]);

			m_hukidasiSprite[playerNum] = NewGO<SpriteRender>();
			m_hukidasiSprite[playerNum]->Init("Hukidasi_PushA",100.0f,75.0f);
		}

		m_playerCamera = FindGO<PlayerCamera>(nsStdafx::PLAYER_CAMERA_NAME);

		//建物モデルを初期化
		BuildingModelInit();
		//ドアモデルを初期化
		DoorModelInit();
		//暗闇入口モデルを初期化
		DarknessEntranceInit();

		switch (m_shopType)
		{
		case enHotel:
		case enItemShop:
			m_buildingEnterLength = 250.0f;
			m_canDoorTatchLength = 250.0f;
			break;

		case enFirstBossHouse:
			m_buildingEnterLength = 200.0f;
			m_canDoorTatchLength = 450.0f;
			break;
		}

		return true;
	}

	StageBuilding::~StageBuilding()
	{
		//建物を削除
		DeleteGO(m_building);
		//ドアモデルを削除
		DeleteGO(m_door);
		//暗闇入口モデルを削除
		DeleteGO(m_darknessEntrance);

		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			DeleteGO(m_hukidasiSprite[playerNum]);
		}

		if (m_hotelScene != nullptr)
		{
			DeleteGO(m_hotelScene);
		}
		if (m_itemShopScene != nullptr)
		{
			DeleteGO(m_itemShopScene);
		}
		if (m_firstBossStage != nullptr)
		{
			DeleteGO(m_firstBossStage);
		}
	}

	void StageBuilding::Update()
	{
		//建物に入っていないとき、
		if (m_sceneChangeDelayTimer == 0)
		{
			//建物に入ったかどうかの判定処理
			JudgmentEntered();
		}
		//建物に入ったとき、
		else
		{
			if (m_sceneChangeDelayTimer == 10/ m_onlineUpdateSpeed->GetSpeed())
			{
				//建物に入る音の初期化
				m_buildingSound = NewGO<SoundSource>();
				m_buildingSound->Init(L"Assets/sound/Building_Entrance.wav");
				m_buildingSound->SetVolume(0.2f);
				//建物に入る音音を再生
				m_buildingSound->Play(false);
			}
			//遅延タイマーを加算
			m_sceneChangeDelayTimer++;
			//40フレーム経ったら、
			if (m_sceneChangeDelayTimer == 40/ m_onlineUpdateSpeed->GetSpeed() && m_fade->GetNowState() == 2)
			{
				//シーン遷移
				SceneTransition();

				DeleteGO(m_fade);
				//ヌルポインタを入れておく。
				m_fade = nullptr;
			}
		}
	}

	//建物モデルの初期化メソッド
	void StageBuilding::BuildingModelInit()
	{
		//建物モデルの初期化
		m_building = NewGO<SkinModelRender>();
		m_building->SetShadowCaster(true);	//シャドウ落とす
		m_building->SetOutline(true);		//輪郭線をつける
		switch (m_shopType)
		{
		//宿屋
		case enHotel:
			//青色の家
			m_building->Init("House");
			break;
		//アイテムショップ
		case enItemShop:
			//赤色の家
			m_building->Init("HouseRed");
			break;
		//最初のボスの家
		case enFirstBossHouse:
			//ボスの家
			m_building->Init("FirstBossHouse");
			break;
		}
		m_building->SetPosition(m_pos);	//位置をセット
		//当たり判定を適応
		m_physicsStaticObject[0].CreateFromModel(*m_building->GetModel(), m_building->GetModel()->GetWorldMatrix());
		//位置を移動させたので位置を更新
		m_physicsStaticObject[0].Update(m_pos, m_rot);
	}

	//ドアモデルの初期化メソッド
	void StageBuilding::DoorModelInit()
	{
		m_door = NewGO<SkinModelRender>();
		m_door->SetShadowCaster(true);	//シャドウ落とす
		m_door->SetOutline(true);		//輪郭線をつける
		switch (m_shopType)
		{
			//宿屋のドア
		case enHotel:
			//青色の家
			m_door->Init("House_Door");
			m_doorPos = m_pos;
			m_doorPos.z += 340.0f;
			m_doorPos.x += 60.0f;
			break;
			//アイテムショップのドア
		case enItemShop:
			//赤色の家
			m_door->Init("HouseRed_Door");
			m_doorPos = m_pos;
			m_doorPos.z += 340.0f;
			m_doorPos.x += 60.0f;
			break;
			//最初のボスの家のドア
		case enFirstBossHouse:
			m_door->Init("FirstBossHouse_Door");
			m_doorPos = m_pos;
			m_doorPos.z += 220.0f;
			break;
		}

		m_door->SetPosition(m_doorPos);		//位置をセット
		//当たり判定を適応
		m_physicsStaticObject[1].CreateFromModel(*m_door->GetModel(), m_door->GetModel()->GetWorldMatrix());
		//位置を移動させたので位置を更新
		m_physicsStaticObject[1].Update(m_doorPos, m_rot);
	}

	//建物に入ったかどうかの判定メソッド
	void StageBuilding::JudgmentEntered()
	{
		//プレイヤーの人数分実行
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//プレイヤーからドアにのびるベクトルを取得
			m_newPlayerMoveDirection = m_doorPos - m_player[playerNum]->GetPosition();
			//ドアとの距離が近かったら、
			if (m_newPlayerMoveDirection.Length() < m_canDoorTatchLength&&m_player[playerNum]->GetNowState() != enGhost)
			{
				m_doorTatchFlg[playerNum] = true;
				//フラグを立ててAボタンを押されたときにドア開閉にプラスして攻撃しないようにしている。
				m_player[playerNum]->SetNowDoorTatch(true);

				//吹き出しを表示させる
				HukidasiActivate(playerNum);

				if (m_gamePad[playerNum] != nullptr)
				{
					//Aボタンが押されたら、
					if (m_gamePad[playerNum]->IsTrigger(enButtonA))
					{
						//ドアの状態の入れ替え
						switch (m_doorState)
						{
						case enClose:
							m_doorState = enOpen;

							//ドアを開ける音
							m_doorOpenSound = NewGO<SoundSource>();
							m_doorOpenSound->Init(L"Assets/sound/Door_Open.wav");
							m_doorOpenSound->SetVolume(0.2f);
							m_doorOpenSound->Play(false);

							break;
						case enOpen:
							m_doorState = enClose;

							//ドアを閉じる音
							m_doorCloseSound = NewGO<SoundSource>();
							m_doorCloseSound->Init(L"Assets/sound/Door_Close.wav");
							m_doorCloseSound->SetVolume(0.2f);
							m_doorCloseSound->Play(false);

							break;
						}
						m_doorSlideTimer = 0;
					}
				}
				else
				{
					//Aボタンが押されたら、
					if (g_pad[playerNum]->IsTrigger(enButtonA))
					{
						//ドアの状態の入れ替え
						switch (m_doorState)
						{
						case enClose:
							m_doorState = enOpen;

							//ドアを開ける音
							m_doorOpenSound = NewGO<SoundSource>();
							m_doorOpenSound->Init(L"Assets/sound/Door_Open.wav");
							m_doorOpenSound->SetVolume(0.2f);
							m_doorOpenSound->Play(false);

							break;
						case enOpen:
							m_doorState = enClose;

							//ドアを閉じる音
							m_doorCloseSound = NewGO<SoundSource>();
							m_doorCloseSound->Init(L"Assets/sound/Door_Close.wav");
							m_doorCloseSound->SetVolume(0.2f);
							m_doorCloseSound->Play(false);

							break;
						}
						m_doorSlideTimer = 0;
					}
				}
			}
			else
			{
				//PushA!!吹き出しを非表示
				m_hukidasiSprite[playerNum]->Deactivate();
				m_doorTatchFlg[playerNum] = false;

			}

			//ドアが開く
			switch (m_doorState)
			{
			case enClose:
				switch (m_shopType)
				{
				case enHotel:
				case enItemShop:
					m_rotationAngle += 0.05f* m_onlineUpdateSpeed->GetSpeed();
					m_rotationAngle = min(m_rotationAngle, 2.0f);
					//回転を更新
					m_rot.SetRotation(Vector3::AxisY, m_rotationAngle);
					m_door->SetRotation(m_rot);

					break;

				case enFirstBossHouse:
					if (m_doorSlideTimer < 60/ m_onlineUpdateSpeed->GetSpeed())
					{
						m_doorPos.x -= 3.0f* m_onlineUpdateSpeed->GetSpeed();
						m_door->SetPosition(m_doorPos);
					}
					m_doorSlideTimer++;

					break;
				}

				break;

			case enOpen:
				switch (m_shopType)
				{
				case enHotel:
				case enItemShop:
					m_rotationAngle -= 0.05f* m_onlineUpdateSpeed->GetSpeed();
					m_rotationAngle = max(m_rotationAngle, 0.0f);
					//回転を更新
					m_rot.SetRotation(Vector3::AxisY, m_rotationAngle);
					m_door->SetRotation(m_rot);

					break;

				case enFirstBossHouse:
					if (m_doorSlideTimer < 60/ m_onlineUpdateSpeed->GetSpeed() && m_doorPos.x != m_pos.x)
					{
						m_doorPos.x += 3.0f* m_onlineUpdateSpeed->GetSpeed();
						m_door->SetPosition(m_doorPos);
					}
					m_doorSlideTimer++;

					break;
				}

				break;
			}
			//ドアの位置、回転の当たり判定を更新
			m_physicsStaticObject[1].Update(m_doorPos, m_rot);


			//プレイヤーから宿屋にのびるベクトルを取得
			m_newPlayerMoveDirection = m_pos - m_player[playerNum]->GetPosition();
			//建物との距離が近かったら、（建物に入ったら、）
			//かつ、プレイヤーがジャンプ状態ではないとき、
			if (m_newPlayerMoveDirection.Length() < m_buildingEnterLength && m_playerAnimation[playerNum]->GetAnimationState() != 3)
			{
				//建物に入る状態にする
				m_player[playerNum]->SetNowState(enDontMove);
				//プレイヤーの移動方向を建物の奥方向にして移動させる
				m_player[playerNum]->SetMoveSpeed({ 0.0f,0.0f,-1.0f });

				//遅延タイマーを加算
				m_sceneChangeDelayTimer++;

				m_fade = NewGO<Fade>();
				m_fade->Init(0.0f,1,0);
			}
		}
	}

	//シーン遷移メソッド
	void StageBuilding::SceneTransition()
	{
		//シーン遷移
		switch (m_shopType)
		{
		//宿屋
		case enHotel:
			m_hotelScene = NewGO<HotelScene>();
			m_hotelScene->SetPlayerNumber(m_playerNum);
			//建物の場所をシーンに渡しておく
			m_hotelScene->SetHotelPosition(m_pos);
			for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
			{
				if (m_gamePad[playerNum] != nullptr)
				{
					m_hotelScene->SetPlayerGamePad(*m_gamePad[playerNum], playerNum);
				}
			}
			break;
		//アイテムショップ
		case enItemShop:
			m_itemShopScene = NewGO<ItemShopScene>();
			m_itemShopScene->SetPlayerNumber(m_playerNum);
			//建物の場所をシーンに渡しておく
			m_itemShopScene->SetItemShopPosition(m_pos);
			break;
		//最初のボスと戦うシーン
		case enFirstBossHouse:
			m_firstBossStage = NewGO<FirstBossStage>();
			m_firstBossStage->SetPlayerNumber(m_playerNum);
		}
		//プレイヤーの人数分実行
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//ボス戦のとき、
			if (m_shopType == enFirstBossHouse)
			{
				m_gyaku = -1;
			}

			//1P,2Pの位置をシーンの位置に移動させる
			switch (playerNum)
			{
			case enPlayer1:
				m_player[playerNum]->Warp({ 130.0f, 10000.0f* m_gyaku, 250.0f });
				break;
			case enPlayer2:
				m_player[playerNum]->Warp({ -130.0f, 10000.0f* m_gyaku, 250.0f });
				break;
			}

			//移動速度を０にする
			m_player[playerNum]->SetMoveSpeed(Vector3::Zero);
			//奥に向かせる
			m_playerRotation[playerNum]->SetRotationAngle(-3.0f);
			//ボス戦以外のとき、
			if (m_shopType != enFirstBossHouse)
			{
				//操作不可状態にする
				m_player[playerNum]->SetNowState(enDontMove);
			}
			//ボス戦に行く且つゴースト状態じゃないとき、
			else if (m_player[playerNum]->GetNowState() != enGhost)
			{
				//通常状態にする
				m_player[playerNum]->SetNowState(0);
			}
			//アイドルアニメーションにする
			m_playerAnimation[playerNum]->SetAnimation(enNormal);
			//回復アイテムを持っていたらしまう。
			m_player[playerNum]->RecoveryItemDelete();
		}

		//ソロモードの時、
		if (m_playerNum == 1)
		{
			m_accompaniAI = FindGO<AccompanyAI>(nsStdafx::ACCOMPANYAI_NAME);
			m_accompaniAI->SetNowState(enNormal);
			m_accompaniAI->SetPosition({ -130.0f, 10000.0f * m_gyaku, 250.0f });
		}

		//シーンに入るまでの遅延タイマーの初期化
		m_sceneChangeDelayTimer = 0;

		//ドアが開いていたなら閉めておく
		if (m_doorState == enOpen)
		{
			m_doorState = enClose;
		}
		else
		{
			m_doorState = enOpen;
		}
	}

	//暗闇入口モデルの初期化メソッド
	void StageBuilding::DarknessEntranceInit()
	{
		m_darknessEntrance = NewGO<SkinModelRender>();
		m_darknessEntrance->SetShadowCaster(false);	//シャドウ落とさない
		m_darknessEntrance->SetOutline(false);		//輪郭線をつけない
		switch (m_shopType)
		{
		case enHotel:
		case enItemShop:
			m_darknessEntrance->Init("DarknessEntrance");
			break;
		case enFirstBossHouse:
			m_darknessEntrance->Init("DarknessEntrance_FirstBossHouse");
			break;
		}
			m_darknessEntrance->SetPosition(m_pos);		//位置をセット
	}

	//吹き出しを表示させるメソッド
	void StageBuilding::HukidasiActivate(const int playerNum)
	{
		//プレイヤーのスクリーン座標を取得
		g_camera3D->CalcScreenPositionFromWorldPosition(m_playerScreenPos, m_player[playerNum]->GetPosition());
		//カメラの注視点になっているプレイヤーに近いときのみ吹き出しを表示させる
		if (((m_playerCamera->GetCameraTarget() - m_player[playerNum]->GetPosition()).Length() > 3000.0f))
		{
			//PushA!!吹き出しを非表示
			m_hukidasiSprite[playerNum]->Deactivate();

			return;
		}
		//位置を調整
		m_playerScreenPos.x -= 50.0f;
		m_playerScreenPos.y += 75.0f;
		//吹き出しの位置をセット
		m_hukidasiSprite[playerNum]->SetPosition({ m_playerScreenPos.x,m_playerScreenPos.y,0.0f });
		//PushA!!吹き出しを表示
		m_hukidasiSprite[playerNum]->Activate();
	}
}