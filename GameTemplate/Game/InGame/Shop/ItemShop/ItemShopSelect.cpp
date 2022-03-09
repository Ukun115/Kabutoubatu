/// <summary>
/// アイテムショップシーンの選択クラス
/// </summary>
#include "stdafx.h"
#include "ItemShopSelect.h"
#include "../../Player/Player.h"
#include "../../Player/PlayerStatus.h"
#include "ItemShopSelectFont.h"
#include "../../Item/Weapon/Weapon.h"
#include "../../GameScreenUI.h"

namespace nsKabutoubatu
{
	bool ItemShopSelect::Start()
	{
		//プレイヤーのインスタンスを検索
		m_player = FindGO<Player>(nsStdafx::PLAYER_NAME[m_playerNum]);
		m_playerStatus = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[m_playerNum]);
		m_weapon = FindGO<Weapon>(nsStdafx::WEAPON_NAME[m_playerNum]);
		m_gameScreenUi = FindGO<GameScreenUI>(nsStdafx::GAMESCREENUI_NAME);

		//選択を文字表示させるクラスを生成
		m_itemShopSelectFont = NewGO<ItemShopSelectFont>();
		//プレイヤー番号をセット
		m_itemShopSelectFont->SetPlayerNum(m_playerNum);
		if (m_playerGamePad != nullptr)
		{
			m_itemShopSelectFont->SetPlayerGamePad(*m_playerGamePad);
		}

		//PushXアイテム説明ボタン画像を初期化
		m_pushXSprite = NewGO<SpriteRender>();
		m_pushXSprite->Init("PushX",300,210);

		//アイテム説明背景画像
		m_ItemSetumeiSprite = NewGO<SpriteRender>();
		m_ItemSetumeiSprite->Init("ItemSetumeiBack", 400, 500);
		m_ItemSetumeiSprite->Deactivate();	//非表示

		switch (m_playerNum)
		{
		case 0:
			m_pushXSprite->SetPosition({-500.0f,-190.0f,0.0f});
			m_ItemSetumeiSprite->SetPosition({-450.0f,0.0f,0.0f});
			break;
		case 1:
			m_pushXSprite->SetPosition({500.0f,-190.0f,0.0f});
			m_ItemSetumeiSprite->SetPosition({450.0f,0.0f,0.0f});
			break;
		}

