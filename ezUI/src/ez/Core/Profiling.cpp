#include "ez/Core/Base.h"
#include "ez/Core/Profiling.h"
#include <GLFW/glfw3.h>
#include <iomanip>

namespace ez::debug {
	ProfilingTimer::ProfilingTimer(const std::string& name) 
	: m_Name(name) {
		m_StartTime = glfwGetTime();
	}

	ProfilingTimer::~ProfilingTimer() {
		double endTime = glfwGetTime();
		double duration = endTime - m_StartTime;

		Profiler::Get().AddEntry(m_Name, m_StartTime, duration);
	}

	Profiler& Profiler::Get() {
		static Profiler instance;
		return instance;
	}

	void Profiler::StartSession(const std::string& name) {
		if (m_IsActive) {
			EZ_CORE_WARN("A new profiling session has been started before the previous one ended.");
			EndSession();
		}

		m_SessionName = name;
		m_IsActive = true;
		m_File.open(name + "_profile_result.json");

		if (!m_File.is_open()) {
			EZ_CORE_FATAL_ERROR("Could not open file for profiling result: ", name, "_profile_result.json");
		}

		m_File << "{\"otherData\": {},\"traceEvents\":[{}";
		m_File.flush();
	}

	void Profiler::AddEntry(const std::string& name, double startTime, double duration) {
		m_File << std::setprecision(6) << std::fixed;
		m_File << ",{";
		m_File << "\"cat\":\"function\",";
		m_File << "\"dur\":" << duration << ',';
		m_File << "\"name\":\"" << name << "\",";
		m_File << "\"ph\":\"X\",";
		m_File << "\"pid\":0,";
		m_File << "\"tid\":0,";
		m_File << "\"ts\":" << startTime;
		m_File << "}";
		m_File.flush();
	}

	void Profiler::EndSession() {
		m_File << "]}";
		m_File.flush();
		m_File.close();
		m_IsActive = false;
	}

}