#pragma once

namespace apn::visual_font
{
	//
	// このクラスはこのプログラムのバージョンを管理します。
	//
	inline struct version_t : version_base_t {
		//
		// コンストラクタです。
		//
		version_t() : version_base_t(L"フォント視覚化MOD", L"🐍フォント視覚化MOD🔖", L"r2") {}
	} version;
}
