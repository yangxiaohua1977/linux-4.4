/*
 * wbr_box_pwrbtn.c: wbr_gpio_keys platform data initilization file
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */

#include <linux/init.h>
#include <linux/input.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/gpio_keys.h>
#include <linux/platform_device.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

static struct proc_dir_entry *proc_monitor = NULL;

static struct gpio_keys_button wbr_gpio_button[] = {
	{
		.code = KEY_POWER,
		.gpio= 381, 
		.desc ="power_btn",	
		.type = EV_KEY, 
		.wakeup = 1, 
		.debounce_interval = 30,
		.can_disable = 1,
	},
};

static struct gpio_keys_platform_data wbr_gpio_keys = {
	.buttons	= wbr_gpio_button,
	.nbuttons	= 1,
};

static struct platform_device wbr_pwrbtn_device = {
	.name		= "gpio-keys",
	.id		= -1,
	.dev		= {
		.platform_data	= &wbr_gpio_keys,
	},
};

static int monitor_dp_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "DP dpms is %s\n", "On");
	return 0;
}

static int monitor_dp_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, monitor_dp_proc_show, NULL);
}

static const struct file_operations monitor_dp_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= monitor_dp_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int monitor_hdmi_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "HDMI dpms is %s\n", "Off");
	return 0;
}

static int monitor_hdmi_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, monitor_hdmi_proc_show, NULL);
}

static const struct file_operations monitor_hdmi_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= monitor_hdmi_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

int __init monitor_proc_init(void)
{
	proc_monitor = proc_mkdir("monitor", NULL);
	if (!proc_monitor)
		return -EIO;

	proc_create("DP-1-DPMS", 0, proc_monitor, &monitor_dp_proc_fops);
	proc_create("HDMI-A-1-DPMS", 0, proc_monitor, &monitor_hdmi_proc_fops);

	return 0;
}

static int __init pb_keys_init(void)
{
	printk("%s!\n", __FUNCTION__);
	monitor_proc_init();
	return platform_device_register(&wbr_pwrbtn_device);
}
late_initcall(pb_keys_init);
