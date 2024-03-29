//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <syslog.h>
//#include <fcntl.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
// #include <linux/string.h>
//#include <netdb.h>
//#include <arpa/inet.h>
//#include <asm/types.h>
// #include <linux/pkt_sched.h>
//#include <time.h>
//#include <sys/time.h>
// #include <errno.h>


// #include "../include/utils.h"
#include "../tc/my_tc.h"

int get_integer(int *val, const char *arg, int base)
{
	long res;
	char *ptr;

	if (!arg || !*arg)
		return -1;
	res = strtol(arg, &ptr, base);
	if (!ptr || ptr == arg || *ptr || res > INT_MAX || res < INT_MIN)
		return -1;
	*val = res;
	return 0;
}EXPORT_SYMBOL(get_integer);

/*
int mask2bits(__u32 netmask)
{
	unsigned bits = 0;
	__u32 mask = ntohl(netmask);
	__u32 host = ~mask;

	if ((host & (host + 1)) != 0)
		return -1;

	for (; mask; mask <<= 1)
		++bits;
	return bits;
}

static int get_netmask(unsigned *val, const char *arg, int base)
{
	inet_prefix addr;

	if (!get_unsigned(val, arg, base))
		return 0;

	if (!get_addr_1(&addr, arg, AF_INET) && addr.family == AF_INET) {
		int b = mask2bits(addr.data[0]);
		
		if (b >= 0) {
			*val = b;
			return 0;
		}
	}

	return -1;
}
*/

int get_unsigned(unsigned *val, const char *arg, int base)
{
	unsigned long res;
	char *ptr;

	if (!arg || !*arg)
		return -1;
	res = strtoul(arg, &ptr, base);
	if (!ptr || ptr == arg || *ptr || res > UINT_MAX)
		return -1;
	*val = res;
	return 0;
}EXPORT_SYMBOL(get_unsigned);

/*
 * get_time_rtt is "translated" from a similar routine "get_time" in
 * tc_util.c.  We don't use the exact same routine because tc passes
 * microseconds to the kernel and the callers of get_time_rtt want to
 * pass milliseconds (standard unit for rtt values since 2.6.27), and
 * have a different assumption for the units of a "raw" number.
 */
/*
int get_time_rtt(unsigned *val, const char *arg, int *raw)
{
	double t;
	unsigned long res;
	char *p;

	if (strchr(arg,'.') != NULL) {
		t = strtod(arg,&p);
		if (t < 0.0)
			return -1;
	}
	else {
		res = strtoul(arg, &p, 0);
		if (res > UINT_MAX)
			return -1;
		t = (double)res;
	}
	if (p == arg)
		return -1;
	*raw = 1;

	if (*p) {
		*raw = 0;
                if (strcasecmp(p, "s") == 0 || strcasecmp(p, "sec")==0 ||
                    strcasecmp(p, "secs")==0)
                        t *= 1000;
                else if (strcasecmp(p, "ms") == 0 || strcasecmp(p, "msec")==0 ||
                         strcasecmp(p, "msecs") == 0)
			t *= 1.0; 
                else
                        return -1;
        }


	*val = t;
	if (*val < t)
		*val += 1;
	
        return 0;

}

*/

/*
unsigned long strtod(const char * str, char ** ptr) {
	unsigned long res = 0L;
	int ret = 0;
	int i = 0;
	char *str_maj;
	char *str_min;
	int str_len;
	int base = 10;
	str_len = strlen(str);
	
	i = str_len;
	str_maj = vmalloc((i+1)*sizeof(char));
	memcpy(str_maj, str, i);
	str_maj[i] = '\0';
	
	str_min = vmalloc(((str_len-i)+1)*sizeof(char));
	memcpy(str_min, str+i, (str_len-i));
	str_min[(str_len-i)] = '\0';
	
	*ptr = str_min;
	
	ret = kstrtoul(str_maj, base, &res);
	return res;	
}
*/

