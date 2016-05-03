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
 * Sebastian Kühlmorgen <sebastian.kuehlmorgen@tud.de>
 * Based on the CAM implementation in iTetris from Michele Rondinone (http://www.ict-itetris.eu/)
 */

#ifndef CONVOY_H
#define CONVOY_H

#include "ns3/application.h"
#include "ns3/data-rate.h"
#include "ns3/double.h"
#include "ns3/socket.h"
#include "ns3/webots_radio.h"
#include "Vec2d.h"
#include "vehicle-data.h"
#include "ns3/traced-callback.h"

namespace ns3
{

  class Convoy : public Application
  {
  public:
    static TypeId
    GetTypeId(void);

    Convoy();

    virtual
    ~Convoy();

    void
    SetDestinationAddress(Ipv4Address address)
    {
      m_ipv4Dest = address;
    }

    void
    SetPort(uint16_t port)
    {
      m_port = port;
    }

    void
    SetPacketSize(uint32_t packetSize)
    {
      m_packetSize = packetSize;
    }

    void
    SetWebotsCallback(void (*func)(const int, const struct WebotsRadioEvent *))
    {
      std::cout << "Setting Webots callback to node " << GetNode()->GetId()+1 << std::endl;
      m_webotsCallback = MakeCallback(func);
    }

    void Transmit (Address address, void const *data, uint32_t dataSize);
    void Receive(Ptr<Socket> socket);

  protected:
    virtual void DoDispose(void);

  private:
    virtual void StartApplication(void);
    virtual void StopApplication(void);

    TypeId m_tid;
    EventId m_sendEvent;

    Ptr<Socket> m_socket;
    Ipv4Address m_ipv4Dest; // ipv4 address of the destination
    uint16_t m_port; // port of the application
    Time m_interval; // message interval
    uint32_t m_packetSize; // packet size
    DataRate m_dataRate; // data rate of sending packets
    Time m_lifeTime; // lifetime of a message
    uint8_t m_hoplimit; // hop limit of the packet

    uint16_t m_sendCount; // total number of messages sent
    uint64_t m_recvCount; // total number of messages received
 
    TracedCallback<Ptr<const Packet>, Ptr<const Node>, Time > m_TxTrace; // trace functions will be implemented in the example file
    TracedCallback<Ptr<const Packet>, Ptr<const Node>, Time > m_RxTrace;
    Callback<void, int, const struct WebotsRadioEvent *> m_webotsCallback;

  };

} // namespace ns3

#endif   /* CONVOY_H */
