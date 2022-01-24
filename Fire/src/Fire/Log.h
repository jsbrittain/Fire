#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Fire {

	class FIRE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define FR_CORE_TRACE(...) ::Fire::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FR_CORE_INFO(...)  ::Fire::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FR_CORE_WARN(...)  ::Fire::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FR_CORE_ERROR(...) ::Fire::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FR_CORE_FATAL(...) ::Fire::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define FR_TRACE(...) ::Fire::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FR_INFO(...)  ::Fire::Log::GetClientLogger()->info(__VA_ARGS__)
#define FR_WARN(...)  ::Fire::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FR_ERROR(...) ::Fire::Log::GetClientLogger()->error(__VA_ARGS__)
#define FR_FATAL(...) ::Fire::Log::GetClientLogger()->fatal(__VA_ARGS__)
