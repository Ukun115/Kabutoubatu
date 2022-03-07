/// <summary>
/// ゲームの制限時間クラス
/// </summary>
#include "stdafx.h"
#include "GameLimitTime.h"
#include "../Graphics/Fade.h"

namespace nsKabutoubatu
{
	namespace nsGameLimitTime
	{
		const char* TIME_NEEDLE_NAME[2] = { "TimeNeedle_Long","TimeNeedle_Short" };

		//ゲームの制限時間(分)
		const int GAMETIME = 4;

		//360度÷秒÷フレーム = 長針が1フレームに移動する角度
		//360 / 60 / 60 = 0.1度
		const float LONG_NEEDLE_ANGLE = 0.1f;

		const Vector4   FONT_COLOR = { 1.0f,1.0f,1.0f,1.0f };	//制限時間フォントの色
		const float     FONT_ROT = 0.0f;			//フォントの傾き
		const Vector2   FONT_PIV = { 1.0f,1.0f };	//フォントの基点

		const int SECONDS = (60 - 1);	//秒
		const int GAME_TIME_LIMIT = (4 - 1);	//ゲームのマックス制限時間（分）

		//制限時間の位置
		const Vector2 LIMIT_TIME_POS[3] = { { -480.0f,210.0f }, {-560.0f,210.0f},{-510.0f,215.0f} };
		const float LIMIT_TIME_SCA = 1.0f;
	}

	bool GameLimitTime::Start()
	{
		//制限時間の背景
		m_timeBackSprite = NewGO<SpriteRender>(nsStdafx::PRIORITY_0);
		m_timeBackSprite->Init("TimeBack", 250, 125);
		m_timeBackSprite->SetPosition({ -500.0f,180.0f,0.0f });
		//制限時間の時計の針(短針と長針)
		for (int timeNeedleNum = enTimeNeedleLong; timeNeedleNum < enTimeNeedleNum; timeNeedleNum++)
		{
			m_timeNeedleSprite[timeNeedleNum] = NewGO<SpriteRender>(nsStdafx::PRIORITY_1);
			m_timeNeedleSprite[timeNeedleNum]->Init(nsGameLimitTime::TIME_NEEDLE_NAME[timeNeedleNum], 68, 34);
			m_timeNeedleSprite[timeNeedleNum]->SetPosition({ -583.5f,180.2f,0.0f });
			m_timeNeedleSprite[timeNeedleNum]->SetPivot({ 0.5f,0.3f });
		}

		//ゲームの制限時間を設定
		m_nowLimitTime[enMinutes] = nsGameLimitTime::GAME_TIME_LIMIT + 1;
		//最初0秒
		m_measureTime = pow(nsGameLimitTime::SECONDS, 2);

		//制限時間のコロン「：」
		m_timeColonFont = NewGO<FontRender>();
		m_timeColonFont->Init
		(
			L":",			//テキスト
			nsGameLimitTime::LIMIT_TIME_POS[2],	//位置
			nsGameLimitTime::FONT_COLOR,	//色
			nsGameLimitTime::FONT_ROT,		//傾き
			nsGameLimitTime::LIMIT_TIME_SCA,		//拡大率
			nsGameLimitTime::FONT_PIV		//基点
		);

		//制限時間フォント(分と秒)
		for (int limitTimeNum = enSeconds; limitTimeNum < enSecondsMinutesNum; limitTimeNum++)
		{
			m_limitTimeFont[limitTimeNum] = NewGO<FontRender>();
			m_limitTimeFont[limitTimeNum]->Init
			(
				L"",			//テキスト
				nsGameLimitTime::LIMIT_TIME_POS[limitTimeNum],	//位置
				nsGameLimitTime::FONT_COLOR,	//色
				nsGameLimitTime::FONT_ROT,		//傾き
				nsGameLimitTime::LIMIT_TIME_SCA,		//拡大率
				nsGameLimitTime::FONT_PIV		//基点
			);
			//秒・分テキスト更新
			swprintf_s(m_limitTimeText, L"%d", m_nowLimitTime[limitTimeNum]);
			m_limitTimeFont[limitTimeNum]->SetText(m_limitTimeText);
		}

		return true;
	}

