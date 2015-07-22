#pragma once

#include "BaseType.h"

namespace wawl {
	namespace util {

		//String関連
#ifdef TRUE

		//AStringをWStringに変換します
		WString toWString(const AString& str) {
			//変換後のサイズ
			size_t wstrLen;
			//サイズ取得&変換エラーチェック
			if (mbstowcs_s(&wstrLen, nullptr, 0, str.c_str(), 0) != 0)
				throw std::runtime_error{ "Falied to mbstowcs_s" };

			WString wstrBuf;
			wstrBuf.reserve(wstrLen);
			if (mbstowcs_s(&wstrLen, &wstrBuf[0], wstrLen, str.c_str(), str.size()) != 0)
				throw std::runtime_error{ "Falied to mbstowcs_s" };

			return wstrBuf;

			//std::vector<WChar> wch(size);
			//if (mbstowcs_s(&size, wch.data(), wch.size(), str.c_str(), str.size()) != 0)
			//	return WString{};
			//return WString{wch.begin(), wch.end()};
		}
		//WStringをAStringに変換します
		AString toAString(const WString& wstr) {
			//変換後のサイズ
			size_t astrLen;
			//サイズ取得&変換エラーチェック
			if (wcstombs_s(&astrLen, nullptr, 0, wstr.c_str(), 0) != 0)
				throw std::runtime_error{ "Falied to wcstombs_s" };

			AString astrBuf;
			astrBuf.reserve(astrLen);
			if (wcstombs_s(&astrLen, &astrBuf[0], astrLen, wstr.c_str(), wstr.size()) != 0)
				throw std::runtime_error{ "Falied to wcstombs_s" };

			return astrBuf;
		}

#ifdef UNICODE
		
		inline TString toTString(const WString& str) {
			return TString{ str };
		}
		inline TString toTString(const AString& str) {
			return toWString(str);
		}

#else //UNICODE

		inline TString toTString(const AString& str) {
			return TString{ str };
		}
		inline TString toTString(const WString& str) {
			return toWString(str);
		}

#endif //UNICODE

#endif //TRUE

#ifdef TRUE

		template <typename EnumType>
		inline auto unpackEnum(const EnumType& val)
			-> typename std::underlying_type<EnumType>::type {
			return static_cast<typename std::underlying_type<EnumType>::type>(val);
		}

#endif //TRUE

	} //::wawl::util
} //::wawl