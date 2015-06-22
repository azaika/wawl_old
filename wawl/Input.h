#pragma once

#include "System.h"
#include <unordered_map>

namespace wawl {
	namespace input {
		
		class Mouse final {
		public:
			Mouse() = delete;
			Mouse(const Mouse&) = delete;
			Mouse(Mouse&&) = delete;
			void operator = (const Mouse&) = delete;

			//マウスのスクリーン座標を取得
			inline static Position GetPos() {
				LPPOINT posPtr;

				::GetCursorPos(posPtr);
				return Position{ posPtr->x, posPtr->y };
			}
			//マウスのスクリーン座標を設定
			inline static bool SetMousePos(const Position& p) {
				::SetCursorPos(p.x, p.y) != 0;
			}
			//マウスを表示する
			static bool Show() {
				//既に表示されていたらfalse
				if (showCount_ >= 0)
					return false;

				//表示されるまで繰り返す
				while (showCount_ < 0)
					showCount_ = ::ShowCursor(true);

				return true;
			}
			//マウスを非表示にする
			static bool Hide() {
				//既に非表示ならfalse
				if (showCount_ < 0)
					return false;

				//非表示になるまで繰り返す
				while (showCount_ >= 0)
					showCount_ = ::ShowCursor(false);

				return true;
			}

		private:
			static std::size_t showCount_;

		};
		std::size_t Mouse::showCount_ = 0;

		enum class Key : int {
			MouseL = VK_LBUTTON,
			MouseR = VK_RBUTTON,
			Break = VK_CANCEL,
			MouseMiddle = VK_MBUTTON,
			MouseX1 = VK_XBUTTON1,
			MouseX2 = VK_XBUTTON2,
			Backspace = VK_BACK,
			Tab = VK_TAB,
			Clear = VK_CLEAR,
			Enter = VK_RETURN,
			Shift = VK_SHIFT,
			Control = VK_CONTROL,
			Alt = VK_MENU,
			Pause = VK_PAUSE,
			Capslock = VK_CAPITAL,
			Kana = VK_KANA,
			Junja = VK_JUNJA,
			Final = VK_FINAL,
			Hanja = VK_HANJA,
			Kanji = VK_KANJI,
			Escape = VK_ESCAPE,
			Convert = VK_CONVERT,
			NonConvert = VK_NONCONVERT,
			ImeAccept = VK_ACCEPT,
			ImeModeChange = VK_MODECHANGE,
			Space = VK_SPACE,
			Pageup = VK_PRIOR,
			Pagedown = VK_NEXT,
			End = VK_END,
			Hoge = VK_HOME,
			Left = VK_LEFT,
			Up = VK_UP,
			Right = VK_RIGHT,
			Down = VK_DOWN,
			Select = VK_SELECT,
			Print = VK_PRINT,
			Execute = VK_EXECUTE,
			PrintScreen = VK_SNAPSHOT,
			Insert = VK_INSERT,
			Delete = VK_DELETE,
			Help = VK_HELP,
			Zero = '0',
			One = '1',
			Two = '2',
			Three = '3',
			Four = '4',
			Five = '5',
			Six = '6',
			Seven = '7',
			Eight = '8',
			Nine = '9',
			A = 'A',
			B = 'B',
			C = 'C',
			D = 'D',
			E = 'E',
			F = 'F',
			G = 'G',
			H = 'H',
			I = 'I',
			J = 'J',
			K = 'K',
			L = 'L',
			M = 'M',
			N = 'N',
			O = 'O',
			P = 'P',
			Q = 'Q',
			R = 'R',
			S = 'S',
			T = 'T',
			U = 'U',
			V = 'V',
			W = 'W',
			X = 'X',
			Y = 'Y',
			Z = 'Z',
			WinL = VK_LWIN,
			WinR = VK_RWIN,
			Application = VK_APPS,
			Sleep = VK_SLEEP,
			Num0 = VK_NUMPAD0,
			Num1 = VK_NUMPAD1,
			Num2 = VK_NUMPAD2,
			Num3 = VK_NUMPAD3,
			Num4 = VK_NUMPAD4,
			Num5 = VK_NUMPAD5,
			Num6 = VK_NUMPAD6,
			Num7 = VK_NUMPAD7,
			Num8 = VK_NUMPAD8,
			Num9 = VK_NUMPAD9,
			Multiply = VK_MULTIPLY,
			Add = VK_ADD,
			Separator = VK_SEPARATOR,
			Subtract = VK_SUBTRACT,
			Decimal = VK_DECIMAL,
			Divide = VK_DIVIDE,
			F1 = VK_F1,
			F2 = VK_F2,
			F3 = VK_F3,
			F4 = VK_F4,
			F5 = VK_F5,
			F6 = VK_F6,
			F7 = VK_F7,
			F8 = VK_F8,
			F9 = VK_F9,
			F10 = VK_F10,
			F11 = VK_F11,
			F12 = VK_F12,
			F13 = VK_F13,
			F14 = VK_F14,
			F15 = VK_F15,
			F16 = VK_F16,
			F17 = VK_F17,
			F18 = VK_F18,
			F19 = VK_F19,
			F20 = VK_F20,
			F21 = VK_F21,
			F22 = VK_F22,
			F23 = VK_F23,
			F24 = VK_F24,
			Numlock = VK_NUMLOCK,
			Scrolllock = VK_SCROLL,
			ShiftL = VK_LSHIFT,
			ShiftR = VK_RSHIFT,
			ControlL = VK_LCONTROL,
			ControlR = VK_RCONTROL,
			AltL = VK_LMENU,
			AltR = VK_RMENU,
			BrowserBack = VK_BROWSER_BACK,
			BrowserForward = VK_BROWSER_FORWARD,
			BrowserRefresh = VK_BROWSER_REFRESH,
			BrowserStop = VK_BROWSER_STOP,
			BrowserSearch = VK_BROWSER_SEARCH,
			BrowserFavorites = VK_BROWSER_FAVORITES,
			BrowserHome = VK_BROWSER_HOME,
			Mute = VK_VOLUME_MUTE,
			VolumeDown = VK_VOLUME_DOWN,
			VolumeUp = VK_VOLUME_UP,
			NextTrack = VK_MEDIA_NEXT_TRACK,
			PreviousTrack = VK_MEDIA_PREV_TRACK,
			MediaStop = VK_MEDIA_STOP,
			PlayPause = VK_MEDIA_PLAY_PAUSE,
			Mail = VK_LAUNCH_MAIL,
			MediaSelect = VK_LAUNCH_MEDIA_SELECT,
			AppLaunch1 = VK_LAUNCH_APP1,
			AppLaunch2 = VK_LAUNCH_APP2,
			Others1 = VK_OEM_1,
			Plus = VK_OEM_PLUS,
			Comma = VK_OEM_COMMA,
			Minus = VK_OEM_MINUS,
			Period = VK_OEM_PERIOD,
			Others2 = VK_OEM_2,
			Others3 = VK_OEM_3,
			Others4 = VK_OEM_4,
			Others5 = VK_OEM_5,
			Others6 = VK_OEM_6,
			Others7 = VK_OEM_7,
			Others8 = VK_OEM_8,
			Other102 = VK_OEM_102,
			Process = VK_PROCESSKEY,
			SpecialPacket = VK_PACKET,
			Attention = VK_ATTN,
			CursorSelect = VK_CRSEL,
			ExSelect = VK_EXSEL,
			EraseEof = VK_EREOF,
			Play = VK_PLAY,
			Zoom = VK_ZOOM,
			PA1 = VK_PA1,
			Clear = VK_OEM_CLEAR,
			_impl_EndEnum = 0xFF
		};

