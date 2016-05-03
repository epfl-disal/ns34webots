/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009-2010, Uwicore Laboratory (www.uwicore.umh.es),
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

#ifndef CONVOY_HELPER_H
#define CONVOY_HELPER_H

#include "ns3/node-container.h"
#include "ns3/application-container.h"
#include "ns3/object-factory.h"
#include "ns3/net-device.h"

namespace ns3 {

/**

 * This class creates one or multiple instances of ns3::ConvoyApp and associates
 * it/them to one/multiple node(s).
 *
 */
class ConvoyHelper
{
public:
  /**
   * Create a ConvoyHelper which is used to make life easier for people wanting
   * to use Convoy App.
   */
  ConvoyHelper (std::string protocol);

  void SetAttribute (std::string name, const AttributeValue &value);

  /**
   * Install ConvoyApp on each Node in the provided NodeContainer.
   *
   * \param nodes The NodeContainer containing all of the nodes on which the C2C-IP-App has to be installed.
   *
   * \returns A list of Convoy-Apps, one for each input node
   */
 ApplicationContainer Install (NodeContainer nodes) const;

  /**
   * Install Convoy-App on the provided Node.  The Node is specified
   * directly by a Ptr<Node>
   *
   * \param node The node to install the application on.
   *
   * \returns An ApplicationContainer holding the application created.
   */
 ApplicationContainer Install (Ptr<Node> node) const;

  /**
   * Install Convoy-App on the provided Node.  The Node is specified
   * by a string that must have previously been associated with a Node using the
   * Object Name Service.
   *
   * \param nodeName The node to install the application on.
   *
   * \returns An ApplicationContainer holding the application created.
   */
  ApplicationContainer Install (std::string nodeName) const;

private:
  /**
   * \internal
   */
  Ptr<Application> InstallPriv (Ptr<Node> node) const;
  ObjectFactory m_factory;
  std::string m_protocol;
};

} // namespace ns3

#endif /* CONVOY_HELPER */
