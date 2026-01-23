#include <string>
#include <iostream>
#include "../sprintfmt.hpp"

namespace simple
{
	//
	// シンプルなテストを実行します。
	//
	void test()
	{
		using namespace sprintfmt;

		std::cout << "\033[36m" "シンプルなテストを始めます。" "\033[m" << std::endl;

		std::cout << std::endl;

		std::cout << "最小の書式をテストします。" << std::endl;
		std::cout << "123" << std::endl;
		std::cout << format("{/}", 123) << std::endl;

		std::cout << std::endl;

		std::cout << "書式をエスケープします。" << std::endl;
		std::cout << "{}{{}}{/ここはエスケープされています。ここは書式化されています。{/" << std::endl;
		std::cout << format("{}{{}}{//ここはエスケープされています。{/}{/", "ここは書式化されています。") << std::endl;

		std::cout << std::endl;

		std::cout << "指定の書式をテストします。" << std::endl;
		std::cout << "0x0ABC" << std::endl;
		std::cout << format("0x{/04X}", 0xABC) << std::endl;

		std::cout << std::endl;

		std::cout << "複数の引数を書式化します。" << std::endl;
		std::cout << "456, 456, 789" << std::endl;
		std::cout << format("{/2$}, {/}, {/}", 123, 456, 789) << std::endl;

		std::cout << std::endl;

		std::cout << "浮動小数点数を幅と精度を指定して書式化します。" << std::endl;
		std::cout << "   123.457" << std::endl;
		std::cout << format("{/*.*f}", sfs(123.456789, 10, 3)) << std::endl;

		std::cout << std::endl;

		std::cout << "ポインタを書式化します。" << std::endl;
		if constexpr(sizeof(void*) <= 4)
			std::cout << "0x00000123" << std::endl;
		else
			std::cout << "0x0000000000000123" << std::endl;
		std::cout << format("{/}", (void*)0x123) << std::endl;

		std::cout << std::endl;

		std::cout << "null文字列を書式化します。" << std::endl;
		std::cout << "(null)" << std::endl;
		std::cout << format("{/}", (const char*)nullptr) << std::endl;

		std::cout << std::endl;

		std::cout << "ワイド文字列をマルチバイト文字列として書式化します。" << std::endl;
		std::cout << "a日b本語のテスcト(マルチバイト文字列として出力)" << std::endl;
		std::cout << format("{/}", L"a日b本語のテスcト(マルチバイト文字列として出力)") << std::endl;

		std::cout << std::endl;

		std::cout << "マルチバイト文字列をワイド文字列として書式化します。" << std::endl;
		std::wcout << L"a日b本語のテスcト(ワイド文字列として出力)" << std::endl;
		std::wcout << format(L"{/}", "a日b本語のテスcト(ワイド文字列として出力)") << std::endl;

		std::cout << std::endl;

		std::cout << "\033[36m" "シンプルなテストを終わります。" "\033[m" << std::endl;

		std::cout << std::endl;
	}
}

namespace for_each_type
{
	//
	// この構造体は書式化対象の型です。
	//
	template <typename T>
	struct Hoge { T t; };

	//
	// Hoge<T>を文字列に変換して返します。
	//
	template <typename T>
	constexpr auto fmt_value_to_string(const std::string& fmt, const Hoge<T>& value, auto... args) -> std::string
	{
		return "ここでHogeを書式化できます";
	}

	//
	// 型固有書式のテストを実行します。
	//
	void test()
	{
		using namespace sprintfmt;

		std::cout << "\033[36m" "型固有書式のテストを始めます。" "\033[m" << std::endl;

		std::cout << std::endl;

		std::cout << "ここでHogeを書式化できます" << std::endl;
		std::cout << format("{/ここでfmtを渡せます}", sfs(Hoge("Hogeのプロパティです"), "ここでargsを渡せます")) << std::endl;

		std::cout << std::endl;

		std::cout << "\033[36m" "型固有書式のテストを終わります。" "\033[m" << std::endl;

		std::cout << std::endl;
	}
}

namespace custom_sub_fmt
{
	template <typename S>
	struct MakeSubFormatter {};

