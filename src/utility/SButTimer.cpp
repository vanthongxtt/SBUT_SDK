#include "Sbut/SButTimer.h"

SButTimerClass::SButTimerClass(bool prioritized)
{
    t_count = 0;
    t_prioritized = prioritized;
}

bool SButTimerClass::setTimeout(TaskCallback0 func, unsigned long interval, byte prio)
{
    return setRepeated(func, interval, 1, prio);
}

bool SButTimerClass::setTimeout(TaskCallback1 func, unsigned long interval, int param, byte prio)
{
    return setRepeated(func, interval, 1, param, prio);
}

bool SButTimerClass::setInterval(TaskCallback1 func, unsigned long interval, int param, byte prio)
{
    return setRepeated(func, interval, 0, param, prio);
}

bool SButTimerClass::setInterval(TaskCallback0 func, unsigned long interval, byte prio)
{
    return setRepeated(func, interval, 0, prio);
}

bool SButTimerClass::setRepeated(TaskCallback0 func, unsigned long interval, unsigned int repeat, byte prio)
{
    return addTask((TaskCallback1)func, interval, repeat, NO_PARAMETER, prio);
}

bool SButTimerClass::setRepeated(TaskCallback1 func, unsigned long interval, unsigned int repeat, int param, byte prio)
{
    if (param < 0)
        param = 0;
    return addTask(func, interval, repeat, param, prio);
}

bool SButTimerClass::cancel(TaskCallback0 func)
{
    return removeTask(findTask(func));
}

bool SButTimerClass::cancel(TaskCallback1 func, int param)
{
    return removeTask(findTask(func, param));
}

unsigned long SButTimerClass::scheduledIn(TaskCallback0 func)
{
    return scheduledIn((TaskCallback1)func, NO_PARAMETER);
}

unsigned long SButTimerClass::scheduledIn(TaskCallback1 func, int param)
{
    int t_idx = findTask(func, param);
    if (t_idx >= 0)
    {
        TASK &t = tasks[t_idx];
        unsigned long now = millis();
        if (now - t.lastRun >= t.interval)
            return 1;
        else
            return t.lastRun + t.interval - now;
    }
    return 0;
}

void SButTimerClass::loop(void)
{
    byte t_idx = 0;
    unsigned long now = millis();
    while (t_idx < t_count)
    {
        bool inc = true;
        TASK &t = tasks[t_idx];
        if (now - t.lastRun >= t.interval)
        {
            int param = t.param;
            TaskCallback1 call = t.call;

            t.lastRun += t.interval;
            if (t.repeat > 0 && --t.repeat == 0)
            {

                removeTask(t_idx);
                inc = false;
            }

            if (param >= 0)
                (*(call))(param);
            else
                (*(TaskCallback0)(call))();

            if (t_prioritized)
                break;
            now = millis();
        }
        if (inc)
            t_idx++;
    }
}

int SButTimerClass::findTask(TaskCallback0 func)
{
    return findTask((TaskCallback1)func, NO_PARAMETER);
}

int SButTimerClass::findTask(TaskCallback1 func, int param)
{
    for (byte t_idx = 0; t_idx < t_count; t_idx++)
    {
        TASK &t = tasks[t_idx];
        if (t.call == func && t.param == param)
            return t_idx;
    }
    return -1;
}

bool SButTimerClass::addTask(TaskCallback1 func, unsigned long interval, unsigned int repeat, int param, byte prio)
{
    byte pos = (prio < t_count) ? prio : t_count;

    int idx = findTask(func, param);
    if (idx >= 0)
    {
        removeTask(idx);
        pos = idx;
    }

    if (t_count >= SBUT_TIMER_MAX_TASKS || interval == 0)
        return false;

    if (pos < t_count)
        memmove(tasks + pos + 1, tasks + pos, sizeof(TASK) * (t_count - pos));
    TASK &t = tasks[pos];
    t.call = func;
    t.interval = interval;
    t.param = param;
    t.lastRun = millis();
    t.repeat = repeat;
    t_count++;
    return true;
}

bool SButTimerClass::removeTask(int t_idx)
{
    if (t_idx >= 0 && t_idx < t_count)
    {
        memmove(tasks + t_idx, tasks + t_idx + 1, sizeof(TASK) * (t_count - t_idx - 1));
        t_count--;
        return true;
    }
    return false;
}