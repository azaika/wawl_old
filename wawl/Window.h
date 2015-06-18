#pragma once

//C++ STL
#include <string>

//WinAPI
#define NOMINMAX

#include <Windows.h>
#include <tchar.h>

#undef NOMINMAX

namespace wawl {
	namespace wnd {

		//TODO : WindowBase�N���X��public�p�������N���X�����

		class WindowBase{
		public:
			//Window�n���h�����擾
			HWND GetHandle() const{
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