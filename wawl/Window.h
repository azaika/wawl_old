#pragma once

#include "System.h"

namespace wawl {
	namespace wnd {

		//TODO : WindowBaseクラスをpublic継承したクラスを作る

		class WindowBase {
		public:
			//Windowハンドルを取得
			HWND GetHandle() const {
				return handle_;
			}

			//コンストラクタ
			WindowBase();

		private:
			//Windowハンドル
			HWND handle_ = nullptr;

		};

	}
}