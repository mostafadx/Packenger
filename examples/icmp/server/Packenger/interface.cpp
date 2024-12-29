#include "interface.h"

#define RX_RING_SIZE 1024
#define TX_RING_SIZE 1024

Interface::Interface(uint16_t portid, rte_mempool *mbuf_pool, uint16_t rx_rings , uint16_t tx_rings)
{
    struct rte_eth_conf port_conf;
	uint16_t nb_rxd = RX_RING_SIZE;
	uint16_t nb_txd = TX_RING_SIZE;
	uint16_t q;
	struct rte_eth_dev_info dev_info;
	struct rte_eth_txconf txconf;

    // class variable init
    this->portid = portid;
    this->rx_rings = rx_rings;
    this->tx_rings = tx_rings;
    rte_eth_macaddr_get(portid, &this->mac_addr);
	interface_name = new char[100];
    rte_eth_dev_get_name_by_port(portid,interface_name);

	memset(&port_conf, 0, sizeof(struct rte_eth_conf));
	rte_eth_dev_info_get(portid, &dev_info);
	if (dev_info.tx_offload_capa & RTE_ETH_TX_OFFLOAD_MBUF_FAST_FREE)
		port_conf.txmode.offloads |=
			RTE_ETH_TX_OFFLOAD_MBUF_FAST_FREE;

	// Configure the Ethernet device.
	rte_eth_dev_configure(portid, rx_rings, tx_rings, &port_conf);

	rte_eth_dev_adjust_nb_rx_tx_desc(portid, &nb_rxd, &nb_txd);

	// Allocate and set up 1 RX queue per Ethernet port.
	for (q = 0; q < rx_rings; q++) {
		rte_eth_rx_queue_setup(portid, q, nb_rxd,
				rte_eth_dev_socket_id(portid), NULL, mbuf_pool);
	}

	txconf = dev_info.default_txconf;
	txconf.offloads = port_conf.txmode.offloads;

	// Allocate and set up 1 TX queue per Ethernet port.
	for (q = 0; q < tx_rings; q++) {
		rte_eth_tx_queue_setup(portid, q, nb_txd,
				rte_eth_dev_socket_id(portid), &txconf);
	}

	// Starting Ethernet port.
	rte_eth_dev_start(portid);

	// Enable RX in promiscuous mode for the Ethernet device.
	rte_eth_promiscuous_enable(portid);
}