#pragma once

#include <memory>

#include "Fire/Core/PlatformDetection.h"


// DLL support 						//// Clearly only needed when compiling using Dynamic linking
#ifdef FR_PLATFORM_WINDOWS
	#if FR_DYNAMIC_LINK
		#ifdef FR_BUILD_DLL
			#define FIRE_API __declspec(dllexport)
		#else
			#define FIRE_API __declspec(dllimport)
		#endif
	#else
		#define FIRE_API
	#endif
#else
//	#error Fire only supports Windows!
	#define FIRE_API		// Blank for MacOS - need to verify on other non-Windows platforms
#endif // End of DLL support

#ifdef FR_DEBUG
	#define FR_ENABLE_ASSERTS
#endif

// TODO: Make this macro able to take in no arguments except condition
#ifdef FR_ENABLE_ASSERTS
	#define FR_ASSERT(x, ...) { if(!(x)) { FR_ERROR("Assertion failed: {0}",__VA_ARGS__); } }//__debugbreak(); } }
	#define FR_CORE_ASSERT(x, ...) { if(!(x)) { FR_CORE_ERROR("Assertion failed: {0}",__VA_ARGS__); } };//__debugbreak(); } }
#else
	#define FR_ASSERT(x, ...)
	#define FR_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define FR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
//#define HZ_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Fire {

	template<typename T>
 	using Scope = std::unique_ptr<T>;
 	template<typename T, typename ... Args>
 	constexpr Scope<T> CreateScope(Args&& ... args)
 	{
 		return std::make_unique<T>(std::forward<Args>(args)...);
 	}

 	template<typename T>
 	using Ref = std::shared_ptr<T>;
 	template<typename T, typename ... Args>
 	constexpr Ref<T> CreateRef(Args&& ... args)
 	{
 		return std::make_shared<T>(std::forward<Args>(args)...);
 	}

}
