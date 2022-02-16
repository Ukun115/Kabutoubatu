/// <summary>
/// 宿屋シーンクラス
/// </summary>
#include "stdafx.h"
#include "HotelScene.h"
#include "../../Player/Player.h"
#include "../../Player/PlayerStatus.h"
#include "../../Camera/PlayerCamera.h"
#include "HotelSelect.h"
#include "../../AccompanyAI/AccompanyAI.h"
#include "../../GameScene.h"
#include "../../GameLimitTime.h"
#include "../../../Graphics/Fade.h"


namespace nsKabutoubatu
{
	namespace nsHotelScene
	{
		Vector2   POS = { -160.0f,170.0f };			//フォントの位置
		const Vector4   FONT_COLOR = { 0.0f,0.0f,0.0f,1.0f };	//制限時間フォントの色
		const float     FONT_ROT = 0.0f;			//フォントの傾き
		const Vector2   FONT_PIV = { 1.0f,1.0f };	//フォントの基点
		const float     FONT_SCA = 0.75f;			//フォントの拡大率
	}

	bool HotelScene::Start()
	{
		//プレイヤーのインスタンスを検索
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			m_player[playerNum] = FindGO<Player>(nsStdafx::PLAYER_NAME[playerNum]);
			m_playerStatus[playerNum] = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[playerNum]);
		}
		m_gameLimitTime = FindGO<GameLimitTime>(nsStdafx::GAMELIMITTIME);
		m_gameLimitTime->SetCanMeasure(false);

		m_fade[0] = NewGO<Fade>();
		m_fade[0]->Init(1.0f,0,0);

		//カメラのインスタンスを検索
		m_playerCamera = FindGO<PlayerCamera>(nsStdafx::PLAYER_CAMERA_NAME);

		//宿屋の内装モデルを初期化
		HotelInteriorModelInit();

		m_hukidasi = NewGO<SpriteRender>(nsStdafx::PRIORITY_0);
		m_hukidasi->Init("Hukidasi", 450, 180);
		m_hukidasi->SetPosition({0.0f, 110.0f, 0.0f});

		//店員モデルを初期化
		SalespersonModelInit();

		//店員の会話文
		m_salespersonTalk = NewGO<FontRender>();
		m_salespersonTalk->Init
		(
			L"",							//テキスト
			nsHotelScene::POS,			//位置
			nsHotelScene::FONT_COLOR,	//色
			nsHotelScene::FONT_ROT,		//傾き
			nsHotelScene::FONT_SCA,		//拡大率
			nsHotelScene::FONT_PIV		//基点
		);

		//プレイヤー２人分生成
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//何を選択しているかのクラス
			m_hotelSelect[playerNum] = NewGO<HotelSelect>();
			//プレイヤーの番号をセット
			m_hotelSelect[playerNum]->SetPlayerNum(playerNum);

			m_coinSprite[playerNum] = NewGO<SpriteRender>();
			m_coinSprite[playerNum]->Init("Coin", 50, 50);
			m_coinSprite[playerNum]->SetPosition({ m_coinPos,115.0f,0.0f });
			m_coinPos += 835.0f;
		}

		return true;
	}

	HotelScene::~HotelScene()
	{
		//宿屋内装モデルを削除
		DeleteGO(m_hotelInterior);
		//店員モデルを削除
		DeleteGO(m_salesperson);
		//店員の会話削除
		DeleteGO(m_salespersonTalk);
		//プレイヤー２人分削除
		for (int playerNum = 0; playerNum < 2; playerNum++)
		{
			DeleteGO(m_hotelSelect[playerNum]);
			//コイン画像を削除
			DeleteGO(m_coinSprite[playerNum]);
		}

		//ショップBGMを削除
		DeleteGO(m_shopBGM);

		DeleteGO(m_hukidasi);

		DeleteGO(m_fade[1]);
	}

	void HotelScene::Update()
	{
		//お辞儀が終わったら、
		if (!m_salesperson->IsPlaying())
		{
			//アイドル状態にする
			m_salesperson->PlayAnimation(enIdle);
		}

		if (m_fade[0] != nullptr && m_fade[0]->GetNowState() == 2)
		{
			DeleteGO(m_fade[0]);
		}

		if (m_gameScene == nullptr)
		{
			//ゲームシーンのインスタンスを検索
			m_gameScene = FindGO< GameScene>(nsStdafx::GAMESCENE_NAME);
			//ゲームBGMを止める
			m_gameScene->GameBGMStop();

			//ショップBGMの初期化
			m_shopBGM = NewGO<SoundSource>();
			m_shopBGM->Init(L"Assets/sound/Shop_BGM.wav");
			m_shopBGM->SetVolume(0.2f);
			//ショップBGMを再生
			m_shopBGM->Play(true);	//ループ再生
		}

		//店員の会話
		swprintf_s(m_talkText, L"ようこそ!\n泊まっていきますか?");
		m_salespersonTalk->SetText(m_talkText);

		//プレイヤー２人分生成
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			CoinDeactivate(playerNum);
		}

		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			if (m_hotelSelect[playerNum]->GetNowSelectState() == 0)
			{
				return;
			}
		}

		//宿屋から出る
		EnterFromHotel();
		//泊まる人は全回復させる
		for (int playerNum = 0; playerNum < m_playerNum; playerNum++)
		{
			if (m_hotelSelect[playerNum]->GetNowSelectState() == 1)
			{
				//満タン回復する
				m_playerStatus[playerNum]->AddHitPoint(10);
			}
		}
	}

	//宿屋の内装モデルの初期化メソッド
	void HotelScene::HotelInteriorModelInit()
	{
		m_hotelInterior = NewGO<SkinModelRender>();
		m_hotelInterior->SetShadowCaster(false);	//シャドウは落とさない
		m_hotelInterior->SetOutline(false);		//輪郭線をつけない
		m_hotelInterior->Init("ItemShopInterior");
		//宿屋の位置を指定
		m_pos = { 0.0f, 10000.0f, 0.0f };
		m_hotelInterior->SetPosition(m_pos);	//宿屋をステージの上空においてステージが切り替わっているように見せる
		//当たり判定を適応
		m_physicsStaticObject.CreateFromModel(*m_hotelInterior->GetModel(), m_hotelInterior->GetModel()->GetWorldMatrix());
		//当たり判定を更新
		m_physicsStaticObject.Update(m_pos, m_rot);
	}

	//店員モデルの初期化メソッド
	void HotelScene::SalespersonModelInit()
	{
		m_salesperson = NewGO<SkinModelRender>();
		//モデルの上方向をZアップに指定
		m_salesperson->SetModelUpAxis(enModelUpAxisZ);
		m_salesperson->SetOutline(true);		//輪郭線をつける
		//アニメーションをロード
		m_animationClips[enIdle].Load("SalesPerson_Idle");
		m_animationClips[enGreething].Load("SalesPerson_Greething");		//お辞儀
		//ループアニメーションかどうかを設定
		m_animationClips[enIdle].SetLoopFlag(true);
		m_animationClips[enGreething].SetLoopFlag(false);
		m_salesperson->Init("SalesPerson", "SalesPerson", m_animationClips, enAnimationNum);
		m_salesperson->SetAnimationSpeed(1.5f);
		//店員のアニメーションの状態更新
		m_salesperson->PlayAnimation(enGreething);
		//位置をセット
		m_salesperson->SetPosition({ m_pos.x,m_pos.y,m_pos.z - 100.0f });
	}

	//宿屋から出るメソッド
	void HotelScene::EnterFromHotel()
	{
		//お辞儀アニメーション
		m_salesperson->PlayAnimation(enGreething);

		if (m_fade[1] == nullptr)
		{
			m_fade[1] = NewGO<Fade>();
			m_fade[1]->Init(0.0f,1,0);
		}
		if (m_fade[1]->GetNowState() == 2)
		{
			//プレイヤーの状態を通常状態にする
			m_player[enPlayer1]->SetNowState(0);
			//プレイヤーの位置を宿屋の前に移動させる
			m_player[enPlayer1]->Warp({ m_hotelPos.x - 50.0f,m_hotelPos.y,m_hotelPos.z + 500.0f });
			if (m_playerNum == 2)
			{
				m_player[enPlayer2]->SetNowState(0);
				m_player[enPlayer2]->Warp({ m_hotelPos.x + 50.0f,m_hotelPos.y,m_hotelPos.z + 500.0f });
			}

			if (m_playerNum == 1)
			{
				m_accompanyAI = FindGO<AccompanyAI>(nsStdafx::ACCOMPANYAI_NAME);
				m_accompanyAI->SetPosition({ m_hotelPos.x + 50.0f,m_hotelPos.y,m_hotelPos.z + 500.0f });
				m_accompanyAI->SetNowState(1);
			}

			//カメラの移動方法をセット
			m_playerCamera->SetCameraState(0);

			//ゲームBGMを再度再生
			m_gameScene->GameBGMPlay();

			//宿屋シーンを削除
			DeleteGO(this);

			//制限時間が経過するように戻す
			m_gameLimitTime->SetCanMeasure(true);
		}
	}

	void HotelScene::CoinDeactivate(const int playerNum)
	{
		if (m_hotelSelect[playerNum]->GetNowSelectState() != 0)
		{
			m_coinSprite[playerNum]->Deactivate();
		}
	}
}