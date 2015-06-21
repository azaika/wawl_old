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

			//�}�E�X�̍��W���擾
			inline static Position GetPos() {
				LPPOINT posPtr;

				::GetCursorPos(posPtr);
				return Position{ posPtr->x, posPtr->y };
			}
			//�}�E�X�̍��W��ݒ�
			inline static bool SetMousePos(const Position& p) {
				::SetCursorPos(p.x, p.y) != 0;
			}
			//�}�E�X��\������
			static bool Show() {
				//���ɕ\������Ă�����false
				if (showCount_ >= 0)
					return false;

				//�\�������܂ŌJ��Ԃ�
				while (showCount_ < 0)
					showCount_ = ::ShowCursor(true);

				return true;
			}
			//�}�E�X���\���ɂ���
			inline static bool Hide() {
				//���ɔ�\���Ȃ�false
				if (showCount_ < 0)
					return false;

				//��\���ɂȂ�܂ŌJ��Ԃ�
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