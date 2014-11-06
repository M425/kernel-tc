#ifndef MY_TC_H
#define MY_TC_H 1


//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <syslog.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/string.h>
#include <linux/rtnetlink.h>
#include <linux/pkt_sched.h>
#include <linux/atm.h>
#include <linux/netdevice.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/skbuff.h>
#include <linux/etherdevice.h>
#include <linux/if_arp.h>
#include <linux/wireless.h>
#include <linux/bitmap.h>
#include <asm-generic/int-ll64.h>

// #include "../../../../include/net/pkt_sched.h"
#include <net/pkt_sched.h>


#include "../../ieee80211_i.h"

//#include "SNAPSHOT.h"
#include "../include/utils.h"
#include "../include/rt_names.h"
#include "../include/pkt_cls.h"
// #include "../include/ll_map.h"
// #include "../include/libnetlink.h"
#include "tc_util.h"
#include "tc_common.h"
#include "tc_core.h"

/* Limits of integral types. */
#ifndef INT8_MIN
#define INT8_MIN               (-128)
#endif
#ifndef INT16_MIN
#define INT16_MIN              (-32767-1)
#endif
#ifndef INT32_MIN
#define INT32_MIN              (-2147483647-1)
#endif
#ifndef INT8_MAX
#define INT8_MAX               (127)
#endif
#ifndef INT16_MAX
#define INT16_MAX              (32767)
#endif
#ifndef INT32_MAX
#define INT32_MAX              (2147483647)
#endif
#ifndef UINT8_MAX
#define UINT8_MAX              (255U)
#endif
#ifndef UINT16_MAX
#define UINT16_MAX             (65535U)
#endif
#ifndef UINT32_MAX
#define UINT32_MAX             (4294967295U)
#endif


struct rtnl_handle
{
	int			fd;
	struct sockaddr_nl	local;
	struct sockaddr_nl	peer;
	__u32			seq;
	__u32			dump;
};


struct mtc_qdisc_list
{
	struct list_head list;

	char * id;
	struct qdisc_util * q;
};
struct mtc_filter_list
{
	struct list_head list;

	char * id;
	struct filter_util * q;
};

/*
int show_stats = 0;
int show_details = 0;
int show_raw = 0;
int show_pretty = 0;
*/

/*
int resolve_hosts = 0;
int use_iec = 0;
int force = 0;
*/

// static void *BODY = NULL;
// static struct qdisc_util * qdisc_list;
// static struct filter_util * filter_list;

struct qdisc_util *get_qdisc_kind(const char *str);
void register_qdisc_tc(struct qdisc_util *q);

struct filter_util *get_filter_kind(const char *str);
void register_filter_tc(struct filter_util *f);

int multiq_parse_opt(struct qdisc_util *qu, int argc, char **argv, struct nlmsghdr *n);
// int multiq_print_opt(struct qdisc_util *qu, FILE *f, struct rtattr *opt);
struct qdisc_util * get_multiq(void);

int prio_parse_opt(struct qdisc_util *qu, int argc, char **argv, struct nlmsghdr *n);
// int prio_print_opt(struct qdisc_util *qu, FILE *f, struct rtattr *opt);
struct qdisc_util * get_prio(void);
void prio_explain(void);

int htb_parse_opt      (struct qdisc_util *qu, int argc, char **argv, struct nlmsghdr *n);
int htb_parse_class_opt(struct qdisc_util *qu, int argc, char **argv, struct nlmsghdr *n);
// int htb_print_opt(struct qdisc_util *qu, FILE *f, struct rtattr *opt)
// int htb_print_xstats(struct qdisc_util *qu, FILE *f, struct rtattr *xstats)
struct qdisc_util * get_htb(void);


/*
 libnetlink.h
 */
extern int rcvbuf;

// extern int rtnl_open(struct rtnl_handle *rth, unsigned subscriptions);
// extern int rtnl_open_byproto(struct rtnl_handle *rth, unsigned subscriptions, int protocol);
// extern void rtnl_close(struct rtnl_handle *rth);
// extern int rtnl_wilddump_request(struct rtnl_handle *rth, int fam, int type);
// extern int rtnl_dump_request(struct rtnl_handle *rth, int type, void *req, int len);
// 
typedef int (*rtnl_filter_t)(const struct sockaddr_nl *, struct nlmsghdr *n, void *);
// 
struct rtnl_dump_filter_arg
{
	rtnl_filter_t filter;
	void *arg1;
};

int start_tc(int argc, char **argv);
int start_tc_bystr(char * str);
void tc_init_local(struct ieee80211_local * local);

// extern int rtnl_dump_filter_l(struct rtnl_handle *rth,
// 			      const struct rtnl_dump_filter_arg *arg);
// extern int rtnl_dump_filter(struct rtnl_handle *rth, rtnl_filter_t filter,
// 			    void *arg);
extern int rtnl_talk(struct rtnl_handle *rtnl, struct nlmsghdr *n, pid_t peer, unsigned groups, struct nlmsghdr *answer);
// extern int rtnl_send(struct rtnl_handle *rth, const void *buf, int);
// extern int rtnl_send_check(struct rtnl_handle *rth, const void *buf, int);


