#include "System.h"
#include "Input.h"
#include "Utility.h"
#include "FileSystem.h"
#include "Window.h"

//�F�Xusing����
#include "Using.h"

void wawlMain() {
	//util�̃e�X�g
	constexpr auto a = unpackEnum(fs::AccessDesc::All);

	while (Sleep(1), true) {
		//�I������
		if (kb::getState(KeyCode::Escape)) {
			mb::show(L"Application end", L"Good bye", mb::Button::OK);
			break;
		}
		//�L�[�C���v�b�g�̃e�X�g
		else if (kb::getState(KeyCode::X))
			mb::show(L"KeyX pushed", L"Input successed", mb::Button::OK);
		//ini�����̃e�X�g
		else if (kb::getState(KeyCode::I)) {
			fs::IniFile aaa(L"./test.ini");
			aaa.write(L"Section", L"Key", L"Data");
		}
		//�}�E�X����̃e�X�g
		else if (kb::getState(KeyCode::One))
			mouse::setPos({ 100, 100 });
		//���A�v���P�[�V�����N���̃e�X�g
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