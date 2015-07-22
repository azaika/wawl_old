#pragma once

//WinAPI
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#undef NOMINMAX
//C++STL
#include <string>

namespace wawl {

	//組み込み型using
#ifdef TRUE

		//Ascii文字型
		using AChar = ::CHAR;
		//ワイド文字型
		using WChar = ::WCHAR;
		//汎用文字型
		using TChar = ::TCHAR;

		//Ascii文字列型
		using AString = std::basic_string < AChar > ;
		//ワイド文字列型
		using WString = std::basic_string < WChar > ;
		//汎用文字列型
		using TString = std::basic_string < TChar > ;

		//符号無し8bit整数
		using Byte = ::BYTE;
		//符号無し16bit整数
		using Word = ::WORD;
		//符号無し32bit整数
		using Dword = ::DWORD;

		//ハンドル
		using GeneralHandle = ::HANDLE;

#endif

	//ライブラリの都合的な型
#ifdef TRUE

		//基本的な汎用座標型
		template <typename ValueType>
		struct Coordinates {

			ValueType x = 0, y = 0;

			//デフォルトコンストラクタ
			Coordinates() = default;

			//コピーコンストラクタ
			template <typename T>
			Coordinates(const Coordinates<T>& p_) :
				x(static_cast<ValueType>(p_.x)),
				y(static_cast<ValueType>(p_.y)) {}

			template <typename T>
			Coordinates(const T& x_, const T& y_) :
				x(static_cast<ValueType>(x_)),
				y(static_cast<ValueType>(y_)) {}

			template <typename T>
			void operator = (const Coordinates<T>& p_) {
				x = static_cast<ValueType>(p_.x);
				y = static_cast<ValueType>(p_.y);
			}

		};

		using Position = Coordinates < int > ;
		using Rectangle = Position;

		//汎用enum組み合わせ型
		template <typename EnumType>
		class _impl_UnifyEnum {
		public:
			//enumの内部型
			using ValueType = typename std::underlying_type<EnumType>::type;

			_impl_UnifyEnum() = default;
			_impl_UnifyEnum(const _impl_UnifyEnum<EnumType>&) = default;
			_impl_UnifyEnum<EnumType>& operator = (const _impl_UnifyEnum<EnumType>&) = default;

			_impl_UnifyEnum(const EnumType& val) :
				vals_(static_cast<ValueType>(val)) {}
			_impl_UnifyEnum(const std::initializer_list<ValueType>& valList) {
				for (auto&& val : valList)
					vals_ |= val;
			}

			//内部の値を取得
			ValueType& get() {
				return vals_;
			}
			const ValueType& get() const {
				return vals_;
			}
			ValueType& operator () () {
				return vals_;
			}
			const ValueType& operator () () const{
				return vals_;
			}

			//合成
			_impl_UnifyEnum<ValueType>& compose(const ValueType& val) {
				vals_ |= static_cast<ValueType>(val);
				return *this;
			}
			_impl_UnifyEnum<ValueType>& operator |= (const ValueType& val) {
				return compose(val);
			}
			_impl_UnifyEnum<ValueType>& operator | (const EnumType& val) const {
				return _impl_UnifyEnum < ValueType > {vals_ | val};
			}

			//含まれているかどうか
			bool isIncluded(const ValueType& val) {
				return (vals_ & val) == val;
			}

		private:
			ValueType vals_ = ValueType{};

		};

#endif

	//WinAPI構造体のラップ
#ifdef TRUE

		//セキュリティ指定子
		class SecurityDesc {
		public:
			SecurityDesc() {
				::InitializeSecurityDescriptor(&secDesc_, SECURITY_DESCRIPTOR_REVISION);
			}
			SecurityDesc(const SecurityDesc&) = default;

