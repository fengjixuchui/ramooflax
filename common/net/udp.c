/*
** Copyright (C) 2014 EADS France, stephane duverger <stephane.duverger@eads.net>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License along
** with this program; if not, write to the Free Software Foundation, Inc.,
** 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include <udp.h>
#include <debug.h>
#include <info_data.h>

extern info_data_t *info;

size_t udp_pkt(udp_hdr_t *hdr, uint16_t sport, uint16_t dport, size_t dlen)
{
   size_t len = sizeof(udp_hdr_t) + dlen;

   hdr->src = swap16(sport);
   hdr->dst = swap16(dport);
   hdr->len = swap16(len);
   hdr->chk = 0;

   return len;
}

void udp_dissect(loc_t pkt, size_t len)
{
   net_info_t *net = &info->hrd.dev.net;
   udp_hdr_t  *hdr = (udp_hdr_t*)pkt.addr;
   buffer_t    buf;

   hdr->src = swap16(hdr->src);
   hdr->dst = swap16(hdr->dst);
   hdr->len = swap16(hdr->len);

   debug(UDP, "received UDP src %d dst %d len %D\n", hdr->src, hdr->dst, hdr->len);

   if(hdr->dst != net->port)
      return;

   buf.data.linear = pkt.linear + sizeof(udp_hdr_t);
   buf.sz = len - sizeof(udp_hdr_t);

   {
      size_t i;
      debug(UDP, "UDP received (%D):", buf.sz);
      for(i=0 ; i<buf.sz ; i++)
	 debug(UDP, "\\x%x", buf.data.u8[i]);
      debug(UDP,"\n");
   }
}