/// <summary>
/// タイトルシーンの画像クラス
/// </summary>
#include "stdafx.h"
#include "TitleSceneSprite.h"
#include "TitleScene.h"

namespace nsKabutoubatu
{
	namespace nsTitleSceneSprite
	{
		const char* WAITJOINPLAYER_POINT_NAME[3] = { "WaitJoinPlayer_Point1","WaitJoinPlayer_Point2","WaitJoinPlayer_Point3" };
		const char* SEARCHJOINROOM_POINT_NAME[3] = { "SearchJoinRoom_Point1","SearchJoinRoom_Point2","SearchJoinRoom_Point3" };
		const float SPRITE_WIDTH = 1300.0f;
		const float SPRITE_HEIGHT = 720.0f;
		const int TIMER_RESET_NO = 0;
	}

	bool TitleSceneSprite::Start()
	{
		//タイトルシーンクラスのインスタンスを検索
		m_titleScene = FindGO<TitleScene>(nsStdafx::TITLESCENE_NAME);

		//背景画像を作成
		m_backSprite = NewGO<SpriteRender>(nsStdafx::PRIORITY_0);
		m_backSprite->Init("TitleSceneBack", nsTitleSceneSprite::SPRITE_WIDTH, nsTitleSceneSprite::SPRITE_HEIGHT);
		//カーソル画像を作成
		m_cursorSprite = NewGO<SpriteRender>(nsStdafx::PRIORITY_1);
		m_cursorSprite->Init("TitleScene_PlayModeSelectCursor", nsTitleSceneSprite::SPRITE_WIDTH, nsTitleSceneSprite::SPRITE_HEIGHT);

		//１つ目の選択画像を作成
		m_firstSelectSprite = NewGO<SpriteRender>(nsStdafx::PRIORITY_1);
		m_firstSelectSprite->Init("TitleScene_StartButton", nsTitleSceneSprite::SPRITE_WIDTH, nsTitleSceneSprite::SPRITE_HEIGHT);
		//２つ目の選択画像を作成
		m_SecondSelectSprite = NewGO<SpriteRender>(nsStdafx::PRIORITY_1);
		m_SecondSelectSprite->Init("TitleScene_PlayModeSelect", nsTitleSceneSprite::SPRITE_WIDTH, nsTitleSceneSprite::SPRITE_HEIGHT);
		m_SecondSelectSprite->Deactivate();	//初め非表示
		//３つ目の選択画像を作成
		m_thirdSelectSprite = NewGO<SpriteRender>(nsStdafx::PRIORITY_1);
		m_thirdSelectSprite->Init("TitleScene_OnlineRoomCreateOrSearch", nsTitleSceneSprite::SPRITE_WIDTH, nsTitleSceneSprite::SPRITE_HEIGHT);
		m_thirdSelectSprite->Deactivate();	//初め非表示
		//４つ目の選択画像を作成
		m_fourthSelectSprite = NewGO<SpriteRender>(nsStdafx::PRIORITY_1);
		m_fourthSelectSprite->Init("TitleScene_OnlineMatchWaiting", nsTitleSceneSprite::SPRITE_WIDTH, nsTitleSceneSprite::SPRITE_HEIGHT);
		m_fourthSelectSprite->Deactivate();	//初め非表示

		//参加プレイヤーを待っています・・・画像を作成
		m_waitJoinPlayerSprite = NewGO<SpriteRender>(nsStdafx::PRIORITY_1);
		m_waitJoinPlayerSprite->Init("WaitJoinPlayer", nsTitleSceneSprite::SPRITE_WIDTH, nsTitleSceneSprite::SPRITE_HEIGHT);
		m_waitJoinPlayerSprite->Deactivate();	//初め非表示
		//参加できる部屋をさがしています・・・画像を作成
		m_searchJoinRoomSprite = NewGO<SpriteRender>(nsStdafx::PRIORITY_1);
		m_searchJoinRoomSprite->Init("SearchJoinRoom", nsTitleSceneSprite::SPRITE_WIDTH, nsTitleSceneSprite::SPRITE_HEIGHT);
		m_searchJoinRoomSprite->Deactivate();	//初め非表示

		//「・・・」画像を作成
		for (int pointNum = enPointOne; pointNum < enPointNum; pointNum++)
		{
			m_waitJoinPlayerPoint[pointNum] = NewGO<SpriteRender>(nsStdafx::PRIORITY_11);
			m_waitJoinPlayerPoint[pointNum]->Init(nsTitleSceneSprite::WAITJOINPLAYER_POINT_NAME[pointNum], 1300.0f, 720.0f);
			//初めは非表示
			m_waitJoinPlayerPoint[pointNum]->Deactivate();
		}
		//「・・・」画像を作成
		for (int pointNum = enPointOne; pointNum < enPointNum; pointNum++)
		{
			m_searchJoinRoomPoint[pointNum] = NewGO<SpriteRender>(nsStdafx::PRIORITY_11);
			m_searchJoinRoomPoint[pointNum]->Init(nsTitleSceneSprite::SEARCHJOINROOM_POINT_NAME[pointNum], 1300.0f, 720.0f);
			//初めは非表示
			m_searchJoinRoomPoint[pointNum]->Deactivate();
		}

		return true;
	}