		return true;
	}

	ItemShopSelect::~ItemShopSelect()
	{
		//アイテムショップ選択文字クラスを削除
		DeleteGO(m_itemShopSelectFont);
		//PushXアイテム説明ボタン画像を削除
		DeleteGO(m_pushXSprite);
		//アイテム説明背景画像
		DeleteGO(m_ItemSetumeiSprite);
	}

	void ItemShopSelect::Update()
	{
		switch (m_nowShoppingState)
		{
			//買う、売る、出る、の選択状態
		case enShoppingModeSelectState:

			//選択カーソル移動
			CursorMove(enBuy, enEnter);

			//Aボタンが押されたら、
			if (m_playerGamePad != nullptr)
			{
				if (m_playerGamePad->IsTrigger(enButtonA) && !m_ItemSetumeiSprite->IsActive())
				{
					//購入モード、売却モード、店から出るスタンバイ状態のいずれかに行く。
					NextState();
					//初期値に戻しておく
					m_nowShoppingMode = 0;

					//決定サウンドの初期化
					m_decideSound = NewGO<SoundSource>();
					m_decideSound->Init(L"Assets/sound/Decide.wav");
					m_decideSound->SetVolume(0.5f);
					m_decideSound->Play(false);	//ワンショット再生
				}
			}
			else
			{
				if (g_pad[m_playerNum]->IsTrigger(enButtonA) && !m_ItemSetumeiSprite->IsActive())
				{
					//購入モード、売却モード、店から出るスタンバイ状態のいずれかに行く。
					NextState();
					//初期値に戻しておく
					m_nowShoppingMode = 0;

					//決定サウンドの初期化
					m_decideSound = NewGO<SoundSource>();
					m_decideSound->Init(L"Assets/sound/Decide.wav");
					m_decideSound->SetVolume(0.5f);
					m_decideSound->Play(false);	//ワンショット再生
				}
			}

			break;

			//購入状態
		case enBuyState:

			//選択カーソル移動
			CursorMove(enFirstItem, enBackState);

			if (m_playerGamePad != nullptr)
			{
				//Aボタンが押されたら、
				if (m_playerGamePad->IsTrigger(enButtonA) && !m_ItemSetumeiSprite->IsActive())
				{
					//購入
					BuySelect();
				}
				//もしXボタンが押されたなら、
				if (m_playerGamePad->IsTrigger(enButtonX) && m_nowShoppingMode != enBackState)
				{
					//表示されているとき、
					if (m_ItemSetumeiSprite->IsActive())
					{
						//非表示にさせる
						m_ItemSetumeiSprite->Deactivate();
					}
					//表示されていないとき、
					else
					{
						//表示させる
						m_ItemSetumeiSprite->Activate();
					}
				}
			}
			else
			{
				//Aボタンが押されたら、
				if (g_pad[m_playerNum]->IsTrigger(enButtonA) && !m_ItemSetumeiSprite->IsActive())
				{
					//購入
					BuySelect();
				}
				//もしXボタンが押されたなら、
				if (g_pad[m_playerNum]->IsTrigger(enButtonX) && m_nowShoppingMode != enBackState)
				{
					//表示されているとき、
					if (m_ItemSetumeiSprite->IsActive())
					{
						//非表示にさせる
						m_ItemSetumeiSprite->Deactivate();
					}
					//表示されていないとき、
					else
					{
						//表示させる
						m_ItemSetumeiSprite->Activate();
					}
				}
			}

			break;

			//売却状態
		case enSoldState:
			//選択カーソル移動
			CursorMove(enStatusUpItem1, enBack);

			if (m_playerGamePad != nullptr)
			{
				//Aボタンが押されたら
				if (m_playerGamePad->IsTrigger(enButtonA) && !m_ItemSetumeiSprite->IsActive())
				{
					//売却
					SoldSelect();
				}
				//もしXボタンが押されたなら、
				if (m_playerGamePad->IsTrigger(enButtonX) && m_nowShoppingMode != enBack)
				{
					//表示されているとき、
					if (m_ItemSetumeiSprite->IsActive())
					{
						//非表示にさせる
						m_ItemSetumeiSprite->Deactivate();
					}
					//表示されていないとき、
					else
					{
						//表示させる
						m_ItemSetumeiSprite->Activate();
					}
				}
			}
			else
			{
				//Aボタンが押されたら
				if (g_pad[m_playerNum]->IsTrigger(enButtonA) && !m_ItemSetumeiSprite->IsActive())
				{
					//売却
					SoldSelect();
				}
				//もしXボタンが押されたなら、
				if (g_pad[m_playerNum]->IsTrigger(enButtonX) && m_nowShoppingMode != enBack)
				{
					//表示されているとき、
					if (m_ItemSetumeiSprite->IsActive())
					{
						//非表示にさせる
						m_ItemSetumeiSprite->Deactivate();
					}
					//表示されていないとき、
					else
					{
						//表示させる
						m_ItemSetumeiSprite->Activate();
					}
				}
			}

			break;

			//店から出るスタンバイ状態
		case enEnterState:
			//何も操作できない
			break;
		}
		//アイテムショップセレクト文字クラスのショッピング状態を更新
		m_itemShopSelectFont->SetNowShoppingState(m_nowShoppingState);
		m_itemShopSelectFont->SetNowShoppingMode(m_nowShoppingMode);
	}

	//選択メソッド
	void ItemShopSelect::CursorMove(const int selectMin, const int selectMax)
	{
		//下入力
		if (m_playerGamePad != nullptr)
		{
			if (m_playerGamePad->IsTrigger(enButtonDown) && !m_ItemSetumeiSprite->IsActive()) {
				//現在セレクトされているのが「出る」だったら、
				if (m_nowShoppingMode == selectMax) {
					//選択を一番上に戻す
					m_nowShoppingMode = selectMin;
				}
				else
				{
					//一つ下にずらす
					m_nowShoppingMode += 1;
				}

				//選択サウンドの初期化
				m_selectSound = NewGO<SoundSource>();
				m_selectSound->Init(L"Assets/sound/Select.wav");
				m_selectSound->SetVolume(0.5f);
				m_selectSound->Play(false);	//ワンショット再生
			}
			//上入力
			if (m_playerGamePad->IsTrigger(enButtonUp) && !m_ItemSetumeiSprite->IsActive()) {
				//現在セレクトされているのが「出る」だったら、
				if (m_nowShoppingMode == selectMin) {
					//選択を一番下に戻す
					m_nowShoppingMode = selectMax;
				}
				else
				{
					//一つ上にずらす
					m_nowShoppingMode -= 1;
				}

				//選択サウンドの初期化
				m_selectSound = NewGO<SoundSource>();
				m_selectSound->Init(L"Assets/sound/Select.wav");
				m_selectSound->SetVolume(0.5f);
				m_selectSound->Play(false);	//ワンショット再生
			}
		}
		else
		{
			if (g_pad[m_playerNum]->IsTrigger(enButtonDown) && !m_ItemSetumeiSprite->IsActive()) {
				//現在セレクトされているのが「出る」だったら、
				if (m_nowShoppingMode == selectMax) {
					//選択を一番上に戻す
					m_nowShoppingMode = selectMin;
				}
				else
				{
					//一つ下にずらす
					m_nowShoppingMode += 1;
				}

				//選択サウンドの初期化
				m_selectSound = NewGO<SoundSource>();
				m_selectSound->Init(L"Assets/sound/Select.wav");
				m_selectSound->SetVolume(0.5f);
				m_selectSound->Play(false);	//ワンショット再生
			}
			//上入力
			if (g_pad[m_playerNum]->IsTrigger(enButtonUp) && !m_ItemSetumeiSprite->IsActive()) {
				//現在セレクトされているのが「出る」だったら、
				if (m_nowShoppingMode == selectMin) {
					//選択を一番下に戻す
					m_nowShoppingMode = selectMax;
				}
				else
				{
					//一つ上にずらす
					m_nowShoppingMode -= 1;
				}

				//選択サウンドの初期化
				m_selectSound = NewGO<SoundSource>();
				m_selectSound->Init(L"Assets/sound/Select.wav");
				m_selectSound->SetVolume(0.5f);
				m_selectSound->Play(false);	//ワンショット再生
			}
		}
	}

	//次の状態に行くメソッド
	void ItemShopSelect::NextState()
	{
		switch (m_nowShoppingMode)
		{
		case enBuy:
			//購入モードにする
			m_nowShoppingState = enBuyState;
			break;
		case enSold:
			//売却モードにする
			m_nowShoppingState = enSoldState;
			break;
		case enEnter:
			//お店を出るスタンバイモードにする
			m_nowShoppingState = enEnterState;
			break;
		}
	}

	//購入するときの選択
	void ItemShopSelect::BuySelect()
	{
		switch (m_nowShoppingMode)
		{
			//１つ目のアイテムを選択していたら、
		case enFirstItem:
			//お金が必要分ないとき、
			if (m_playerStatus->GetCoinNum() < 100)
			{
				//ぶぶーサウンドの初期化
				m_cantDecideSound = NewGO<SoundSource>();
				m_cantDecideSound->Init(L"Assets/sound/bubu.wav");
				m_cantDecideSound->SetVolume(0.5f);
				m_cantDecideSound->Play(false);	//ワンショット再生

				return;
			}
			if (m_playerStatus->GetMovePower() >= 90)
			{
				//ぶぶーサウンドの初期化
				m_cantDecideSound = NewGO<SoundSource>();
				m_cantDecideSound->Init(L"Assets/sound/bubu.wav");
				m_cantDecideSound->SetVolume(0.5f);
				m_cantDecideSound->Play(false);	//ワンショット再生

				return;
			}

			//移動速度をアップさせる
			m_playerStatus->AddMovePower();
			//お金を減らす
			m_playerStatus->ReduceMoney(100);

			//アイテム所持数を増やす
			m_gameScreenUi->AddItemNum(m_playerNum,0);

			//購入サウンドの初期化
			m_buySound = NewGO<SoundSource>();
			m_buySound->Init(L"Assets/sound/buy.wav");
			m_buySound->SetVolume(0.5f);
			m_buySound->Play(false);	//ワンショット再生

			break;

			//２つ目のアイテムを選択していたら、
		case enSecondItem:
			//お金が必要分ないとき、
			if (m_playerStatus->GetCoinNum() < 200)
			{
				//ぶぶーサウンドの初期化
				m_cantDecideSound = NewGO<SoundSource>();
				m_cantDecideSound->Init(L"Assets/sound/bubu.wav");
				m_cantDecideSound->SetVolume(0.5f);
				m_cantDecideSound->Play(false);	//ワンショット再生

				return;
			}
			//回復アイテムの数を１増やす
			m_playerStatus->AddRecoveryItemNum();
			//お金を減らす
			m_playerStatus->ReduceMoney(200);

			//購入サウンドの初期化
			m_buySound = NewGO<SoundSource>();
			m_buySound->Init(L"Assets/sound/buy.wav");
			m_buySound->SetVolume(0.5f);
			m_buySound->Play(false);	//ワンショット再生

			break;

			//３つ目のアイテムを選択していたら、
		case enThirdItem:
			//お金が必要分ないとき、
			if (m_playerStatus->GetCoinNum() < 500)
			{
				//ぶぶーサウンドの初期化
				m_cantDecideSound = NewGO<SoundSource>();
				m_cantDecideSound->Init(L"Assets/sound/bubu.wav");
				m_cantDecideSound->SetVolume(0.5f);
				m_cantDecideSound->Play(false);	//ワンショット再生

				return;
			}
			if (m_playerStatus->GetHaveWeapon() ==3)
			{
				//ぶぶーサウンドの初期化
				m_cantDecideSound = NewGO<SoundSource>();
				m_cantDecideSound->Init(L"Assets/sound/bubu.wav");
				m_cantDecideSound->SetVolume(0.5f);
				m_cantDecideSound->Play(false);	//ワンショット再生

				return;
			}
			//剣を購入
			m_playerStatus->ChangeHavingWeapon(3);
			//プレイヤーの持っている武器のモデルを変更
			m_weapon->Init(3);
			//お金を減らす
			m_playerStatus->ReduceMoney(500);

			//購入サウンドの初期化
			m_buySound = NewGO<SoundSource>();
			m_buySound->Init(L"Assets/sound/buy.wav");
			m_buySound->SetVolume(0.5f);
			m_buySound->Play(false);	//ワンショット再生

			break;

			//戻る
		case enBackState:
			//買う、売る、出る、の選択状態に戻す
			m_nowShoppingState = enShoppingModeSelectState;
			//初期値に戻しておく
			m_nowShoppingMode = 0;

			//決定サウンドの初期化
			m_decideSound = NewGO<SoundSource>();
			m_decideSound->Init(L"Assets/sound/Decide.wav");
			m_decideSound->SetVolume(0.5f);
			m_decideSound->Play(false);	//ワンショット再生

			break;
		}
	}

	//売却するときの選択
	void ItemShopSelect::SoldSelect()
	{
		//「戻る」が選択されていたら、
		if (m_nowShoppingMode == enBack)
		{
			//買う、売る、出る、の選択状態に戻す
			m_nowShoppingState = enShoppingModeSelectState;
			//初期値に戻しておく
			m_nowShoppingMode = 0;

			//決定サウンドの初期化
			m_decideSound = NewGO<SoundSource>();
			m_decideSound->Init(L"Assets/sound/Decide.wav");
			m_decideSound->SetVolume(0.5f);
			m_decideSound->Play(false);	//ワンショット再生

			return;
		}

		//数が０の時、売却をしない。
		if (m_gameScreenUi->GetStatusUpItemNum(m_playerNum, m_nowShoppingMode) <= 0)
		{
			//ぶぶーサウンドの初期化
			m_cantDecideSound = NewGO<SoundSource>();
			m_cantDecideSound->Init(L"Assets/sound/bubu.wav");
			m_cantDecideSound->SetVolume(0.5f);
			m_cantDecideSound->Play(false);	//ワンショット再生

			return;
		}

		//購入サウンドの初期化
		m_buySound = NewGO<SoundSource>();
		m_buySound->Init(L"Assets/sound/buy.wav");
		m_buySound->SetVolume(0.5f);
		m_buySound->Play(false);	//ワンショット再生

		//能力アップしたものを元に戻す
		switch (m_nowShoppingMode)
		{
		case enStatusUpItem1:
			//プレイヤーの移動力を下げる
			m_playerStatus->ReduceMovePower();
			break;

		case enStatusUpItem2:
			//プレイヤーのコインを増やす倍率を減らす
			m_playerStatus->ReduceMoneyMagnification();
			break;

		case enStatusUpItem3:
			//プレイヤーのジャンプ力を下げる
			m_playerStatus->ReduceJumpPower();
			break;

		case enStatusUpItem4:
			//回復アイテムの数を減らす
			m_playerStatus->ReduceRecoveryItemNum();
			break;

		case enStatusUpItem5:
			//回復アイテムの回復力を下げる
			m_playerStatus->ReduceRecoveryPower();
			break;
		}

		//数を１減らす
		m_gameScreenUi->ReduceStatusUpItemNum(m_playerNum, m_nowShoppingMode);
		//売却の値段がもらえる
		m_playerStatus->AddMoney(100);

	}
}


//商品一覧//

//俊足シューズ
//m_player->AddMovePower();					//移動速度アップ

//金運小判
//m_player->AddMoneyMagnification();		//取得マネーの取得倍率アップ

//天使の羽
//m_player->AddJumpPower();					//ジャンプ力アップ

//回復箱
//m_player->AddRecoveryItemNum();			//回復アイテム+1コ

//ハートクッキー
//m_player->AddRecoveryPower();				//回復力を+1

//武器名
//m_player->ChangeHavingWeapon(武器番号);	//武器購入(すでに持っている武器と入れ替え)