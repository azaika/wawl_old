#pragma once

#include "System.h"

namespace wawl {
	namespace util {

		//AString��WString�ɕϊ����܂�
		WString toWString(const AString& str) {
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
		AString toAString(const WString& wstr) {
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
		
		inline const TString& toTString(const WString& str) {
			return str;
		}
		inline const TString& toTString(const AString& str) {
			auto&& ret = toWString(str);
			return ret;
		}

#else //UNICODE

		inline const TString& toTString(const AString& str) {
			return str;
		}
		inline const TString& toTString(const WString& str) {
			auto&& ret = toWString(str);
			return ret;
		}

#endif //UNICODE

	} //::wawl::util
} //::wawl