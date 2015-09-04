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
		//符号なし64bit整数
		using Qword = unsigned long long;

		//ポインタを格納する整数型
		using IntPtr = ::INT_PTR;
		using UintPtr = ::UINT_PTR;

#endif

	//ライブラリの都合的な型
#ifdef TRUE

		//基本的な汎用座標型
		template <typename VTy>
		struct Coordinates {

			using ValueType = VTy;

			ValueType x = 0, y = 0;

			//デフォルトコンストラクタ
			Coordinates() = default;

			//コピーコンストラクタ
			template <typename T>
			Coordinates(const Coordinates<T>& p_) :
				x(static_cast<ValueType>(p_.x)),
				y(static_cast<ValueType>(p_.y)) {}

			Coordinates(const ValueType& x_, const ValueType& y_) :
				x(x_), y(y_) {}

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
			_impl_UnifyEnum(_impl_UnifyEnum<EnumType>&&) = default;
			_impl_UnifyEnum<EnumType>& operator = (const _impl_UnifyEnum<EnumType>&) = default;
			_impl_UnifyEnum<EnumType>& operator = (_impl_UnifyEnum<EnumType>&&) = default;

			constexpr _impl_UnifyEnum(const EnumType& val) :
				vals_(static_cast<ValueType>(val)) {}
			constexpr _impl_UnifyEnum(const std::initializer_list<ValueType>& valList) {
				for (auto&& val : valList)
					vals_ |= val;
			}

			//内部の値を取得
			ValueType& get() {
				return vals_;
			}
			constexpr const ValueType& get() const {
				return vals_;
			}
			ValueType& operator () () {
				return vals_;
			}
			constexpr const ValueType& operator () () const{
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
			constexpr bool isIncluded(const ValueType& val) const {
				return (vals_ & val) != 0;
			}
			constexpr bool isIncluded(const EnumType& val) const {
				return (vals_ & static_cast<ValueType>(val)) != 0;
			}
			bool isIncluded(const std::initializer_list<EnumType> vals) const {
				return std::find_if(
					vals.begin(),
					vals.end(),
					[&vals_](EnumType& val) { return (vals_ & val) != 0 }
				) == vals.end();
			}

		private:
			ValueType vals_ = ValueType{};

		};

#endif

	//WinAPI構造体のラップ
#ifdef TRUE

		//test code
		enum class WndShowmode_ : Dword {
			Hide = 1,
			Show = 2,
			Restore = 4,

			Active = 8,
			Inactive = 16,

			Max = 32,
			Min = 64,
			Normal = 128,

			Default = 256,
			ForceMin = 512
		};
		using UnifyWndShowmode_ = _impl_UnifyEnum < WndShowmode_ >;
		Word _impl_toSWConstant(const UnifyWndShowmode_& sw) {
			if (sw.isIncluded(WndShowmode_::Show)) {
				if (sw.isIncluded(WndShowmode_::Max))
					return SW_SHOWMAXIMIZED;
				else if (sw.isIncluded(WndShowmode_::Min)) {
					if (sw.isIncluded(WndShowmode_::Inactive))
						return SW_SHOWMINNOACTIVE;
					else
						return SW_SHOWMINIMIZED;
				}
				else if (sw.isIncluded(WndShowmode_::Normal)) {
					if (sw.isIncluded(WndShowmode_::Inactive))
						return SW_SHOWNOACTIVATE;
					else
						return SW_SHOWNORMAL;
				}
				else if (sw.isIncluded(WndShowmode_::Inactive))
					return SW_SHOWNA;
				else
					return SW_SHOW;
			}
			else if (sw.isIncluded(WndShowmode_::Default))
				return SW_SHOWDEFAULT;
			else if (sw.isIncluded(WndShowmode_::ForceMin))
				return SW_FORCEMINIMIZE;
		}
		//~test code

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
			NormalShow = SW_SHOWNORMAL,
			NoactivateNormalShow = SW_SHOWNOACTIVATE
		};
		using UnifyWndShowMode = _impl_UnifyEnum < WndShowMode > ;
		
#endif

} //::wawl