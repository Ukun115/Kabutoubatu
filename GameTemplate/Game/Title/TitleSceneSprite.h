/// <summary>
/// �^�C�g���V�[���̉摜�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class TitleScene;	//�^�C�g���V�[��

	class TitleSceneSprite : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�_�摜
		enum enPoint
		{
			enPointOne,		//�_�P��
			enPointTwo,		//�_�Q��
			enPointThree,	//�_�R��
			enPointNum		//�_�̑���
		};

		TitleScene* m_titleScene = nullptr;
		SpriteRender* m_backSprite = nullptr;
		//�J�[�\���摜
		SpriteRender* m_cursorSprite = nullptr;
		SpriteRender* m_firstSelectSprite = nullptr;
		SpriteRender* m_SecondSelectSprite = nullptr;
		SpriteRender* m_thirdSelectSprite = nullptr;
		SpriteRender* m_fourthSelectSprite = nullptr;
		SpriteRender* m_waitJoinPlayerSprite = nullptr;
		SpriteRender* m_searchJoinRoomSprite = nullptr;
		SpriteRender* m_waitJoinPlayerPoint[enPointNum] = { nullptr };
		SpriteRender* m_searchJoinRoomPoint[enPointNum] = { nullptr };

		int m_pointActiveTimer = 0;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~TitleSceneSprite();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;
	};
}