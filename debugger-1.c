#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>  // For accessing the task list
#include <linux/sched/signal.h>
#include <linux/ktime.h>  // For time tracking
#include <linux/unistd.h> // For system call numbers
#include <linux/jiffies.h> // For jiffies and uptime

MODULE_LICENSE("GPL");

static unsigned long cpu_threshold = 1000; // CPU usage threshold in jiffies (for example, 1000 jiffies)
module_param(cpu_threshold, ulong, 0644);
MODULE_PARM_DESC(cpu_threshold, "CPU usage threshold in jiffies");

static void log_process_info(void) {
    struct task_struct *task;
    unsigned long cpu_time;
    pr_info("Listening for processes over CPU threshold...\n");

    for_each_process(task) {
        cpu_time = task->utime + task->stime; // Get the user time and system time (in jiffies)

        if (cpu_time > cpu_threshold) {
            pr_info("Process: %s (PID: %d) exceeds CPU threshold: %lu jiffies\n", task->comm, task->pid, cpu_time);
        }
    }
}
static int __init debugger_init(void) {
    pr_info("Debugger module loaded.\n");
    log_process_info();
    return 0;
}

static void __exit debugger_exit(void) {
    pr_info("Debugger module unloaded.\n");
}

module_init(debugger_init);
module_exit(debugger_exit);