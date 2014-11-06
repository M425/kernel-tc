#include "my_tc.h"

static void explain(void)
{
	printk(KERN_DEBUG "[MTC] [F_FW] Invalid input\n");
}

static int fw_parse_opt(struct filter_util *qu, char *handle, int argc, char **argv, struct nlmsghdr *n)
{
	struct tc_police tp;
	struct tcmsg *t = NLMSG_DATA(n);
	struct rtattr *tail;
	__u32 mask = 0;
	int mask_set = 0;

	memset(&tp, 0, sizeof(tp));

	if (handle) {
		char *slash;
		if ((slash = strchr(handle, '/')) != NULL)
			*slash = '\0';
		if (get_u32(&t->tcm_handle, handle, 0)) {
			printk(KERN_DEBUG "[MTC] [F_FW] Illegal \"handle\"\n");
			return -1;
		}
		if (slash) {
			if (get_u32(&mask, slash+1, 0)) {
				printk(KERN_DEBUG "[MTC] [F_FW] Illegal \"handle\" mask\n");
				return -1;
			}
			mask_set = 1;
		}
	}

	if (argc == 0)
		return 0;

	tail = NLMSG_TAIL(n);
	addattr_l(n, 4096, TCA_OPTIONS, NULL, 0);

	if (mask_set)
		addattr32(n, MAX_MSG, TCA_FW_MASK, mask);

	while (argc > 0) {
		if (matches(*argv, "classid") == 0 ||
		    matches(*argv, "flowid") == 0) {
			unsigned handle;
			NEXT_ARG();
			if (get_tc_classid(&handle, *argv)) {
				printk(KERN_DEBUG "[MTC] [FW] Illegal \"classid\"\n");
				return -1;
			}
			addattr_l(n, 4096, TCA_FW_CLASSID, &handle, 4);
		} else if (matches(*argv, "police") == 0) {
			NEXT_ARG();
			if (parse_police(&argc, &argv, TCA_FW_POLICE, n)) {
				printk(KERN_DEBUG "[MTC] [F_FW] Illegal \"police\"\n");
				return -1;
			}
			continue;
		} /* else if (matches(*argv, "action") == 0) {
			NEXT_ARG();
			if (parse_action(&argc, &argv, TCA_FW_ACT, n)) {
				printk(KERN_DEBUG "Illegal fw \"action\"\n");
				return -1;
			}
			continue;
		} */ else if (strcmp(*argv, "indev") == 0) {
			char d[IFNAMSIZ+1];
			memset(d, 0, sizeof (d));
			argc--;
			argv++;
			if (argc < 1) {
				printk(KERN_DEBUG "[MTC] [F_FW] Illegal indev\n");
				return -1;
			}
			strncpy(d, *argv, sizeof (d) - 1);
			addattr_l(n, MAX_MSG, TCA_FW_INDEV, d, strlen(d) + 1);
		} else if (strcmp(*argv, "help") == 0) {
			explain();
			return -1;
		} else {
			printk(KERN_DEBUG "[MTC] [F_FW] What is \"%s\"?\n", *argv);
			explain();
			return -1;
		}
		argc--; argv++;
	}
	tail->rta_len = (void *) NLMSG_TAIL(n) - (void *) tail;
	return 0;
}

/*
static int fw_print_opt(struct filter_util *qu, FILE *f, struct rtattr *opt, __u32 handle)
{
	struct rtattr *tb[TCA_FW_MAX+1];

	if (opt == NULL)
		return 0;

	parse_rtattr_nested(tb, TCA_FW_MAX, opt);

	if (handle || tb[TCA_FW_MASK]) {
		__u32 mark = 0, mask = 0;
		if(handle)
			mark = handle;
		if(tb[TCA_FW_MASK] &&
		    (mask = rta_getattr_u32(tb[TCA_FW_MASK])) != 0xFFFFFFFF)
			fprintf(f, "handle 0x%x/0x%x ", mark, mask);
		else
			fprintf(f, "handle 0x%x ", handle);
	}

	if (tb[TCA_FW_CLASSID]) {
		SPRINT_BUF(b1);
		fprintf(f, "classid %s ", sprint_tc_classid(rta_getattr_u32(tb[TCA_FW_CLASSID]), b1));
	}

	if (tb[TCA_FW_POLICE])
		tc_print_police(f, tb[TCA_FW_POLICE]);
	if (tb[TCA_FW_INDEV]) {
		struct rtattr *idev = tb[TCA_FW_INDEV];
		fprintf(f, "input dev %s ",rta_getattr_str(idev));
	}

	if (tb[TCA_FW_ACT]) {
		fprintf(f, "\n");
		tc_print_action(f, tb[TCA_FW_ACT]);
	}
	return 0;
}
*/

/*
struct filter_util fw_filter_util = {
	.id = "fw",
	.parse_fopt = fw_parse_opt,
	.print_fopt = fw_print_opt,
};
*/

struct filter_util * get_fw() {
	struct filter_util * q = vmalloc(sizeof(struct filter_util));
	if(!q)
	    return NULL;
	memset(q, 0, sizeof(struct filter_util));
	q->id	 	= "fw";
	q->parse_fopt	= fw_parse_opt;
// 	q->print_fopt	= fw_print_opt,
 	return q;
};



static int __init
tc_f_fw_init(void)
{
    printk(KERN_DEBUG "[MTC] [F_FW] loaded\n");
    struct filter_util * q = get_fw();
    printk(KERN_DEBUG "[MTC] [F_FW] preregister\n");
    
    register_filter_tc(q);
    printk(KERN_DEBUG "[MTC] [F_FW] register\n");
    
    return 0;
}


static void __exit
tc_f_fw_exit(void)
{
	printk(KERN_DEBUG "[MTC] [F_FW] exit\n");
}


subsys_initcall(tc_f_fw_init);
module_exit(tc_f_fw_exit);

MODULE_DESCRIPTION("F_FW");
MODULE_LICENSE("GPL");

