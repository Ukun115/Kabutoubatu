/// <summary>
/// アイテムショップシーンクラス
/// </summary>
#include "stdafx.h"
#include "ItemShopScene.h"
#include "ItemShopSelect.h"
#include "../../Player/Player.h"
#include "../../Camera/PlayerCamera.h"
#include "../../AccompanyAI/AccompanyAI.h"
#include "../../GameScene.h"
#include "../../GameLimitTime.h"
#include "../../../Graphics/Fade.h"


namespace nsKabutoubatu
{
	namespace nsItemShopScene
	{
		Vector2   POS = { -160.0f,170.0f };			//制限時間フォントの位置
		const Vector4   FONT_COLOR = { 0.0f,0.0f,0.0f,1.0f };	//制限時間フォントの色
		const float     FONT_ROT = 0.0f;			//フォントの傾き
		const Vector2   FONT_PIV = { 1.0f,1.0f };	//フォントの基点
		const float     FONT_SCA = 0.75f;			//フォントの拡大率

		const char ITEM_FILEPATH[3][256] = { "Bow_Item","RecoveryItem","Shoes" };		//ファイルパス
	}

	bool ItemShopScene::Start()
	{
		//プレイヤーのインスタンスを検索
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			m_player[playerNum] = FindGO<Player>(nsStdafx::PLAYER_NAME[playerNum]);
		}
		//カメラのインスタンスを検索
		m_playerCamera = FindGO<PlayerCamera>(nsStdafx::PLAYER_CAMERA_NAME);

		m_fade[0] = NewGO<Fade>();
		m_fade[0]->Init(1.0f,0,0);

		m_gameLimitTime = FindGO<GameLimitTime>(nsStdafx::GAMELIMITTIME);
		m_gameLimitTime->SetCanMeasure(false);

		//アイテムショップの内装モデルを初期化
		ItemShopInteriorModelInit();

		//アイテムショップの商品モデル
		for (int shopItemNum = 0; shopItemNum < 3; shopItemNum++)
		{
			m_shopItem[shopItemNum] = NewGO<SkinModelRender>();
			m_shopItem[shopItemNum]->SetShadowCaster(false);	//シャドウは落とさない
			m_shopItem[shopItemNum]->SetOutline(true);		//輪郭線はつける
			m_shopItem[shopItemNum]->Init(nsItemShopScene::ITEM_FILEPATH[shopItemNum]);
			//位置を設定
			m_itemPos = m_pos;
			m_itemPos.y += 40.0f;
			m_itemPos.x -= 150.0f;
			m_itemPos.x += shopItemNum * 150.0f;
			m_shopItem[shopItemNum]->SetPosition(m_itemPos);
		}

		m_coinSpritePos = -295.0f;
		for (int i = 0; i < 3; i++)
		{
			//コイン画像を初期化
			m_coinSprite[i] = NewGO<SpriteRender>();
			m_coinSprite[i]->Init("Coin", 25, 25);
			m_coinSprite[i]->SetPosition({ m_coinSpritePos,-177.0f,0.0f });
			m_coinSpritePos += 270.0f;
		}

		m_hukidasi = NewGO<SpriteRender>(nsStdafx::PRIORITY_0);
		m_hukidasi->Init("Hukidasi", 450, 180);
		m_hukidasi->SetPosition({ 0.0f, 110.0f, 0.0f });

		//店員モデルを初期化
		SalespersonModelInit();

		//店員の会話文
		m_salespersonTalk = NewGO<FontRender>();
		m_salespersonTalk->Init
		(
			L"",							//テキスト
			nsItemShopScene::POS,			//位置
			nsItemShopScene::FONT_COLOR,	//色
			nsItemShopScene::FONT_ROT,		//傾き
			nsItemShopScene::FONT_SCA,		//拡大率
			nsItemShopScene::FONT_PIV		//基点
		);

		//プレイヤー２人分生成
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//何を選択しているかのクラス
			m_ItemShopSelect[playerNum] = NewGO<ItemShopSelect>();
			//プレイヤーの番号をセット
			m_ItemShopSelect[playerNum]->SetPlayerNum(playerNum);

