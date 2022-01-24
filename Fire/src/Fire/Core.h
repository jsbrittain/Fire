#pragma once

#include <memory>

#define FIRE_API		// Blank, but may be needed for cross-platform compatibility later on

#ifdef FR_DEBUG
	#define FR_ENABLE_ASSERTS
#endif

#ifdef FR_ENABLE_ASSERTS
	#define FR_ASSERT(x, ...) { if(!(x)) { FR_ERROR("Assertion failed: {0}",__VA_ARGS__); } }//__debugbreak(); } }
	#define FR_CORE_ASSERT(x, ...) { if(!(x)) { FR_CORE_ERROR("Assertion failed: {0}",__VA_ARGS__); } };//__debugbreak(); } }
#else
	#define FR_ASSERT(x, ...)
	#define FR_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define FR_BIND_EVENTS_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Fire {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

}
