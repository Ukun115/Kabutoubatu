/// <summary>
/// �R�s�[�֎~�|���V�[��񋟂��邽�߂̃N���X
/// </summary>
#pragma once

class Noncopyable
{
protected:
    // �f�t�H���g�R���X�g���N�^���g�p����B
    Noncopyable() = default;
    //�f�t�H���g�f�X�g���N�^���g�p����B
    ~Noncopyable() = default;

private:
    // �R�s�[�R���X�g���N�^���폜�B
    Noncopyable(const Noncopyable&) = delete;
    // ������Z�q���폜�B
    Noncopyable& operator=(const Noncopyable&) = delete;
};