unsigned long strtoul(const char * str, char ** ptr, int base) {
	unsigned long res = 0L;
	int ret = 0;
	int i = 0;
	char *str_maj;
	char *str_min;
	int str_len;
	str_len = strlen(str);
	
	/**
	 * retrieve major
	 */
	while (str[i] >= 48 && str[i] <= 57)
	    i++;
	
	str_maj = vmalloc((i+1)*sizeof(char));
	memcpy(str_maj, str, i);
	str_maj[i] = '\0';
	
	str_min = vmalloc(((str_len-i)+1)*sizeof(char));
	memcpy(str_min, str+i, (str_len-i));
	str_min[(str_len-i)] = '\0';
	
	*ptr = str_min;
	
	ret = kstrtoul(str_maj, base, &res);
	return res;	
}EXPORT_SYMBOL(strtoul);

long strtol(const char * str, char ** ptr, int base) {
	long res = 0L;
	int ret = 0;
	int i = 0;
	char *str_maj;
	char *str_min;
	int str_len;
	str_len = strlen(str);
		
	/**
	 * retrieve major
	 */
	while (str[i] >= 48 && str[i] <= 57)
	    i++;
	
	str_maj = vmalloc((i+1)*sizeof(char));
	memcpy(str_maj, str, i);
	str_maj[i] = '\0';
		
	str_min = vmalloc(((str_len-i)+1)*sizeof(char));
	memcpy(str_min, str+i, (str_len-i));
	str_min[(str_len-i)] = '\0';
	
	*ptr = str_min;
	
	ret = kstrtol(str_maj, base, &res);
	return res;	
}EXPORT_SYMBOL(strtol);

unsigned long long strtoull(const char * str, char ** ptr, int base) {
	unsigned long long res = 0LL;
	int ret = 0;
	int i = 0;
	char *str_maj;
	char *str_min;
	int str_len;
	str_len = strlen(str);
		
	/**
	 * retrieve major
	 */
	while (str[i] >= 48 && str[i] <= 57)
	    i++;
	str_maj = vmalloc((i+1)*sizeof(char));
	memcpy(str_maj, str, i);
	str_maj[i] = '\0';
	str_min = vmalloc(((str_len-i)+1)*sizeof(char));
	memcpy(str_min, str+i, (str_len-i));
	str_min[(str_len-i)] = '\0';
	
	*ptr = str_min;
	
	ret = kstrtoull(str_maj, base, &res);
	return res;	
}EXPORT_SYMBOL(strtoull);

int get_u64(__u64 *val, const char *arg, int base)
{
	unsigned long long res;
	char *ptr;

	if (!arg || !*arg)
		return -1;
	res = strtoull(arg, &ptr, base);
	if (!ptr || ptr == arg || *ptr || res == 0xFFFFFFFFULL)
 		return -1;
 	*val = res;
 	return 0;
}EXPORT_SYMBOL(get_u64);

int get_u32(__u32 *val, const char *arg, int base)
{
	unsigned long res;
	char *ptr;

	if (!arg || !*arg)
		return -1;
	res = strtoul(arg, &ptr, base);
	if (!ptr || ptr == arg || *ptr || res > 0xFFFFFFFFUL)
		return -1;
	*val = res;
	return 0;
}EXPORT_SYMBOL(get_u32);

int get_u16(__u16 *val, const char *arg, int base)
{
	unsigned long res;
	char *ptr;

	if (!arg || !*arg)
		return -1;
	res = strtoul(arg, &ptr, base);
	if (!ptr || ptr == arg || *ptr || res > 0xFFFF)
		return -1;
	*val = res;
	return 0;
}EXPORT_SYMBOL(get_u16);

int get_u8(__u8 *val, const char *arg, int base)
{
	unsigned long res;
	char *ptr;

	if (!arg || !*arg)
		return -1;
	res = strtoul(arg, &ptr, base);
	if (!ptr || ptr == arg || *ptr || res > 0xFF)
		return -1;
	*val = res;
	return 0;
}EXPORT_SYMBOL(get_u8);

