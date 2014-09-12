#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gfp.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>


MODULE_LICENSE("GPL");

#define CMD_INFLATE "inflate"
#define CMD_DEFLATE "deflate"
struct inflate_req {
	struct page* pg;
	unsigned long order;
	struct list_head req_list;
};
static LIST_HEAD(inflate_req_list);

void
do_inflate(int order)
{
	struct page *pg;
	struct inflate_req *inf_req;
	pg = alloc_pages(GFP_KERNEL, order);
	if(!pg)
	{
		printk(KERN_EMERG "mem_balloon: alloc_pages failed for order=%d\n", order);
		return;
	}
	inf_req = kmalloc(sizeof(struct inflate_req), GFP_KERNEL);
	if(inf_req)
	{
		inf_req->pg = pg;
		inf_req->order = order;
		list_add_tail(&inf_req->req_list, &inflate_req_list);
		printk(KERN_EMERG "mem_balloon: added %d pages\n", 1<<order);
	}
	else
	{
		__free_pages(pg, order);
		printk(KERN_EMERG "mem_balloon: kmalloc failed\n");
	}
	
}
void
do_deflate(int reqs)
{
	struct inflate_req *inf_req;
	int order;
	if(!list_empty(&inflate_req_list))
	{
		while(!list_empty(&inflate_req_list) && (reqs == -99 || reqs-->0) )
		{	
			inf_req = list_first_entry(&inflate_req_list, struct inflate_req, req_list);
			order = inf_req->order;
			__free_pages(inf_req->pg, inf_req->order);
			list_del(&inf_req->req_list);
			kfree(inf_req);
			printk(KERN_EMERG "mem_balloon: removed %d pages\n", 1<<order);
		}
		return;
	}
	printk(KERN_EMERG "mem_balloon: is empty\n");
}
	
ssize_t
mem_balloon_fwrite(struct file *filep, const char __user *user_buffer,
	size_t nbytes, loff_t *off)
{
	char cmd[32];
	int option;
	char buffer[256];

	if (copy_from_user(buffer, user_buffer, sizeof(buffer)))
		return -EFAULT;

	sscanf(buffer, "%s %d", cmd, &option);
	
	if(!strcmp(cmd, CMD_INFLATE))
	{
		printk(KERN_DEBUG "cmd=%s order=%d\n", cmd, option);
		do_inflate(option);
	}
	else
	if(!strcmp(cmd, CMD_DEFLATE))
	{
		do_deflate(option);
	}
	else
	{
		printk(KERN_EMERG "mem_balloon: bad command\n");
	}
	return nbytes;	

}


int
mem_balloon_seq_show(struct seq_file *s, void *unused)
{
	unsigned long total_pages = 0, nr_pages;
	
	struct inflate_req *inf_req;
	list_for_each_entry(inf_req, &inflate_req_list, req_list)
	{
			nr_pages = 1<<inf_req->order;
			total_pages += nr_pages;
	}
	seq_printf(s, "mem_balloon has %lu pages\n", total_pages);
	return 0;
}

static int
mem_balloon_fopen(struct inode* inode, struct file *file) {
	return single_open(file, mem_balloon_seq_show, inode);
}


static struct file_operations mem_balloon_fops = {
	.open = mem_balloon_fopen,
	.read = seq_read,
	.write = mem_balloon_fwrite,
	.release = single_release,
};

static struct proc_dir_entry *mem_balloon_entry;

static int 
mem_balloon_init(void){

	mem_balloon_entry = proc_create("mem_balloon", 0666, NULL, &mem_balloon_fops);
	if(!mem_balloon_entry)
	{
		printk(KERN_DEBUG "could not create mem_balloon entry\n");
		return 0;
	}
	return 0;
}
	

static void 
mem_balloon_exit(void){
	if(mem_balloon_entry)
		remove_proc_entry("mem_balloon", NULL);
}


module_init(mem_balloon_init);
module_exit(mem_balloon_exit);
