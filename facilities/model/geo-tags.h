/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef GEO_TAGS_H
#define GEO_TAGS_H

#include "ns3/tag.h"
#include "ns3/nstime.h"

namespace ns3 {
    //###############################################################################
    // convoy communication service
    class ConvoyTag : public Tag
    {
    public:
      ConvoyTag ();
      ConvoyTag (const void *data);
      virtual ~ConvoyTag ();

      static TypeId GetTypeId (void);
      virtual TypeId GetInstanceTypeId (void) const;
      virtual uint32_t GetSerializedSize (void) const;
      virtual void Serialize (TagBuffer i) const;
      virtual void Deserialize (TagBuffer i);
      virtual void Print (std::ostream &os) const;

      void SetData(void *data) { m_data = data; }
      const void * GetData () { return m_data; }

    private:
      const void *m_data;
    };

    class JoinConvoyTag : public Tag
    {
    public:
      JoinConvoyTag ();
      JoinConvoyTag (uint32_t convoyId);
      virtual ~JoinConvoyTag ();

      static TypeId GetTypeId (void);
      virtual TypeId GetInstanceTypeId (void) const;
      virtual uint32_t GetSerializedSize (void) const;
      virtual void Serialize (TagBuffer i) const;
      virtual void Deserialize (TagBuffer i);
      virtual void Print (std::ostream &os) const;

      void SetConvoyId(uint32_t convoyId) { m_convoyId = convoyId; }
      uint32_t GetConvoyId () { return m_convoyId; }

    private:
      uint16_t m_convoyId;
    };

} // namespace ns3

#endif /* GEO_ROUTING_PROTOCOL_H */
