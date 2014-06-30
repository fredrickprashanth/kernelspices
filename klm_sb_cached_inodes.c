#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/fs.h>
MODULE_LICENSE("GPL");

static void 
list_sb_cached_inodes(struct super_block *sb, void *unused)
{
        struct inode *inode;

        list_for_each_entry(inode, &sb->s_inodes, i_sb_list) {
                if (inode->i_state & (I_FREEING|I_WILL_FREE|I_NEW))
                        continue;
                if (inode->i_mapping->nrpages == 0)
                        continue;
		if(list_is_singular(&inode->i_dentry))
		{
			struct dentry *d = list_entry(inode->i_dentry.next,struct dentry, d_alias);
			printk(KERN_DEBUG "%s\n", d->d_name.name);
		}
        }

}
static int 
sb_cached_inodes_init(void)
{
	printk(KERN_DEBUG "list of sb cached files\n");	
	iterate_supers(list_sb_cached_inodes, NULL);
	return 0;
}

static void 
sb_cached_inodes_exit(void)
{
}
module_init(sb_cached_inodes_init);
module_exit(sb_cached_inodes_exit);

