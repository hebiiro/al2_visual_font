#pragma once

namespace apn::visual_font
{
	//
	// このクラスはアプリケーションのインターフェイスです。
	//
	inline struct app_interface_t : app_base_t
	{
		//
		// コンストラクタです。
		//
		app_interface_t() { app = this; }
	} *app = nullptr;
}
