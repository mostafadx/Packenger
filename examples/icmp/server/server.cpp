#include "Packenger/packenger.h"
#include "Packenger/protocols/ethernet.h"

#define NUM_MBUFS 8191
#define MBUF_CACHE_SIZE 250

void packet_processor(struct rte_mbuf * pkt, Packenger* pknger)
{	
	printf("received packet:\n");
	rte_pktmbuf_dump(stdout, pkt, pkt->pkt_len);
	Ethernet::process(pknger, pkt);
}

int
main(int argc, char *argv[])
{
	struct rte_mempool *mbuf_pool;
	unsigned nb_ports = 1;

	// init eal
	int ret = rte_eal_init(argc, argv);
	if (ret < 0)
		rte_exit(EXIT_FAILURE, "Error with EAL initialization\n");

    // create memory pool and allocate memory
	mbuf_pool = rte_pktmbuf_pool_create("MBUF_POOL", NUM_MBUFS * nb_ports,
		MBUF_CACHE_SIZE, 0, RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id());
	if (mbuf_pool == NULL)
		rte_exit(EXIT_FAILURE, "Cannot create mbuf pool\n");
	
	Interface in(0,mbuf_pool);
	Packenger pkenger(&in, mbuf_pool);

	// main loop
	for (;;) {
         pkenger.listen(packet_processor);
	}

	rte_eal_cleanup();
	return 0;
}