int addattr(struct nlmsghdr *n, int maxlen, int type);
int addattr8(struct nlmsghdr *n, int maxlen, int type, __u8 data);
int addattr16(struct nlmsghdr *n, int maxlen, int type, __u16 data);
int addattr32(struct nlmsghdr *n, int maxlen, int type, __u32 data);
int addattr64(struct nlmsghdr *n, int maxlen, int type, __u64 data);
int addattrstrz(struct nlmsghdr *n, int maxlen, int type, const char *data);

int addattr_l(struct nlmsghdr *n, int maxlen, int type, const void *data, int alen);


// extern int addraw_l(struct nlmsghdr *n, int maxlen, const void *data, int len);
struct rtattr *addattr_nest(struct nlmsghdr *n, int maxlen, int type);
int addattr_nest_end(struct nlmsghdr *n, struct rtattr *nest);
struct rtattr *addattr_nest_compat(struct nlmsghdr *n, int maxlen, int type, const void *data, int len);
int addattr_nest_compat_end(struct nlmsghdr *n, struct rtattr *nest);
// extern int rta_addattr32(struct rtattr *rta, int maxlen, int type, __u32 data);
// extern int rta_addattr_l(struct rtattr *rta, int maxlen, int type, const void *data, int alen);

// extern int parse_rtattr(struct rtattr *tb[], int max, struct rtattr *rta, int len);
// extern int parse_rtattr_byindex(struct rtattr *tb[], int max, struct rtattr *rta, int len);
// extern int __parse_rtattr_nested_compat(struct rtattr *tb[], int max, struct rtattr *rta, int len);


int parse_police(int *argc_p, char ***argv_p, int tca_id, struct nlmsghdr *n);
int act_parse_police(struct action_util *a,int *argc_p, char ***argv_p, int tca_id, struct nlmsghdr *n);

#define parse_rtattr_nested(tb, max, rta) \
	(parse_rtattr((tb), (max), RTA_DATA(rta), RTA_PAYLOAD(rta)))

#define parse_rtattr_nested_compat(tb, max, rta, data, len) \
	({ data = RTA_PAYLOAD(rta) >= len ? RTA_DATA(rta) : NULL;	\
		__parse_rtattr_nested_compat(tb, max, rta, len); })

// static inline __u8 rta_getattr_u8(const struct rtattr *rta)
// {
// 	return *(__u8 *)RTA_DATA(rta);
// }
// static inline __u16 rta_getattr_u16(const struct rtattr *rta)
// {
// 	return *(__u16 *)RTA_DATA(rta);
// }
// static inline __u32 rta_getattr_u32(const struct rtattr *rta)
// {
// 	return *(__u32 *)RTA_DATA(rta);
// }
// static inline __u64 rta_getattr_u64(const struct rtattr *rta)
// {
// 	__u64 tmp;
// 	memcpy(&tmp, RTA_DATA(rta), sizeof(__u64));
// 	return tmp;
// }
// static inline const char *rta_getattr_str(const struct rtattr *rta)
// {
// 	return (const char *)RTA_DATA(rta);
// }

// extern int rtnl_listen(struct rtnl_handle *, rtnl_filter_t handler,
// 		       void *jarg);
// extern int rtnl_from_file(FILE *, rtnl_filter_t handler,
// 		       void *jarg);


#define NLMSG_TAIL(nmsg) \
	((struct rtattr *) (((void *) (nmsg)) + NLMSG_ALIGN((nmsg)->nlmsg_len)))

#ifndef IFA_RTA
#define IFA_RTA(r) \
	((struct rtattr*)(((char*)(r)) + NLMSG_ALIGN(sizeof(struct ifaddrmsg))))
#endif
#ifndef IFA_PAYLOAD
#define IFA_PAYLOAD(n)	NLMSG_PAYLOAD(n,sizeof(struct ifaddrmsg))
#endif

#ifndef IFLA_RTA
#define IFLA_RTA(r) \
	((struct rtattr*)(((char*)(r)) + NLMSG_ALIGN(sizeof(struct ifinfomsg))))
#endif
#ifndef IFLA_PAYLOAD
#define IFLA_PAYLOAD(n)	NLMSG_PAYLOAD(n,sizeof(struct ifinfomsg))
#endif

#ifndef NDA_RTA
#define NDA_RTA(r) \
	((struct rtattr*)(((char*)(r)) + NLMSG_ALIGN(sizeof(struct ndmsg))))
#endif
#ifndef NDA_PAYLOAD
#define NDA_PAYLOAD(n)	NLMSG_PAYLOAD(n,sizeof(struct ndmsg))
#endif

#ifndef NDTA_RTA
#define NDTA_RTA(r) \
	((struct rtattr*)(((char*)(r)) + NLMSG_ALIGN(sizeof(struct ndtmsg))))
#endif
#ifndef NDTA_PAYLOAD
#define NDTA_PAYLOAD(n) NLMSG_PAYLOAD(n,sizeof(struct ndtmsg))
#endif

#endif 