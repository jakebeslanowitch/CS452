#ifndef __BOOGA_H
#define __BOOGA_H
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/version.h> /* printk() */
#include <linux/init.h>  /*  for module_init and module_cleanup */
#include <linux/slab.h>  /*  for kmalloc/kfree */
#include <linux/fs.h>     /* everything... */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include <linux/proc_fs.h>	/* for the proc filesystem */
#include <linux/seq_file.h>
#include <linux/random.h>
#include <linux/sched.h>
#include <linux/uaccess.h>

/*
 * booga.h -- definitions for the char module
 *
 * Name: Jake Beslanowitch
 * Email: jakebeslanowitch@u.boisestate.edu
 * Github Username: jakebeslanowitch
 */
#ifndef booga_MAJOR
#define booga_MAJOR 0   /* dynamic major by default */
#endif

#ifndef booga_NR_DEVS
#define booga_NR_DEVS 4    /* booga0 through booga3 */
#endif
/*
 * Split minors in two parts
 */
#define TYPE(dev)   (MINOR(dev) >> 4)  /* high nibble */
#define NUM(dev)    (MINOR(dev) & 0xf) /* low  nibble */

#define BOOGA "booga! booga!"
#define GOOGOO "googoo! gaagaa!"
#define NEKA "neka! maka!"
#define WOOGA "wooga! wooga!"

/*
 * The different configurable parameters
 */
struct booga_stats {
	long int num_open;
	long int num_read; 
	long int num_write; 
	long int num_close; 
	struct semaphore sem;
};
typedef struct booga_stats booga_stats;

static int booga_open (struct inode *inode, struct file *filp);
static int booga_release (struct inode *inode, struct file *filp);
static ssize_t booga_read (struct file *filp, char *buf, size_t count, loff_t *f_pos);
static ssize_t booga_write (struct file *filp, const char *buf, size_t count, loff_t *f_pos);
static int booga_proc_open(struct inode *inode, struct file *filp); 

#endif /* __BOOGA_H */
