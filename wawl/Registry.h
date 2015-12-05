#pragma once

//wawl Header
#include "BaseType.h"
#include "FileSystem.h"
#include "Utility.h"

namespace wawl {
	namespace reg {

		enum class Option : Dword {
			BackupRestore = REG_OPTION_BACKUP_RESTORE,
			CreateLink = REG_OPTION_CREATE_LINK,
			NonVolatile = REG_OPTION_NON_VOLATILE,
			OpenLink = REG_OPTION_OPEN_LINK,
			Reserved = REG_OPTION_RESERVED,
			Volatile = REG_OPTION_VOLATILE,
		};

		enum class Desc : Dword {
			AllAccess = KEY_ALL_ACCESS,
			CreateLink = KEY_CREATE_LINK,
			CreateSubKey = KEY_CREATE_SUB_KEY,
			EnamerateSubKey = KEY_ENUMERATE_SUB_KEYS,
			Event = KEY_EVENT,
			Execute = KEY_EXECUTE,
			LengthMask = KEY_LENGTH_MASK,
			Notify = KEY_NOTIFY,
			QueryValue = KEY_QUERY_VALUE,
			Read = KEY_READ,
			SetValue = KEY_SET_VALUE,
			Wow64_32 = KEY_WOW64_32KEY,
			Wow64_64 = KEY_WOW64_64KEY,
			Wow64Res = KEY_WOW64_RES,
			Write = KEY_WRITE,
		};

		enum class Type : Dword {
			Binary = REG_BINARY,
			DWord = REG_DWORD,
			ExpandString = REG_EXPAND_SZ,
			MultiString = REG_MULTI_SZ,
			QWord = REG_QWORD,
			String = REG_SZ,
		};

		class Key {
		public:
			Key() = default;
			Key(Key&&) = default;
			Key& operator = (Key&&) = default;

			Key(const Key&) = delete;
			Key& operator = (const Key&) = delete;

			explicit Key(::HKEY thisKey) :
				hkey_(thisKey) {}

			Key(HKEY currentKey,
				const TString& name,
				const Option& regOption,
				const Desc& keyOption,
				const fs::SecurityAttrib& secAttr
				) {
				Dword disPosition;

				::RegCreateKeyEx(
					currentKey,
					name.c_str(),
					0,
					TEXT(""),
					util::unpackEnum(regOption),
					util::unpackEnum(keyOption),
					const_cast<LPSECURITY_ATTRIBUTES>(&secAttr.get()),
					&hkey_,
					&disPosition
					);
			}
			~Key() {
				::RegCloseKey(hkey_);
			}

			void write(const TString& name, const TString& value) {
				::RegSetValueEx(
					hkey_,
					name.c_str(),
					0,
					util::unpackEnum(Type::String),
					reinterpret_cast<const Byte*>(value.c_str()),
					static_cast<Dword>(value.size() * sizeof(TChar))
					);
			}
			void write(const TString& name, const Dword& value) {
				::RegSetValueEx(hkey_, name.c_str(), NULL, static_cast<UINT>(Type::DWord), (const Byte*)&value, sizeof(Dword));
			}
			void write(const TString& name, const Qword& value) {
				::RegSetValueEx(hkey_, name.c_str(), NULL, static_cast<UINT>(Type::QWord), (const Byte*)&value, sizeof(Qword));
			}

			auto& getValue(const TString& name) {
				Byte *data;
				::RegQueryValueEx(hkey_, name.c_str(), 0, 0, data, 0);
				return data;
			}

			void remove(const TString& name) {
				RegDeleteValue(hkey_, name_.c_str());
			}

			Key getKey(const TString& name) {
				::HKEY retHKey;
				RegOpenKeyEx(hkey_, name.c_str(), 0, 0, &retHKey);
				
				return Key(retHKey);
			}

		private:
			::HKEY hkey_ = nullptr;
			TString name_;
			fs::SecurityDesc secDesc;
			fs::FileTime fileTime_;
			void setInfo() {
				Dword numOfSubkey;
				Dword numOfValues;

				::RegQueryInfoKey(
					hkey_,
					const_cast<TChar*>(name_.c_str()),
					0, 0,
					&numOfSubkey,
					0, 0,
					&numOfValues,
					0, 0, 0,
					&fileTime_()
					);
			}

		};

	} //::wawl::reg
}