	TitleSceneSprite::~TitleSceneSprite()
	{
		//背景画像を削除
		DeleteGO(m_backSprite);
		//カーソル画像を削除
		DeleteGO(m_cursorSprite);

		//選択画像を削除
		DeleteGO(m_firstSelectSprite);
		DeleteGO(m_SecondSelectSprite);
		DeleteGO(m_thirdSelectSprite);
		DeleteGO(m_fourthSelectSprite);

		//参加プレイヤーを待っています・・・画像を削除
		DeleteGO(m_fourthSelectSprite);
		//参加できる部屋をさがしています・・・画像を削除
		DeleteGO(m_fourthSelectSprite);

		for (int pointNum = enPointOne; pointNum < enPointNum; pointNum++)
		{
			DeleteGO(m_waitJoinPlayerPoint[pointNum]);
		}
		for (int pointNum = enPointOne; pointNum < enPointNum; pointNum++)
		{
			DeleteGO(m_searchJoinRoomPoint[pointNum]);
		}
		DeleteGO(m_waitJoinPlayerSprite);
		DeleteGO(m_searchJoinRoomSprite);
	}

	void TitleSceneSprite::Update()
	{
		if (m_titleScene == nullptr)
		{
			return;
		}

		switch (m_titleScene->GetState())
		{
		case enStartOrEnd:
			m_firstSelectSprite->Activate();
			m_SecondSelectSprite->Deactivate();

			break;

		case enGameModeSelect:
			m_SecondSelectSprite->Activate();
			m_firstSelectSprite->Deactivate();
			m_thirdSelectSprite->Deactivate();

			break;

		case enOnlineRoomCreateOrJoin:
			m_thirdSelectSprite->Activate();
			m_SecondSelectSprite->Deactivate();
			m_fourthSelectSprite->Deactivate();

			//参加プレイヤーを待っています・・・
			m_waitJoinPlayerSprite->Deactivate();
			//参加できる部屋をさがしています・・・
			m_searchJoinRoomSprite->Deactivate();
			for (int pointNum = enPointOne; pointNum < enPointNum; pointNum++)
			{
				m_waitJoinPlayerPoint[pointNum]->Deactivate();
				m_searchJoinRoomPoint[pointNum]->Deactivate();
			}
			m_pointActiveTimer = nsTitleSceneSprite::TIMER_RESET_NO;

			break;

		case enOnlineMatchWaiting:
			m_fourthSelectSprite->Activate();
			m_thirdSelectSprite->Deactivate();

			//部屋を作ったとき、
			if (m_titleScene->GetIsRoomCreate())
			{
				PointMove(m_waitJoinPlayerSprite, m_searchJoinRoomPoint);
			}
			else
			{
				PointMove(m_searchJoinRoomSprite, m_searchJoinRoomPoint);
			}

			break;
		}

		//カーソル画像の位置を更新
		m_cursorSprite->SetPosition({ 0.0f,m_titleScene->GetCursorSelect() * -65.0f,0.0f });
	}

	void TitleSceneSprite::PointMove(SpriteRender* writings,SpriteRender* pointSprite[])
	{
		//参加できる部屋をさがしています・・・
		//or
		//参加プレイヤーを待っています・・・
		writings->Activate();

		m_pointActiveTimer++;
		switch (m_pointActiveTimer)
		{
		case enFirstCool:
			break;
		case enSecondCool:
			pointSprite[enPointOne]->Activate();
			break;
		case enThirdCool:
			pointSprite[enPointTwo]->Activate();
			break;
		case enFourthCool:
			pointSprite[enPointThree]->Activate();
			break;
		case enFifthCool:
			//全て非表示
			for (int pointNum = enPointOne; pointNum < enPointNum; pointNum++)
			{
				pointSprite[pointNum]->Deactivate();
			}
			m_pointActiveTimer = nsTitleSceneSprite::TIMER_RESET_NO;
			break;
		}
	}
}