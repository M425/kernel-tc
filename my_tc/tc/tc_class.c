
#include "my_tc.h"

extern struct rtnl_handle rth;

static void usage(void);

static void usage(void)
{  
	printk(KERN_DEBUG "[MTC] Invalid input\n");
	return;
}

int tc_class_modify(int cmd, unsigned flags, int argc, char **argv)
{
	struct qdisc_util *q = NULL;
	struct tc_estimator est;
	struct {
		struct nlmsghdr 	n;
		struct tcmsg 		t;
		char  			buf[TCA_BUF_MAX];
	} * req;
	char  d[16];
	char  k[16];

	req = vmalloc(sizeof(*req));
	
	memset(req, 0, sizeof(*req));
	memset(&est, 0, sizeof(est));
	memset(d, 0, sizeof(d));
	memset(k, 0, sizeof(k));
	
	req->n.nlmsg_len = NLMSG_LENGTH(sizeof(struct tcmsg));
	req->n.nlmsg_flags = NLM_F_REQUEST|flags;
	req->n.nlmsg_type = cmd;
	req->t.tcm_family = AF_UNSPEC;

	/*
	printk(KERN_DEBUG "NLM_F_CREATE:  %d\n", NLM_F_CREATE);
	printk(KERN_DEBUG "NLM_F_REPLACE: %d\n", NLM_F_REPLACE);
	printk(KERN_DEBUG "NLM_F_REQUEST: %d\n", NLM_F_REQUEST);
	printk(KERN_DEBUG "NLM_F_EXCL:    %d\n", NLM_F_EXCL);
	printk(KERN_DEBUG "RTM_NEWQDISC:  %d\n", RTM_NEWQDISC);
	printk(KERN_DEBUG "RTM_DELQDISC:  %d\n", RTM_DELQDISC);
	printk(KERN_DEBUG "cmd:           %d\n", cmd);
	printk(KERN_DEBUG "TCA_BUF_MAX:   %d\n", (TCA_BUF_MAX*sizeof(char)));
	printk(KERN_DEBUG "req:           %d\n", sizeof(*req) );
	*/
	
	while (argc > 0) {
		if (strcmp(*argv, "dev") == 0) {
			NEXT_ARG();
			if (d[0])
				duparg("dev", *argv);
			strncpy(d, *argv, sizeof(d)-1);
		} else if (strcmp(*argv, "classid") == 0) {
			__u32 handle;
			NEXT_ARG();
			if (req->t.tcm_handle)
				duparg("classid", *argv);
			if (get_tc_classid(&handle, *argv))
				invarg("invalid class ID", *argv);
			req->t.tcm_handle = handle;
		} else if (strcmp(*argv, "handle") == 0) {
			printk(KERN_DEBUG "[MTC] Error: try \"classid\" instead of \"handle\"\n");
			return -1;
 		} else if (strcmp(*argv, "root") == 0) {
			if (req->t.tcm_parent) {
				printk(KERN_DEBUG "[MTC] Error: \"root\" is duplicate parent ID.\n");
				return -1;
			}
			req->t.tcm_parent = TC_H_ROOT;
		} else if (strcmp(*argv, "parent") == 0) {
			__u32 handle;
			NEXT_ARG();
			if (req->t.tcm_parent)
				duparg("parent", *argv);
			if (get_tc_classid(&handle, *argv))
				invarg("[MTC] invalid parent ID", *argv);
			req->t.tcm_parent = handle;
		} /* else if (matches(*argv, "estimator") == 0) {
			if (parse_estimator(&argc, &argv, &est))
				return -1;
		} */ else if (matches(*argv, "help") == 0) {
			usage();
		} else {
			strncpy(k, *argv, sizeof(k)-1);

			q = get_qdisc_kind(k);
			argc--; argv++;
			break;
		}
		argc--; argv++;
	}

	if (k[0])
		addattr_l(&req->n, sizeof(*req), TCA_KIND, k, strlen(k)+1);
	if (est.ewma_log)
		addattr_l(&req->n, sizeof(*req), TCA_RATE, &est, sizeof(est));

	if (q) {
		if (q->parse_copt == NULL) {
			printk(KERN_DEBUG "[MTC] Error: Qdisc \"%s\" is classless.\n", k);
			return 1;
		}
		if (q->parse_copt(q, argc, argv, &req->n))
			return 1;
	} else {
		if (argc) {
			if (matches(*argv, "help") == 0)
				usage();
			printk(KERN_DEBUG "[MTC] Garbage instead of arguments \"%s ...\". Try \"tc class help\".\n", *argv);
			return -1;
		}
	}

	/*
	if (d[0])  {
		ll_init_map(&rth);

		if ((req.t.tcm_ifindex = ll_name_to_index(d)) == 0) {
			fprintf(stderr, "Cannot find device \"%s\"\n", d);
			return 1;
		}
	}
	*/
	
	if (rtnl_talk(&rth, &req->n, 0, 0, NULL) < 0)
		return 2;
	return 0;
}

