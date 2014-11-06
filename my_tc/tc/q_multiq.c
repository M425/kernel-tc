#include "my_tc.h"

int multiq_parse_opt(struct qdisc_util *qu, int argc, char **argv, struct nlmsghdr *n)
{
	struct tc_multiq_qopt opt;

	if (argc) {
		if (strcmp(*argv, "help") == 0) {
			return -1;
		} else {
			return -1;
		}
	}

	addattr_l(n, 1024, TCA_OPTIONS, &opt, sizeof(opt));
	return 0;
}

/*
int multiq_print_opt(struct qdisc_util *qu, FILE *f, struct rtattr *opt)
{
	struct tc_multiq_qopt *qopt;

	if (opt == NULL)
		return 0;
	if (RTA_PAYLOAD(opt) < sizeof(*qopt))
		return 0;

	qopt = RTA_DATA(opt);

	fprintf(f, "bands %u/%u ", qopt->bands, qopt->max_bands);

	return 0;
}
*/

struct qdisc_util * get_multiq() {
	struct qdisc_util * q = vmalloc(sizeof(struct qdisc_util));
	if(!q)
	    return NULL;
	memset(q, 0, sizeof(struct qdisc_util));
	q->id	 	= "multiq";
	q->parse_qopt	= multiq_parse_opt;
// 	q->print_qopt	= multiq_print_opt,
 	return q;
};


// struct qdisc_util multiq_qdisc_util = {
// 	.id	 	= "multiq",
// 	.parse_qopt	= multiq_parse_opt,
// 	.print_qopt	= multiq_print_opt,
// };

