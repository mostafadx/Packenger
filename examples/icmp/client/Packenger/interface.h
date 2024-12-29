#include <rte_mempool.h>
#include <rte_ethdev.h>

class Interface{
public:
    Interface(uint16_t portid, rte_mempool *mbuf_pool,uint16_t rx_rings = 1,uint16_t tx_rings = 1);

    uint16_t portid;
    struct rte_ether_addr mac_addr;
    uint16_t rx_rings;
    uint16_t tx_rings;
    char *interface_name;

};