#include "System.h"
#include "Input.h"

void WawlMain() {
	using namespace wawl;
	using namespace input;

	while (Sleep(1), Keyboard::Update(), true) {

		if ((Keyboard::Get(Key::Escape).Pushed())) {
			::MessageBox(nullptr, L"Good bye", L"Application end", MB_OK);
			break;
		}
		if (Keyboard::Get(Key::X).Released())
			::MessageBox(nullptr, L"Input successed", L"KeyX pushed", MB_OK);
		else if (Keyboard::Get(Key::One).Clicked())
			Mouse::SetPos({ 100, 100 });
		else if (Keyboard::Get(Key::Two).Clicked())
			sys::RunCmdLine(L"cmd /k ipconfig", true);

	}
}