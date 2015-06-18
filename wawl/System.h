#pragma once

//WinAPI
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#undef NOMINMAX

#include "String.h"

//ユーザー定義Main
void WawlMain();

namespace wawl {
	namespace sys {

		using AppHandle = HINSTANCE;

		class System final{
		public:
			inline static AppHandle GetAppHandle() {
				return static_cast<HINSTANCE>(::GetModuleHandle(0));
			}

			inline static str::TString GetCmdLine() {
				return cmdArgStr;
			}
			inline static str::TString GetFullCmdLine() {
				return ::GetCommandLine();
			}

			inline static int GetWindowShowmode() {
				return windowShowmode;
			}

			static void _impl_SetWinMainArgs(const str::TString& cmdLine, int cmdShow) {
				//初回呼び出しかどうか
				static bool isFirst = true;

				if (isFirst) { //初回呼び出しならば
					isFirst = false;
					cmdArgStr = cmdLine;
					windowShowmode = cmdShow;
				}
			}

		private:
			static str::TString cmdArgStr;
			static int windowShowmode;

		};
		str::TString System::cmdArgStr;
		int System::windowShowmode = 0;

	}
}

int WINAPI _tWinMain(
	HINSTANCE hInst,
	HINSTANCE hPrevInst,
	LPTSTR cmdLine,
	int cmdShow
	) {
	using namespace wawl;
	sys::System::_impl_SetWinMainArgs(cmdLine, cmdShow);

	return 0;
}