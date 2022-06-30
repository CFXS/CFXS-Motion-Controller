//*****************************************************************************
//
// lwipopts.h - Configuration file for lwIP
//
// Copyright (c) 2013-2014 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 2.1.0.12573 of the EK-TM4C1294XL Firmware Package.
//
//*****************************************************************************
//
// NOTE:  This file has been derived from the lwIP/src/include/lwip/opt.h
// header file.
//
// For additional details, refer to the original "opt.h" file, and lwIP
// documentation.
//
//*****************************************************************************

#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
//*****************************************************************************
//
// ---------- Stellaris / lwIP Port Options ----------
//
//*****************************************************************************
#define HOST_TMR_INTERVAL 0 //100         // default is 0

#ifdef PART_TM4C1292NCPDT
    #define EMAC_PHY_IS_EXT_MII
#endif

#if !defined(EMAC_PHY_IS_EXT_MII) && !defined(EMAC_PHY_IS_EXT_RMII)
    #define EMAC_PHY_CONFIG (EMAC_PHY_TYPE_INTERNAL | EMAC_PHY_INT_MDIX_EN | EMAC_PHY_AN_100B_T_FULL_DUPLEX)
    #define PHY_PHYS_ADDR   0
#endif
#if defined(EMAC_PHY_IS_EXT_MII)
    #define EMAC_PHY_CONFIG EMAC_PHY_TYPE_EXTERNAL_MII
    #define PHY_PHYS_ADDR   1
//#define EEE_SUPPORT        1
#endif
#if defined(EMAC_PHY_IS_EXT_RMII)
    #define EMAC_PHY_CONFIG EMAC_PHY_TYPE_EXTERNAL_RMII
    #define PHY_PHYS_ADDR   1
//#define EEE_SUPPORT        1
#endif

#define NUM_TX_DESCRIPTORS 32
#define NUM_RX_DESCRIPTORS 32

#define LWIP_RAND get_random_value
static uint32_t get_random_value(void) {
    static uint32_t seed_value = 1337;
    srand(seed_value);
    uint32_t num = rand();
    seed_value   = num;
    return num;
}
//*****************************************************************************
//
// ---------- Platform specific locking ----------
//
//*****************************************************************************
#define SYS_LIGHTWEIGHT_PROT 1 // default is 0
#define NO_SYS               1 // default is 0
//#define SYS_POLL_TASKS										1 // to force lwip in poll mode
//#define MEMCPY(dst,src,len)             memcpy(dst,src,len)
//#define SMEMCPY(dst,src,len)            memcpy(dst,src,len)

#ifndef WIN32
    #ifdef __cplusplus
extern "C" {
    #endif

extern bool e_LWIPLock;
extern void CheckGlobalLWIPLock();

    #ifdef __cplusplus
}
    #endif
#endif

#define LWIP_ASSERT_CORE_LOCKED() CheckGlobalLWIPLock()

//*****************************************************************************
//
// ---------- Memory options ----------
//
//*****************************************************************************
#define MEM_LIBC_MALLOC                        1
#define MEMP_MEM_MALLOC                        1
#define MEM_ALIGNMENT                          4 // default is 1
#define MEM_SIZE                               0 //((NUM_TX_DESCRIPTORS + 4) * 1600) // default is 1600
#define LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT 0

/**
 * MEMP_OVERFLOW_CHECK: memp overflow protection reserves a configurable
 * amount of bytes before and after each memp element in every pool and fills
 * it with a prominent default value.
 *    MEMP_OVERFLOW_CHECK == 0 no checking
 *    MEMP_OVERFLOW_CHECK == 1 checks each element when it is freed
 *    MEMP_OVERFLOW_CHECK >= 2 checks each element in every pool every time
 *      memp_malloc() or memp_free() is called (useful but slow!)
 */
#define MEMP_OVERFLOW_CHECK 0

/**
 * MEMP_SANITY_CHECK==1: run a sanity check after each memp_free() to make
 * sure that there are no cycles in the linked lists.
 */
#define MEMP_SANITY_CHECK 0

