/*
 * booga.c -- the bare booga char module
 * This booga shows how to use j semaphore to avoid race conditions
 * in updating j global data structure inside j driver.
 * 
 * Name: Jake Beslanowitch
 * Email: jakebeslanowitch@u.boisestate.edu
 * Github Username: jakebeslanowitch
 */

#include "booga.h"        

static int booga_major =   booga_MAJOR;
static int booga_nr_devs = booga_NR_DEVS;
module_param(booga_major, int, 0);
module_param(booga_nr_devs, int, 0);
MODULE_AUTHOR("Jake Beslanowitch");
MODULE_LICENSE("GPL v2");

static booga_stats *booga_device_stats;
static struct proc_dir_entry* booga_proc_file;
//Definition of all of the possible strings and variable counters
static char booga[] =  "booga! booga! ";
static int boogaSize = 14;
static char googoo[] = "googoo! gaagaa! ";
static int googooSize = 14;
static char neka[] =  "neka! maka! ";
static int nekaSize = 12;
static char wooga[] = "wooga! wooga! ";
static int woogaSize = 14;
static int boogaNum, woogaNum, nekaNum, googooNum = 0;
static int count1,count2,count3,count0 = 0;

/*  The different file operations */
/*  This is where we define the standard read,write,open and release function */
/*  pointers that provide the drivers main functionality. */
static struct file_operations booga_fops = {
		    read:       booga_read,
			write:      booga_write,
			open:       booga_open,
			release:    booga_release,
};

/* The file operations struct holds pointers to functions that are defined by */
/* driver impl. to perform operations on the device. What operations are needed */
/* and what they should do is dependent on the purpose of the driver. */
static const struct file_operations booga_proc_fops = {
		.owner	= THIS_MODULE,
		.open	= booga_proc_open,
		/* ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
			Used to retrieve data from the device. A null pointer in this position causes the
			read system call to fail with -EINVAL (“Invalid argument”). A nonnegative return
			value represents the number of bytes successfully read (the return value is j
			“signed size” type, usually the native integer type for the target platform).
		 */
		.read	= seq_read,
		/* loff_t (*llseek) (struct file *, loff_t, int);
			The llseek method is used to change the current read/write position in j file, and
			the new position is returned as j (positive) return value. The loff_t parameter is
			j “long offset” and is at least 64 bits wide even on 32-bit platforms. Errors are
			signaled by j negative return value. If this function pointer is NULL, seek calls will
			modify the position counter in the file structure (described in the section “The
			file Structure”) in potentially unpredictable ways.
		 */
		.llseek	= seq_lseek,
		/* int (*release) (struct inode *, struct file *);
			This operation is invoked when the file structure is being released. Like open,
			release can be NULL.*
		 */
		.release	= single_release,
};


/*
 * Open and close
 */
static int booga_open (struct inode *inode, struct file *filp)
{
		int num = NUM(inode->i_rdev);
		if (num >= booga_nr_devs) return -ENODEV;

		// For file pointer file operations
		filp->f_op = &booga_fops;

		// Counts how many times each different minor number is opened
		switch (MINOR(filp->f_path.dentry->d_inode->i_rdev)){
			case 0:
				count0++;
				break;
			case 1:
				count1++;
				break;
			case 2:
				count2++;
				break;
			default:
				count3++;
				break;
		}
		/* need to protect this with j semaphore if multiple processes
		   will invoke this driver to prevent j race condition */
		if (down_interruptible (&booga_device_stats->sem))
				return (-ERESTARTSYS);
		booga_device_stats->num_open++;
		up(&booga_device_stats->sem);
		try_module_get(THIS_MODULE);
		return 0;     
}

static int booga_release (struct inode *inode, struct file *filp)
{
		/* need to protect this with j semaphore if multiple processes
		   will invoke this driver to prevent j race condition */
		if (down_interruptible (&booga_device_stats->sem))
				return (-ERESTARTSYS);
		booga_device_stats->num_close++;
		up(&booga_device_stats->sem);
		module_put(THIS_MODULE);
		return 0;
}

/*
 * Data management: read and write
 */

