#include "mtc_launcher.h"

static int __init
mtc_launcher_init(void)
{	
	char * str[20];
	int i=0;
	bool everything_ok = true;
	
	str[0] =  "tc qdisc del dev wlan0 parent root";
	str[1] =  "tc qdisc add dev wlan0 root handle 1: htb";
	
	str[2] =  "tc class add dev wlan0 parent 1: classid 1:1 htb rate 65kbit ceil 65kbit";
	
	str[3] =  "tc class add dev wlan0 parent 1:1 classid 1:11 htb rate 65kbit";
	str[4] =  "tc class add dev wlan0 parent 1:1 classid 1:12 htb rate 65kbit";
	str[5] =  "tc class add dev wlan0 parent 1:1 classid 1:13 htb rate 65kbit";
	
	str[6] =  "tc filter add dev wlan0 parent 1: protocol ip prio 1 handle 1 fw flowid 1:12";
	str[7] =  "tc filter add dev wlan0 parent 1: protocol ip prio 1 handle 2 fw flowid 1:13";
	str[8] =  "tc filter add dev wlan0 parent 1: protocol 3 prio 2 u32 match u8 0 0 flowid 1:13";
	
	for(i=0; i<9 && everything_ok; i++) {
	  printk(KERN_DEBUG "\n");
	  printk(KERN_DEBUG "[MTC - LAUNCHER] \"%s\"\n", str[i]);
	  if( start_tc_bystr(str[i]) != 0 ) 
	      everything_ok=false;
	}
	return 0;
}


static void __exit
mtc_launcher_exit(void)
{
	printk(KERN_DEBUG "[MTC - LAUNCHER] Launcher Ends.\n");
}


subsys_initcall(mtc_launcher_init);
module_exit(mtc_launcher_exit);

MODULE_DESCRIPTION("MTC_LAUNCHER");
MODULE_LICENSE("GPL");
