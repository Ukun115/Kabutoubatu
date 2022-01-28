/// <summary>
/// �^�C�g���V�[���̉摜�N���X
/// </summary>
#include "stdafx.h"
#include "TitleSceneSprite.h"
#include "TitleScene.h"

namespace nsKabutoubatu
{
	namespace nsTitleSceneSprite
	{
		const char* WAITJOINPLAYER_POINT_NAME[6] = { "WaitJoinPlayer_Point1","WaitJoinPlayer_Point2","WaitJoinPlayer_Point3" };
		const char* SEARCHJOINROOM_POINT_NAME[6] = { "SearchJoinRoom_Point1","SearchJoinRoom_Point2","SearchJoinRoom_Point3" };
	}

	bool TitleSceneSprite::Start()
	{
		//�^�C�g���V�[���N���X�̃C���X�^���X������
		m_titleScene = FindGO<TitleScene>(nsStdafx::TITLESCENE_NAME);

		//�w�i�摜���쐬
		m_backSprite = NewGO<SpriteRender>(nsStdafx::PRIORITY_0);
		m_backSprite->Init("TitleSceneBack", 1300.0f, 720.0f);
		//�J�[�\���摜���쐬
		m_cursorSprite = NewGO<SpriteRender>(nsStdafx::PRIORITY_1);
		m_cursorSprite->Init("TitleScene_PlayModeSelectCursor", 1300.0f, 720.0f);

		//�P�ڂ̑I���摜���쐬
		m_firstSelectSprite = NewGO<SpriteRender>(nsStdafx::PRIORITY_1);
		m_firstSelectSprite->Init("TitleScene_StartButton", 1300.0f, 720.0f);
		//�Q�ڂ̑I���摜���쐬
		m_SecondSelectSprite = NewGO<SpriteRender>(nsStdafx::PRIORITY_1);
		m_SecondSelectSprite->Init("TitleScene_PlayModeSelect", 1300.0f, 720.0f);
		m_SecondSelectSprite->Deactivate();	//���ߔ�\��
		//�R�ڂ̑I���摜���쐬
		m_thirdSelectSprite = NewGO<SpriteRender>(nsStdafx::PRIORITY_1);
		m_thirdSelectSprite->Init("TitleScene_OnlineRoomCreateOrSearch", 1300.0f, 720.0f);
		m_thirdSelectSprite->Deactivate();	//���ߔ�\��
		//�S�ڂ̑I���摜���쐬
		m_fourthSelectSprite = NewGO<SpriteRender>(nsStdafx::PRIORITY_1);
		m_fourthSelectSprite->Init("TitleScene_OnlineMatchWaiting", 1300.0f, 720.0f);
		m_fourthSelectSprite->Deactivate();	//���ߔ�\��

		//�Q���v���C���[��҂��Ă��܂��E�E�E�摜���쐬
		m_waitJoinPlayerSprite = NewGO<SpriteRender>(nsStdafx::PRIORITY_1);
		m_waitJoinPlayerSprite->Init("WaitJoinPlayer", 1300.0f, 720.0f);
		m_waitJoinPlayerSprite->Deactivate();	//���ߔ�\��
		//�Q���ł��镔�����������Ă��܂��E�E�E�摜���쐬
		m_searchJoinRoomSprite = NewGO<SpriteRender>(nsStdafx::PRIORITY_1);
		m_searchJoinRoomSprite->Init("SearchJoinRoom", 1300.0f, 720.0f);
		m_searchJoinRoomSprite->Deactivate();	//���ߔ�\��

		//�u�E�E�E�v�摜���쐬
		for (int pointNum = enPointOne; pointNum < enPointNum; pointNum++)
		{
			m_waitJoinPlayerPoint[pointNum] = NewGO<SpriteRender>(nsStdafx::PRIORITY_11);
			m_waitJoinPlayerPoint[pointNum]->Init(nsTitleSceneSprite::WAITJOINPLAYER_POINT_NAME[pointNum], 1300.0f, 720.0f);
			//���߂͔�\��
			m_waitJoinPlayerPoint[pointNum]->Deactivate();
		}
		//�u�E�E�E�v�摜���쐬
		for (int pointNum = enPointOne; pointNum < enPointNum; pointNum++)
		{
			m_searchJoinRoomPoint[pointNum] = NewGO<SpriteRender>(nsStdafx::PRIORITY_11);
			m_searchJoinRoomPoint[pointNum]->Init(nsTitleSceneSprite::SEARCHJOINROOM_POINT_NAME[pointNum], 1300.0f, 720.0f);
			//���߂͔�\��
			m_searchJoinRoomPoint[pointNum]->Deactivate();
		}

		return true;
	}

