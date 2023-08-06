#pragma once
#include <string>
#include <fstream>

namespace ez::debug {
	class ProfilingTimer {
	public:
		explicit ProfilingTimer(const std::string& name);
		~ProfilingTimer();

	private:
		std::string m_name;
		double m_start_time;
	};

	class Profiler {
	public:
		static Profiler& get();
		
		void start_session(const std::string& name);
		void add_entry(const std::string& name, double startTime, double duration);
		void end_session();
	
	private:
		std::string m_session_name;
		std::ofstream m_file;
		bool m_is_active;
	};
}

#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define EZ_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define EZ_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define EZ_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define EZ_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define EZ_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define EZ_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define EZ_FUNC_SIG __func__
#else
#define EZ_FUNC_SIG "EZ_FUNC_SIG: Unknown compiler"
#endif

#if EZ_BUILD_ENABLE_PROFILING
#define EZ_PROFILE_BEGIN_SESSION(name) ez::debug::Profiler::Get().StartSession(name)
#define EZ_PROFILE_END_SESSION() ez::debug::Profiler::Get().EndSession()

#define EZ_PROFILE_FUNCTION() ez::debug::ProfilingTimer(EZ_FUNC_SIG);
#else 
#define EZ_PROFILE_BEGIN_SESSION(name) 
#define EZ_PROFILE_END_SESSION() 
#define EZ_PROFILE_FUNCTION() 
#endif