#pragma once

namespace apn::visual_font
{
	//
	// 指定された文字列をクリップボードにコピーします。
	//
	inline BOOL set_clipboard_text(const std::wstring& text, HWND hwnd = nullptr)
	{
		// クリップボードを開きます。
		struct clipboard_locker_t {
			clipboard_locker_t(HWND hwnd) {
				::OpenClipboard(hwnd);
			}
			~clipboard_locker_t() {
				::CloseClipboard();
			}
		} clipboard_locker(hwnd);

		// クリップボードを空にします。
		if (!::EmptyClipboard()) return FALSE;

		// メモリハンドルを作成します。
		auto handle = ::GlobalAlloc(GHND, (text.length() + 1) * sizeof(WCHAR));
		if (!handle) return FALSE;

		// メモリハンドルをロックしてメモリバッファを取得します。
		auto buffer = (LPWSTR)::GlobalLock(handle);
		if (!buffer) return FALSE;

		// メモリバッファにテキストをコピーします。
		::lstrcpynW(buffer, text.c_str(), (int)text.length() + 1);

		// メモリハンドルのロックを解除します。
		::GlobalUnlock(handle);

		// メモリハンドルをクリップボードにセットします。
		// (メモリハンドルをセットしたあとは開放できません)
		return !!::SetClipboardData(CF_UNICODETEXT, handle);
	}
}
