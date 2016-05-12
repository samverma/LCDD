
#include <linux/init.h>          
#include <linux/module.h>        
#include <linux/device.h>         
#include <linux/kernel.h>         
#include <linux/fs.h>             
#include <asm/uaccess.h>          
#define  NAME "lcd"  
#define  CLASS_NAME  "lcd"       
MODULE_LICENSE("GPL");         
MODULE_AUTHOR("Sam Verma");    
MODULE_DESCRIPTION("Linux character device driver for OS"); 
static int    majornum;             
static char   msg[512] = {0};        
static short  msgsize;         
static int    times = 0;              
static struct class*  lcdclass  = NULL; 
static struct device* lcddev = NULL; 
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);
static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};
static int __init lcd_init(void){
   printk(KERN_INFO "Initializing LCD module\n");
   majornum = register_chrdev(0, NAME, &fops);
   if (majornum<0){
      printk(KERN_ALERT "LCD failed to register\n");
      return majornum;
   }
   printk(KERN_INFO "LCD: registered with major number %d\n", majornum);
   lcdclass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(lcdclass)){
      unregister_chrdev(majornum, NAME);
      printk(KERN_ALERT "LCD class registration failed\n");
      return PTR_ERR(lcdclass);
   }
   printk(KERN_INFO "LCD: device class registered\n");
   lcddev = device_create(lcdclass, NULL, MKDEV(majornum, 0), NULL, NAME);
   if (IS_ERR(lcddev)){               
      class_destroy(lcdclass);           
      unregister_chrdev(majornum, NAME);
      printk(KERN_ALERT "LCD device creation failed\n");
      return PTR_ERR(lcddev);
   }
   printk(KERN_INFO "LCD: device class created\n"); 
   return 0;
}
static void __exit lcd_exit(void){
   device_destroy(lcdclass, MKDEV(majornum, 0));     
   class_unregister(lcdclass);                       
   class_destroy(lcdclass);                             
   unregister_chrdev(majornum, NAME);             
   printk(KERN_INFO "Exiting LCD\n");
}
static int dev_open(struct inode *inodep, struct file *filep){
   times++;
   printk(KERN_INFO "LCD: Device has been opened %d time(s)\n", times);
   return 0;
}
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
   int error_count = 0;
   error_count = copy_to_user(buffer, msg, msgsize);
   if (error_count==0){         
      printk(KERN_INFO "Read %d characters\n", msgsize);
      return (msgsize=0);
   }
   else {
      printk(KERN_INFO "Failed to read %d characters", error_count);
      return -EAGAIN;           
   }
}
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
   sprintf(msg, "%s(%d letters)", buffer, len);  
   msgsize = strlen(msg);                
   printk(KERN_INFO "Wrote %d characters\n", len);
   return len;
}
static int dev_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "Closed LCD\n");
   return 0;
}
module_init(lcd_init);
module_exit(lcd_exit);