	GameLimitTime::~GameLimitTime()
	{
		//制限時間のコロン「：」
		DeleteGO(m_timeColonFont);
		//制限時間を削除
		for (int limitTimeNum = enSeconds; limitTimeNum < enSecondsMinutesNum; limitTimeNum++)
		{
			DeleteGO(m_limitTimeFont[limitTimeNum]);
		}

		//制限時間の背景を削除
		DeleteGO(m_timeBackSprite);
		//制限時間の時計の針を削除
		for (int timeNeedleNum = enTimeNeedleLong; timeNeedleNum < enTimeNeedleNum; timeNeedleNum++)
		{
			DeleteGO(m_timeNeedleSprite[timeNeedleNum]);
		}
	}

	void GameLimitTime::Update()
	{
		if (m_fade != nullptr && m_fade->GetNowState() == 2)
		{
			DeleteGO(m_fade);
			m_fade = nullptr;
		}

		//制限時間(秒)を経過していく
		if (m_canMeasure)
		{
			m_measureTime++;
		}
		m_nowLimitTime[enSeconds] = nsGameLimitTime::SECONDS - (m_measureTime / nsGameLimitTime::SECONDS);
		//秒が０になったら、
		if (m_nowLimitTime[enSeconds] == -1)
		{
			//計測しているタイムの初期化
			m_measureTime = 0;
			//残り時間（分）を減らす
			m_nowLimitTime[enMinutes]--;

			m_minuteChangeFlg = true;
		}
		//秒・分テキスト更新
		for (int limitTimeNum = enSeconds; limitTimeNum < enSecondsMinutesNum; limitTimeNum++)
		{
			//負の数にならないように補正
			m_nowLimitTime[limitTimeNum]  = max(0,m_nowLimitTime[limitTimeNum]);

			if (limitTimeNum == enSeconds && m_nowLimitTime[enSeconds] < 10)
			{
			swprintf_s(m_limitTimeText, L"0%d", m_nowLimitTime[limitTimeNum]);
			}
			else
			{
				swprintf_s(m_limitTimeText, L"%d", m_nowLimitTime[limitTimeNum]);
			}
			m_limitTimeFont[limitTimeNum]->SetText(m_limitTimeText);
		}

		//１フレーム針を回転角度を代入
		m_timeNeedleRotAngle[enTimeNeedleLong] -= nsGameLimitTime::LONG_NEEDLE_ANGLE;
		m_timeNeedleRotAngle[enTimeNeedleShort] -= double((nsGameLimitTime::LONG_NEEDLE_ANGLE) / (nsGameLimitTime::GAMETIME));

		//double型とか使ってしても角度は正確な値は出ないため、制限時間の分が変化するごとに短針長針を所定の位置に移動し補正をするようにする
		if(m_minuteChangeFlg&& m_nowLimitTime[enMinutes] != nsGameLimitTime::GAME_TIME_LIMIT)
		{
			//長針を0度の位置に移動
			m_timeNeedleRotAngle[enTimeNeedleLong] = 0.0f;
			//短針の角度指定
			m_timeNeedleRotAngle[enTimeNeedleShort] = (360/ nsGameLimitTime::GAMETIME)* (m_nowLimitTime[enMinutes]+1);

			m_minuteChangeFlg = false;
		}

		//時計の針の回転を更新
		for (int timeNeedleNum = enTimeNeedleLong; timeNeedleNum < enTimeNeedleNum; timeNeedleNum++)
		{
			//デグリー単位での回転
			m_timeNeedleRot[timeNeedleNum].SetRotationDeg(Vector3::AxisZ, m_timeNeedleRotAngle[timeNeedleNum]);
			m_timeNeedleSprite[timeNeedleNum]->SetRotation(m_timeNeedleRot[timeNeedleNum]);
		}

	}

	//制限時間が無くなったかどうかを取得するメソッド
	bool GameLimitTime::TimeOver()
	{
		if (m_nowLimitTime[enSeconds] == 0 && m_nowLimitTime[enMinutes] == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void GameLimitTime::SetCanMeasure(const bool canMeasure)
	{
		m_canMeasure = canMeasure;

		m_fade = NewGO<Fade>();
		m_fade->Init(1.0f,0,0);
	}
}