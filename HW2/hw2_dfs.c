#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/kernel.h>
#include <linux/module.h>

void depth_first_search (struct task_struct *task) {
    struct list_head *children_list;
    struct task_struct *new_child;

    printk(KERN_INFO "pid: %d\t | pname: %-20s\t | state: %ld\n", task->pid, task->comm, task->state);

    // loop over children_list pointer
    list_for_each (children_list, &task->children) { 
        // initialize a new child that points to the next child
        new_child = list_entry(children_list, struct task_struct, sibling); 
        
        // iterate from new_child
        depth_first_search(new_child); 
    }
}

int start (void) {
    printk(KERN_INFO "Start listing tasks with DFS Data Structure\n");
    depth_first_search(&init_task);
    printk(KERN_INFO "Finish listing tasks with DFS Data Structure\n");
    return 0;
}

void terminate (void) {
    printk(KERN_INFO "DONE!!!\n");
}

module_init(start);
module_exit(terminate);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Listing tasks with DFS Data Structure");
MODULE_AUTHOR("Bijon Setyawan Raya, 104062181");