/**
 * MEM_OVERFLOW_CHECK: mem overflow protection reserves a configurable
 * amount of bytes before and after each heap allocation chunk and fills
 * it with a prominent default value.
 *    MEM_OVERFLOW_CHECK == 0 no checking
 *    MEM_OVERFLOW_CHECK == 1 checks each element when it is freed
 *    MEM_OVERFLOW_CHECK >= 2 checks all heap elements every time
 *      mem_malloc() or mem_free() is called (useful but slow!)
 */
#define MEM_OVERFLOW_CHECK 0

/**
 * MEM_SANITY_CHECK==1: run a sanity check after each mem_free() to make
 * sure that the linked list of heap elements is not corrupted.
 */
#define MEM_SANITY_CHECK 0

//*****************************************************************************
//
// ---------- Internal Memory Pool Sizes ----------
//
//*****************************************************************************
#define MEMP_NUM_PBUF (NUM_TX_DESCRIPTORS / 4) // Default 16
//#define MEMP_NUM_RAW_PCB                4
//#define MEMP_NUM_UDP_PCB 8
//#define MEMP_NUM_TCP_PCB 5 // Default 5
#define MEMP_NUM_TCP_PCB_LISTEN 64
#define MEMP_NUM_TCP_SEG        64
//#define MEMP_NUM_REASSDATA              5
//#define MEMP_NUM_ARP_QUEUE              30
#define MEMP_NUM_IGMP_GROUP 256
//#define MEMP_NUM_SYS_TIMEOUT 8
//#define MEMP_NUM_NETBUF                 2
//#define MEMP_NUM_NETCONN                4
//#define MEMP_NUM_TCPIP_MSG_API   64
//#define MEMP_NUM_TCPIP_MSG_INPKT 64
#define PBUF_POOL_SIZE (NUM_RX_DESCRIPTORS + 4) // Default 16
//#define MEMP_SEPARATE_POOLS           1
//#define MEM_USE_POOLS_TRY_BIGGER_POOL 1

//*****************************************************************************
//
// ---------- ARP options ----------
//
//*****************************************************************************
//#define LWIP_ARP                        1
//#define ARP_TABLE_SIZE                  10
//#define ARP_QUEUEING                    1
//#define ETHARP_TRUST_IP_MAC             1

//*****************************************************************************
//
// ---------- IP options ----------
//
//*****************************************************************************
//#define IP_FORWARD                      0
//#define IP_OPTIONS_ALLOWED              1
#define IP_REASSEMBLY 1 // default is 1
#define IP_FRAG       1 // default is 1
//#define IP_REASS_MAXAGE                 3
//#define IP_REASS_MAX_PBUFS              10
//#define IP_FRAG_USES_STATIC_BUF         1
//#define IP_FRAG_MAX_MTU                 1500
//#define IP_DEFAULT_TTL                  255
//#define IP_SOF_BROADCAST      1
//#define IP_SOF_BROADCAST_RECV 1

//*****************************************************************************
//
// ---------- ICMP options ----------
//
//*****************************************************************************
#define LWIP_ICMP 1
//#define ICMP_TTL                       (IP_DEFAULT_TTL)

//*****************************************************************************
//
// ---------- RAW options ----------
//
//*****************************************************************************
#define LWIP_RAW 1
//#define RAW_TTL                        (IP_DEFAULT_TTL)

//*****************************************************************************
//
// ---------- DHCP options ----------
//
//*****************************************************************************
#define LWIP_DHCP 0 // default is 0
//#define DHCP_DOES_ARP_CHECK             ((LWIP_DHCP) && (LWIP_ARP))

//*****************************************************************************
//
// ---------- UPNP options ----------
//
//*****************************************************************************
//#define LWIP_UPNP                       0

//*****************************************************************************
//
// ---------- PTPD options ----------
//
//*****************************************************************************
//#define LWIP_PTPD                       0

//*****************************************************************************
//
// ---------- AUTOIP options ----------
//
//*****************************************************************************
#define LWIP_AUTOIP 0 // default is 0
//#define LWIP_DHCP_AUTOIP_COOP ((LWIP_DHCP) && (LWIP_AUTOIP))
// default is 0
//#define LWIP_DHCP_AUTOIP_COOP_TRIES 5 // default is 9

