#include "pch.h"
#include "resource.h"
#include "version.hpp"
#include "app_interface.hpp"
#include "lockable.hpp"
#include "hive.hpp"
#include "utils.hpp"
#include "available_fonts.hpp"
#include "instant_window/manager.hpp"
#include "instant_window/entity/font_listbox.hpp"
#include "instant_window/entity/font_menu_dialog.hpp"
#include "hook/entry.hpp"
#include "hook/manager.hpp"
#include "hook/entry/menu_api.hpp"
#include "hook/entry/gdi_api.hpp"
#include "hook/entry/call_wnd_proc_ret.hpp"
#include "config_dialog.hpp"
#include "io_base.hpp"
#include "config_io.hpp"
#include "app.hpp"

namespace apn::visual_font
{
	//
	// エントリポイントです。
	//
	EXTERN_C BOOL APIENTRY DllMain(HMODULE instance, DWORD reason, LPVOID reserved)
	{
		switch (reason)
		{
		case DLL_PROCESS_ATTACH:
			{
				::DisableThreadLibraryCalls(hive.instance = instance);

				// このdllがアンロードされないようにします。
				::LoadLibrary(my::get_module_file_name(instance).c_str());

				break;
			}
		case DLL_PROCESS_DETACH:
			{
				break;
			}
		}

		return TRUE;
	}

	//
	// プラグインDLL初期化関数です。
	//
	EXTERN_C bool InitializePlugin(DWORD version)
	{
		if (version < 2003000) return false;
#ifdef _DEBUG
		// デバッグ用のコードです。
		{
//			if (0)
			{
				// カスタムロガーを設定します。
				static struct Logger : my::Tracer::Logger {
					virtual void output(LPCTSTR raw, LPCTSTR text) override {
						// SHIFTキーが押されているときだけログを出力します。
						if (::GetKeyState(VK_SHIFT) < 0) ::OutputDebugString(text);
					}
				} logger;
				my::Tracer::logger = &logger;
			}
		}
#endif
		app->dll_init();

		return true;
	}

	//
	// プラグインDLL終了関数です。
	//
	EXTERN_C void UninitializePlugin()
	{
		app->dll_exit();

		return;
	}

	//
	// プラグイン登録関数です。
	//
	EXTERN_C void RegisterPlugin(HOST_APP_TABLE* host)
	{
		// プラグイン情報をセットします。
		host->set_plugin_information(my::format(L"{/}{/}",
			tr(version.information), version.revision).c_str());

		// プラグインウィンドウを登録します。
		host->register_window_client(tr(version.name), hive.plugin_window);

		// aviutl2ウィンドウを取得します。
		{
			hive.aviutl2_window = ::GetParent(hive.plugin_window);

			if (auto owner = ::GetWindow(hive.aviutl2_window, GW_OWNER))
				hive.aviutl2_window = owner;
		}
	}

	//
	// aviutl2のコンフィグを取得します。
	//
	EXTERN_C void InitializeConfig(CONFIG_HANDLE* config)
	{
		hive.aviutl2.config = config;
	}
}
