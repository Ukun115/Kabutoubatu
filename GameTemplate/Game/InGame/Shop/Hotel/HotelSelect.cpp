/// <summary>
/// 宿屋シーンの選択クラス
/// </summary>
#include "stdafx.h"
#include "HotelSelect.h"
#include "../../Player/PlayerStatus.h"
#include "HotelSelectFont.h"

namespace nsKabutoubatu
{
	namespace nsHotelSelect
	{
		//宿泊代
		const int STAY_MONEY = 1000;
	}

	bool HotelSelect::Start()
	{
		//プレイヤーのインスタンスを検索
		m_playerStatus = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[m_playerNum]);

		//選択を文字表示させるクラスを生成
		m_hotelSelectFont = NewGO<HotelSelectFont>();
		//プレイヤー番号をセット
		m_hotelSelectFont->SetPlayerNum(m_playerNum);

		return true;
	}

	HotelSelect::~HotelSelect()
	{
		//ホテル選択文字クラスを削除
		DeleteGO(m_hotelSelectFont);
	}

	void HotelSelect::Update()
	{
		switch (m_nowHotelState)
		{
			//泊まる泊まらないの選択状態
		case enSelectState:

			//選択カーソル移動
			CursorMove(enStay, enEnter);

			//Aボタンが押されたら、
			if (m_playerGamePad != nullptr)
			{
				if (m_playerGamePad->IsTrigger(enButtonA))
				{
					//泊まる状態、店から出るスタンバイ状態のいずれかに行く。
					NextState();
					//初期値に戻しておく
					m_nowHotelMode = 0;
				}
			}
			else
			{
				if (g_pad[m_playerNum]->IsTrigger(enButtonA))
				{
					//泊まる状態、店から出るスタンバイ状態のいずれかに行く。
					NextState();
					//初期値に戻しておく
					m_nowHotelMode = 0;
				}
			}

			break;

			//泊まる状態
		case enStayState:
			//何も操作できない
			break;

			//店から出るスタンバイ状態
		case enEnterState:
			//何も操作できない
			break;
		}
		//ホテルセレクト文字クラスの状態を更新
		m_hotelSelectFont->SetNowHotelState(m_nowHotelState);
		m_hotelSelectFont->SetNowHotelMode(m_nowHotelMode);
	}

	//選択メソッド
	void HotelSelect::CursorMove(const int selectMin, const int selectMax)
	{
		//下入力
		if (m_playerGamePad != nullptr)
		{
			if (m_playerGamePad->IsTrigger(enButtonDown)) {
				//現在セレクトされているのが「出る」だったら、
				if (m_nowHotelMode == selectMax) {
					//選択を一番上に戻す
					m_nowHotelMode = selectMin;
				}
				else
				{
					//一つ下にずらす
					m_nowHotelMode += 1;
				}

				//選択サウンドの初期化
				m_selectSound = NewGO<SoundSource>();
				m_selectSound->Init(L"Assets/sound/Select.wav");
				m_selectSound->SetVolume(0.5f);
				m_selectSound->Play(false);	//ワンショット再生
			}
			//上入力
			if (m_playerGamePad->IsTrigger(enButtonUp)) {
				//現在セレクトされているのが「出る」だったら、
				if (m_nowHotelMode == selectMin) {
					//選択を一番下に戻す
					m_nowHotelMode = selectMax;
				}
				else
				{
					//一つ上にずらす
					m_nowHotelMode -= 1;
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
			if (g_pad[m_playerNum]->IsTrigger(enButtonDown)) {
				//現在セレクトされているのが「出る」だったら、
				if (m_nowHotelMode == selectMax) {
					//選択を一番上に戻す
					m_nowHotelMode = selectMin;
				}
				else
				{
					//一つ下にずらす
					m_nowHotelMode += 1;
				}

				//選択サウンドの初期化
				m_selectSound = NewGO<SoundSource>();
				m_selectSound->Init(L"Assets/sound/Select.wav");
				m_selectSound->SetVolume(0.5f);
				m_selectSound->Play(false);	//ワンショット再生
			}
			//上入力
			if (g_pad[m_playerNum]->IsTrigger(enButtonUp)) {
				//現在セレクトされているのが「出る」だったら、
				if (m_nowHotelMode == selectMin) {
					//選択を一番下に戻す
					m_nowHotelMode = selectMax;
				}
				else
				{
					//一つ上にずらす
					m_nowHotelMode -= 1;
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
	void HotelSelect::NextState()
	{
		switch (m_nowHotelMode)
		{
		case enStay:
			//お金が必要分ないとき、泊まるモードにならない。
			if (m_playerStatus->GetCoinNum() < nsHotelSelect::STAY_MONEY)
			{
				//ぶぶーサウンドの初期化
				m_cantDecideSound = NewGO<SoundSource>();
				m_cantDecideSound->Init(L"Assets/sound/bubu.wav");
				m_cantDecideSound->SetVolume(0.5f);
				m_cantDecideSound->Play(false);	//ワンショット再生

				return;
			}
			//ヒットポイントが満タンの時、泊まるモードにならない。
			if (m_playerStatus->GetHitPoint() == m_playerStatus->GetMaxHitPoint())
			{
				//ぶぶーサウンドの初期化
				m_cantDecideSound = NewGO<SoundSource>();
				m_cantDecideSound->Init(L"Assets/sound/bubu.wav");
				m_cantDecideSound->SetVolume(0.5f);
				m_cantDecideSound->Play(false);	//ワンショット再生

				return;
			}
			//泊まるモードにする
			m_nowHotelState = enStayState;
			//宿泊分のお金を使う
			m_playerStatus->ReduceMoney(nsHotelSelect::STAY_MONEY);

			//購入サウンドの初期化
			m_buySound = NewGO<SoundSource>();
			m_buySound->Init(L"Assets/sound/buy.wav");
			m_buySound->SetVolume(0.5f);
			m_buySound->Play(false);	//ワンショット再生

			break;

		case enEnter:
			//お店を出るスタンバイモードにする
			m_nowHotelState = enEnterState;

			//決定サウンドの初期化
			m_decideSound = NewGO<SoundSource>();
			m_decideSound->Init(L"Assets/sound/Decide.wav");
			m_decideSound->SetVolume(0.5f);
			m_decideSound->Play(false);	//ワンショット再生

			break;
		}
	}
}