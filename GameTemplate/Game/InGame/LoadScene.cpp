/// <summary>
/// ���[�h��ʃN���X
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
		//TODO:���[�h��BGM�Đ�

		//�w�i�摜���쐬
		m_backScreen = NewGO<SpriteRender>(nsStdafx::PRIORITY_10);
		m_backScreen->Init("LoadBack", 1300.0f, 720.0f);

		//�u�E�E�E�v�摜���쐬
		for (int pointNum = enPointOne; pointNum < enPointNum; pointNum++)
		{
			m_point[pointNum] = NewGO<SpriteRender>(nsStdafx::PRIORITY_11);
			m_point[pointNum]->Init(nsLoadScene::POINT_NAME[pointNum], 1300.0f, 720.0f);
			//���߂͔�\��
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
			//�S�Ĕ�\��
			for (int pointNum = enPointOne; pointNum < enPointNum; pointNum++)
			{
				m_point[pointNum]->Deactivate();
			}
			m_pointActiveTimer = 0;
			break;
		}
	}
}