int get_s32(__s32 *val, const char *arg, int base)
{
	long res;
	char *ptr;

	if (!arg || !*arg)
		return -1;
	res = strtol(arg, &ptr, base);
	if (ptr == arg || *ptr ||
	    ((res ==  LONG_MIN || res == LONG_MAX)) ||
	    res > INT32_MAX || res < INT32_MIN)
		return -1;
	*val = res;
	return 0;
}EXPORT_SYMBOL(get_s32);

int get_s16(__s16 *val, const char *arg, int base)
{
	long res;
	char *ptr;

	if (!arg || !*arg)
		return -1;
	res = strtol(arg, &ptr, base);
	if (!ptr || ptr == arg || *ptr || res > 0x7FFF || res < -0x8000)
		return -1;
	*val = res;
	return 0;
}EXPORT_SYMBOL(get_s16);

int get_s8(__s8 *val, const char *arg, int base)
{
	long res;
	char *ptr;

	if (!arg || !*arg)
		return -1;
	res = strtol(arg, &ptr, base);
	if (!ptr || ptr == arg || *ptr || res > 0x7F || res < -0x80)
		return -1;
	*val = res;
	return 0;
}EXPORT_SYMBOL(get_s8);

/* This uses a non-standard parsing (ie not inet_aton, or inet_pton)
 * because of legacy choice to parse 10.8 as 10.8.0.0 not 10.0.0.8
 */

/*
static int get_addr_ipv4(__u8 *ap, const char *cp)
{
	int i;

	for (i = 0; i < 4; i++) {
		unsigned long n;
		char *endp;
		
		n = strtoul(cp, &endp, 0);
		if (n > 255)
			return -1;	

		if (endp == cp) 
			return -1;

		ap[i] = n;

		if (*endp == '\0')
			break;

		if (i == 3 || *endp != '.')
			return -1; 	
		cp = endp + 1;
	}

	return 1;
}

int get_addr_1(inet_prefix *addr, const char *name, int family)
{
	memset(addr, 0, sizeof(*addr));

	if (strcmp(name, "default") == 0 ||
	    strcmp(name, "all") == 0 ||
	    strcmp(name, "any") == 0) {
		if (family == AF_DECnet)
			return -1;
		addr->family = family;
		addr->bytelen = (family == AF_INET6 ? 16 : 4);
		addr->bitlen = -1;
		return 0;
	}

	if (strchr(name, ':')) {
		addr->family = AF_INET6;
		if (family != AF_UNSPEC && family != AF_INET6)
			return -1;
		if (inet_pton(AF_INET6, name, addr->data) <= 0)
			return -1;
		addr->bytelen = 16;
		addr->bitlen = -1;
		return 0;
	}

	if (family == AF_DECnet) {
		struct dn_naddr dna;
		addr->family = AF_DECnet;
		if (dnet_pton(AF_DECnet, name, &dna) <= 0)
			return -1;
		memcpy(addr->data, dna.a_addr, 2);
		addr->bytelen = 2;
		addr->bitlen = -1;
		return 0;
	}

	addr->family = AF_INET;
	if (family != AF_UNSPEC && family != AF_INET)
		return -1;

	if (get_addr_ipv4((__u8 *)addr->data, name) <= 0)
		return -1;

	addr->bytelen = 4;
	addr->bitlen = -1;
	return 0;
}

int get_prefix_1(inet_prefix *dst, char *arg, int family)
{
	int err;
	unsigned plen;
	char *slash;

	memset(dst, 0, sizeof(*dst));

	if (strcmp(arg, "default") == 0 ||
	    strcmp(arg, "any") == 0 ||
	    strcmp(arg, "all") == 0) {
		if (family == AF_DECnet)
			return -1;
		dst->family = family;
		dst->bytelen = 0;
		dst->bitlen = 0;
		return 0;
	}

	slash = strchr(arg, '/');
	if (slash)
		*slash = 0;

	err = get_addr_1(dst, arg, family);
	if (err == 0) {
		switch(dst->family) {
			case AF_INET6:
				dst->bitlen = 128;
				break;
			case AF_DECnet:
				dst->bitlen = 16;
				break;
			default:
			case AF_INET:
				dst->bitlen = 32;
		}
		if (slash) {
			if (get_netmask(&plen, slash+1, 0)
					|| plen > dst->bitlen) {
				err = -1;
				goto done;
			}
			dst->flags |= PREFIXLEN_SPECIFIED;
			dst->bitlen = plen;
		}
	}
done:
	if (slash)
		*slash = '/';
	return err;
}

int get_addr(inet_prefix *dst, const char *arg, int family)
{
	if (family == AF_PACKET) {
		printk(KERN_DEBUG "[MAT] Error: \"%s\" may be inet address, but it is not allowed in this context.\n", arg);
		exit(1);
	}
	if (get_addr_1(dst, arg, family)) {
		printk(KERN_DEBUG "[MAT] Error: an inet address is expected rather than \"%s\".\n", arg);
		exit(1);
	}
	return 0;
}

int get_prefix(inet_prefix *dst, char *arg, int family)
{
	if (family == AF_PACKET) {
		printk(KERN_DEBUG "[MAT] Error: \"%s\" may be inet prefix, but it is not allowed in this context.\n", arg);
		exit(1);
	}
	if (get_prefix_1(dst, arg, family)) {
		printk(KERN_DEBUG "[MAT] Error: an inet prefix is expected rather than \"%s\".\n", arg);
		exit(1);
	}
	return 0;
}

__u32 get_addr32(const char *name)
{
	inet_prefix addr;
	if (get_addr_1(&addr, name, AF_INET)) {
		printk(KERN_DEBUG "[MAT] Error: an IP address is expected rather than \"%s\"\n", name);
		exit(1);
	}
	return addr.data[0];
}
*/

