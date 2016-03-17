/*
 * This file is part of Ginkgo CADx
 *
 * Copyright (c) 2015-2016 Gert Wollny
 * Copyright (c) 2008-2014 MetaEmotion S.L. All rights reserved.
 *
 * Ginkgo CADx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with Ginkgo CADx; if not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef __EP__PROTOCOLS_H
#define __EP__PROTOCOLS_H

// Logically AND (&) this with a type field to get the socket type (below)
#define EP_SOCK_MASK ~1

// Client/server is LSB; do (type & SERVER) to see whether its a passive socket
#define CLIENT 0  // (default)
#define SERVER 1  // 

// Protocol type, occupies all but the LSB
#define TCP       (0<<1)
#define UDP       (1<<1)
#define RAW_BASE  (2<<1)        // all >= RAW_BASE is raw, L4 protocol encoded

#define RAW       RAW_RAW       // raw IP packet shortcut

// comments & definitions from /usr/include/netinet/in.h on BSD
#define RAW_IP         ((2+IPPROTO_IP)<<1)     /* dummy for IP */
#define RAW_ICMP       ((2+IPPROTO_ICMP)<<1)   /* control message protocol */
#define RAW_TCP        ((2+IPPROTO_TCP)<<1)    /* tcp */
#define RAW_UDP        ((2+IPPROTO_UDP)<<1)    /* user datagram protocol */
#define RAW_RAW        ((2+IPPROTO_RAW)<<1)    /* raw IP packet */
#define RAW_HOPOPTS    ((2+IPPROTO_HOPOPTS)<<1)/* IP6 hop-by-hop options */
#define RAW_IGMP       ((2+IPPROTO_IGMP)<<1)   /* group mgmt protocol */
#define RAW_GGP        ((2+IPPROTO_GGP)<<1)    /* gateway^2 (deprecated) */
#define RAW_IPV4       ((2+IPPROTO_IPV4)<<1)   /* IPv4 encapsulation */
#define RAW_IPIP       ((2+IPPROTO_IPV4)<<1)   /* for compatibility */
#define RAW_ST         ((2+IPPROTO_ST)<<1)     /* Stream protocol II */
#define RAW_EGP        ((2+IPPROTO_EGP)<<1)    /* exterior gateway protocol */
#define RAW_PIGP       ((2+IPPROTO_PIGP)<<1)   /* private interior gateway */
#define RAW_RCCMON     ((2+IPPROTO_RCCMON)<<1) /* BBN RCC Monitoring */
#define RAW_NVPII      ((2+IPPROTO_NVPII)<<1)  /* network voice protocol*/
#define RAW_PUP        ((2+IPPROTO_PUP)<<1)    /* pup */
#define RAW_ARGUS      ((2+IPPROTO_ARGUS)<<1)  /* Argus */
#define RAW_EMCON      ((2+IPPROTO_EMCON)<<1)  /* EMCON */
#define RAW_XNET       ((2+IPPROTO_XNET)<<1)   /* Cross Net Debugger */
#define RAW_CHAOS      ((2+IPPROTO_CHAOS)<<1)  /* Chaos*/
#define RAW_MUX        ((2+IPPROTO_MUX)<<1)    /* Multiplexing */
#define RAW_MEAS       ((2+IPPROTO_MEAS)<<1)   /* DCN Measurement Subsystems */
#define RAW_HMP        ((2+IPPROTO_HMP)<<1)    /* Host Monitoring */
#define RAW_PRM        ((2+IPPROTO_PRM)<<1)    /* Packet Radio Measurement */
#define RAW_IDP        ((2+IPPROTO_IDP)<<1)    /* xns idp */
#define RAW_TRUNK1     ((2+IPPROTO_TRUNK1)<<1) /* Trunk-1 */
#define RAW_TRUNK2     ((2+IPPROTO_TRUNK2)<<1) /* Trunk-2 */
#define RAW_LEAF1      ((2+IPPROTO_LEAF1)<<1)  /* Leaf-1 */
#define RAW_LEAF2      ((2+IPPROTO_LEAF2)<<1)  /* Leaf-2 */
#define RAW_RDP        ((2+IPPROTO_RDP)<<1)    /* Reliable Data */
#define RAW_IRTP       ((2+IPPROTO_IRTP)<<1)   /* Reliable Transaction */
#define RAW_TP         ((2+IPPROTO_TP)<<1)     /* tp-4 w/ class negotiation */
#define RAW_BLT        ((2+IPPROTO_BLT)<<1)    /* Bulk Data Transfer */
#define RAW_NSP        ((2+IPPROTO_NSP)<<1)    /* Network Services */
#define RAW_INP        ((2+IPPROTO_INP)<<1)    /* Merit Internodal */
#define RAW_SEP        ((2+IPPROTO_SEP)<<1)    /* Sequential Exchange */
#define RAW_3PC        ((2+IPPROTO_3PC)<<1)    /* Third Party Connect */
#define RAW_IDPR       ((2+IPPROTO_IDPR)<<1)   /* InterDomain Policy Routing */
#define RAW_XTP        ((2+IPPROTO_XTP)<<1)    /* XTP */
#define RAW_DDP        ((2+IPPROTO_DDP)<<1)    /* Datagram Delivery */
#define RAW_CMTP       ((2+IPPROTO_CMTP)<<1)   /* Control Message Transport */
#define RAW_TPXX       ((2+IPPROTO_TPXX)<<1)   /* TP++ Transport */
#define RAW_IL         ((2+IPPROTO_IL)<<1)     /* IL transport protocol */
#define RAW_IPV6       ((2+IPPROTO_IPV6)<<1)   /* IP6 header */
#define RAW_SDRP       ((2+IPPROTO_SDRP)<<1)   /* Source Demand Routing */
#define RAW_ROUTING    ((2+IPPROTO_ROUTING)<<1)/* IP6 routing header */
#define RAW_FRAGMENT   ((2+IPPROTO_FRAGMENT)<<1)/* IP6 fragmentation header */
#define RAW_IDRP       ((2+IPPROTO_IDRP)<<1)   /* InterDomain Routing*/
#define RAW_RSVP       ((2+IPPROTO_RSVP)<<1)   /* resource reservation */
#define RAW_GRE        ((2+IPPROTO_GRE)<<1)    /* General Routing Encap. */
#define RAW_MHRP       ((2+IPPROTO_MHRP)<<1)   /* Mobile Host Routing */
#define RAW_BHA        ((2+IPPROTO_BHA)<<1)    /* BHA */
#define RAW_ESP        ((2+IPPROTO_ESP)<<1)    /* IP6 Encap Sec. Payload */
#define RAW_AH         ((2+IPPROTO_AH)<<1)     /* IP6 Auth Header */
#define RAW_INLSP      ((2+IPPROTO_INLSP)<<1)  /* Integ. Net Layer Security */
#define RAW_SWIPE      ((2+IPPROTO_SWIPE)<<1)  /* IP with encryption */
#define RAW_NHRP       ((2+IPPROTO_NHRP)<<1)   /* Next Hop Resolution */
#define RAW_MOBILE     ((2+IPPROTO_MOBILE)<<1) /* IP Mobility */
#define RAW_TLSP       ((2+IPPROTO_TLSP)<<1)   /* Transport Layer Security */
#define RAW_SKIP       ((2+IPPROTO_SKIP)<<1)   /* SKIP */
#define RAW_ICMPV6     ((2+IPPROTO_ICMPV6)<<1) /* ICMP6 */
#define RAW_NONE       ((2+IPPROTO_NONE)<<1)   /* IP6 no next header */
#define RAW_DSTOPTS    ((2+IPPROTO_DSTOPTS)<<1)/* IP6 destination option */
#define RAW_AHIP       ((2+IPPROTO_AHIP)<<1)   /* any host internal protocol */
#define RAW_CFTP       ((2+IPPROTO_CFTP)<<1)   /* CFTP */
#define RAW_HELLO      ((2+IPPROTO_HELLO)<<1)  /* "hello" routing protocol */
#define RAW_SATEXPAK   ((2+IPPROTO_SATEXPAK)<<1)/* SATNET/Backroom EXPAK */
#define RAW_KRYPTOLAN  ((2+IPPROTO_KRYPTOLAN)<<1)/* Kryptolan */
#define RAW_RVD        ((2+IPPROTO_RVD)<<1)    /* Remote Virtual Disk */
#define RAW_IPPC       ((2+IPPROTO_IPPC)<<1)   /* Pluribus Packet Core */
#define RAW_ADFS       ((2+IPPROTO_ADFS)<<1)   /* Any distributed FS */
#define RAW_SATMON     ((2+IPPROTO_SATMON)<<1) /* Satnet Monitoring */
#define RAW_VISA       ((2+IPPROTO_VISA)<<1)   /* VISA Protocol */
#define RAW_IPCV       ((2+IPPROTO_IPCV)<<1)   /* Packet Core Utility */
#define RAW_CPNX       ((2+IPPROTO_CPNX)<<1)   /* Comp. Prot. Net. Executive */
#define RAW_CPHB       ((2+IPPROTO_CPHB)<<1)   /* Comp. Prot. HeartBeat */
#define RAW_WSN        ((2+IPPROTO_WSN)<<1)    /* Wang Span Network */
#define RAW_PVP        ((2+IPPROTO_PVP)<<1)    /* Packet Video Protocol */
#define RAW_BRSATMON   ((2+IPPROTO_BRSATMON)<<1)/* BackRoom SATNET Monitoring */
#define RAW_ND         ((2+IPPROTO_ND)<<1)     /* Sun net disk proto (temp.) */
#define RAW_WBMON      ((2+IPPROTO_WBMON)<<1)  /* WIDEBAND Monitoring */
#define RAW_WBEXPAK    ((2+IPPROTO_WBEXPAK)<<1)/* WIDEBAND EXPAK */
#define RAW_EON        ((2+IPPROTO_EON)<<1)    /* ISO cnlp */
#define RAW_VMTP       ((2+IPPROTO_VMTP)<<1)   /* VMTP */
#define RAW_SVMTP      ((2+IPPROTO_SVMTP)<<1)  /* Secure VMTP */
#define RAW_VINES      ((2+IPPROTO_VINES)<<1)  /* Banyon VINES */
#define RAW_TTP        ((2+IPPROTO_TTP)<<1)    /* TTP */
#define RAW_IGP        ((2+IPPROTO_IGP)<<1)    /* NSFNET-IGP */
#define RAW_DGP        ((2+IPPROTO_DGP)<<1)    /* dissimilar gateway prot. */
#define RAW_TCF        ((2+IPPROTO_TCF)<<1)    /* TCF */
#define RAW_IGRP       ((2+IPPROTO_IGRP)<<1)   /* Cisco/GXS IGRP */
#define RAW_OSPFIGP    ((2+IPPROTO_OSPFIGP)<<1)/* OSPFIGP */
#define RAW_SRPC       ((2+IPPROTO_SRPC)<<1)   /* Strite RPC protocol */
#define RAW_LARP       ((2+IPPROTO_LARP)<<1)   /* Locus Address Resoloution */
#define RAW_MTP        ((2+IPPROTO_MTP)<<1)    /* Multicast Transport */
#define RAW_AX25       ((2+IPPROTO_AX25)<<1)   /* AX.25 Frames */
#define RAW_IPEIP      ((2+IPPROTO_IPEIP)<<1)  /* IP encapsulated in IP */
#define RAW_MICP       ((2+IPPROTO_MICP)<<1)   /* Mobile Int.ing control */
#define RAW_SCCSP      ((2+IPPROTO_SCCSP)<<1)  /* Semaphore Comm. security */
#define RAW_ETHERIP    ((2+IPPROTO_ETHERIP)<<1)/* Ethernet IP encapsulation */
#define RAW_ENCAP      ((2+IPPROTO_ENCAP)<<1)  /* encapsulation header */
#define RAW_APES       ((2+IPPROTO_APES)<<1)   /* any private encr. scheme */
#define RAW_GMTP       ((2+IPPROTO_GMTP)<<1)   /* GMTP*/
#define RAW_IPCOMP     ((2+IPPROTO_IPCOMP)<<1) /* payload compression(IPComp) */
#define RAW_PIM        ((2+IPPROTO_PIM)<<1)    /* Protocol Independent Mcast */
#define RAW_PGM        ((2+IPPROTO_PGM)<<1)    /* PGM */
#define RAW_OLD_DIVERT  ((2+IPPROTO_OLD_DIVERT)<<1)/* OLD divert pseudo-proto */
#define RAW_MAX        ((2+IPPROTO_MAX)<<1)
#define RAW_DONE       ((2+IPPROTO_DONE)<<1)
#define RAW_DIVERT     ((2+IPPROTO_DIVERT)<<1) /* divert pseudo-protocol */


#endif

