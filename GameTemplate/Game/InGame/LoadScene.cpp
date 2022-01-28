/// <summary>
/// ロード画面クラス
/// </summary>
#include "stdafx.h"
#include "LoadScene.h"

namespace nsKabutoubatu
{
	namespace nsLoadScene
	{
		const char* POINT_NAME[3] = { "LoadPointOne","LoadPointTwo","LoadPointThree" };
	}

	bool LoadScene::Start()
	{
		//TODO:ロード中BGM再生

		//背景画像を作成
		m_backScreen = NewGO<SpriteRender>(nsStdafx::PRIORITY_10);
		m_backScreen->Init("LoadBack", 1300.0f, 720.0f);

		//「・・・」画像を作成
		for (int pointNum = enPointOne; pointNum < enPointNum; pointNum++)
		{
			m_point[pointNum] = NewGO<SpriteRender>(nsStdafx::PRIORITY_11);
			m_point[pointNum]->Init(nsLoadScene::POINT_NAME[pointNum], 1300.0f, 720.0f);
			//初めは非表示
			m_point[pointNum]->Deactivate();
		}

		return true;
	}

	LoadScene::~LoadScene()
	{
		DeleteGO(m_backScreen);

		for (int pointNum = enPointOne; pointNum < enPointNum; pointNum++)
		{
			DeleteGO(m_point[pointNum]);
		}
	}

	void LoadScene::Update()
	{
		m_pointActiveTimer++;
		switch (m_pointActiveTimer)
		{
		case 1:
			break;
		case 30:
			m_point[enPointOne]->Activate();
			break;
		case 60:
			m_point[enPointTwo]->Activate();
			break;
		case 90:
			m_point[enPointThree]->Activate();
			break;
		case 120:
			//全て非表示
			for (int pointNum = enPointOne; pointNum < enPointNum; pointNum++)
			{
				m_point[pointNum]->Deactivate();
			}
			m_pointActiveTimer = 0;
			break;
		}
	}
}