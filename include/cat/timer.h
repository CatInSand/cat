#ifndef CAT_TIMER_H
#define CAT_TIMER_H

#include <cassert>

namespace cat
{
	class timer final
	{
	public:
		timer(float duration)
			: m_duration{ duration }
		{ }

		/*
		Increments elapsed time by delta_time
		*/
		void update(float delta_time)
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
		void reset(float duration)
		{
			m_elapsed_time = 0.f;
			m_duration = duration;
		}

		/*
		Returns the number of times duration has elapsed and applies modulo to elapsed time using duration
		*/
		int modulo_get()
		{
			assert(m_duration == 0.f && "timer::modulo_get() is invalid for duration 0");

			int times_done{ static_cast<int>(m_elapsed_time / m_duration) };
			m_elapsed_time -= m_duration * times_done;
			return times_done;
		}
		/*
		Returns true if done and decrements elapsed time by duration
		*/
		bool decrement_get()
		{
			if (m_elapsed_time >= m_duration)
			{
				m_elapsed_time -= m_duration;
				return true;
			}

			return false;
		}

		float elapsed() const
		{
			return m_elapsed_time;
		}
		float remaining() const
		{
			if (m_elapsed_time < m_duration)
				return m_duration - m_elapsed_time;

			return 0.f;
		}
		float duration() const
		{
			return m_duration;
		}
		bool done() const
		{
			return m_elapsed_time >= m_duration;
		}

	private:
		float m_duration;
		float m_elapsed_time{ 0.f };
	};
}

#endif