	//
	// このクラスはstd::string型用のサブフォーマッタ生成関数オブジェクトです。
	//
	template <>
	struct MakeSubFormatter<std::string> {
		template <typename T, typename... Args>
		constexpr auto operator()(const T& value, Args&&... args) const {
			return [&](const std::string& fmt)
			{
				using namespace sprintfmt;

				if (fmt == "hex")
					return sprintf(sizeof(value) <= 4 ? "0x%08X" : "0x%016X", value);

				if constexpr (std::is_integral_v<T>)
				{
					if (fmt == "wordword")
						return sprintf("0x%04X, 0x%04X", (uint16_t)(value >> 16) , (uint16_t)value);
				}

				return fmt_value_to_string(fmt, value, args...);
			};
		}
	};

	//
	// このクラスはstd::wstring型用のサブフォーマッタ生成関数オブジェクトです。
	//
	template <>
	struct MakeSubFormatter<std::wstring> {
		template <typename T, typename... Args>
		constexpr auto operator()(const T& value, Args&&... args) const {
			return [&](const std::wstring& fmt)
			{
				using namespace sprintfmt;

				if (fmt == L"hex")
					return sprintf(sizeof(value) <= 4 ? L"0x%08X" : L"0x%016X", value);

				if constexpr (std::is_integral_v<T>)
				{
					if (fmt == L"wordword")
						return sprintf(L"0x%04X, 0x%04X", (uint16_t)(value >> 16) , (uint16_t)value);
				}

				return fmt_value_to_string(fmt, value, args...);
			};
		}
	};

	//
	// std::string型用のカスタムフォーマッタを作成して返します。
	// 少ない文字数でコーディングできるようにするための処理なので、必須の定義ではありません。
	//
	template <typename T, typename... Args>
	_NODISCARD constexpr auto sfs(const T& value, Args&&... args)
	{
		return MakeSubFormatter<std::string>()(value, args...);
	}

	//
	// std::wstring型用のカスタムフォーマッタを作成して返します。
	// 少ない文字数でコーディングできるようにするための処理なので、必須の定義ではありません。
	template <typename T, typename... Args>
	_NODISCARD constexpr auto sfw(const T& value, Args&&... args)
	{
		return MakeSubFormatter<std::wstring>()(value, args...);
	}

	//
	// カスタムフォーマッタを使用して
	// フォーマット化された文字列を返します。
	//
	template <typename... Args>
	_NODISCARD constexpr auto format(const std::string& fmt, Args&&... args) -> std::string
	{
		return sprintfmt::Formatter<std::string, MakeSubFormatter<std::string>>::format(fmt, args...);
	}

	//
	// カスタムフォーマッタを使用して
	// フォーマット化されたワイド文字列を返します。
	//
	template <typename... Args>
	_NODISCARD constexpr auto format(const std::wstring& fmt, Args&&... args) -> std::wstring
	{
		return sprintfmt::Formatter<std::wstring, MakeSubFormatter<std::wstring>>::format(fmt, args...);
	}

	//
	// カスタムサブフォーマットのテストを実行します。
	//
	void test()
	{
		std::cout << "\033[36m" "カスタムサブフォーマットのテストを始めます。" "\033[m" << std::endl;

		std::cout << std::endl;

		std::cout << "0xDEADBEEF(マルチバイト文字列)" << std::endl;
		std::cout << format("{/hex}(マルチバイト文字列)", 0xdeadbeef) << std::endl;

		std::cout << "0xDEAD, 0xBEEF(マルチバイト文字列)" << std::endl;
		std::cout << format("{/wordword}(マルチバイト文字列)", 0xdeadbeef) << std::endl;

		std::wcout << L"0xDEADBEEF(ワイド文字列)" << std::endl;
		std::wcout << format(L"{/hex}(ワイド文字列)", 0xdeadbeef) << std::endl;

		std::wcout << L"0xDEAD, 0xBEEF(ワイド文字列)" << std::endl;
		std::wcout << format(L"{/wordword}(ワイド文字列)", 0xdeadbeef) << std::endl;

		std::cout << std::endl;

		std::cout << "\033[36m" "カスタムサブフォーマットのテストを終わります。" "\033[m" << std::endl;

		std::cout << std::endl;
	}
}

//
// エントリポイントです。
//
int main()
{
	// ワイド文字列を使用する場合は
	// 予めロケールを設定する必要があります。
	setlocale(LC_CTYPE, "");

	// テストを開始します。
	simple::test();
	for_each_type::test();
	custom_sub_fmt::test();

	return 0;
}
