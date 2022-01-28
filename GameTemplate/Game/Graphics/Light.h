/// <summary>
/// ���C�g�N���X
/// </summary>
#pragma once

/// <summary>
/// �O���錾
/// </summary>
class Camera;	//�J����

namespace nsKabutoubatu
{
	/// <summary>
	/// �\����
	/// </summary>

	//�f�B���N�V�������C�g
	struct DirLigData
	{
		Vector3 dirDirection;	//����
		float pad1 = 0.0f;		//HLSL���̒萔�o�b�t�@��float3�^(12��)�̕ϐ���16�̔{���̃A�h���X�ɔz�u����邽�߁AC++���ɂ̓p�f�B���O�𖄂߂Ă����A���ߍ��킹�����Ă����B
		Vector3 dirColor;		//�F
		float pad2 = 0.0f;		//�p�f�B���O
	};

	//�|�C���g���C�g
	struct PoiLigData
	{
		Vector3 ptPosition;		//�ʒu
		float pad = 0.0f;		//�p�f�B���O
		Vector3 ptColor;		//�F
		float ptRange = 0.0f;	//�e���͈�
	};

	//�X�|�b�g���C�g
	struct SpotLigData
	{
		Vector3 spPosition;		//�ʒu
		float pad = 0.0f;		//�p�f�B���O
		Vector3 spColor;		//�F
		float spRange = 0.0f;	//�e���͈�
		Vector3 spDirection;	//�ˏo����
		float spAngle = 0.0f;	//�ˏo�p�x
	};

	//�������C�g
	struct HemiSphereLight
	{
		Vector3 hemiGroundColor;	//�n�ʐF�i�Ƃ�Ԃ��̃��C�g�j
		float pad1 = 0.0f;			//�p�f�B���O
		Vector3 hemiSkyColor;		//�V���F
		float pad2 = 0.0f;			//�p�f�B���O
		Vector3 hemiGroundNormal;	//�n�ʂ̖@��
		float pad3 = 0.0f;			//�p�f�B���O
	};

	//���C�g�S��
	struct ligData
	{
		DirLigData  dirLigData;				//�f�B���N�V�������C�g�\���̃f�[�^
		PoiLigData  poiLigData[12];			//�|�C���g���C�g�\���̃f�[�^
		SpotLigData spotLigData;			//�X�|�b�g���C�g�\���̃f�[�^
		HemiSphereLight hemiSphereLigData;	//�������C�g�\���̃f�[�^

		Vector3 eyePos;						//���_�̈ʒu

		float pad = 0.0f;					//�p�f�B���O

		Matrix mLVP;						//���C�g�r���[�v���W�F�N�V�����s��
	};

	class Light : public IGameObject
	{
	private:
		ligData m_ligData;		//���C�g�\���̂̃f�[�^
		Camera m_ligCamera;		//���C�g�J����

		Vector3 m_ligCameraPos;		//���C�g�J�����̎��_
		Vector3 m_ligCameraTarget;	//���C�g�J�����̒����_

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

	public:
		/// <summary>
		/// �Z�b�^�[
		/// </summary>
		//�f�B���N�V�������C�g�̕���,�F��ݒ肷�郁�\�b�h
		void SetDirectionLightData();

		//�|�C���g���C�g�̍��W,�F,�e���͈͂�ݒ肷�郁�\�b�h
		void SetPointLightData(const Vector3 position, const Vector3 color, const float range, const int lignum);

		//�X�|�b�g���C�g�̍��W,�F,����,�ˏo�͈�,�ˏo�p�x��ݒ肷�郁�\�b�h
		void SetSpotLightData();

		//�������C�g�̒n�ʐF�i�Ƃ�Ԃ��̃��C�g�j,�V���F,�n�ʂ̖@����ݒ肷�郁�\�b�h
		void SetHemiSphereLightData();

		void ChangeDirectionLightDataColor(Vector3& color) { m_ligData.dirLigData.dirColor = color; };

		/// <summary>
		/// ���C�g�̃f�[�^���擾
		/// </summary>
		/// <returns>���C�g�̃f�[�^</returns>
		ligData GetLight()const { return m_ligData; }

		/// <summary>
		/// ���C�g�̃f�[�^�̃A�h���X���擾
		/// </summary>
		/// <returns>���C�g�̃f�[�^�̃A�h���X</returns>
		ligData* GetLightAdoress() { return &m_ligData; }

		/// <summary>
		/// ���C�g�J�����̃A�h���X���擾
		/// </summary>
		/// <returns>���C�g�J�����̃A�h���X</returns>
		Camera* GetLightCamera() { return &m_ligCamera; }
	};
}