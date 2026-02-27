#pragma once

namespace apn::visual_font
{
	//
	// このクラスはアプリケーションのインターフェイスです。
	//
	inline struct app_interface_t
	{
		//
		// コンストラクタです。
		//
		app_interface_t() { app = this; }

		virtual BOOL dll_init() = 0;
		virtual BOOL dll_exit() = 0;
	} *app = nullptr;
}
