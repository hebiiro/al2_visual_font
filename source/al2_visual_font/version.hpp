#pragma once

namespace apn::visual_font
{
	//
	// このクラスはこのプログラムのバージョンを管理します。
	//
	inline struct version_t {
		//
		// プラグイン名とプラグイン情報です。
		//
		const std::wstring name = L"フォント視覚化MOD";
		const std::wstring information = L"🐍" + name + L"🔖";
		const std::wstring revision = L"r2";
	} version;
}