// int filter_ifindex;
// __u32 filter_qdisc;
// __u32 filter_classid;

/*
int print_class(const struct sockaddr_nl *who,
		       struct nlmsghdr *n, void *arg)
{
	FILE *fp = (FILE*)arg;
	struct tcmsg *t = NLMSG_DATA(n);
	int len = n->nlmsg_len;
	struct rtattr * tb[TCA_MAX+1];
	struct qdisc_util *q;
	char abuf[256];

	if (n->nlmsg_type != RTM_NEWTCLASS && n->nlmsg_type != RTM_DELTCLASS) {
		fprintf(stderr, "Not a class\n");
		return 0;
	}
	len -= NLMSG_LENGTH(sizeof(*t));
	if (len < 0) {
		fprintf(stderr, "Wrong len %d\n", len);
		return -1;
	}
	if (filter_qdisc && TC_H_MAJ(t->tcm_handle^filter_qdisc))
		return 0;

	if (filter_classid && t->tcm_handle != filter_classid)
		return 0;

	memset(tb, 0, sizeof(tb));
	parse_rtattr(tb, TCA_MAX, TCA_RTA(t), len);

	if (tb[TCA_KIND] == NULL) {
		fprintf(stderr, "print_class: NULL kind\n");
		return -1;
	}

	if (n->nlmsg_type == RTM_DELTCLASS)
		fprintf(fp, "deleted ");

	abuf[0] = 0;
	if (t->tcm_handle) {
		if (filter_qdisc)
			print_tc_classid(abuf, sizeof(abuf), TC_H_MIN(t->tcm_handle));
		else
			print_tc_classid(abuf, sizeof(abuf), t->tcm_handle);
	}
	fprintf(fp, "class %s %s ", rta_getattr_str(tb[TCA_KIND]), abuf);

	if (filter_ifindex == 0)
		fprintf(fp, "dev %s ", ll_index_to_name(t->tcm_ifindex));

	if (t->tcm_parent == TC_H_ROOT)
		fprintf(fp, "root ");
	else {
		if (filter_qdisc)
			print_tc_classid(abuf, sizeof(abuf), TC_H_MIN(t->tcm_parent));
		else
			print_tc_classid(abuf, sizeof(abuf), t->tcm_parent);
		fprintf(fp, "parent %s ", abuf);
	}
	if (t->tcm_info)
		fprintf(fp, "leaf %x: ", t->tcm_info>>16);
	q = get_qdisc_kind(RTA_DATA(tb[TCA_KIND]));
	if (tb[TCA_OPTIONS]) {
		if (q && q->print_copt)
			q->print_copt(q, fp, tb[TCA_OPTIONS]);
		else
			fprintf(fp, "[cannot parse class parameters]");
	}
	fprintf(fp, "\n");
	if (show_stats) {
		struct rtattr *xstats = NULL;

		if (tb[TCA_STATS] || tb[TCA_STATS2]) {
			print_tcstats_attr(fp, tb, " ", &xstats);
			fprintf(fp, "\n");
		}
		if (q && (xstats || tb[TCA_XSTATS]) && q->print_xstats) {
			q->print_xstats(q, fp, xstats ? : tb[TCA_XSTATS]);
			fprintf(fp, "\n");
		}
	}
	fflush(fp);
	return 0;
}
*/

