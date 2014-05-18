#ifndef CONTIKI_CONF_H
#define CONTIKI_CONF_H

#include <stdint.h>
#include <MKL25Z4.h>

#define CCIF
#define CLIF

#define CLOCK_CONF_SECOND 100

/* These names are deprecated, use C99 names. */
typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef int8_t s8_t;
typedef int16_t s16_t;
typedef int32_t s32_t;

typedef unsigned int clock_time_t;
typedef unsigned int uip_stats_t;

#ifndef BV
#define BV(x) (1<<(x))
#endif

#define CC11xx_ARCH_SPI_ENABLE  cc1120_arch_spi_enable
#define CC11xx_ARCH_SPI_DISABLE cc1120_arch_spi_disable
#define CC11xx_ARCH_SPI_RW_BYTE cc1120_arch_spi_rw_byte
#define CC11xx_ARCH_SPI_RW      cc1120_arch_spi_rw

#define cc11xx_arch_spi_enable  cc1120_arch_spi_enable
#define cc11xx_arch_spi_disable cc1120_arch_spi_disable
#define cc11xx_arch_spi_rw_byte cc1120_arch_spi_rw_byte
#define cc11xx_arch_spi_rw      cc1120_arch_spi_rw
#define cc11xx_arch_interrupt_enable cc1120_arch_interrupt_enable

#define cc11xx_arch_init        cc1120_arch_init



#define RAND_MAX 0x7fff





/*---------------------------------------- IP Config ----------------------------------------*/
//#define WITH_UIP 1
#define WITH_ASCII 1

#define PLATFORM_HAS_LEDS   0
#define PLATFORM_HAS_BUTTON 0

#define NETSTACK_CONF_NETWORK sicslowpan_driver
#define NETSTACK_CONF_MAC     csma_driver
#define NETSTACK_CONF_RDC     contikimac_driver
#define NETSTACK_CONF_RADIO   cc11xx_driver
#define NETSTACK_CONF_FRAMER  framer_802154

#define NETSTACK_RADIO_MAX_PAYLOAD_LEN 125
#define SICSLOWPAN_CONF_COMPRESSION_THRESHOLD 63
#define CONTIKIMAC_CONF_WITH_CONTIKIMAC_HEADER 0

#define CC11xx_CONF_AUTOACK              1
#define NETSTACK_RDC_CHANNEL_CHECK_RATE  8
#define RIME_CONF_NO_POLITE_ANNOUCEMENTS 0
#define CXMAC_CONF_ANNOUNCEMENTS         0
#define XMAC_CONF_ANNOUNCEMENTS          0

#define QUEUEBUF_CONF_NUM                4 

#define PACKETBUF_CONF_ATTRS_INLINE 1

#ifndef RF_CHANNEL
#define RF_CHANNEL              42
#endif /* RF_CHANNEL */

#define IEEE802154_CONF_PANID       0xABCD

#define RIMEADDR_CONF_SIZE              8

#define UIP_CONF_LL_802154              1
#define UIP_CONF_LLH_LEN                0

#define UIP_CONF_ROUTER                 1
#define UIP_CONF_IPV6_RPL               1

/* Handle 10 neighbors */
#define NBR_TABLE_CONF_MAX_NEIGHBORS     15
/* Handle 10 routes    */
#define UIP_CONF_MAX_ROUTES   15

#define UIP_CONF_ND6_SEND_RA		0
#define UIP_CONF_ND6_REACHABLE_TIME     600000
#define UIP_CONF_ND6_RETRANS_TIMER      10000

/* uIP configuration */
#define UIP_CONF_BROADCAST       1
#define UIP_CONF_LOGGING 1
#define UIP_CONF_BUFFER_SIZE 116

#define UIP_CONF_TCP_FORWARD 1

#define UIP_CONF_IPV6                   1
#define UIP_CONF_IPV6_QUEUE_PKT         0
#define UIP_CONF_IPV6_CHECKS            1
#define UIP_CONF_IPV6_REASSEMBLY        0
#define UIP_CONF_NETIF_MAX_ADDRESSES    3
#define UIP_CONF_ND6_MAX_PREFIXES       3
#define UIP_CONF_ND6_MAX_DEFROUTERS     2

#define SICSLOWPAN_CONF_COMPRESSION_IPV6        0
#define SICSLOWPAN_CONF_COMPRESSION_HC1         1
#define SICSLOWPAN_CONF_COMPRESSION_HC01        2
#define SICSLOWPAN_CONF_COMPRESSION             SICSLOWPAN_COMPRESSION_HC06


#define SICSLOWPAN_CONF_CONVENTIONAL_MAC	1
#define SICSLOWPAN_CONF_MAX_ADDR_CONTEXTS       2


#define UIP_CONF_ICMP_DEST_UNREACH 1

#define UIP_CONF_DHCP_LIGHT
#define UIP_CONF_LLH_LEN         0
#define UIP_CONF_RECEIVE_WINDOW  48
#define UIP_CONF_TCP_MSS         48
#define UIP_CONF_MAX_CONNECTIONS 4
#define UIP_CONF_MAX_LISTENPORTS 8
#define UIP_CONF_UDP_CONNS       12
#define UIP_CONF_FWCACHE_SIZE    30
#define UIP_CONF_BROADCAST       1
#define UIP_ARCH_IPCHKSUM        1
#define UIP_CONF_UDP             1
#define UIP_CONF_UDP_CHECKSUMS   1
#define UIP_CONF_PINGADDRCONF    0
#define UIP_CONF_LOGGING         0

#define UIP_CONF_TCP_SPLIT       0


#endif /* CONTIKI_CONF_H_CDBB4VIH3I__ */
