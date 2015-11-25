#pragma once

#define WAWL_ENABLE_UTILITY

#include "BaseType.h"

namespace wawl {
	namespace util {

		//AStringをWStringに変換
		WString toWString(const AString& str) {
			//変換後のサイズ
			size_t wstrLen;

			//サイズ取得&変換エラーチェック
			if (mbstowcs_s(&wstrLen, nullptr, 0, str.c_str(), 0) != 0)
				return WString();

			//変換
			WString wstrBuf(wstrLen, L'\0');
			if (mbstowcs_s(&wstrLen, &wstrBuf[0], wstrLen, str.c_str(), str.size()) != 0)
				wstrBuf.clear();

			return wstrBuf;
		}
		//WStringをAStringに変換
		AString toAString(const WString& wstr) {
			//変換後のサイズ
			size_t astrLen;

			//サイズ取得&変換エラーチェック
			if (wcstombs_s(&astrLen, nullptr, 0, wstr.c_str(), 0) != 0)
				return AString();

			//変換
			AString astrBuf(astrLen, '\0');
			if (wcstombs_s(&astrLen, &astrBuf[0], astrLen, wstr.c_str(), wstr.size()) != 0)
				astrBuf.clear();

			return astrBuf;
		}

#ifdef UNICODE

		inline TString toTString(const WString& str) {
			return TString(str);
		}
		inline TString toTString(const AString& str) {
			return toWString(str);
		}
		template<typename T>
		inline TString valToTStr(const T& val) {
			return std::to_wstring(val);
		}

#else //UNICODE

		inline TString toTString(const AString& str) {
			return TString(str);
		}
		inline TString toTString(const WString& str) {
			return toAString(str);
		}
		template<typename T>
		inline TString valToStr(const T& val) {
			return std::to_string(val);
		}

#endif //UNICODE

		//enumまたはenum classの内部の値を取得
		template <typename EnumType>
		inline constexpr auto unpackEnum(const EnumType& val) {
			return static_cast<typename std::underlying_type<EnumType>::type>(val);
		}

	} //::wawl::util
} //::wawl