#include "System.h"
#include "Input.h"
#include "Utility.h"
#include "FileSystem.h"
#include "Window.h"

//色々usingする
#include "Using.h"

void wawlMain() {
	//utilのテスト
	constexpr auto a = unpackEnum(fs::AccessDesc::All);

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
			mouse::setPos({ 100, 100 });
		//他アプリケーション起動のテスト
		else if (kb::getState(KeyCode::Two)) {
			try {
				fs::Process(L"notepad.exe", fs::StartupInfo{});
			}
			catch (Error& error) {
				mb::show(L"Error", error.msg(), mb::Button::OK);
			}
		}

	}
}