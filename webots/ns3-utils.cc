/*
 * Copyright (c) 2017 EPFL TUD
 * Distributed Intelligent Systems and Algorithms Laboratory,
 * Ecole polytechnique fédérale de Lausanne
 * Technische Universität, Dresden
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors: Ignacio Llatser <llatser@gmail.com>
 */

#include "ns3/ns3-utils.h"
#include "ns3/convoy-helper.h"
#include "ns3/uinteger.h" 

using namespace ns3;

NodeContainer CreateNodes (uint32_t size)
{
  std::cout << "Creating " << size << " nodes.\n";

  NodeContainer nodes;
  nodes.Create (size);

  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");

  // Name nodes
  for (uint32_t i = 0; i < size; ++i)
    {
      // Causes an error "Names::Add(): Error adding name node-0"
      // when restarting a Webots simulation
      //std::ostringstream os;
      //os << "node-" << i;
      //Names::Add (os.str (), nodes.Get (i));
      std::cout << "Creating node " << i << std::endl;
    }

  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install(nodes);

  return nodes;
}


// Accurate channel model for a V2X environment (multi-slope log-distance with Nagamami fading), but it does not consider the parameters path loss exp and tx power set by Webots
NetDeviceContainer
ConfigurePhyMacLayers(NodeContainer nodes, double pathLossExp, double txPower, double rxSensitivity, uint32_t frequency, uint32_t channelNumber)
{
  // Channel model based on the paper Abbas, Taimoor, Katrin Sjöberg et al. "A Measurement Based Shadow Fading Model
  // for Vehicle-to-Vehicle Network Simulations." arXiv preprint arXiv:1203.3370 (2012).

  NqosWaveMacHelper wifiMac = NqosWaveMacHelper::Default ();
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();

  YansWifiChannelHelper wifiChannel;

  wifiChannel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");

  wifiChannel.AddPropagationLoss("ns3::NakagamiPropagationLossModel");

  Config::SetDefault("ns3::ThreeLogDistancePropagationLossModel::ReferenceLoss",DoubleValue(20+9.1-(-46))); // TxPower + TxGain + RxGain - Reference received power
  Config::SetDefault("ns3::ThreeLogDistancePropagationLossModel::Distance0",DoubleValue(10));
  Config::SetDefault("ns3::ThreeLogDistancePropagationLossModel::Distance1",DoubleValue(104));
  Config::SetDefault("ns3::ThreeLogDistancePropagationLossModel::Distance2",DoubleValue(104));
  Config::SetDefault("ns3::ThreeLogDistancePropagationLossModel::Exponent0",DoubleValue(1.9));
  Config::SetDefault("ns3::ThreeLogDistancePropagationLossModel::Exponent1",DoubleValue(3.8));
  Config::SetDefault("ns3::ThreeLogDistancePropagationLossModel::Exponent2",DoubleValue(3.8));

  // Fitted values to reproduce the results in the paper
  Config::SetDefault("ns3::NakagamiPropagationLossModel::Distance1",DoubleValue(71.6));
  Config::SetDefault("ns3::NakagamiPropagationLossModel::Distance2",DoubleValue(177.3));
  Config::SetDefault("ns3::NakagamiPropagationLossModel::m0",DoubleValue(1));
  Config::SetDefault("ns3::NakagamiPropagationLossModel::m1",DoubleValue(1));
  Config::SetDefault("ns3::NakagamiPropagationLossModel::m2",DoubleValue(0.85));

  wifiPhy.SetChannel (wifiChannel.Create ());

  // Values from the Katrin paper
  wifiPhy.Set("TxPowerStart", DoubleValue(20));
  wifiPhy.Set("TxPowerEnd", DoubleValue(20));
  wifiPhy.Set("TxGain", DoubleValue(9.1));
  wifiPhy.Set("RxGain", DoubleValue(0));
  wifiPhy.Set("EnergyDetectionThreshold", DoubleValue(-95.0));

  if (frequency != 0) {
      wifiPhy.Set("Frequency", UintegerValue(frequency));
  }

  if (channelNumber != 0) {
      wifiPhy.Set("ChannelNumber", UintegerValue(channelNumber));
  }

  WifiHelper wifi;
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, nodes);
  return devices;
}

void SetForwarding(NodeContainer nodes, char routingProtocol)
{
  // Here it is possible to install a routing/forwarding protocol in the nodes, in order to enable multi-hop forwarding of packets
  
  InternetStackHelper stack;


  // Install selected forwarding protocol to all nodes
  stack.Install(nodes);
}


Ipv4InterfaceContainer CreateInterfaces(NetDeviceContainer wifiDevices)
{
  Ipv4AddressHelper address;
  Ipv4InterfaceContainer interfaces;

  address.SetBase("10.0.0.0", "255.255.0.0");
  interfaces = address.Assign(wifiDevices);

  return interfaces;
}

void InstallApplications (NodeContainer nodes, Ipv4InterfaceContainer interfaces)
{
  // Here it is possible to install applications on the nodes, such as CAM or DENM

   ApplicationContainer apps;


  // Install convoy application
  ConvoyHelper convoyHelper("ns3::UdpSocketFactory");
  convoyHelper.SetAttribute("PacketSize", UintegerValue(1000));

  apps = convoyHelper.Install(nodes);
  apps.Start(Seconds(0));
  //apps.Stop(Seconds(100));

}