int incomplete_command(void)
{
	printk(KERN_DEBUG "[MAT] Command line is not complete. Try option \"help\"\n");
	return 0;
}EXPORT_SYMBOL(incomplete_command);

/*
void missarg(const char *key)
{
	printk(KERN_DEBUG "[MAT] Error: argument \"%s\" is required\n", key);
	exit(-1);
}
*/

int invarg(const char *msg, const char *arg)
{
	printk(KERN_DEBUG "[MAT] Error: argument \"%s\" is wrong: %s\n", arg, msg);
	return 0;
}EXPORT_SYMBOL(invarg);

int duparg(const char *key, const char *arg)
{
	printk(KERN_DEBUG "[MAT] Error: duplicate \"%s\": \"%s\" is the second value.\n", key, arg);
	return 0;
}EXPORT_SYMBOL(duparg);


/*
void duparg2(const char *key, const char *arg)
{
	printk(KERN_DEBUG "[MAT] Error: either \"%s\" is duplicate, or \"%s\" is a garbage.\n", key, arg);
	exit(-1);
}
*/

int matches(const char *cmd, const char *pattern)
{
	int len = strlen(cmd);
	if (len > strlen(pattern))
		return -1;
	return memcmp(pattern, cmd, len);
}EXPORT_SYMBOL(matches);

/*
int inet_addr_match(const inet_prefix *a, const inet_prefix *b, int bits)
{
	const __u32 *a1 = a->data;
	const __u32 *a2 = b->data;
	int words = bits >> 0x05;

	bits &= 0x1f;

	if (words)
		if (memcmp(a1, a2, words << 2))
			return -1;

	if (bits) {
		__u32 w1, w2;
		__u32 mask;

		w1 = a1[words];
		w2 = a2[words];

		mask = htonl((0xffffffff) << (0x20 - bits));

		if ((w1 ^ w2) & mask)
			return 1;
	}

	return 0;
}
*/

