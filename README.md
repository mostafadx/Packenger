# Packenger
a DPDK-based library designed to facilitate the swift and effortless creation, transmission, and reception of network packets

## Overview

Welcome to the DPDK Interconnectivity Setup project! This repository aims to guide you through the process of establishing a seamless communication channel between two host systems or virtual machines, both equipped with the Data Plane Development Kit (DPDK). By leveraging DPDK's capabilities and setting up dedicated DPDK interfaces, this project facilitates efficient data exchange between the hosts.

## Requirements

To successfully replicate and implement this project, make sure you have the following prerequisites in place:

1. **DPDK-Enabled Hosts:** Prepare two distinct host systems or virtual machines, each with DPDK installed. DPDK optimizes packet processing, enhances network performance, and reduces latency.

2. **Dedicated DPDK Interface:** Equip both hosts (or virtual machines) with dedicated DPDK interfaces. These interfaces serve as the foundation for a direct and efficient communication channel between the hosts.

3. **Interconnectivity:** Ensure that the DPDK-enabled hosts are reachable to each other through their dedicated DPDK interfaces. This direct path guarantees effective data packet exchange.

## Repository Contents

- `Packenger/`: Contains the library files.
- `examples/`: Contains examples using the Packenger library.


## Getting Started

1. Clone this repository.
2. Select one of the provided examples.
3. Designate one VM or Host as the server and the other as the client.
4. Transfer the server contents folder to the server system, build the content, and then execute it.
5. Transfer the client contents folder to the client system, build the content, and then execute it.
