/// <summary>
/// �I�����C���ʐM���̒ʐM�G���[�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class GameScene;		//�Q�[���V�[��
	class Pause;			//�|�[�Y�@�\
	class SpriteRenderSub;	//�摜�`�揈���T�u

	class OnlineErrorScene : public IGameObject
	{
	private:
		GameScene* m_gameScene = nullptr;
		Pause* m_pause = nullptr;
		SpriteRenderSub* m_grayBack = nullptr;
		SpriteRenderSub* m_errorMessageBox = nullptr;

		int m_padNo = 0;
		float m_errorMessageBoxXscale = 0.0f;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~OnlineErrorScene();

		//�|�[�Y�p�̃A�b�v�f�[�g�֐��𗘗p����
		void PauseUpdate()override final;

		void BackTitle(const int padNo);

	public:
		void SetPadNumber(const int padNo) { m_padNo = padNo; };
	};
}