int __iproute2_hz_internal;

int get_hz(void)
{
	if (__iproute2_hz_internal == 0)
		__iproute2_hz_internal = __get_hz();
	return __iproute2_hz_internal;
} EXPORT_SYMBOL(get_hz);


int __get_hz(void)
{
	int hz = 0;
	/*
	 /proc/net/psched
	 */
	struct timespec ts;
	char * seq = vmalloc(36 * sizeof(char));

	hrtimer_get_res(CLOCK_MONOTONIC, &ts);
	sprintf(seq, "%08x %08x %08x %08x\0",
		   (u32)NSEC_PER_USEC, (u32)PSCHED_TICKS2NS(1),
		   1000000,
		   (u32)NSEC_PER_SEC/(u32)ktime_to_ns(timespec_to_ktime(ts)));

	unsigned nom, denom;
	if (sscanf(seq, "%*08x%*08x%08x%08x", &nom, &denom) == 2)
		if (nom == 1000000)
			hz = denom;
		
	
	printk(KERN_DEBUG "[MAT] post denom %u, nom %u\n",denom, nom);
	if (hz)
		return hz;
	printk(KERN_DEBUG "[MAT] unexpected value in get_hz HZ=%d\n",HZ);
	return HZ;
}EXPORT_SYMBOL(__get_hz);


int __iproute2_hz_internal;
int __iproute2_user_hz_internal;

/*
int __get_user_hz(void)
{
	return sysconf(_SC_CLK_TCK);
}
*/

/*
const char *rt_addr_n2a(int af, int len, const void *addr, char *buf, int buflen)
{
	switch (af) {
	case AF_INET:
	case AF_INET6:
		return inet_ntop(af, addr, buf, buflen);
	case AF_IPX:
		return ipx_ntop(af, addr, buf, buflen);
	case AF_DECnet:
	{
		struct dn_naddr dna = { 2, { 0, 0, }};
		memcpy(dna.a_addr, addr, 2);
		return dnet_ntop(af, &dna, buf, buflen);
	}
	default:
		return "???";
	}
}
*/

/*
#ifdef RESOLVE_HOSTNAMES
struct namerec
{
	struct namerec *next;
	const char *name;
	inet_prefix addr;
};

#define NHASH 257
static struct namerec *nht[NHASH];

static const char *resolve_address(const void *addr, int len, int af)
{
	struct namerec *n;
	struct hostent *h_ent;
	unsigned hash;
	static int notfirst;


	if (af == AF_INET6 && ((__u32*)addr)[0] == 0 &&
	    ((__u32*)addr)[1] == 0 && ((__u32*)addr)[2] == htonl(0xffff)) {
		af = AF_INET;
		addr += 12;
		len = 4;
	}

	hash = *(__u32 *)(addr + len - 4) % NHASH;

	for (n = nht[hash]; n; n = n->next) {
		if (n->addr.family == af &&
		    n->addr.bytelen == len &&
		    memcmp(n->addr.data, addr, len) == 0)
			return n->name;
	}
	if ((n = malloc(sizeof(*n))) == NULL)
		return NULL;
	n->addr.family = af;
	n->addr.bytelen = len;
	n->name = NULL;
	memcpy(n->addr.data, addr, len);
	n->next = nht[hash];
	nht[hash] = n;
	if (++notfirst == 1)
		sethostent(1);
	fflush(stdout);

	if ((h_ent = gethostbyaddr(addr, len, af)) != NULL)
		n->name = strdup(h_ent->h_name);

	return n->name;
}
#endif
*/

