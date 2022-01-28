/// <summary>
/// コピー禁止ポリシーを提供するためのクラス
/// </summary>
#pragma once

class Noncopyable
{
    // デフォルトコンストラクタを使用する。
    Noncopyable() = default;

    // コピーコンストラクタを削除。
    Noncopyable(const Noncopyable&) = delete;

    // 代入演算子を削除。
    Noncopyable& operator=(const Noncopyable&) = delete;
};