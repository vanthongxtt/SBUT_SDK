
#ifndef SBUT_TIMER_H
#define SBUT_TIMER_H

#ifndef SBUT_TIMER_MAX_TASKS
#define SBUT_TIMER_MAX_TASKS 10 
#endif

#include "Arduino.h"

#define SButTimerClass SButTimer

typedef void (*TaskCallback0)(void);
typedef void (*TaskCallback1)(int);

class SButTimerClass
{
public:
	SButTimerClass(bool prioritized = false);

	bool setTimeout(TaskCallback0 func, unsigned long interval, byte prio = SBUT_TIMER_MAX_TASKS);
	bool setTimeout(TaskCallback1 func, unsigned long interval, int param, byte prio = SBUT_TIMER_MAX_TASKS);

	bool setInterval(TaskCallback0 func, unsigned long interval, byte prio = SBUT_TIMER_MAX_TASKS);
	bool setInterval(TaskCallback1 func, unsigned long interval, int param, byte prio = SBUT_TIMER_MAX_TASKS);

	bool setRepeated(TaskCallback0 func, unsigned long interval, unsigned int repeat, byte prio = SBUT_TIMER_MAX_TASKS);
	bool setRepeated(TaskCallback1 func, unsigned long interval, unsigned int repeat, int param, byte prio = SBUT_TIMER_MAX_TASKS);

	bool cancel(TaskCallback0 func);
	bool cancel(TaskCallback1 func, int param);

	bool clearTimeout(TaskCallback0 func) { return cancel(func); };
	bool clearTimeout(TaskCallback1 func, int param) { return cancel(func, param); };

	bool clearInterval(TaskCallback0 func) { return cancel(func); };
	bool clearInterval(TaskCallback1 func, int param) { return cancel(func, param); };

	unsigned long scheduledIn(TaskCallback0 func);
	unsigned long scheduledIn(TaskCallback1 func, int param);

	void loop(void);

	bool isPrioritized() { return t_prioritized; }
	void setPrioritized(bool prioritized) { t_prioritized = prioritized; }

private:
	struct TASK {
		TaskCallback1 call;
		int param;
		unsigned long interval;
		unsigned long lastRun;
		unsigned int repeat;
	};

	int findTask(TaskCallback0 func);
	int findTask(TaskCallback1 func, int param);
	bool addTask(TaskCallback1 func, unsigned long interval, unsigned int repeat, int param, byte prio);
	bool removeTask(int t_idx);

	TASK tasks[SBUT_TIMER_MAX_TASKS];
	byte t_count;
	bool t_prioritized;
	static const int NO_PARAMETER = -1;
};

#endif