/*
const char *format_host(int af, int len, const void *addr,
			char *buf, int buflen)
{
#ifdef RESOLVE_HOSTNAMES
	if (resolve_hosts) {
		const char *n;

		if (len <= 0) {
			switch (af) {
			case AF_INET:
				len = 4;
				break;
			case AF_INET6:
				len = 16;
				break;
			case AF_IPX:
				len = 10;
				break;
#ifdef AF_DECnet
			case AF_DECnet:
				len = 2;
				break;
#endif
			default: ;
			}
		}
		if (len > 0 &&
		    (n = resolve_address(addr, len, af)) != NULL)
			return n;
	}
#endif
	return rt_addr_n2a(af, len, addr, buf, buflen);
}
*/

/*
char *hexstring_n2a(const __u8 *str, int len, char *buf, int blen)
{
	char *ptr = buf;
	int i;

	for (i=0; i<len; i++) {
		if (blen < 3)
			break;
		sprintf(ptr, "%02x", str[i]);
		ptr += 2;
		blen -= 2;
		if (i != len-1 && blen > 1) {
			*ptr++ = ':';
			blen--;
		}
	}
	return buf;
}
*/

/*
__u8* hexstring_a2n(const char *str, __u8 *buf, int blen)
{
	int cnt = 0;

	for (;;) {
		unsigned acc;
		char ch;

		acc = 0;

		while ((ch = *str) != ':' && ch != 0) {
			if (ch >= '0' && ch <= '9')
				ch -= '0';
			else if (ch >= 'a' && ch <= 'f')
				ch -= 'a'-10;
			else if (ch >= 'A' && ch <= 'F')
				ch -= 'A'-10;
			else
				return NULL;
			acc = (acc<<4) + ch;
			str++;
		}

		if (acc > 255)
			return NULL;
		if (cnt < blen) {
			buf[cnt] = acc;
			cnt++;
		}
		if (ch == 0)
			break;
		++str;
	}
	if (cnt < blen)
		memset(buf+cnt, 0, blen-cnt);
	return buf;
}
*/

/*
int print_timestamp(FILE *fp)
{
	struct timeval tv;
	char *tstr;

	memset(&tv, 0, sizeof(tv));
	gettimeofday(&tv, NULL);

	tstr = asctime(localtime(&tv.tv_sec));
	tstr[strlen(tstr)-1] = 0;
	fprintf(fp, "Timestamp: %s %lu usec\n", tstr, tv.tv_usec);
	return 0;
}
*/

int cmdlineno;

/* Like glibc getline but handle continuation lines and comments */
/*
ssize_t getcmdline(char **linep, size_t *lenp, FILE *in)
{
	ssize_t cc;
	char *cp;

	if ((cc = getline(linep, lenp, in)) < 0)
		return cc;
	++cmdlineno;

	cp = strchr(*linep, '#');
	if (cp)
		*cp = '\0';

	while ((cp = strstr(*linep, "\\\n")) != NULL) {
		char *line1 = NULL;
		size_t len1 = 0;
		ssize_t cc1;

		if ((cc1 = getline(&line1, &len1, in)) < 0) {
			printk(KERN_DEBUG "[MAT] Missing continuation line\n");
			return cc1;
		}

		++cmdlineno;
		*cp = 0;

		cp = strchr(line1, '#');
		if (cp)
			*cp = '\0';

		*lenp = strlen(*linep) + strlen(line1) + 1;
		*linep = realloc(*linep, *lenp);
		if (!*linep) {
			printk(KERN_DEBUG "[MAT] Out of memory\n");
			*lenp = 0;
			return -1;
		}
		cc += cc1 - 2;
		strcat(*linep, line1);
		free(line1);
	}
	return cc;
}
*/

/* split command line into argument vector */
/*
int makeargs(char *line, char *argv[], int maxargs)
{
	static const char ws[] = " \t\r\n";
	char *cp;
	int argc = 0;

	for (cp = strtok(line, ws); cp; cp = strtok(NULL, ws)) {
		if (argc >= (maxargs - 1)) {
			printk(KERN_DEBUG "[MAT] Too many arguments to command\n");
			exit(1);
		}
		argv[argc++] = cp;
	}
	argv[argc] = NULL;

	return argc;
}
*/
