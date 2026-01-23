#pragma once

namespace apn::visual_font
{
	//
	// このクラスは他クラスから共通して使用される変数を保持します。
	//
	inline struct hive_t
	{
		//
		// このアドインのインスタンスハンドルです。
		//
		HINSTANCE instance = nullptr;

		//
		// プラグインウィンドウです。
		//
		HWND plugin_window = nullptr;

		//
		// aviutl2ウィンドウです。
		//
		HWND aviutl2_window = nullptr;

		//
		// コンフィグのファイル名です。
		//
		std::wstring config_file_name;

		//
		// このクラスはaviutl2関連の変数を保持します。
		//
		struct aviutl2_t {
			CONFIG_HANDLE* config = {};
		} aviutl2;

		//
		// このクラスはフォントメニューに関連した設定です。
		//
		struct font_menu_t
		{
			//
			// TRUEの場合はフォントメニューをビジュアライズします。
			//
			BOOL flag_use = TRUE;

			//
			// メニュー項目のサイズです。
			//
			SIZE item_size = { 320, 36 };

			//
			// メニュー項目の矩形と文字列間のスペースです。
			//
			SIZE space_size = { 4, 4 };

			//
			// フォントの高さです。
			//
			int32_t font_height = 28;

			//
			// メニューの行数です。この数を超えると改行されます。
			// 0の場合は改行しません。
			//
			int32_t nb_rows = 0;

			//
			// サンプル文字列の書式です。
			//
			std::wstring sample_text_format = L"";
		} font_menu;

		//
		// このクラスはフォントリストボックスに関連した設定です。
		//
		struct font_listbox_t
		{
			//
			// TRUEの場合はフォントリストボックスをビジュアライズします。
			//
			BOOL flag_use = TRUE;

			//
			// リストボックスのサイズです。
			//
			SIZE window_size = { 1000, 800 };

			//
			// アイテムの高さです。
			//
			int32_t item_height = 48;

			//
			// フォントの高さです。
			//
			int32_t font_height = 48;

			//
			// サンプル文字列の書式です。
			//
			std::wstring sample_text_format = L"%font% サンプル0123456789";
		} font_listbox;

		//
		// このクラスは「フォントメニューの設定」ダイアログに関連した設定です。
		//
		struct font_menu_dialog_t
		{
			//
			// TRUEの場合はフォントリストビューをビジュアライズします。
			//
			BOOL flag_use = TRUE;

			//
			// TRUEの場合は「フォントメニューの設定」ダイアログをリサイズ可能にします。
			//
			BOOL flag_resize = TRUE;

			//
			// TRUEの場合はホットキーでフォント名をコピーできるようにします。
			//
			BOOL flag_copy = TRUE;

			//
			// 「フォントメニューの設定」ダイアログのタイトル名です。
			//
			std::wstring title = L"フォントメニューの設定";
		} font_menu_dialog;

		//
		// メッセージボックスを表示します。
		//
		int32_t message_box(const std::wstring& text, HWND hwnd = nullptr, int32_t type = MB_OK | MB_ICONWARNING) {
			if (!hwnd) hwnd = plugin_window;
			return ::MessageBoxW(hwnd, text.c_str(), version.information.c_str(), type);
		}
	} hive;
}
