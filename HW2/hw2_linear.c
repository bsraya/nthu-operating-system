#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/module.h>

int start (void) {
    struct task_struct *task;

    printk(KERN_INFO "Start listing tasks linearly\n");
    for_each_process (task) {
        printk(KERN_INFO "pid: %d | pname: %-20s | state: %ld", task->pid, task->comm, task->state);
    }
    printk(KERN_INFO "Stop listing task linearly");
    return 0;
}

void terminate (void) {
    printk(KERN_INFO "DONE!");
}

module_init(start);
module_exit(terminate);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Listing LINUX OS tasks linearly");
MODULE_AUTHOR("BIJON SETYAWAN RAYA, 104062181");
