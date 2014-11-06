#ifndef _SCH_API_H_
#define _SCH_API_H_ 1

#define MATTEO_M_OWNER 25
/*
 * net/sched/sch_api.c	Packet scheduler API.
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:	Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 *
 * Fixes:
 *
 * Rani Assaf <rani@magic.metawire.com> :980802: JIFFIES and CPU clock sources are repaired.
 * Eduardo J. Blanco <ejbs@netlabs.com.uy> :990222: kmod support
 * Jamal Hadi Salim <hadi@nortelnetworks.com>: 990601: ingress support
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/skbuff.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kmod.h>
#include <linux/list.h>
#include <linux/hrtimer.h>
#include <linux/lockdep.h>
#include <linux/slab.h>

#include <net/net_namespace.h>
#include <net/sock.h>
#include <net/netlink.h>
#include <net/pkt_sched.h>



int tc_modify_qdisc(struct sk_buff *skb, struct nlmsghdr *n, void *arg);
int tc_ctl_tfilter(struct sk_buff *skb, struct nlmsghdr *n, void *arg);
int tc_ctl_tclass(struct sk_buff *skb, struct nlmsghdr *n, void *arg);
int tc_get_qdisc(struct sk_buff *skb, struct nlmsghdr *n, void *arg);

#endif