#include "System.h"
#include "Input.h"
#include "Utility.h"
#include "FileSystem.h"
#include "Window.h"

void WawlMain() {
	using namespace wawl;
	using namespace input;

	constexpr auto a = util::unpackEnum(fs::AccessDesc::All);

	while (Sleep(1), true) {

		if (getKeyState(Key::Escape)) {
			::MessageBox(nullptr, L"Good bye", L"Application end", MB_OK);
			break;
		}
		if (getKeyState(Key::X))
			::MessageBox(nullptr, L"Input successed", L"KeyX pushed", MB_OK);
		else if (getKeyState(Key::One))
			setMousePos({ 100, 100 });
		else if (getKeyState(Key::Two))
			sys::runCmdLine(L"cmd /k ipconfig", true);

	}
}