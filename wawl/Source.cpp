#include "System.h"
#include "Input.h"

void WawlMain() {
	using namespace wawl::input;

	while (Sleep(1), !((Keyboard::Get(Key::Escape).Pressed()))) {
		if (Keyboard::Get(Key::X).Pressed())
			::MessageBox(nullptr, L"Input successed", L"KeyX pushed", MB_OK);
	}
}