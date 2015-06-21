#pragma once

//WinAPI
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#undef NOMINMAX
//C++STL
#include <string>

namespace wawl {

	//Ascii�����^
	using AChar = ::CHAR;
	//���C�h�����^
	using WChar = ::WCHAR;
	//�ėp�����^
	using TChar = ::TCHAR;

	//Ascii������^
	using AString = std::basic_string < AChar > ;
	//���C�h������^
	using WString = std::basic_string < WChar > ;
	//�ėp������^
	using TString = std::basic_string < TChar > ;

	//��{�I�Ȕėp���W�^
	template <typename Val>
	struct Coordinates {
		using ValueType = Val;

		ValueType x = 0, y = 0;

		//�f�t�H���g�R���X�g���N�^
		Coordinates() = default;
		//�R�s�[�R���X�g���N�^
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