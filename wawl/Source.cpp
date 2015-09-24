#include "System.h"
#include "Input.h"
#include "Utility.h"
#include "FileSystem.h"
#include "Window.h"

void wawlMain() {
	using namespace wawl;
	using namespace input;

	constexpr auto a = util::unpackEnum(fs::AccessDesc::All);

	while (Sleep(1), true) {

		if (getKeyState(Key::Escape)) {
			wnd::showMessage(L"Application end", L"Good bye", wnd::ChoiceButton::OK);
			break;
		}
		else if (getKeyState(Key::X))
			wnd::showMessage(L"KeyX pushed", L"Input successed", wnd::ChoiceButton::OK);
		else if (getKeyState(Key::I)) {
			fs::IniFile aaa(L"./test.ini");
			aaa.writeData(L"Section", L"Key", L"Data");
		}
		else if (getKeyState(Key::One))
			setMousePos({ 100, 100 });
		else if (getKeyState(Key::Two)) {
			try {
				fs::Process(L"notepad.exe", fs::StartupInfo{});
			}
			catch (std::runtime_error& re) {
				wnd::showMessage(L"Error", util::toTString(re.what()).c_str(), wnd::ChoiceButton::OK);
			}
		}

	}
}