			//内部の値を取得
			::SECURITY_DESCRIPTOR& get() {
				return secDesc_;
			}
			const ::SECURITY_DESCRIPTOR& get() const {
				return secDesc_;
			}
			::SECURITY_DESCRIPTOR& operator () () {
				return secDesc_;
			}
			const ::SECURITY_DESCRIPTOR& operator () () const{
				return secDesc_;
			}

		private:
			::SECURITY_DESCRIPTOR secDesc_;

		};
		//セキュリティ記述子
		class SecurityAttrib{
		public:
			SecurityAttrib(bool doInheritHandle, SecurityDesc& secDesc) {
				secAttr_.nLength = sizeof(::SECURITY_ATTRIBUTES);
				secAttr_.bInheritHandle = doInheritHandle;
				secAttr_.lpSecurityDescriptor = &secDesc;
			}
			explicit SecurityAttrib(bool doInheritHandle = false) {
				secAttr_.nLength = sizeof(::SECURITY_ATTRIBUTES);
				secAttr_.bInheritHandle = doInheritHandle;
				secAttr_.lpSecurityDescriptor = nullptr;
			}
			SecurityAttrib(const SecurityAttrib& secAttr) = default;
			SecurityAttrib(SecurityAttrib&& secAttr) :
				secAttr_(std::move(secAttr.get())) {}

			//内部の値を取得
			::SECURITY_ATTRIBUTES& get() {
				return secAttr_;
			}
			const ::SECURITY_ATTRIBUTES& get() const{
				return secAttr_;
			}
			::SECURITY_ATTRIBUTES& operator () () {
				return secAttr_;
			}
			const ::SECURITY_ATTRIBUTES& operator () () const {
				return secAttr_;
			}
			
		private:
			::SECURITY_ATTRIBUTES secAttr_;

		};

		//CUIでの文字列色
		enum class ConsoleStrColor : Dword {
			Blue = FOREGROUND_BLUE,
			Green = FOREGROUND_GREEN,
			Red = FOREGROUND_RED,
			Intensity = FOREGROUND_INTENSITY
		};
		using UnifyConsoleStrColor = _impl_UnifyEnum < ConsoleStrColor > ;

		//CUIでの背景色
		enum class ConsoleBgColor : Dword{
			Blue = BACKGROUND_BLUE,
			Green = BACKGROUND_GREEN,
			Red = BACKGROUND_RED,
			Intensity = BACKGROUND_INTENSITY
		};
		using UnifyConsoleBgColor = _impl_UnifyEnum < ConsoleBgColor > ;

		//アプリケーション起動時のオプション
		enum class StartupOption : Dword {
			Fullscreen = STARTF_RUNFULLSCREEN,
			ForceChangeCursor = STARTF_FORCEONFEEDBACK,
			ForceImmutCursor = STARTF_FORCEOFFFEEDBACK,
			NoPinningTaskbar = STARTF_PREVENTPINNING || STARTF_TITLEISAPPID,
			RelateTitleWithAppID = STARTF_TITLEISAPPID,
			RelateTitleWithLnk = STARTF_TITLEISLINKNAME
		};
		using UnifyStartupOption = _impl_UnifyEnum < StartupOption > ;

		//Window表示形式
		enum class WndShowMode : Word {
			ForceMinimize = SW_FORCEMINIMIZE,
			Hide = SW_HIDE,
			Maximize = SW_MAXIMIZE,
			Minimize = SW_MINIMIZE,
			Restore = SW_RESTORE,
			Show = SW_SHOW,
			DefaultShow = SW_SHOWDEFAULT,
			MaximizedShow = SW_SHOWMAXIMIZED,
			MinimizedShow = SW_SHOWMINIMIZED,
			InactivateMinimized = SW_SHOWMINNOACTIVE,
			NoactivateShow = SW_SHOWNA,
			NoactivateNormalShow = SW_SHOWNOACTIVATE,
			NormalShow = SW_SHOWNORMAL
		};
		using UnifyWndShowMode = _impl_UnifyEnum < WndShowMode > ;
		
#endif

} //::wawl