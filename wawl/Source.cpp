#include "System.h"
#include "Input.h"
#include "Utility.h"
#include "FileSystem.h"
#include "Window.h"

//色々usingする
#include "Using.h"

void wawlMain() {
	wnd::RootWindow window;
	try {
		window = wnd::RootWindow(
			L"Kitty",
			wnd::Prop({ wnd::PropOption::HRedraw, wnd::PropOption::VRedraw }),
			{ 640, 480 },
			wnd::Style::Overlapped
			);
	}
	catch (Error& error) {
		mb::show(L"Error", error.msg(), mb::Button::OK);
	}

	while (Sleep(1), true) {
		//終了処理
		if (kb::getState(KeyCode::Escape)) {
			mb::show(L"Application end", L"Good bye", mb::Button::OK);
			break;
		}
		//キーインプットのテスト
		else if (kb::getState(KeyCode::X))
			mb::show(L"KeyX pushed", L"Input successed", mb::Button::OK);
		//ini生成のテスト
		else if (kb::getState(KeyCode::I)) {
			fs::IniFile aaa(L"./test.ini");
			aaa.write(L"Section", L"Key", L"Data");
		}
		//マウス操作のテスト
		else if (kb::getState(KeyCode::One))
			mc::setPos({ 100, 100 });
		//他アプリケーション起動のテスト
		else if (kb::getState(KeyCode::Two)) {
			fs::Process proc(L"notepad.exe", fs::StartupInfo{});
			if(!proc)
				mb::show(L"Error", proc.getError().msg(), mb::Button::OK);
		}
		else if (kb::getState(KeyCode::W)) {
			if(!window.show())
				mb::show(L"Error", L"Failed to ShowWindow.", mb::Button::OK);
		}

	}
}