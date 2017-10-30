#include<stdlib.h>
#include<stdio.h>
#include <chrono>

using namespace std::chrono;
class TimerBase {
	public:
		TimerBase() : m_start(system_clock::time_point::min()) {}
		/* TimerBase(): m_start(system_clock::timer_point::min()) { */
		/* } */
		
		bool IsStarted() const {
			return (m_start.time_since_epoch() != system_clock::duration(0));
		}

		void Clear() {
			m_start = system_clock::time_point::min();
		}
		void Start() {
			m_start = system_clock::now();
		}

		unsigned long GetMs() {
			if (IsStarted()) {
				system_clock::duration diff;
				diff = system_clock::now() - m_start;
				return (unsigned)(duration_cast<milliseconds>(diff).count());
			}
			return 0;
		}
	private:
		system_clock::time_point m_start;
};
	
int main(void)
{

	TimerBase timer;
	timer.Start();
	printf("Hello World!\n");
	for (int i = 0; i < 1000000000;  i++)
	{
		int a = 0;
	}
	/* timer.Clear(); */
	printf("%lu ms\n", timer.GetMs());
	return 0;
}
