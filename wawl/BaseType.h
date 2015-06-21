#pragma once

//WinAPI
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#undef NOMINMAX
//C++STL
#include <string>

namespace wawl {

	//Ascii文字型
	using AChar = ::CHAR;
	//ワイド文字型
	using WChar = ::WCHAR;
	//汎用文字型
	using TChar = ::TCHAR;

	//Ascii文字列型
	using AString = std::basic_string < AChar > ;
	//ワイド文字列型
	using WString = std::basic_string < WChar > ;
	//汎用文字列型
	using TString = std::basic_string < TChar > ;

	//基本的な汎用座標型
	template <typename Val>
	struct Coordinates {
		using ValueType = Val;

		ValueType x = 0, y = 0;

		//デフォルトコンストラクタ
		Coordinates() = default;
		//コピーコンストラクタ
		template <typename T>
		explicit Coordinates(const Coordinates<T>& p_) :
			x(static_cast<ValueType>(p_.x)),
			y(static_cast<ValueType>(p_.y)) {}
		template <typename T>
		Coordinates(const T& x_, const T& y_) :
			x(static_cast<ValueType>(x_)),
			y(static_cast<ValueType>(y_)) {}

		template <typename T>
		void operator = (const Coordinates<T>& p_) {
			x = static_cast<ValueType>(p_.x);
			y = static_cast<ValueType>(p_.y);
		}
		
	};

	using Position = Coordinates < int > ;

} //::wawl