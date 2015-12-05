#pragma once

#ifdef WAWL_ENABLE
	using namespace ::wawl;

#ifdef WAWL_ENABLE_UTILITY
	using namespace ::wawl::util;
#endif

#ifdef WAWL_ENABLE_INPUT
	using namespace ::wawl::input;
	using KeyCode = ::wawl::input::kb::Code;
	namespace mc = ::wawl::input::mouse;
#endif

#ifdef WAWL_ENABLE_WINDOW
	namespace mb = ::wawl::wnd::mb;
	using ::wawl::wnd::Msg;
#endif

#endif