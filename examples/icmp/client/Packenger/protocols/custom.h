#include <rte_mbuf.h>


class Custom {
public:
    Custom (struct Custom_hdr *custom_hdr){
        this->custom_hdr = custom_hdr;
    }

    static Custom parse(rte_mbuf * packet,unsigned offset){
        return Custom(rte_pktmbuf_mtod_offset(packet, struct Custom_hdr *, offset));
    }

    static void process(Packenger* pknger, rte_mbuf * packet,unsigned offset){
        Custom custom = Custom::parse(packet,offset);
        // Process on custon header
        // simple printing
        printf("A=%16lX\n",custom.custom_hdr->A);
        printf("B=%16lX\n",custom.custom_hdr->B);
        printf("Proto=%04X\n",custom.custom_hdr->Proto);
    }
    
    struct Custom_hdr *custom_hdr;
};