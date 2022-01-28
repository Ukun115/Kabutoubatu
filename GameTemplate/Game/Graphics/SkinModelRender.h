/// <summary>
/// ���f���`��N���X
/// </summary>
#pragma once

/// <summary>
/// �O���錾
/// </summary>
class Model;	//���f��

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class Light;			//���C�g
	class DepthShadow;		//�f�v�X�V���h�E
	class RenderingEngine;	//�����_�����O�G���W��
	class Silhouette;		//�V���G�b�g�`��
	class MiniMap;			//�~�j�}�b�v
	class PlayerCamera;		//�v���C���[�J����
	class SkyCube;			//�X�J�C�L���[�u

	/// <summary>
	/// �\����
	/// </summary>

	//���f���̃V�F�[�_�[�ɓn�����
	struct stModelMode
	{
		//�v���C���[�̃��[�h
		int playerMode = 2;	//�����l�͂��̑��̂Q��

		//�֊s�������郂�f�����ǂ����̃t���O
		int outline = false;

		//�V���G�b�g�����邩�ǂ����̃t���O
		int silhouetteFlg = false;

		//�_���[�W�󂯂����̐ԐF�Ƀ`�J���Ƃ���t���O
		int damageColorFlg = false;
	};

	class SkinModelRender : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�֊s���̐F
		enum enOutLineColor
		{
			enRed,		//�ԐF
			enBlue,		//�F
			enBlack		//���F
		};

		/// <summary>
		/// �N���X�̃|�C���^
		/// </summary>
		Light* m_light = nullptr;					//���C�g
		DepthShadow* m_depthShadow = nullptr;		//�f�v�X�V���h�E
		RenderingEngine* m_zPrepass = nullptr;				//ZPrepass
		Silhouette* m_silhouette = nullptr;			//�V���G�b�g
		MiniMap* m_miniMap = nullptr;				//�~�j�}�b�v
		SkyCube* m_skyCube = nullptr;				//�X�J�C�L���[�u
		PlayerCamera* m_playerCamera = nullptr;		//�v���C���[�J����
		AnimationClip* m_animationClips = nullptr;	//�A�j���[�V�����N���b�v�B

		ModelInitData m_modelInitData;				//���f���̃f�[�^
		ModelInitData m_shadowModelInitData;		//�V���h�E���f���̃f�[�^
		ModelInitData m_zPrepassModelInitData;		//ZPrepass���f���̃f�[�^
		ModelInitData m_silhouetteModelInitData;	//�V���G�b�g���f���̃f�[�^
		ModelInitData m_miniMapModelInitData;		//�~�j�}�b�v���f���̃f�[�^
		Model m_model;				//���f��
		Model m_shadowModel;		//�V���h�E�̃��f��
		Model m_zPrepassModel;		//ZPrepass�̃��f��
		Model m_silhouetteModel;	//�V���G�b�g�̃��f��
		Model m_miniMapModel;		//�~�j�}�b�v�̃��f��
		Skeleton m_skeleton;		//�X�P���g��
		Animation m_animation;		//�A�j���[�V����
		stModelMode m_modelMode;
		//�X�P���g������̃��f���̏�����B�ʏ펞��Y�A�b�v�B
		//�X�P���g���Ȃ��̃��f���̏�����͒ʏ펞Z�A�b�v�B
		EnModelUpAxis m_skinModelUpAxis = enModelUpAxisY;

		int	m_animationClipsNum = 0;				//�A�j���[�V�����N���b�v�̐��B
		Vector3    m_pos = Vector3::Zero;			//�ʒu
		Quaternion m_rot = Quaternion::Identity;	//��]
		Vector3    m_sca = Vector3::One;			//�g�嗦
		const char* m_tksFilePath = nullptr;					//�X�P���g���̃t�@�C���p�X
		char m_filePathM[256];						//���f���̃t�@�C���p�X
		char m_filePathS[256];						//�X�P���g���̃t�@�C���p�X
		//�V���h�E�L���X�^�[���ǂ���(�V���h�E�𗎂Ƃ����̂��ǂ���)�̃t���O
		bool m_shadowCaster = false;
		bool m_silhouetteFlg = false;
		float m_animationSpeed = 1.0f;
		//�΂̋ʃ��[�h���ǂ����t���O
		bool m_fireBallFlg = false;

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// (�ʏ�̍X�V���~�߂�UI�����X�V�����������Ɏg���X�V�����֐�)
		/// </summary>
		void UiUpdate()override final;

	public:
		static void PreLoadModel(const char* tkmFilePath);

		//���f�������������郁�\�b�h
		void Init(const char* tkmFilePath, const char* tksFilePath = nullptr, AnimationClip* animationClips = nullptr, int animationClipsNum = 0);

		//���f���̕`�惁�\�b�h
		void ModelRender(RenderContext& rc)override final;

		//�V���h�E�̕`�惁�\�b�h
		void ShadowRender(RenderContext& rc)override final;

		//�֊s���̕`�惁�\�b�h
		void ZPrepassRender(RenderContext& rc)override final;

		//�V���G�b�g���f���̕`�惁�\�b�h
		void SilhouetteRender(RenderContext& rc)override final;

		//�~�j�}�b�v�̕`�惁�\�b�h
		void MiniMapRender(RenderContext& rc)override final;

		/*!
		* @brief	�A�j���[�V�������Đ��B
		*@param[in]	clipNo	�A�j���[�V�����N���b�v�̔ԍ��B�R���X�g���N�^�ɓn����animClipList�̕��тƂȂ�B
		*@param[in]	interpolateTime		�⊮����(�P�ʁF�b)	�⊮���Ԃ������قǂ�����莟�̃A�j���[�V�����ɐ؂�ւ��
		*/
		void PlayAnimation(int animNo, float interpolateTime = 0.1f)
		{
			m_animation.Play(animNo, interpolateTime);
		}

		/// <summary>
		/// �Z�b�^�[
		/// </summary>
		//���f���̈ʒu��ݒ肷��֐�
		void SetPosition(const Vector3& pos) { m_pos = pos; }

		//���f���̉�]��ݒ肷��֐�
		void SetRotation(const Quaternion& rot) { m_rot = rot; }

		//���f���̊g�嗦��ݒ肷��֐�
		void SetScale(const Vector3& sca) { m_sca = sca; }

		//�V���h�E�����邩�ǂ����̃t���O���Z�b�g���郁�\�b�h
		void SetShadowCaster(const bool shadowCaster) { m_shadowCaster = shadowCaster; }

		//�V���G�b�g�����邩�ǂ����̃t���O���Z�b�g���郁�\�b�h
		void SetSilhouette(const bool silhouetteFlg) { m_modelMode.silhouetteFlg = silhouetteFlg; }

		//�֊s�������邩�ǂ����̃t���O���Z�b�g���郁�\�b�h
		void SetOutline(const bool outline) { m_modelMode.outline = outline; }

		//�PP�QP���̑��𔻕ʂ���ԍ����Z�b�g���郁�\�b�h
		void SetPlayerMode(const int playerMode) { m_modelMode.playerMode = playerMode; }

		//�X�P���g������̃��f���̏�������w�肷�郁�\�b�h
		void SetModelUpAxis(const EnModelUpAxis modelUpAxis) { m_skinModelUpAxis = modelUpAxis; }

		void SetAnimationSpeed(const float animationSpeed) { m_animationSpeed = animationSpeed; };

		void SetDameageRed(const bool flg) { m_modelMode.damageColorFlg = flg; };

		void SetFireBall(const bool flg) { m_fireBallFlg = flg; };

		/// <summary>
		/// ���f���̈ʒu���擾
		/// </summary>
		/// <returns>���f���̈ʒu</returns>
		Vector3 GetPosition()const { return m_pos; }

		/// <summary>
		/// ��]�N�H�[�^�j�I�����擾
		/// </summary>
		/// <returns>��]�N�H�[�^�j�I��</returns>
		Quaternion GetRotation()const { return m_rot; }

		/// <summary>
		/// ���f���̃A�h���X���擾
		/// </summary>
		/// <returns>���f���̃A�h���X</returns>
		Model* GetModel() { return &m_model; }

		/// <summary>
		/// �X�P���g���̃A�h���X���擾
		/// </summary>
		/// <returns>�X�P���g��</returns>
		Skeleton* GetSkeleton() { return &m_skeleton; }

		/// <summary>
		/// �A�j���[�V�����Đ��������擾
		/// </summary>
		/// <returns>�A�j���[�V�����Đ������ǂ���</returns>
		bool IsPlaying() { return m_animation.IsPlaying(); };

		/// <summary>
		/// �΂̋ʃ��[�h���ǂ������擾
		/// </summary>
		/// <returns>�΂̋ʃ��[�h���ǂ���</returns>
		bool GetFireBall()const { return m_fireBallFlg; };
	};
}