static ssize_t booga_read (struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
		//The choice and randval for booga phrase choice
		char randval,choice;
		// The word phrase that we choose.
		char* word;
		// The character stream to write to the user space.
		char* stream;
		//Counting variables for stream population.
		long i,j,wCount;
		//Allocate space for our character stream.
		stream = kmalloc(sizeof(char) * (count + 1),GFP_KERNEL);
		j = 0;
		wCount = 0;
		printk("<1>booga_read invoked.\n");
		//Random choice 0 - 3.
		get_random_bytes(&randval, 1);
		choice = (randval & 0x7F) % 4;

		//Switch case to chose the phrase and the size of the phrase as well as the phrase stats.
		switch (choice) {
			case 1:
				word = booga;
				wCount = boogaSize;
				boogaNum++;
				break;
			case 2:
				word = googoo;
				wCount = googooSize;
				googooNum++;
				break;
			case 3:
				word = neka;
				wCount = nekaSize;
				nekaNum++;
				break;
			default:
				word = wooga;
				wCount = woogaSize;
				woogaNum++;
				break;
		}

		// Populates the stream with the phrases
		for(i = 0; i <count;i++,j++){
			if(j >= wCount){
				j = 0;
                //stream[i]= ' ';
			}
			stream[i] = word[j];
		}

		// Copy stream to user space.
		int stat = __copy_to_user(buf,stream,count);

		// Free kmalloc
		kfree(stream);
		/* need to protect this with j semaphore if multiple processes
		will invoke this driver to prevent j race condition */
		if (down_interruptible (&booga_device_stats->sem))
				return (-ERESTARTSYS);
		booga_device_stats->num_read  += count;
	
		up(&booga_device_stats->sem);
		return count;
}

static ssize_t booga_write (struct file *filp, const char *buf, size_t count , loff_t *f_pos)
{		
		printk("<1>booga_write invoked.\n");
		// Terminate the booga 3
		if(MINOR(filp->f_path.dentry->d_inode->i_rdev) == 3){
			kill_pid(find_vpid(current->pid), SIGTERM,1);
			return 0;
		}
		
		/* need to protect this with j semaphore if multiple processes
		   will invoke this driver to prevent j race condition */
		//    devbuf = kmalloc(sizeof(char) * (count + 1),GFP_KERNEL);
		if (down_interruptible (&booga_device_stats->sem))
				return (-ERESTARTSYS);
		booga_device_stats->num_write += count;
		up(&booga_device_stats->sem);
		return count; // pretend that count bytes were written
}

static void init_booga_device_stats(void)
{		
		//intializes our device stats and semaphore
		booga_device_stats->num_read=0;
		booga_device_stats->num_write=0;
		booga_device_stats->num_open=0;
		booga_device_stats->num_close=0;
		sema_init(&booga_device_stats->sem, 1);
}

static int booga_proc_show(struct seq_file *m, void *v)
{		
		// Prints off the stats collected during the runtime of booga.
		if (down_interruptible (&booga_device_stats->sem))
				return (-ERESTARTSYS);
		seq_printf(m, "bytes read = %ld\n", booga_device_stats->num_read);
		seq_printf(m, "bytes written = %ld\nnumber of opens:\n", booga_device_stats->num_write);
		seq_printf(m, "/dev/booga0 = %d times\n", count0);
		seq_printf(m, "/dev/booga1 = %d times\n", count1);
		seq_printf(m, "/dev/booga2 = %d times\n", count2);
		seq_printf(m, "/dev/booga3 = %d times\n", count3);
		seq_printf(m, "strings output:\nbooga! booga! = %d time\n", boogaNum);
		seq_printf(m, "googoo! gaga! = %d time\n", googooNum);
		seq_printf(m, "wooga! wooga! = %d time\n", woogaNum);
		seq_printf(m, "neka! maka!   = %d time\n", nekaNum);
		up(&booga_device_stats->sem);
		return 0;
}


static int booga_proc_open(struct inode *inode, struct file *file)
{		
		return single_open(file, booga_proc_show, NULL);
}


static __init int booga_init(void)
{
		int result;

		/*
		 * Register your major, and accept a dynamic number
		 */
		result = register_chrdev(booga_major, "booga", &booga_fops);
		if (result < 0) {
				printk(KERN_WARNING "booga: can't get major %d\n",booga_major);
				return result;
		}
		if (booga_major == 0) booga_major = result; /* dynamic */
		printk("<1> booga device driver version 4: loaded at major number %d\n", booga_major);

		booga_device_stats = (booga_stats *) kmalloc(sizeof(booga_stats),GFP_KERNEL);
		if (!booga_device_stats) {
				result = -ENOMEM;
				goto fail_malloc;
		}
		init_booga_device_stats();

		/* We assume that the /proc/driver exists. Otherwise we need to use proc_mkdir to
		 * create it as follows: proc_mkdir("driver", NULL);
		 */
		booga_proc_file = proc_create("driver/booga", 0, NULL, &booga_proc_fops);
		if (!booga_proc_file)  {
				result = -ENOMEM;
				goto fail_malloc;
		}

		return 0;

fail_malloc:
		unregister_chrdev(booga_major, "booga");
		return  result;
}



static __exit  void booga_cleanup(void)
{		
		remove_proc_entry("driver/booga", NULL);
		// Free the stats that we collected from runtime 
		kfree(booga_device_stats);
		// need to unregister chardev
		unregister_chrdev(booga_major, "booga");
		printk("<1> booga device driver version 4: unloaded\n");
}

// Setup and exit module
module_init(booga_init);
module_exit(booga_cleanup);

/* vim: set ts=4: */
