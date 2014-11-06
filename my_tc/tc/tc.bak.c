#include "my_tc.h"

struct rtnl_handle rth;

int show_stats = 0;
int show_details = 0;
int show_raw = 0;
int show_pretty = 0;

int resolve_hosts = 0;
int use_iec = 0;
int force = 0;

// static struct qdisc_util * qdisc_list;

struct ieee80211_sub_if_data * dummy_sdata = NULL;
struct net_device            * dummy_dev   = NULL;
struct ieee80211_local       * dummy_local = NULL;

/*
static int print_noqopt(struct qdisc_util *qu, FILE *f,
			struct rtattr opt)
{
	if (opt && RTA_PAYLOAD(opt))
		printk(KERN_DEBUG "[MAT] [Unknown qdisc, optlen=%u] ",
			(unsigned) RTA_PAYLOAD(opt));
	return 0;
	printk(KERN_DEBUG "[MAT] tc.c | print_noqopt()");
	return 0;
}
*/

/*
static int parse_noqopt(struct qdisc_util *qu, int argc, char **argv, struct nlmsghdr *n)
{
	if (argc) {
		printk(KERN_DEBUG "[MAT] Unknown qdisc \"%s\", hence option \"%s\" is unparsable\n", qu->id, *argv);
		return -1;
	}
	return 0;
	printk(KERN_DEBUG "[MAT] tc.c | parse_noqopt()");
	return 0;
}
*/

/*
static int print_nofopt(struct filter_util *qu, FILE *f, struct rtattr *opt, __u32 fhandle)
{
	if (opt && RTA_PAYLOAD(opt))
		printk(KERN_DEBUG  "[MAT] fh %08x [Unknown filter, optlen=%u] ",
			fhandle, (unsigned) RTA_PAYLOAD(opt));
	else if (fhandle)
		printk(KERN_DEBUG "[MAT] fh %08x ", fhandle);
	return 0;
	printk(KERN_DEBUG "[MAT] tc.c | print_nofopt()");
	return 0;
}
*/


/*
static int parse_nofopt(struct filter_util *qu, char *fhandle, int argc, char **argv, struct nlmsghdr *n)
{
	__u32 handle;

	if (argc) {
		printk(KERN_DEBUG "[MAT] Unknown filter \"%s\", hence option \"%s\" is unparsable\n", qu->id, *argv);
		return -1;
	}
	if (fhandle) {
		struct tcmsg *t = NLMSG_DATA(n);
		if (get_u32(&handle, fhandle, 16)) {
			printk(KERN_DEBUG "[MAT] Unparsable filter ID \"%s\"\n", fhandle);
			return -1;
		}
		t->tcm_handle = handle;
	}
	printk(KERN_DEBUG "[MAT] tc.c | parse_nofopt()");
	return 0;
}
*/

struct qdisc_util *get_qdisc_kind(const char *str)
{
	struct qdisc_util *q=NULL;
	if (strcmp(str, "multiq")==0) {
	    printk(KERN_DEBUG "[MAT] is a multiq\n");
	    q=get_multiq(); 
	}
	else if (strcmp(str, "htb")==0) {
	    printk(KERN_DEBUG "[MAT] is a htb\n");
	    q=get_htb();
	}
	else if (strcmp(str, "prio")==0) {
	    printk(KERN_DEBUG "[MAT] is a prio\n");
	    q=get_prio();
	}
	else {
	    printk(KERN_DEBUG "[MAT] NO qdisc found\n");
	    q=NULL;
	}
	if(!q)
	  printk(KERN_DEBUG "[MAT] wrong...\n");
	return q;
}

/*
struct filter_util *get_filter_kind(const char *str)
{
	void *dlh;
	char buf[256];
	struct filter_util *q;

	for (q = filter_list; q; q = q->next)
		if (strcmp(q->id, str) == 0)
			return q;

	snprintf(buf, sizeof(buf), "%s/f_%s.so", get_tc_lib(), str);
	dlh = dlopen(buf, RTLD_LAZY);
	if (dlh == NULL) {
		dlh = BODY;
		if (dlh == NULL) {
			dlh = BODY = dlopen(NULL, RTLD_LAZY);
			if (dlh == NULL)
				goto noexist;
		}
	}

	snprintf(buf, sizeof(buf), "%s_filter_util", str);
	q = dlsym(dlh, buf);
	if (q == NULL)
		goto noexist;

reg:
	q->next = filter_list;
	filter_list = q;
	return q;
noexist:
	q = malloc(sizeof(*q));
	if (q) {
		memset(q, 0, sizeof(*q));
		strncpy(q->id, str, 15);
		q->parse_fopt = parse_nofopt;
		q->print_fopt = print_nofopt;
		goto reg;
	}
	return q;
	printk(KERN_DEBUG "[MAT] tc.c | get_filter_kind()");
	return 0;
}
*/

static void usage(void)
{
	printk(KERN_DEBUG "[MAT] tc.c | usage()");
}

int do_cmd(int argc, char **argv)
{
	printk( KERN_DEBUG "[MAT] -> %s\n", *argv);
	if (matches(*argv, "qdisc") == 0)
		return do_qdisc(argc-1, argv+1);
	if (matches(*argv, "class") == 0)
		return do_class(argc-1, argv+1);

/*
	if (matches(*argv, "filter") == 0)
		return do_filter(argc-1, argv+1);

	if (matches(*argv, "actions") == 0)
		return do_action(argc-1, argv+1);

	if (matches(*argv, "monitor") == 0)
		return do_tcmonitor(argc-1, argv+1);

	if (matches(*argv, "help") == 0) {
		usage();
		return 0;
	}
*/
	printk(KERN_DEBUG "[MAT] Object \"%s\" is unknown, try \"tc help\".\n",
		*argv);
	return -1;
}

