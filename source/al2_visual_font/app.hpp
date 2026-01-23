#pragma once

namespace apn::visual_font
{
	//
	// このクラスはアプリケーションです。
	//
	inline struct app_t : app_interface_t
	{
		//
		// dllの初期化処理を実行します。
		//
		virtual BOOL dll_init() override
		{
			MY_TRACE_FUNC("");

			// 各種開始処理を実行します。
			config_io.init();
			available_fonts.init();
			instant_window::manager.init();
			hook::manager.init();
			config_dialog.init();

			// コンフィグをファイルから読み込みます。
			config_io.read();

			return TRUE;
		}

		//
		// dllの後始末処理を実行します。
		//
		virtual BOOL dll_exit() override
		{
			MY_TRACE_FUNC("");

			// コンフィグをファイルに書き込みます。
			config_io.write();

			// 各種終了処理を実行します。
			config_dialog.exit();
			hook::manager.exit();
			instant_window::manager.exit();
			available_fonts.exit();
			config_io.exit();

			return TRUE;
		}
	} app_impl;
}