//*****************************************************************************
//
// ---------- SNMP options ----------
//
//*****************************************************************************
//#define LWIP_SNMP                       0
//#define SNMP_CONCURRENT_REQUESTS        1
//#define SNMP_TRAP_DESTINATIONS          1
//#define SNMP_PRIVATE_MIB                0
//#define SNMP_SAFE_REQUESTS              1

//*****************************************************************************
//
// ---------- IGMP options ----------
//
//*****************************************************************************
#define LWIP_IGMP 1

//*****************************************************************************
//
// ---------- DNS options -----------
//
//*****************************************************************************
//#define LWIP_DNS                        0
//#define DNS_TABLE_SIZE                  4
//#define DNS_MAX_NAME_LENGTH             256
//#define DNS_MAX_SERVERS                 2
//#define DNS_DOES_NAME_CHECK             1
//#define DNS_USES_STATIC_BUF             1
//#define DNS_MSG_SIZE                    512

//*****************************************************************************
//
// ---------- UDP options ----------
//
//*****************************************************************************
#define LWIP_UDP 1
//#define LWIP_UDPLITE                    0
//#define UDP_TTL                         (IP_DEFAULT_TTL)

//*****************************************************************************
//
// ---------- TCP options ----------
//
//*****************************************************************************
#define LWIP_TCP 1
//#define LWIP_WND_SCALE 1
//#define TCP_TTL                         (IP_DEFAULT_TTL)
//#define TCP_WND 4096
//#define TCP_RCV_SCALE 0
//#define TCP_MAXRTX                      12
//#define TCP_SYNMAXRTX                   6
//#define TCP_QUEUE_OOSEQ                 1
#define TCP_MSS                    2048
#define TCP_CALCULATE_EFF_SEND_MSS 1
//#define TCP_SND_BUF (4 * TCP_MSS)
//#define TCP_SND_QUEUELEN ((4 * (TCP_SND_BUF) + (TCP_MSS - 1)) / (TCP_MSS))
//#define TCP_SNDLOWAT                    (TCP_SND_BUF/2)
//#define TCP_LISTEN_BACKLOG 0
//#define TCP_DEFAULT_LISTEN_BACKLOG      0xff
#define TCP_OVERSIZE (TCP_MSS * 2)

//*****************************************************************************
//
// ---------- API options ----------
//
//*****************************************************************************
//#define LWIP_EVENT_API                  0
//#define LWIP_CALLBACK_API               1

//*****************************************************************************
//
// ---------- Pbuf options ----------
//
//*****************************************************************************
#define PBUF_LINK_HLEN    16 // default is 14
#define PBUF_POOL_BUFSIZE 1600
//#define ETH_PAD_SIZE 0 // default is 0

//*****************************************************************************
//
// ---------- Network Interfaces options ----------
//
//*****************************************************************************
//#define LWIP_NETIF_HOSTNAME             0
//#define LWIP_NETIF_API                  0
//#define LWIP_NETIF_STATUS_CALLBACK      0
//#define LWIP_NETIF_LINK_CALLBACK        0
//#define LWIP_NETIF_HWADDRHINT           0

//*****************************************************************************
//
// ---------- LOOPIF options ----------
//
//*****************************************************************************
//#define LWIP_HAVE_LOOPIF                0
//#define LWIP_LOOPIF_MULTITHREADING      1

//*****************************************************************************
//
// ---------- Thread options ----------
//
//*****************************************************************************
//#define TCPIP_THREAD_NAME              "tcpip_thread"
//#define TCPIP_THREAD_STACKSIZE          0
//#define TCPIP_THREAD_PRIO               1
//#define TCPIP_MBOX_SIZE                 0
//#define SLIPIF_THREAD_NAME             "slipif_loop"
//#define SLIPIF_THREAD_STACKSIZE         0
//#define SLIPIF_THREAD_PRIO              1
//#define PPP_THREAD_NAME                "pppMain"
//#define PPP_THREAD_STACKSIZE            0
//#define PPP_THREAD_PRIO                 1
//#define DEFAULT_THREAD_NAME            "lwIP"
//#define DEFAULT_THREAD_STACKSIZE        0
//#define DEFAULT_THREAD_PRIO             1
//#define DEFAULT_RAW_RECVMBOX_SIZE       0
//#define DEFAULT_UDP_RECVMBOX_SIZE       0
//#define DEFAULT_TCP_RECVMBOX_SIZE       0
//#define DEFAULT_ACCEPTMBOX_SIZE         0

