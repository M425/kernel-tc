#ifndef _TC_CORE_H_
#define _TC_CORE_H_ 1

//#include <asm/types.h>
//#include <linux/pkt_sched.h>
//#include <linux/delay.h>
//#include <linux/if_ether.h>
//#include <linux/etherdevice.h>
//#include <linux/rtnetlink.h>
// #include <linux/pm_qos_params.h>
//#include <linux/crc32.h>
//#include <net/mac80211.h>
//#include <asm/unaligned.h>
//#include <linux/module.h>
//#include <linux/sysinfo.h>
//#include <linux/init.h>
//#include <linux/netdevice.h>
//#include <linux/types.h>
//#include <linux/slab.h>
//#include <linux/pkt_sched.h>
//#include <net/sch_generic.h>
//#include <linux/skbuff.h>
//#include <linux/etherdevice.h>
//#include <linux/if_arp.h>
//#include <linux/wireless.h>
//#include <linux/bitmap.h>
//#include <linux/pm_qos.h>
//#include <net/net_namespace.h>
//#include "my_tc.h"

#define TIME_UNITS_PER_SEC	1000000

enum link_layer {
	LINKLAYER_UNSPEC,
	LINKLAYER_ETHERNET,
	LINKLAYER_ATM,
};

// int  tc_core_time2big(unsigned time);
unsigned tc_core_time2tick(unsigned time);
// unsigned tc_core_tick2time(unsigned tick);
// unsigned tc_core_time2ktime(unsigned time);
// unsigned tc_core_ktime2time(unsigned ktime);
unsigned tc_calc_xmittime(unsigned rate, unsigned size);
// unsigned tc_calc_xmitsize(unsigned rate, unsigned ticks);
int tc_calc_rtable(struct tc_ratespec *r, __u32 *rtab, int cell_log, unsigned mtu, enum link_layer link_layer);
int tc_calc_size_table(struct tc_sizespec *s, __u16 **stab);
// 
// int tc_setup_estimator(unsigned A, unsigned time_const, struct tc_estimator *est);
// 
// int tc_core_init(void);

// extern struct rtnl_handle g_rth;
// extern int is_batch_mode;
#endif
