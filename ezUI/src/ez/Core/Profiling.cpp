#include "ez/Core/Base.hpp"
#include "ez/Core/Profiling.hpp"
#include <GLFW/glfw3.h>
#include <iomanip>

namespace ez::debug {
	ProfilingTimer::ProfilingTimer(const std::string& name)
	: m_name(name) {
		m_start_time = glfwGetTime();
	}

	ProfilingTimer::~ProfilingTimer() {
		double endTime = glfwGetTime();
		double duration = endTime - m_start_time;

		Profiler::get().add_entry(m_name, m_start_time, duration);
	}

	Profiler& Profiler::get() {
		static Profiler instance;
		return instance;
	}

	void Profiler::start_session(const std::string& name) {
		if (m_is_active) {
			EZ_CORE_WARN("A new profiling session has been started before the previous one ended.");
			end_session();
		}

		m_session_name = name;
		m_is_active = true;
		m_file.open(name + "_profile_result.json");

		if (!m_file.is_open()) {
			EZ_CORE_FATAL_ERROR("Could not open file for profiling result: ", name, "_profile_result.json");
		}

		m_file << R"({"otherData": {},"traceEvents":[{})";
		m_file.flush();
	}

	void Profiler::add_entry(const std::string &name, double startTime, double duration) {
		m_file << std::setprecision(6) << std::fixed;
		m_file << ",{";
		m_file << R"("cat":"function",)";
		m_file << R"("dur":)" << duration << ',';
		m_file << R"("name":")" << name << "\",";
		m_file << R"("ph":"X",)";
		m_file << R"("pid":0,)";
		m_file << R"("tid":0,)";
		m_file << R"("ts":)" << startTime;
		m_file << "}";
		m_file.flush();
	}

	void Profiler::end_session() {
		m_file << "]}";
		m_file.flush();
		m_file.close();
		m_is_active = false;
	}
}
