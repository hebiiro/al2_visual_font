# 🎉String Print Formatter for C++

* 文字列を書式化します。
* 書式をカスタマイズすることができます。
* UTF-8以外のマルチバイト文字列が使用できます。

## ⚠注意点

* C++20が必要です。
* `std::format()`と互換性がありません。
* `sprintf()`と同様に型のチェック機能がありません。

## 🚀導入方法

1. `<string>`をインクルードします。
1. `"sprintfmt.hpp"`をインクルードします。

## 💡使い方

```C++
auto s = sprintfmt::format("{/}", 123);
```

* `{/arg_index$sub_fmt}`の形式で書式化できます。
* `arg_index$`の部分は省略できます。
* `{//`でエスケープできます。
* デフォルトでは`sub_fmt`は`sprintf("%" + sub_fmt + "dなど型別のサフィックス", args..., value);`のように使用されます。

* 引数の番号は1から始まります。
```C++
auto s = sprintfmt::format("{/2$}, {/}, {/}", 123, 456, 789); // "456, 456, 789"と書式化されます。
```

* サフィックスが必要ない場合は引数を`sprintfmt::own`でラップします。
```C++
auto s = sprintfmt::format("{/8.3f}", sprintfmt::own(123.456));
```

* 複数の引数を使用する書式の場合は`sprintfmt::sfs`でラップします。
	* **※ただし、`sprintf()`とは引数の順番が異なるので注意してください。**
```C++
auto s = sprintfmt::format("{/*.*}", sprintfmt::sfs(123.456, 8, 3));
```

## ⚗️テスト環境

* Win11 Home 24H2

## 👽️作成者情報

* 作成者 - 蛇色 (へびいろ)
* GitHub - https://github.com/hebiiro
* Twitter - https://x.com/io_hebiiro

## 🚨免責事項

この作成物および同梱物を使用したことによって生じたすべての障害・損害・不具合等に関しては、私と私の関係者および私の所属するいかなる団体・組織とも、一切の責任を負いません。各自の責任においてご使用ください。
