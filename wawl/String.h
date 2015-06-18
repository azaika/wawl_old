#pragma once

//WinAPI
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#undef NOMINMAX
//C++STL
#include <string>

namespace wawl {
	namespace str{

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

		//AStringをWStringに変換します
		WString ToWString(const AString& str) {
			//変換後のサイズ
			size_t wstrLen;
			//サイズ取得&変換エラーチェック
			if (mbstowcs_s(&wstrLen, nullptr, 0, str.c_str(), 0) != 0)
				return WString{};

			WString wstrBuf;
			wstrBuf.reserve(wstrLen);
			if (mbstowcs_s(&wstrLen, &wstrBuf[0], wstrLen, str.c_str(), str.size()) != 0)
				return WString{};
			
			return wstrBuf;

			//std::vector<WChar> wch(size);
			//if (mbstowcs_s(&size, wch.data(), wch.size(), str.c_str(), str.size()) != 0)
			//	return WString{};
			//return WString{wch.begin(), wch.end()};
		}
		//WStringをAStringに変換します
		AString ToAString(const WString& wstr) {
			//変換後のサイズ
			size_t astrLen;
			//サイズ取得&変換エラーチェック
			if (wcstombs_s(&astrLen, nullptr, 0, wstr.c_str(), 0) != 0)
				return AString{};

			AString astrBuf;
			astrBuf.reserve(astrLen);
			if (wcstombs_s(&astrLen, &astrBuf[0], astrLen, wstr.c_str(), wstr.size()) != 0)
				return AString{};

			return astrBuf;
		}


#ifdef UNICODE

		inline TString& ToTString(WString& str) {
			return str;
		}
		inline const TString& ToTString(const WString& str) {
			return str;
		}
		inline TString& ToTString(AString& str) {
			auto&& ret = ToWString(str);
			return ret;
		}
		inline const TString& ToTString(const AString& str) {
			auto&& ret = ToWString(str);
			return ret;
		}
		
#else //UNICODE

		inline TString& ToTString(AString& str) {
			return str;
		}
		inline const TString& ToTString(const AString& str) {
			return str;
		}
		inline TString& ToTString(WString& str) {
			auto&& ret = ToWString(str);
			return ret;
		}
		inline const TString& ToTString(const WString& str) {
			auto&& ret = ToWString(str);
			return ret;
		}

#endif //UNICODE

	} //::wawl::str
} //::wawl