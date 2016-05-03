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

#include "ns3/string.h"
#include "ns3/convoy.h"
#include "convoy-helper.h"
#include "ns3/log.h"
#include "ns3/names.h"



namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ConvoyHelper");
ConvoyHelper::ConvoyHelper (std::string protocol)
{
  m_factory.SetTypeId ("ns3::Convoy");
  m_factory.Set ("Protocol", StringValue (protocol));
}

void
ConvoyHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
ConvoyHelper::Install (Ptr<Node> node) const
{
  return ApplicationContainer (InstallPriv (node));
}

ApplicationContainer
ConvoyHelper::Install (std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  return ApplicationContainer (InstallPriv (node));
}

ApplicationContainer
ConvoyHelper::Install (NodeContainer c) const
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      apps.Add (InstallPriv (*i));
    }

  return apps;
}

Ptr<Application>
ConvoyHelper::InstallPriv (Ptr<Node> node) const
{

  NS_LOG_INFO("[ns3][Convoy Helper] install Convoy application on node " << node->GetId());
  NS_LOG_INFO("\n");

  Ptr<Application> app = m_factory.Create<Application> ();
  node->AddApplication (app);

  return app;
}

} // namespace ns3
