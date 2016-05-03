/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2009-2010, Uwicore Laboratory (www.uwicore.umh.es),
 * University Miguel Hernandez, EU FP7 iTETRIS project
 *
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
 * Authors:
 * Ignacio Llatser <llatser@gmail.com>, 
 * Sebastian Kühlmorgen <sebastian.kuehlmorgen@tud.de>,
 * Guillaume Jornod <guillaume.jornod@gmail.com>
 * Based on the CAM implementation in iTetris from Michele Rondinone (http://www.ict-itetris.eu/)
 */
#include <string>
#include <sstream>
#include "convoy.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/socket-factory.h"
#include "ns3/simulator.h"
#include "ns3/mobility-model.h"
#include "ns3/packet.h"
#include "geo-tags.h"

namespace ns3
{
	NS_LOG_COMPONENT_DEFINE("Convoy");

  NS_OBJECT_ENSURE_REGISTERED(Convoy);

  TypeId
  Convoy::GetTypeId(void)
  {
  static TypeId tid = TypeId ("ns3::Convoy")
    .SetParent<Application> ()
    .AddConstructor<Convoy> ()
    .AddAttribute("Protocol", "The type of protocol to use.",
              TypeIdValue(SocketFactory::GetTypeId()),
              MakeTypeIdAccessor(&Convoy::m_tid),
              MakeTypeIdChecker())
    .AddAttribute ("IpAddressDest", "The IP address of the destination",
              Ipv4AddressValue (Ipv4Address("255.255.255.255")),
              MakeIpv4AddressAccessor (&Convoy::m_ipv4Dest),
              MakeIpv4AddressChecker ())
    .AddAttribute("Port", "The Port for Convoy Application",
              UintegerValue(11),
              MakeUintegerAccessor(&Convoy::m_port),
              MakeUintegerChecker<uint16_t>())
    .AddAttribute ("PacketSize", "The size of packets sent",
              UintegerValue (2048),
              MakeUintegerAccessor (&Convoy::m_packetSize),
              MakeUintegerChecker<uint32_t> ())
    .AddTraceSource("Tx", "A new packet is created and it is sent",
              MakeTraceSourceAccessor(&Convoy::m_TxTrace))
    .AddTraceSource ("Rx", "A packet has been received",
              MakeTraceSourceAccessor (&Convoy::m_RxTrace))
    ;
    return tid;
  }

  Convoy::Convoy() :
      m_socket(0),
      m_sendCount(0),
      m_recvCount(0)
  {
  }

  Convoy::~Convoy()
  {
  }

  void
  Convoy::DoDispose(void)
  {
    m_socket = 0;
    Application::DoDispose();
  }

  void
  Convoy::StartApplication(void)
  {
    if (!m_socket)
      {
        m_socket = Socket::CreateSocket(GetNode(), m_tid);
        InetSocketAddress src = InetSocketAddress(Ipv4Address::GetAny(), m_port); // accept every source address with port
        m_socket->Bind(src);

        m_socket->SetAllowBroadcast(true); // necessary for broadcast
        m_socket->SetRecvCallback(MakeCallback(&Convoy::Receive, this));
      }
    else
      {
        NS_FATAL_ERROR("Convoy: creation attempt of a socket for Convoy that has already a socket active");
        return;
      }
  }

  void
  Convoy::StopApplication(void)
  {
    if (m_sendEvent.IsRunning())
      {
        Simulator::Cancel(m_sendEvent);
      }
    if (m_socket)
      {
        m_socket->Close();
      }
  }

  void
  Convoy::Transmit(Address address, const void *data, uint32_t dataSize)
  {
    Ptr<MobilityModel> MM = GetNode()->GetObject<MobilityModel>();
    Ptr<Packet> packet = Create<Packet>(m_packetSize);

    std::cout << "Convoy: " << Simulator::Now ().GetSeconds () << " s. Node "
        << GetNode()->GetId()+1 << " Sending to " << InetSocketAddress::ConvertFrom (address).GetIpv4 ()
        << ", data pointer " << data << std::endl;

    // Add sharedData pointer to convoy tag
    ConvoyTag convoyTag(data);
    packet->AddPacketTag(convoyTag);

    m_sendCount++;

    m_TxTrace (packet, GetNode(),Simulator::Now());
    m_socket->SendTo (packet, 0, address);
  }

  void
  Convoy::Receive(Ptr<Socket> socket)
  {
    Ptr<Packet> packet;
    Address from;

    packet = socket->RecvFrom(from);

    m_recvCount++;
    m_RxTrace (packet, GetNode(), Simulator::Now());


    // If a Webots callback is defined, call it to send packet info to Webots
    if(!m_webotsCallback.IsNull()) {

        // Create a WebotsRadioEvent with information about the received message
        struct WebotsRadioEvent *event = new struct WebotsRadioEvent();

        ConvoyTag convoyTag;

        event->type = WEBOTS_RADIO_EVENT_RECEIVE;

        event->from = (char *)malloc(sizeof(InetSocketAddress::ConvertFrom(from).GetIpv4()));
        std::stringstream str_from;
        str_from << InetSocketAddress::ConvertFrom(from).GetIpv4();
        event->from = str_from.str().c_str();

        if (packet->PeekPacketTag(convoyTag))
          {
            event->data = convoyTag.GetData();
            event->data_size = packet->GetSize() ;
          }
        else
          {
            std::cout << "No recognized tag in the packet" << std::endl;
          }

        // User data not used for now
        //event->user_data=userData;

        event->rssi = 100; // should be fixed

//        std::cout << "Calling webots callback of node " << GetNode()->GetId()+1 << std::endl;
        m_webotsCallback(GetNode()->GetId()+1, event);
    }
    else
      std::cout << "Webots callback of node " << GetNode()->GetId()+1 << " does not exist!" << std::endl;
  }

} // Namespace ns3
