#include "Packenger/packenger.h"

#define NUM_MBUFS 8191
#define MBUF_CACHE_SIZE 250

void packet_processor(struct rte_mbuf * pkt, Packenger* pknger){
	printf("arp response packet:\n");
	rte_pktmbuf_dump(stdout, pkt, pkt->pkt_len);
}

int
main(int argc, char *argv[])
{
	struct rte_mempool *mbuf_pool;
	unsigned nb_ports = 1;
	struct rte_ether_addr src_addr;
	struct rte_ether_addr peer_eth_addr = {
		.addr_bytes = { 0x02, 0x00, 0x05, 0x06, 0x07, 0x08 }
	};
	rte_ether_hdr eth_hdr{
		.dst_addr = peer_eth_addr,
		.src_addr = src_addr,
		.ether_type = 0x0800 // IPV4
	};
	struct rte_ipv4_hdr ipv4_hdr{
		.next_proto_id = 0x01 // ICMP
	};
	struct rte_icmp_hdr icmp_hdr;

	// init eal
	int ret = rte_eal_init(argc, argv);
	if (ret < 0)
		rte_exit(EXIT_FAILURE, "Error with EAL initialization\n");

    // create memory pool and allocate memory
	mbuf_pool = rte_pktmbuf_pool_create("MBUF_POOL", NUM_MBUFS * nb_ports,
		MBUF_CACHE_SIZE, 0, RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id());
	if (mbuf_pool == NULL)
		rte_exit(EXIT_FAILURE, "Cannot create mbuf pool\n");
	
	Interface interface(0,mbuf_pool);
	Packenger pknger(&interface,mbuf_pool);

	// create arp packet
	rte_mbuf* pkt = pknger.create_raw_pkt(64);
	pknger.setup_etherent_hdr(pkt,&eth_hdr,0);
	pknger.setup_ipv4_hdr(pkt,&ipv4_hdr,sizeof(struct rte_ether_hdr));
	pknger.setup_icmp_hdr(pkt,&icmp_hdr,sizeof(struct rte_ipv4_hdr)+sizeof(struct rte_ether_hdr));
	pknger.send(pkt);
	pknger.listen(packet_processor);

	rte_eal_cleanup();
	return 0;
}