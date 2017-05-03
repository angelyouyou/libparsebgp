/*
 * Copyright (c) 2013-2015 Cisco Systems, Inc. and others.  All rights reserved.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v1.0 which accompanies this distribution,
 * and is available at http://www.eclipse.org/legal/epl-v10.html
 *
 */

#ifndef UPDATEMSG_H_
#define UPDATEMSG_H_

#include "bgp_common.h"
#include "parse_common.h"
#include <string>
#include <list>
#include <array>
#include <map>

/**
 * Update header - defined in RFC4271
 */
struct update_bgp_hdr {
    /**
     * indicates the total len of withdrawn routes field in octets.
     */
    uint16_t withdrawn_len;

    /**
     * Withdrawn routes data pointer
     */
    u_char *withdrawn_ptr;

    /**
     * Total length of the path attributes field in octets
     *
     * A value of 0 indicates NLRI nor path attrs are present
     */
    uint16_t attr_len;

    /**
     * Attribute data pointer
     */
    u_char *attr_ptr;

    /**
     * NLRI data pointer
     */
    u_char *nlri_ptr;
};

/*
typedef std::map<uint16_t, std::array<uint8_t, 255>> parsed_ls_attrs_map;

struct attr_type_tuple {
    uint8_t attr_flags;
    uint8_t attr_type_code;
};

typedef struct as_path_segment {
    uint8_t         seg_type;
    uint8_t         seg_len;
    list<uint32_t>  seg_asn;
}as_path_segment;

*/
/**
 * struct defines the MP_UNREACH_NLRI (RFC4760 Section 4)
 *//*

struct mp_unreach_nlri {
    uint16_t       afi;                 ///< Address Family Identifier
    uint8_t        safi;                ///< Subsequent Address Family Identifier
    unsigned char  *nlri_data;          ///< NLRI data - Pointer to data (normally does not require freeing)
    uint16_t       nlri_len;            ///< Not in RFC header; length of the NLRI data
    list<update_prefix_tuple> wdrawn_routes;   ///< Withdrawn routes
};

struct mp_reach_nlri {
    uint16_t       afi;                 ///< Address Family Identifier
    uint8_t        safi;                ///< Subsequent Address Family Identifier
    uint8_t        nh_len;              ///< Length of next hop
    //unsigned char  *next_hop;           ///< Next hop - Pointer to data (normally does not require freeing)
    uint8_t        reserved;            ///< Reserved

    //unsigned char  *nlri_data;          ///< NLRI data - Pointer to data (normally does not require freeing)
    uint16_t       nlri_len;            ///< Not in RFC header; length of the NLRI data
};

*/
/**
 * Extended Community header
 *      RFC4360 size is 8 bytes total (6 for value)
 *      RFC5701 size is 20 bytes total (16 for global admin, 2 for local admin)
 *//*

struct extcomm_hdr {
    uint8_t      high_type;                      ///< Type high byte
    uint8_t      low_type;                       ///< Type low byte - subtype
    u_char       *value;                         ///<
    string       val;
};

typedef  std::map<uint16_t, std::array<uint8_t, 255>>        parsed_ls_attrs_map;

*/
/**
     * Parsed data structure for BGP-LS
     *//*

struct parsed_data_ls {
    std::list<obj_ls_node>   nodes;        ///< List of Link state nodes
    std::list<obj_ls_link>   links;        ///< List of link state links
    std::list<obj_ls_prefix> prefixes;     ///< List of link state prefixes
};

typedef struct update_path_attrs {
    attr_type_tuple attr_type;
    uint16_t        attr_len;
    union attr_value{
        uint8_t                 origin;
        list<as_path_segment>   as_path;
        u_char                  ipv4_raw[4];
        uint32_t                value32bit;
        uint16_t                value16bit;
        string                  aggregator;
        list<u_char[4]>         cluster_list;
        list<uint16_t>          attr_type_comm;
        list<extcomm_hdr>       ext_comm;
        mp_unreach_nlri         mp_unreach_nlri_data;
        mp_reach_nlri           mp_reach_nlri_data;
        list<vpn_tuple>         vpn_withdrawn;      ///< List of vpn prefixes withdrawn
        list<evpn_tuple>        evpn;               ///< List of evpn nlris advertised
        list<evpn_tuple>        evpn_withdrawn;     ///< List of evpn nlris withdrawn
        parsed_ls_attrs_map     ls_attrs;
        parsed_data_ls          ls;                 ///< REACH: Link state parsed data
        parsed_data_ls          ls_withdrawn;       ///< UNREACH: Parsed Withdrawn data
    }attr_value;
    parsed_attrs_map            attrs;
    list<vpn_tuple>         vpn;                ///< List of vpn prefixes advertised
    std::list<update_prefix_tuple>  advertised;
    parsed_data_ls mp_ls_data;
    libparsebgp_evpn_data evpn_data;
    libparsebgp_addpath_map add_path_map;
}update_path_attrs;
*/

struct libparsebgp_update_msg_data {
    uint16_t wdrawn_route_len;
    list<update_prefix_tuple> wdrawn_routes;
    uint16_t total_path_attr_len;
    list<update_path_attrs> path_attributes;
    list<update_prefix_tuple> nlri;

    /**
     * parsed path attributes map
     */
//        std::map<update_attr_types, std::string> parsed_attrs;

//    bool debug;                           ///< debug flag to indicate debugging
//    //Logger                  *logger;                         ///< Logging class pointer
//    std::string peer_addr;                       ///< Printed form of the peer address for logging
//    std::string router_addr;                     ///< Router IP address - used for logging
//    bool four_octet_asn;                  ///< Indicates true if 4 octets or false if 2
//    peer_info *peer_inf;                      ///< Persistent Peer info pointer

//    parsed_update_data parsed_data;
//    std::vector<obj_vpn> obj_vpn_rib_list;
//    std::vector<obj_evpn> obj_evpn_rib_list;
//    std::vector<obj_rib> adv_obj_rib_list;
//    std::vector<obj_rib> wdrawn_obj_rib_list;
};

void libparsebgp_update_msg_init(libparsebgp_update_msg_data *update_msg, std::string peer_addr,
                                           std::string router_addr, peer_info *peer_info);

 /**
  * Parses the update message
  *
  * \details
  *      Reads the update message from socket and parses it.  The parsed output will
  *      be added to the DB.
  *
  * \param [in]   data           Pointer to raw bgp payload data, starting at the notification message
  * \param [in]   size           Size of the data available to read; prevent overrun when reading
  * \param [out]  parsed_data    Reference to parsed_update_data; will be updated with all parsed data
  *
  * \return ZERO is error, otherwise a positive value indicating the number of bytes read from update message
  */
 size_t libparsebgp_update_msg_parse_update_msg(libparsebgp_update_msg_data *update_msg, u_char *data, size_t size,
                                                bool &has_end_of_rib_marker);

/**
 * Parses the BGP attributes in the update
 *
 * \details
 *     Parses all attributes.  Decoded values are updated in 'parsed_data'
 *
 * \param [in]   data       Pointer to the start of the prefixes to be parsed
 * \param [in]   len        Length of the data in bytes to be read
 * \param [out]  parsed_data    Reference to parsed_update_data; will be updated with all parsed data
 */
void libparsebgp_update_msg_parse_attributes(libparsebgp_update_msg_data *update_msg, u_char *data, uint16_t len, bool &has_end_of_rib_marker);

/* namespace bgp_msg */

#endif /* UPDATEMSG_H_ */
