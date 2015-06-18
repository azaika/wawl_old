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

		//AString��WString�ɕϊ����܂�
		WString ToWString(const AString& str) {
			//�ϊ���̃T�C�Y
			size_t wstrLen;
			//�T�C�Y�擾&�ϊ��G���[�`�F�b�N
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
		//WString��AString�ɕϊ����܂�
		AString ToAString(const WString& wstr) {
			//�ϊ���̃T�C�Y
			size_t astrLen;
			//�T�C�Y�擾&�ϊ��G���[�`�F�b�N
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