		class KeyState {
		public:
			//このフレームに押されているか
			inline bool Pushed() const {
				return nowState_;
			}
			//クリックした瞬間かどうか
			inline bool Clicked() const {
				return !beforeState_ && nowState_;
			}
			//押し続けているかどうか
			inline bool Pressed() const {
				return beforeState_ && nowState_;
			}
			//キーを離した瞬間かどうか
			inline bool Released() const {
				return beforeState_ && !nowState_;
			}

			//ターゲットキーを設定
			inline void SetTarget(const Key key) {
				target_ = key;
			}
			//stateをアップデート
			inline KeyState& Update() {
				beforeState_ = nowState_;
				nowState_ = ::GetAsyncKeyState(static_cast<int>(target_)) & 0x8000;

				return *this;
			}

			KeyState() = default;
			KeyState(const KeyState&) = default;
			KeyState(Key key) :
				target_(key) {}

			KeyState& operator=(KeyState&& other) {
				if (this != &other){
					this->target_ = std::move(other.target_);
					this->beforeState_ = std::move(other.beforeState_);
					this->nowState_ = std::move(other.nowState_);
				}

				return *this;
			}

		private:
			//ターゲットキー
			Key target_;
			//過去のステートと現在のステート
			bool beforeState_ = false, nowState_ = false;

		};

		class Keyboard final {
		public:
			Keyboard() = delete;
			Keyboard(const Keyboard&) = delete;
			Keyboard(Keyboard&&) = delete;
			void operator = (const Keyboard&) = delete;

			const KeyState& Get(const Key key) {
				//初めての呼び出しならメモリを事前に確保
				static bool isFirst = true;
				if (isFirst)
					keyStates.reserve(static_cast<std::size_t>(Key::_impl_EndEnum)),
					isFirst = false;

				keyStates[key] = std::move(KeyState{ keyStates[key].Update() });

				return keyStates[key];
			}

		private:
			static std::unordered_map < Key, KeyState > keyStates;

		};

	} //::wawl::input
} //::wawl