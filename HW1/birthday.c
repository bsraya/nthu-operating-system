#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>

typedef struct _birthday {
	int day, month, year;
	struct list_head list;
} birthday;

static LIST_HEAD(birthday_list);

int add_person(int year, int month, int day)
{
	birthday *person;
	person = kmalloc(sizeof(*person), GFP_KERNEL);
	person->day = day;
	person->month = month;
	person->year = year;
	INIT_LIST_HEAD(&person->list);
	listst_add_tail(&person->list, &birthday_list);
	return 0;
}

int birthday_init(void)
{
	birthday *ptr;
	printk(KERN_INFO "LOADING Module\n");
	add_person(1999, 12, 28);
	add_person(1993, 12, 4);
	add_person(1995, 6, 4);
	add_person(1998, 2, 4);
	add_person(2018, 9, 9);

	list_for_each_entry(ptr, &birthday_list, list) {
		printk(KERN_INFO "added person %d-%d-%d\n", ptr->year, ptr->month, ptr->day);
	}
	return 0;
}

void birthday_exit(void)
{
	birthday *ptr, *next;
	printk(KERN_INFO "UNLOADING Module\n");
	list_for_each_entry_safe(ptr, next, &birthday_list, list) {
		printk(KERN_INFO "deleting person %d-%d-%d\n", ptr->year, ptr->month, ptr->day);
		list_del(&ptr->list);
		kfree(ptr);
	}
}

module_init(birthday_init);
module_exit(birthday_exit);