/*
int tc_class_list(int argc, char **argv)
{
	struct tcmsg t;
	char d[16];

	memset(&t, 0, sizeof(t));
	t.tcm_family = AF_UNSPEC;
	memset(d, 0, sizeof(d));

	while (argc > 0) {
		if (strcmp(*argv, "dev") == 0) {
			NEXT_ARG();
			if (d[0])
				duparg("dev", *argv);
			strncpy(d, *argv, sizeof(d)-1);
		} else if (strcmp(*argv, "qdisc") == 0) {
			NEXT_ARG();
			if (filter_qdisc)
				duparg("qdisc", *argv);
			if (get_qdisc_handle(&filter_qdisc, *argv))
				invarg("invalid qdisc ID", *argv);
		} else if (strcmp(*argv, "classid") == 0) {
			NEXT_ARG();
			if (filter_classid)
				duparg("classid", *argv);
			if (get_tc_classid(&filter_classid, *argv))
				invarg("invalid class ID", *argv);
		} else if (strcmp(*argv, "root") == 0) {
			if (t.tcm_parent) {
				fprintf(stderr, "Error: \"root\" is duplicate parent ID\n");
				return -1;
			}
			t.tcm_parent = TC_H_ROOT;
		} else if (strcmp(*argv, "parent") == 0) {
			__u32 handle;
			if (t.tcm_parent)
				duparg("parent", *argv);
			NEXT_ARG();
			if (get_tc_classid(&handle, *argv))
				invarg("invalid parent ID", *argv);
			t.tcm_parent = handle;
		} else if (matches(*argv, "help") == 0) {
			usage();
		} else {
			fprintf(stderr, "What is \"%s\"? Try \"tc class help\".\n", *argv);
			return -1;
		}

		argc--; argv++;
	}

 	ll_init_map(&rth);

	if (d[0]) {
		if ((t.tcm_ifindex = ll_name_to_index(d)) == 0) {
			fprintf(stderr, "Cannot find device \"%s\"\n", d);
			return 1;
		}
		filter_ifindex = t.tcm_ifindex;
	}

 	if (rtnl_dump_request(&rth, RTM_GETTCLASS, &t, sizeof(t)) < 0) {
		perror("Cannot send dump request");
		return 1;
	}

 	if (rtnl_dump_filter(&rth, print_class, stdout) < 0) {
		fprintf(stderr, "Dump terminated\n");
		return 1;
	}

	return 0;
}
*/

int do_class(int argc, char **argv)
{
	get_hz();
	if (argc < 1)
		return -1;
	if (matches(*argv, "add") == 0)
		return tc_class_modify(RTM_NEWTCLASS, NLM_F_EXCL|NLM_F_CREATE, argc-1, argv+1);
	if (matches(*argv, "change") == 0)
		return tc_class_modify(RTM_NEWTCLASS, 0, argc-1, argv+1);
	if (matches(*argv, "replace") == 0)
		return tc_class_modify(RTM_NEWTCLASS, NLM_F_CREATE, argc-1, argv+1);
	if (matches(*argv, "delete") == 0)
		return tc_class_modify(RTM_DELTCLASS, 0,  argc-1, argv+1);
/*	
#if 0
	if (matches(*argv, "get") == 0)
		return tc_class_get(RTM_GETTCLASS, 0,  argc-1, argv+1);
#endif
	if (matches(*argv, "list") == 0 || matches(*argv, "show") == 0
	    || matches(*argv, "lst") == 0)
		return tc_class_list(argc-1, argv+1);
*/
	if (matches(*argv, "help") == 0) {
		usage();
		return 0;
	}
	
	printk(KERN_DEBUG "[MTC] Command \"%s\" is unknown, try \"tc class help\".\n", *argv);
	return -1;
}
