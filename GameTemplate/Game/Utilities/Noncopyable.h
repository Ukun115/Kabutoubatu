/// <summary>
/// �R�s�[�֎~�|���V�[��񋟂��邽�߂̃N���X
/// </summary>
#pragma once

class Noncopyable
{
    // �f�t�H���g�R���X�g���N�^���g�p����B
    Noncopyable() = default;

    // �R�s�[�R���X�g���N�^���폜�B
    Noncopyable(const Noncopyable&) = delete;

    // ������Z�q���폜�B
    Noncopyable& operator=(const Noncopyable&) = delete;
};