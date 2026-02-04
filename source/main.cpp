#include "pch.h"
#include "resource.h"
#include "version.hpp"
#include "app_interface.hpp"
#include "hive.hpp"
#include "utils.hpp"
#include "debug.hpp"
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
	// aviutl2のコンフィグを取得します。
	//
	EXTERN_C void InitializeConfig(CONFIG_HANDLE* config)
	{
		hive.aviutl2.config = config;
	}

	//
	// プラグインDLL初期化関数です。
	//
	EXTERN_C bool InitializePlugin(DWORD version)
	{
		if (version < 2003000) return false;

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
	// 入力プラグインの構造体を返します。
	//
	EXTERN_C INPUT_PLUGIN_TABLE* GetInputPluginTable()
	{
		// 入力プラグインの構造体を自動的に構築します。
		static input_plugin_table_t input_plugin_table;

		return &input_plugin_table;
	}
}
