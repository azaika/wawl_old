#pragma once

//C++ STL
#include <string>
#include <sstream>

namespace wawl {
	namespace type{

#ifdef UNICODE

		using String = std::wstring;
		using StringStream = std::wstringstream;

#else //UNICODE

		using String = std::string;
		using StringStream = std::stringstream;

#endif //UNICODE

		template <typename... Ts>
		String ConvertStringFormat(const Ts&... args) {
			return String{};
		}

		//string‚ðwstring‚É•ÏŠ·
		std::wstring ToWString(const std::string& str) {
			return std::wstring{};
		}

	}
}