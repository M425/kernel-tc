#include "my_tc.h"

void prio_explain(void)
{
	printk(KERN_DEBUG "[MAT] [prio.c] wrong use\n");
}

int prio_parse_opt(struct qdisc_util *qu, int argc, char **argv, struct nlmsghdr *n)
{
	int pmap_mode = 0;
	int idx = 0;
	struct tc_prio_qopt opt={3,{ 1, 2, 2, 2, 1, 2, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 }};
	struct rtattr *nest;
	unsigned char mq = 0;

	while (argc > 0) {
		if (strcmp(*argv, "bands") == 0) {
			if (pmap_mode)
				prio_explain();
			NEXT_ARG();
			if (get_integer(&opt.bands, *argv, 10)) {
				printk(KERN_DEBUG "Illegal \"bands\"\n");
				return -1;
			}
		} else if (strcmp(*argv, "priomap") == 0) {
			if (pmap_mode) {
				printk(KERN_DEBUG "Error: duplicate priomap\n");
				return -1;
			}
			pmap_mode = 1;
		} else if (strcmp(*argv, "multiqueue") == 0) {
			mq = 1;
		} else if (strcmp(*argv, "help") == 0) {
			prio_explain();
			return -1;
		} else {
			unsigned band;
			if (!pmap_mode) {
				printk(KERN_DEBUG "What is \"%s\"?\n", *argv);
				prio_explain();
				return -1;
			}
			if (get_unsigned(&band, *argv, 10)) {
				printk(KERN_DEBUG "Illegal \"priomap\" element\n");
				return -1;
			}
			if (band >= opt.bands) {
				printk(KERN_DEBUG "\"priomap\" element is out of bands\n");
				return -1;
			}
			if (idx > TC_PRIO_MAX) {
				printk(KERN_DEBUG "\"priomap\" index > TC_PRIO_MAX=%u\n", TC_PRIO_MAX);
				return -1;
			}
			opt.priomap[idx++] = band;
		}
		argc--; argv++;
	}

/*
	if (pmap_mode) {
		for (; idx < TC_PRIO_MAX; idx++)
			opt.priomap[idx] = opt.priomap[TC_PRIO_BESTEFFORT];
	}
*/

	nest = addattr_nest_compat(n, 1024, TCA_OPTIONS, &opt, sizeof(opt));
	if (mq)
		addattr_l(n, 1024, TCA_PRIO_MQ, NULL, 0);
	addattr_nest_compat_end(n, nest);
	return 0;
}

/*
int prio_print_opt(struct qdisc_util *qu, FILE *f, struct rtattr *opt)
{
	int i;
	struct tc_prio_qopt *qopt;
	struct rtattr *tb[TCA_PRIO_MAX+1];

	if (opt == NULL)
		return 0;

	if (parse_rtattr_nested_compat(tb, TCA_PRIO_MAX, opt, qopt,
					sizeof(*qopt)))
                return -1;

	fprintf(f, "bands %u priomap ", qopt->bands);
	for (i=0; i<=TC_PRIO_MAX; i++)
		fprintf(f, " %d", qopt->priomap[i]);

	if (tb[TCA_PRIO_MQ])
		fprintf(f, " multiqueue: %s ",
			rta_getattr_u8(tb[TCA_PRIO_MQ]) ? "on" : "off");

	return 0;
}
*/

struct qdisc_util * get_prio() {
	struct qdisc_util * q = vmalloc(sizeof(struct qdisc_util));
	if(!q)
	    return NULL;
	memset(q, 0, sizeof(struct qdisc_util));
	q->id	 	= "prio";
	q->parse_qopt	= prio_parse_opt;
// 	q->print_qopt	= prio_print_opt;
	return q;
};

/*
struct qdisc_util prio_qdisc_util = {
	.id	 	= "prio",
	.parse_qopt	= prio_parse_opt,
	.print_qopt	= prio_print_opt,
};
*/