/*
static int batch(const char *name)
{
	char *line = NULL;
	size_t len = 0;
	int ret = 0;

	if (name && strcmp(name, "-") != 0) {
		if (freopen(name, "r", stdin) == NULL) {
			printk(KERN_DEBUG "[MAT] Cannot open file \"%s\" for reading: %s\n",
				name, strerror(errno));
			return -1;
		}
	}

	tc_core_init();

	if (rtnl_open(&rth, 0) < 0) {
		fprintf(stderr, "Cannot open rtnetlink\n");
		return -1;
	}

	cmdlineno = 0;
	while (getcmdline(&line, &len, stdin) != -1) {
		char *largv[100];
		int largc;

		largc = makeargs(line, largv, 100);
		if (largc == 0)
			continue;	

		if (do_cmd(largc, largv)) {
			fprintf(stderr, "Command failed %s:%d\n", name, cmdlineno);
			ret = 1;
			if (!force)
				break;
		}
	}
	if (line)
		free(line);

	rtnl_close(&rth);
	return ret;
	return 0;
}
*/

int start_tc_bystr(char *str) {
    int i = 0;
    int i_w = 0;
    int last_space = 0;
    int argc = 0;
    char ** argv;
    
    while(str[i]!='\0') {
      if(str[i]==' ') {
	argc++;
      }
      i++;
    }
    argc++;
    argv = vmalloc( argc*(sizeof(char*)) );
    i=0;
    do {
      i++;
      if(str[i]==' ' || str[i]=='\0') {
	argv[i_w] = vmalloc((i-last_space)*sizeof(char));
	strncpy( (argv[i_w]), (str+last_space), 	(i-last_space));
	*(argv[i_w]+(i-last_space)) = '\0';
	i_w++;
	last_space = i+1;
      }
    } while (str[i]!='\0');
    
    start_tc(argc, argv);
}
EXPORT_SYMBOL(start_tc_bystr);

int start_tc(int argc, char **argv)
{
	int ret;
//	int do_batching = 0;
//	char *batchfile = NULL;
/*
	while (argc > 1) {
		if (argv[1][0] != '-')
			break;
		if (matches(argv[1], "-stats") == 0 ||
			 matches(argv[1], "-statistics") == 0) {
			++show_stats;
		} else if (matches(argv[1], "-details") == 0) {
			++show_details;
		} else if (matches(argv[1], "-raw") == 0) {
			++show_raw;
		} else if (matches(argv[1], "-pretty") == 0) {
			++show_pretty;
		} else if (matches(argv[1], "-Version") == 0) {
			printf("tc utility, iproute2-ss%s\n", SNAPSHOT);
			return 0;
		} else if (matches(argv[1], "-iec") == 0) {
			++use_iec;
		} else if (matches(argv[1], "-help") == 0) {
			usage();
			return 0;
		} else if (matches(argv[1], "-force") == 0) {
			++force;
		} else 	if (matches(argv[1], "-batch") == 0) {
			do_batching = 1;
			if (argc > 2)
				batchfile = argv[2];
			argc--;	argv++;
		} else {
			fprintf(stderr, "Option \"%s\" is unknown, try \"tc -help\".\n", argv[1]);
			return -1;
		}
		argc--;	argv++;
	}

	if (do_batching)
		return batch(batchfile);
*/
	if (argc <= 1) {
		usage();
		return 0;
	}

/*
	tc_core_init();
	if (rtnl_open(&rth, 0) < 0) {
		fprintf(stderr, "[MAT] attempted to open \n");
		exit(1);
	}
*/
	ret = do_cmd(argc-1, argv+1);
//	rtnl_close(&rth);

	return ret;
}
EXPORT_SYMBOL(start_tc);

void tc_init_local(struct ieee80211_local * local) {
	struct ieee80211_sub_if_data *sdata;
	dummy_local = local;
	
	printk(KERN_DEBUG "[FLAVIA] [MATTEO] [TC] LOCAL INIT\n");
	
	if(!dummy_local) goto genericerror;
	printk(KERN_DEBUG "[FLAVIA] [MATTEO] [TC] local setted (%x).\n", (unsigned int) dummy_local);
		
	mutex_lock(&(dummy_local->iflist_mtx));
	list_for_each_entry(sdata, &local->interfaces, list) {
		if (sdata->vif.type == NL80211_IFTYPE_STATION) {
			dummy_sdata = sdata;
			printk(KERN_DEBUG "[FLAVIA] [MATTEO] [TC] sub_if_data setted (%x).\n", (unsigned int) dummy_sdata);
			break;
		}
	}
	mutex_unlock(&(local->iflist_mtx));
	
	return;
	
genericerror:
	printk(KERN_DEBUG "[FLAVIA] [MATTEO] [TC] something somewhere went wrong...\n");
} 
EXPORT_SYMBOL(tc_init_local);

static int __init
tc_init(void)
{
  printk(KERN_DEBUG "[FLAVIA] [MATTEO] [TC] loaded\n");
  struct mtc_ops my_ops = {
    .set_local = tc_init_local
  };
  
  register_mtc_ops(&my_ops);
  return 0;
}


static void __exit
tc_exit(void)
{
	printk(KERN_DEBUG "[FLAVIA] [TC] module unloaded.\n");
}


subsys_initcall(tc_init);
module_exit(tc_exit);

MODULE_DESCRIPTION("FLAVIA TC");
MODULE_LICENSE("GPL");