			m_ItemShopSelect[playerNum]->SetPlayerGamePad(*m_playerGamePad[playerNum]);
		}

		return true;
	}

	ItemShopScene::~ItemShopScene()
	{
		//コイン画像を削除
		for (int i = 0; i < 3; i++)
		{
			DeleteGO(m_coinSprite[i]);
		}
		DeleteGO(m_hukidasi);
		//アイテムショップ内装モデルを削除
		DeleteGO(m_itemShopInterior);
		//店員モデルを削除
		DeleteGO(m_salesperson);
		//店員の会話削除
		DeleteGO(m_salespersonTalk);
		//プレイヤー２人分削除
		for (int playerNum = enPlayer1; playerNum < enPlayerNum; playerNum++)
		{
			DeleteGO(m_ItemShopSelect[playerNum]);
		}

		//ショップBGMを削除
		DeleteGO(m_shopBGM);

		DeleteGO(m_fade[1]);
	}

	void ItemShopScene::Update()
	{
		//お辞儀が終わったら、
		if (!m_salesperson->IsPlaying())
		{
			//アイドル状態にする
			m_salesperson->PlayAnimation(enIdle,1.0f);
		}

		if (m_fade[0] != nullptr && m_fade[0]->GetNowState() == 2)
		{
			DeleteGO(m_fade[0]);
			//ヌルポインタを入れておく。
			m_fade[0] = nullptr;
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
		swprintf_s(m_talkText, L"こんにちは!\n何が欲しいですか?");
		m_salespersonTalk->SetText(m_talkText);

		//プレイヤー２人分生成
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//二人ともお店を出る状態になっていたら、
			if (m_ItemShopSelect[enPlayer1]->GetNowShoppingState() == 3)
			{
				//ソロプレイのとき、
				if (m_ItemShopSelect[enPlayer2] == nullptr)
				{
					//アイテムショップから出る
					EnterFromShop();
				}
				//オンラインorマルチプレイの時、
				else if (m_ItemShopSelect[enPlayer2]->GetNowShoppingState() == 3)
				{
					//アイテムショップから出る
					EnterFromShop();
				}
			}
		}
	}

	//アイテムショップの内装モデルの初期化メソッド
	void ItemShopScene::ItemShopInteriorModelInit()
	{
		m_itemShopInterior = NewGO<SkinModelRender>();
		m_itemShopInterior->SetShadowCaster(false);	//シャドウは落とさない
		m_itemShopInterior->SetOutline(false);		//輪郭線をつけない
		m_itemShopInterior->Init("ItemShopInterior");
		//アイテムショップの位置を指定
		m_pos = { 0.0f, 10000.0f, 0.0f };
		m_itemShopInterior->SetPosition(m_pos);	//アイテムショップをステージの上空においてステージが切り替わっているように見せる
		//当たり判定を適応
		m_physicsStaticObject.CreateFromModel(*m_itemShopInterior->GetModel(), m_itemShopInterior->GetModel()->GetWorldMatrix());
		//当たり判定を更新
		m_physicsStaticObject.Update(m_pos, m_rot);
	}

	//店員モデルの初期化メソッド
	void ItemShopScene::SalespersonModelInit()
	{
		m_salesperson = NewGO<SkinModelRender>();
		//モデルの上方向をZアップに指定
		m_salesperson->SetModelUpAxis(enModelUpAxisZ);
		m_salesperson->SetOutline(true);		//輪郭線をつける
		//アニメーションをロード
		m_animationClips[enIdle].Load("SalesPerson_Idle");			//アイドル
		m_animationClips[enGreething].Load("SalesPerson_Greething");		//お辞儀
		//ループアニメーションかどうかを設定
		m_animationClips[enIdle].SetLoopFlag(true);
		m_animationClips[enGreething].SetLoopFlag(false);
		m_salesperson->Init("SalesPerson","SalesPerson", m_animationClips, enAnimationNum);
		m_salesperson->SetAnimationSpeed(1.5f);
		//アニメーションの状態更新
		m_salesperson->PlayAnimation(enGreething,1.0f);
		//位置をセット
		m_salesperson->SetPosition({ m_pos.x,m_pos.y,m_pos.z - 100.0f });
	}

	//アイテムショップから出るメソッド
	void ItemShopScene::EnterFromShop()
	{
		//お辞儀アニメーション
		m_salesperson->PlayAnimation(enGreething,1.0f);

		if (m_fade[1] == nullptr)
		{
			m_fade[1] = NewGO<Fade>();
			m_fade[1]->Init(0.0f,1,0);
		}
		if (m_fade[1]->GetNowState() == 2)
		{
			//プレイヤーの状態を通常状態にする
			m_player[enPlayer1]->SetNowState(0);
			//プレイヤーの位置をショップの前に移動させる
			m_player[enPlayer1]->Warp({ m_shopPos.x - 50.0f,m_shopPos.y,m_shopPos.z + 500.0f });
			if (m_playerNum == 1)
			{
				m_accompanyAI = FindGO<AccompanyAI>(nsStdafx::ACCOMPANYAI_NAME);
				m_accompanyAI->SetNowState(1);
				m_accompanyAI->SetPosition({ m_shopPos.x + 50.0f,m_shopPos.y,m_shopPos.z + 500.0f });
			}
			if (m_playerNum == 2)
			{
				m_player[enPlayer2]->SetNowState(0);
				m_player[enPlayer2]->Warp({ m_shopPos.x + 50.0f,m_shopPos.y,m_shopPos.z + 500.0f });
			}
			//カメラの移動方法をセット
			m_playerCamera->SetCameraState(0);

			//ゲームのBGMを再度再生
			m_gameScene->GameBGMPlay();

			//アイテムショップシーンを削除
			DeleteGO(this);

			//制限時間が経過するように戻す
			m_gameLimitTime->SetCanMeasure(true);
		}
	}
}