	TitleSceneSprite::~TitleSceneSprite()
	{
		//�w�i�摜���폜
		DeleteGO(m_backSprite);
		//�J�[�\���摜���폜
		DeleteGO(m_cursorSprite);

		//�I���摜���폜
		DeleteGO(m_firstSelectSprite);
		DeleteGO(m_SecondSelectSprite);
		DeleteGO(m_thirdSelectSprite);
		DeleteGO(m_fourthSelectSprite);

		//�Q���v���C���[��҂��Ă��܂��E�E�E�摜���폜
		DeleteGO(m_fourthSelectSprite);
		//�Q���ł��镔�����������Ă��܂��E�E�E�摜���폜
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
		case 0:
			m_firstSelectSprite->Activate();
			m_SecondSelectSprite->Deactivate();

			break;

		case 1:
			m_SecondSelectSprite->Activate();
			m_firstSelectSprite->Deactivate();
			m_thirdSelectSprite->Deactivate();

			break;

		case 2:
			m_thirdSelectSprite->Activate();
			m_SecondSelectSprite->Deactivate();
			m_fourthSelectSprite->Deactivate();

			//�Q���v���C���[��҂��Ă��܂��E�E�E
			m_waitJoinPlayerSprite->Deactivate();
			//�Q���ł��镔�����������Ă��܂��E�E�E
			m_searchJoinRoomSprite->Deactivate();
			for (int pointNum = enPointOne; pointNum < enPointNum; pointNum++)
			{
				m_waitJoinPlayerPoint[pointNum]->Deactivate();
				m_searchJoinRoomPoint[pointNum]->Deactivate();
			}
			m_pointActiveTimer = 0;

			break;

		case 3:
			m_fourthSelectSprite->Activate();
			m_thirdSelectSprite->Deactivate();

			//������������Ƃ��A
			if (m_titleScene->GetIsRoomCreate())
			{
				//�Q���v���C���[��҂��Ă��܂��E�E�E
				m_waitJoinPlayerSprite->Activate();

				m_pointActiveTimer++;
				switch (m_pointActiveTimer)
				{
				case 1:
					break;
				case 30:
					m_waitJoinPlayerPoint[enPointOne]->Activate();
					break;
				case 60:
					m_waitJoinPlayerPoint[enPointTwo]->Activate();
					break;
				case 90:
					m_waitJoinPlayerPoint[enPointThree]->Activate();
					break;
				case 120:
					//�S�Ĕ�\��
					for (int pointNum = enPointOne; pointNum < enPointNum; pointNum++)
					{
						m_waitJoinPlayerPoint[pointNum]->Deactivate();
					}
					m_pointActiveTimer = 0;
					break;
				}
			}
			else
			{
				//�Q���ł��镔�����������Ă��܂��E�E�E
				m_searchJoinRoomSprite->Activate();

				m_pointActiveTimer++;
				switch (m_pointActiveTimer)
				{
				case 1:
					break;
				case 30:
					m_searchJoinRoomPoint[enPointOne]->Activate();
					break;
				case 60:
					m_searchJoinRoomPoint[enPointTwo]->Activate();
					break;
				case 90:
					m_searchJoinRoomPoint[enPointThree]->Activate();
					break;
				case 120:
					//�S�Ĕ�\��
					for (int pointNum = enPointOne; pointNum < enPointNum; pointNum++)
					{
						m_searchJoinRoomPoint[pointNum]->Deactivate();
					}
					m_pointActiveTimer = 0;
					break;
				}
			}

			break;
		}

		//�J�[�\���摜�̈ʒu���X�V
		m_cursorSprite->SetPosition({ 0.0f,m_titleScene->GetCursorSelect() * -65.0f,0.0f });
	}
}