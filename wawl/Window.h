#pragma once

#include "System.h"

namespace wawl {
	namespace wnd {

		//TODO : WindowBase�N���X��public�p�������N���X�����

		class WindowBase {
		public:
			//Window�n���h�����擾
			HWND GetHandle() const {
				return handle_;
			}

			//�R���X�g���N�^
			WindowBase();

		private:
			//Window�n���h��
			HWND handle_ = nullptr;

		};

	}
}