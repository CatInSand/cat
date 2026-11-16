#ifndef CAT_TIMER_H
#define CAT_TIMER_H

#include <cassert>
#include <chrono>
#include <concepts>

namespace cat
{
	constexpr float _micro_to_seconds = 1000000.f;

	template<std::floating_point T = float>
	class stopwatch final
	{
	public:
		explicit stopwatch() = default;

		/*
		Resets stopwatch and stops timing.
		*/
		void reset()
		{
			m_start = std::chrono::high_resolution_clock::now();
			m_elapsed_time = 0.f;
			m_running = false;
		}
		/*
		Resets stopwatch and starts timing.
		*/
		void start()
		{
			m_start = std::chrono::high_resolution_clock::now();
			m_running = true;
		}
		/*
		Stops timing.
		*/
		void stop()
		{
			m_elapsed_time = elapsed();
			m_running = false;
		}
		/*
		Resumes timing;
		*/
		void resume()
		{
			m_running = true;
		}
		
		bool running() const
		{
			return m_running;
		}
		/*
		Returns the elapsed time in seconds.
		*/
		T elapsed() const
		{
			if (m_running)
			{
				return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_start).count() / _micro_to_seconds;
			}

			return m_elapsed_time;
		}

	protected:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_start{ std::chrono::high_resolution_clock::now() };
		T m_elapsed_time{ 0.f };
		bool m_running{ false };
	};

	template<std::floating_point T = float>
	class mtimer final
	{
	public:
		explicit mtimer(T duration)
			: m_duration{ duration }
		{ }

		/*
		Increments elapsed time by delta_time
		*/
		void update(T delta_time)
		{
			m_elapsed_time += delta_time;
		}
		/*
		Sets elapsed time to 0
		*/
		void reset()
		{
			m_elapsed_time = 0.f;
		}
		/*
		Sets elapsed time to 0 and sets duration
		*/
		void reset(T duration)
		{
			m_elapsed_time = 0.f;
			m_duration = duration;
		}
		/*
		Sets elapsed time
		*/
		void set(T elapsed_time)
		{
			m_elapsed_time = elapsed_time;
		}
		/*
		Sets elapsed time and duration
		*/
		void set(T elapsed_time, T duration)
		{
			m_elapsed_time = elapsed_time;
			m_duration = duration;
		}

		/*
		Returns the number of times duration has elapsed and applies modulo to elapsed time using duration
		*/
		int modulo_get()
		{
			assert(m_duration != 0.f && "timer::modulo_get() is invalid for duration 0");

			int amount{ times_done() };
			m_elapsed_time -= m_duration * amount;
			return amount;
		}
		/*
		If done, returns true and decrements elapsed time by duration. Otherwise returns false.
		*/
		bool decrement_get()
		{
			if (done())
			{
				m_elapsed_time -= m_duration;
				return true;
			}

			return false;
		}

		T elapsed() const
		{
			return m_elapsed_time;
		}
		T remaining() const
		{
			if (m_elapsed_time < m_duration)
				return m_duration - m_elapsed_time;

			return 0.f;
		}
		T duration() const
		{
			return m_duration;
		}
		bool done() const
		{
			return m_elapsed_time >= m_duration;
		}
		int times_done() const
		{
			return static_cast<int>(m_elapsed_time / m_duration);
		}

	private:
		T m_duration;
		T m_elapsed_time{ 0.f };
	};
}

#endif
