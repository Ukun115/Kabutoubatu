/// <summary>
/// �X�J�C�L���[�u�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class Light;	//���C�g

	class SkyCube : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�X�J�C�L���[�u�̃e�N�X�`���[�^�C�v
		enum enSkyCubeType
		{
			enDayType,
			enSunlizeType,
			enNightType,
			enSkyCubeType_Num
		};

		Light* m_light = nullptr;

		ModelInitData m_skyCubeInitData;
		Model m_skyCubeModel;
		Texture m_texture[enSkyCubeType_Num];

		int	m_skyCubeType = enDayType;
		const wchar_t* m_mapTextureFilePaths[enSkyCubeType_Num];
		Vector3 m_directionLightColor;
		float m_rotAngle = 0.0f;
		float m_luminance = 1.0f;
		Vector3    m_pos = Vector3::Zero;			//�ʒu
		Quaternion m_rot = Quaternion::Identity;	//��]
		Vector3    m_sca = Vector3::One;			//�g�嗦

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

		//���f���̕`�惁�\�b�h
		void ModelRender(RenderContext& rc)override final;

	public:
		void SetChangeMap(const int mapNum)
		{
			m_skyCubeType = mapNum;
			if (m_skyCubeType == enSkyCubeType_Num)
			{
				m_skyCubeType = enDayType;
			}
		};

		/// <summary>
		/// �X�J�C�L���[�u�}�b�v�e�N�X�`�����擾
		/// </summary>
		/// <returns>�X�J�C�L���[�u�}�b�v�e�N�X�`��</returns>
		Texture& GetSkyCubeMapTexture()
		{
			return m_texture[enSkyCubeType_Num];
		}
	};
}