//*****************************************************************************
//
// ---------- Sequential layer options ----------
//
//*****************************************************************************
//#define LWIP_TCPIP_CORE_LOCKING         0
#define LWIP_NETCONN 0 // default is 1

//*****************************************************************************
//
// ---------- Socket Options ----------
//
//*****************************************************************************
#define LWIP_SOCKET 0 // default is 1
//#define LWIP_COMPAT_SOCKETS             1
//#define LWIP_POSIX_SOCKETS_IO_NAMES     1
//#define LWIP_TCP_KEEPALIVE              0
//#define LWIP_SO_RCVTIMEO                0
//#define LWIP_SO_RCVBUF                  0
//#define SO_REUSE                        0

//*****************************************************************************
//
// ---------- Statistics options ----------
//
//*****************************************************************************
//#define LWIP_STATS         1
//#define LWIP_STATS_DISPLAY 0
//#define LINK_STATS         1
//#define ETHARP_STATS       (LWIP_ARP)
//#define IP_STATS           1
//#define IPFRAG_STATS       (IP_REASSEMBLY || IP_FRAG)
//#define ICMP_STATS         1
//#define IGMP_STATS         (LWIP_IGMP)
//#define UDP_STATS          (LWIP_UDP)
//#define TCP_STATS          (LWIP_TCP)
//#define MEM_STATS          1
//#define MEMP_STATS         1
//#define SYS_STATS          1

//*****************************************************************************
//
// ---------- PPP options ----------
//
//*****************************************************************************
//#define PPP_SUPPORT                     0
//#define PPPOE_SUPPORT                   0
//#define PPPOS_SUPPORT                   PPP_SUPPORT

#if PPP_SUPPORT
//#define NUM_PPP                         1
//#define PAP_SUPPORT                     0
//#define CHAP_SUPPORT                    0
//#define MSCHAP_SUPPORT                  0
//#define CBCP_SUPPORT                    0
//#define CCP_SUPPORT                     0
//#define VJ_SUPPORT                      0
//#define MD5_SUPPORT                     0
//#define FSM_DEFTIMEOUT                  6
//#define FSM_DEFMAXTERMREQS              2
//#define FSM_DEFMAXCONFREQS              10
//#define FSM_DEFMAXNAKLOOPS              5
//#define UPAP_DEFTIMEOUT                 6
//#define UPAP_DEFREQTIME                 30
//#define CHAP_DEFTIMEOUT                 6
//#define CHAP_DEFTRANSMITS               10
//#define LCP_ECHOINTERVAL                0
//#define LCP_MAXECHOFAILS                3
//#define PPP_MAXIDLEFLAG                 100

//#define PPP_MAXMTU                      1500
//#define PPP_DEFMRU                      296
#endif

//*****************************************************************************
//
// ---------- checksum options ----------
//
//*****************************************************************************
#define CHECKSUM_GEN_IP    0
#define CHECKSUM_GEN_ICMP  1
#define CHECKSUM_GEN_UDP   0
#define CHECKSUM_GEN_TCP   0
#define CHECKSUM_CHECK_IP  0
#define CHECKSUM_CHECK_UDP 0
#define CHECKSUM_CHECK_TCP 0

//*****************************************************************************
//
// ---------- http options ----------
//
//*****************************************************************************

#define LWIP_HTTPD_CGI_SSI               0
#define LWIP_HTTPD_SSI                   1
#define LWIP_HTTPD_SSI_BY_FILE_EXTENSION 1
#define LWIP_HTTPD_SSI_INCLUDE_TAG       0
#define LWIP_HTTPD_SUPPORT_POST          1
#define LWIP_HTTPD_MAX_CGI_PARAMETERS    32
#define LWIP_HTTPD_MAX_TAG_NAME_LEN      32
#define LWIP_HTTPD_MAX_TAG_INSERT_LEN    2048
#define LWIP_HTTPD_SUPPORT_EXTSTATUS     1

#define LWIP_HTTPD_MAX_REQ_LENGTH   2048
#define HTTPD_LIMIT_SENDING_TO_2MSS 0

