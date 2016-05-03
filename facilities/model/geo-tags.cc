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

#include "geo-tags.h"
#include "ns3/log.h"

namespace ns3
{

 
    //#################################################################################################

    NS_OBJECT_ENSURE_REGISTERED(ConvoyTag);

    ConvoyTag::ConvoyTag() :
              m_data(0)
    {
      //      NS_LOG_FUNCTION(this);
    }

    ConvoyTag::ConvoyTag(const void *data) :
              m_data(data)
    {
      //      NS_LOG_FUNCTION(this);
    }

    ConvoyTag::~ConvoyTag()
    {
      //      NS_LOG_FUNCTION(this);
    }

    TypeId
    ConvoyTag::GetTypeId(void)
    {
      static TypeId tid = TypeId("ns3::ConvoyTag").SetParent<Tag>().AddConstructor<ConvoyTag>();
      return tid;
    }

    TypeId
    ConvoyTag::GetInstanceTypeId(void) const
    {
      return GetTypeId();
    }

    uint32_t
    ConvoyTag::GetSerializedSize(void) const
    {
      return 8;
    }

    void
    ConvoyTag::Serialize(TagBuffer i) const
    {
      i.WriteU64((uint64_t)m_data); //  8 Byte
    }

    void
    ConvoyTag::Deserialize(TagBuffer i)
    {
      m_data = (void *)i.ReadU64();
    }

    void
    ConvoyTag::Print(std::ostream &os) const
    {
      os << "Data pointer: " << m_data;
    }

    //#################################################################################################

    NS_OBJECT_ENSURE_REGISTERED(JoinConvoyTag);

    JoinConvoyTag::JoinConvoyTag() :
              m_convoyId(0)
    {
      //      NS_LOG_FUNCTION(this);
    }

    JoinConvoyTag::JoinConvoyTag(uint32_t convoyId) :
              m_convoyId(convoyId)
    {
      //      NS_LOG_FUNCTION(this);
    }

    JoinConvoyTag::~JoinConvoyTag()
    {
      //      NS_LOG_FUNCTION(this);
    }

    TypeId
    JoinConvoyTag::GetTypeId(void)
    {
      static TypeId tid = TypeId("ns3::JoinConvoyTag").SetParent<Tag>().AddConstructor<JoinConvoyTag>();
      return tid;
    }
    TypeId
    JoinConvoyTag::GetInstanceTypeId(void) const
    {
      return GetTypeId();
    }
    uint32_t
    JoinConvoyTag::GetSerializedSize(void) const
    {
      return 4;
    }
    void
    JoinConvoyTag::Serialize(TagBuffer i) const
    {
      i.WriteU32(m_convoyId); //  4 Byte
    }
    void
    JoinConvoyTag::Deserialize(TagBuffer i)
    {
      m_convoyId = i.ReadU32();
    }
    void
    JoinConvoyTag::Print(std::ostream &os) const
    {
      os << "Convoy Id: " << m_convoyId;
    }


}

