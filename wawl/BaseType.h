#pragma once

#define WAWL_ENABLE_BASETYPE
#define WAWL_ENABLE

//WinAPI
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#undef NOMINMAX
//C++STL
#include <string>
#include <algorithm>

namespace wawl {

	//組み込み型using
#ifdef TRUE

	//Ascii文字型
	using AChar = ::CHAR;
	//ワイド文字型
	using WChar = ::WCHAR;
	//汎用文字型
	using TChar = ::TCHAR;

	//Ascii文字列型
	using AString = std::basic_string < AChar >;
	//ワイド文字列型
	using WString = std::basic_string < WChar >;
	//汎用文字列型
	using TString = std::basic_string < TChar >;

	//符号無し8bit整数
	using Byte = ::BYTE;
	//符号無し16bit整数
	using Word = ::WORD;
	//符号無し32bit整数
	using Dword = ::DWORD;
	//符号なし64bit整数
	using Qword = unsigned long long;

	//ポインタを格納する整数型
	using IntPtr = ::INT_PTR;
	using UintPtr = ::UINT_PTR;
	using LongPtr = ::LONG_PTR;

	//ハンドル
	using Handle = ::HANDLE;

#endif

	//ライブラリの都合的な型
#ifdef TRUE

	//基本的な汎用座標型
	template <typename VTy>
	struct Coordinates {

		using ValueType = VTy;

		ValueType x = 0, y = 0;

		//デフォルトコンストラクタ
		Coordinates() = default;

		//コピーコンストラクタ
		template <typename T>
		Coordinates(const Coordinates<T>& p_) :
			x(static_cast<ValueType>(p_.x)),
			y(static_cast<ValueType>(p_.y)) {}

		Coordinates(const ValueType& x_, const ValueType& y_) :
			x(x_), y(y_) {}

		template <typename T>
		void operator = (const Coordinates<T>& p_) {
			x = static_cast<ValueType>(p_.x);
			y = static_cast<ValueType>(p_.y);
		}

	};

	using Position = Coordinates < int >;
	using Rectangle = Position;

	//言語コード
	//TODO: 追加
	enum class Lang : Dword {
		UserDefault = LANG_USER_DEFAULT,
		SysDefault = LANG_SYSTEM_DEFAULT,
	};

	class Error {
	public:
		Error() = default;
		Error(const Error&) = default;
		Error& operator = (const Error&) = default;

		Error(Dword errorCode) :
			code_(errorCode) {}
		Error(Dword errorCode, const TString& funcName) :
			code_(errorCode),
			funcName_(funcName) {}

		TString msg(Lang lang = Lang::UserDefault) const {
			static TString buf;
			buf.resize(512);

			Dword bufSize = ::FormatMessage(
				FORMAT_MESSAGE_FROM_SYSTEM,
				0,
				code_,
				static_cast<Dword>(lang),
				&buf[0],
				static_cast<Dword>(buf.size() + 1),
				nullptr
				);

			if (bufSize == 0)
				::MessageBox(nullptr, L"Error", L"buf is 0", 0);

			return buf.substr(
				0,
				bufSize
				);
		}
		Dword getCode() const {
			return code_;
		}
		const TString& getFuncName() const {
			return funcName_;
		}

	private:
		Dword code_ = 0;
		TString funcName_;

	};

	//WinAPI定数用enum組み合わせ型
	template <typename EnumType>
	class UnifyEnum {
	public:
		//enumの内部型
		using ValueType = typename std::underlying_type<EnumType>::type;

		UnifyEnum() = default;
		UnifyEnum(const UnifyEnum<EnumType>&) = default;
		UnifyEnum(UnifyEnum<EnumType>&&) = default;
		UnifyEnum<EnumType>& operator = (const UnifyEnum<EnumType>&) = default;
		UnifyEnum<EnumType>& operator = (UnifyEnum<EnumType>&&) = default;

		constexpr UnifyEnum(const EnumType& val) :
			vals_(static_cast<ValueType>(val)) {}
		constexpr UnifyEnum(ValueType val) :
			vals_(val) {}
		UnifyEnum(const std::initializer_list<ValueType>& valList) {
			for (auto&& val : valList)
				vals_ |= val;
		}
		UnifyEnum(const std::initializer_list<EnumType>& valList) {
			for (auto&& val : valList)
				vals_ |= static_cast<ValueType>(val);
		}

		//内部の値を取得
		ValueType& get() {
			return vals_;
		}
		constexpr const ValueType& get() const {
			return vals_;
		}
		ValueType& operator () () {
			return vals_;
		}
		constexpr const ValueType& operator () () const {
			return vals_;
		}

		//合成
		auto& compose(ValueType val) {
			vals_ |= static_cast<ValueType>(val);
			return *this;
		}
		auto& operator |= (ValueType val) {
			return compose(val);
		}
		UnifyEnum<ValueType> operator | (const EnumType& val) const {
			return UnifyEnum<ValueType>(vals_ | val);
		}

		//要素の削除
		auto& except(const UnifyEnum<EnumType>& val) {
			vals_ &= ~val();

			return *this;
		}

		//含まれているかどうか
		constexpr bool has(const ValueType& val) const {
			return (vals_ & val) != 0;
		}
		constexpr bool has(const EnumType& val) const {
			return (vals_ & static_cast<ValueType>(val)) != 0;
		}
		bool has(const std::initializer_list<EnumType> vals) const {
			return std::find_if(
				vals.begin(),
				vals.end(),
				[&vals_](EnumType& val) { return (vals_ & val) != 0 }
			) == vals.end();
		}

	private:
		ValueType vals_ = ValueType{};

	};

#endif

	//WinAPI構造体のラップ
#ifdef TRUE

	//Window表示形式
	enum class WndShowMode : Word {
		ForceMinimize = SW_FORCEMINIMIZE,
		Hide = SW_HIDE,
		Maximize = SW_MAXIMIZE,
		Minimize = SW_MINIMIZE,
		Restore = SW_RESTORE,
		Show = SW_SHOW,
		DefaultShow = SW_SHOWDEFAULT,
		MaximizedShow = SW_SHOWMAXIMIZED,
		MinimizedShow = SW_SHOWMINIMIZED,
		InactivateMinimized = SW_SHOWMINNOACTIVE,
		NoactivateShow = SW_SHOWNA,
		NormalShow = SW_SHOWNORMAL,
		NoactivateNormalShow = SW_SHOWNOACTIVATE
	};

#endif

} //::wawl