//*****************************************************************************
//
// ---------- Debugging options ----------
//
//*****************************************************************************

//#ifdef DEBUG
//#define LWIP_DEBUG
//#else
#define LWIP_STATS 0
//#endif

//
// Define a generic ASSERT display macro here ... use the DIAG macro to display
// the message, then use the __error__ function, which should always be
// defined by the user application for DEBUG builds, to abandon execution.
//
#ifndef LWIP_PLATFORM_ASSERT
    #ifdef DEBUG

        #ifdef __cplusplus
extern "C" {
        #endif
void __error__(char *pcFilename, uint32_t ui32Line);
        #ifdef __cplusplus
}
        #endif

        #define LWIP_PLATFORM_ASSERT(msg)      \
            {                                  \
                LWIP_PLATFORM_DIAG(msg);       \
                __error__(__FILE__, __LINE__); \
            }
    #else
        #define LWIP_PLATFORM_ASSERT(msg)
    #endif
#endif

//#define LWIP_DBG_MIN_LEVEL LWIP_DBG_LEVEL_OFF
//#define LWIP_DBG_MIN_LEVEL              LWIP_DBG_LEVEL_WARNING
//#define LWIP_DBG_MIN_LEVEL              LWIP_DBG_LEVEL_SERIOUS
//#define LWIP_DBG_MIN_LEVEL              LWIP_DBG_LEVEL_SEVERE

//#define LWIP_DBG_TYPES_ON LWIP_DBG_ON
//#define LWIP_DBG_TYPES_ON (LWIP_DBG_ON | LWIP_DBG_TRACE | LWIP_DBG_STATE | LWIP_DBG_FRESH)

//#define HTTPD_DEBUG LWIP_DBG_ON
//#define ETHARP_DEBUG LWIP_DBG_ON // default is OFF
//#define NETIF_DEBUG  LWIP_DBG_ON // default is OFF
//#define PBUF_DEBUG LWIP_DBG_ON // default is OFF
//#define API_LIB_DEBUG                   LWIP_DBG_OFF
//#define API_MSG_DEBUG                   LWIP_DBG_OFF
//#define SOCKETS_DEBUG                   LWIP_DBG_OFF
//#define ICMP_DEBUG LWIP_DBG_ON
//#define IGMP_DEBUG LWIP_DBG_ON
//#define INET_DEBUG                      LWIP_DBG_OFF
//#define IP_DEBUG LWIP_DBG_ON // default is OFF
//#define IP_REASS_DEBUG                  LWIP_DBG_OFF
//#define RAW_DEBUG                       LWIP_DBG_OFF
//#define MEM_DEBUG  LWIP_DBG_ON
//#define MEMP_DEBUG LWIP_DBG_ON
//#define SYS_DEBUG                       LWIP_DBG_OFF
//#define TCP_DEBUG        LWIP_DBG_ON
//#define TCP_INPUT_DEBUG  LWIP_DBG_ON
//#define TCP_FR_DEBUG     LWIP_DBG_ON
//#define TCP_RTO_DEBUG    LWIP_DBG_ON
//#define TCP_CWND_DEBUG   LWIP_DBG_ON
//#define TCP_WND_DEBUG    LWIP_DBG_ON
//#define TCP_OUTPUT_DEBUG LWIP_DBG_ON
//#define TCP_RST_DEBUG    LWIP_DBG_ON
//#define TCP_QLEN_DEBUG   LWIP_DBG_ON
//#define UDP_DEBUG LWIP_DBG_ON // default is OFF
//#define TCPIP_DEBUG                     LWIP_DBG_OFF
//#define PPP_DEBUG                       LWIP_DBG_OFF
//#define SLIP_DEBUG                      LWIP_DBG_OFF
//#define DHCP_DEBUG                      LWIP_DBG_ON     // default is OFF
//#define AUTOIP_DEBUG                    LWIP_DBG_OFF
//#define SNMP_MSG_DEBUG                  LWIP_DBG_OFF
//#define SNMP_MIB_DEBUG                  LWIP_DBG_OFF
//#define DNS_DEBUG                       LWIP_DBG_OFF

#endif /* __LWIPOPTS_H__ */
