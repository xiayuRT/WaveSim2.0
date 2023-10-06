#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0xde55e795, "_raw_spin_lock_irqsave" },
	{ 0xf3d0b495, "_raw_spin_unlock_irqrestore" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x9d669763, "memcpy" },
	{ 0xf3d3ccd3, "usb_submit_urb" },
	{ 0x41453899, "_dev_err" },
	{ 0x135d387d, "usb_autopm_put_interface_async" },
	{ 0x7e0a6aff, "usb_kill_urb" },
	{ 0x4205ad24, "cancel_work_sync" },
	{ 0x37275b64, "tty_port_put" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x6ebe366f, "ktime_get_mono_fast_ns" },
	{ 0x406bfe55, "tty_port_tty_hangup" },
	{ 0x2d3385d3, "system_wq" },
	{ 0xb2d48a2e, "queue_work_on" },
	{ 0x676bbc0f, "_set_bit" },
	{ 0xfb46a8a7, "tty_insert_flip_string_fixed_flag" },
	{ 0xb89f56a9, "tty_flip_buffer_push" },
	{ 0x2a3aa678, "_test_and_clear_bit" },
	{ 0xf7cef6a3, "tty_port_tty_wakeup" },
	{ 0x62abda62, "tty_port_hangup" },
	{ 0x270dae16, "usb_autopm_get_interface_async" },
	{ 0x355a2427, "tty_port_close" },
	{ 0x41e8c38e, "usb_deregister" },
	{ 0x635415a9, "tty_unregister_driver" },
	{ 0x8f298f30, "tty_driver_kref_put" },
	{ 0x53950e70, "__tty_alloc_driver" },
	{ 0x67b27ec1, "tty_std_termios" },
	{ 0xcc1399ac, "tty_register_driver" },
	{ 0x1a09d93, "usb_register_driver" },
	{ 0x92997ed8, "_printk" },
	{ 0x828ce6bb, "mutex_lock" },
	{ 0x9618ede0, "mutex_unlock" },
	{ 0x9c4b4e3f, "tty_standard_install" },
	{ 0x296695f, "refcount_warn_saturate" },
	{ 0x87eeef28, "usb_put_intf" },
	{ 0x37a0cba, "kfree" },
	{ 0xd86b61c4, "_raw_spin_lock_irq" },
	{ 0xd697e69a, "trace_hardirqs_on" },
	{ 0xebe3c962, "device_remove_file" },
	{ 0x3a3e97b1, "tty_port_tty_get" },
	{ 0x8bf40f06, "tty_vhangup" },
	{ 0xa406d804, "tty_kref_put" },
	{ 0x29bc54, "tty_unregister_device" },
	{ 0x33800e0b, "usb_free_urb" },
	{ 0xe6eacfef, "usb_free_coherent" },
	{ 0x176f88fe, "usb_driver_release_interface" },
	{ 0xae577d60, "_raw_spin_lock" },
	{ 0x764bdc19, "usb_control_msg" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x3545b222, "usb_autopm_get_interface" },
	{ 0x5dd1ec3, "usb_autopm_put_interface" },
	{ 0xe707d823, "__aeabi_uidiv" },
	{ 0x4b487483, "tty_port_open" },
	{ 0xbd394d8, "tty_termios_baud_rate" },
	{ 0x71c90087, "memcmp" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0xc6cbbc89, "capable" },
	{ 0x353e3fa5, "__get_user_4" },
	{ 0xe702d4c8, "kmalloc_caches" },
	{ 0x5995f766, "kmalloc_trace" },
	{ 0xbc10dd97, "__put_user_4" },
	{ 0x5f754e5a, "memset" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0x35b5b215, "usb_ifnum_to_if" },
	{ 0xde4bf88b, "__mutex_init" },
	{ 0x40aad7cd, "tty_port_init" },
	{ 0x8a278170, "usb_alloc_coherent" },
	{ 0xc76e789d, "usb_alloc_urb" },
	{ 0xe6df3144, "device_create_file" },
	{ 0x7a5d0700, "_dev_warn" },
	{ 0x2d6fcc06, "__kmalloc" },
	{ 0x154aa398, "_dev_info" },
	{ 0xab58a322, "usb_driver_claim_interface" },
	{ 0x5883ce7f, "usb_get_intf" },
	{ 0x5d283a58, "tty_port_register_device" },
	{ 0xc84d16dc, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("usb:v04E2p1410d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v04E2p1411d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v04E2p1412d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v04E2p1414d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v04E2p1420d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v04E2p1421d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v04E2p1422d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v04E2p1424d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v04E2p1400d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v04E2p1401d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v04E2p1402d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v04E2p1403d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v2890p0213d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "2738D765B3266EB37CA6A14");
