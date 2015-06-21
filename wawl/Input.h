#pragma once

#include "System.h"

namespace wawl {
	namespace input {
		
		class Mouse final {
		public:
			Mouse() = delete;
			Mouse(const Mouse&) = delete;
			Mouse(Mouse&&) = delete;
			void operator = (const Mouse&) = delete;

			//マウスの座標を取得
			inline static Position GetPos() {
				LPPOINT posPtr;

				::GetCursorPos(posPtr);
				return Position{ posPtr->x, posPtr->y };
			}
			//マウスの座標を設定
			inline static bool SetMousePos(const Position& p) {
				::SetCursorPos(p.x, p.y) != 0;
			}
			//マウスを表示する
			static bool Show() {
				//既に表示されていたらfalse
				if (showCount_ >= 0)
					return false;

				//表示されるまで繰り返す
				while (showCount_ < 0)
					showCount_ = ::ShowCursor(true);

				return true;
			}
			//マウスを非表示にする
			inline static bool Hide() {
				//既に非表示ならfalse
				if (showCount_ < 0)
					return false;

				//非表示になるまで繰り返す
				while (showCount_ >= 0)
					showCount_ = ::ShowCursor(false);

				return true;
			}

		private:
			static std::size_t showCount_;

		};
		std::size_t Mouse::showCount_ = 0;

	} //::wawl::input
} //::wawl