//
//  mypro.cpp
//  
//
//  Created by 许恬颖 on 2016-12-21.
//
//

#include "mypro.hpp"
#include <iostream>
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/application-module.h"
#include "ns3/bridge-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ns3project");

int
main(int argc, char *argv[])
{
    
 bool verbose = true;
CommandLine cmd;
cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
cmd.Parse (argc,argv);
    
    NS_LOG_INFO("Create nodes");
NodeContainer SeverNodes;
SeverNodes.create(2);
NodeContainer SwitchNodes;
SwitchNodes.create(2);
NodeContainer RouterNodes;
RouterNodes.create(1);
NodeContainer ApNodes;
ApNodes.create(2);
NodeContainer sta_ap0_nodes;
Sta_ap0_nodes.create(2);

NetDeviceContainer SeverDevices;
NetDeviceContainer SwitchDevices_0;
NetDeviceContainer SwitchDevices_1;
NetDeviceContainer RouterDevices;
NetDeviceContainer ApDevices_wifi;
NetDeviceContainer ApDevices_csma;
NetDeviceContainer Sta_ap0_devices;

NS_LOG_INFO ("Create channels.");
CsmaHelper csma;
csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));
    NetDeviceContainer link0=csma.Install(NodeContainer(ApNodes.Get(0),SwitchNodes.Get(0));
    SwitchDevices_0.Add(link0.Get(1));
    ApDevices_csma.Add(link0.Get(0));
    NetDeviceContainer link1=csma.Install(NodeContainer(ApNodes.Get(1),SwitchNodes.Get(0));
    SwitchDevices_0.Add(link1.Get(1));
    ApDevices_csma.Add(link1.Get(0));
    NetDeviceContainer sev0_swi0_link=csma.Install(NodeContainer(SeverNodes.Get(0),SwitchNodes.Get(0)));
    NetDeviceContainer sev1_swi1_link=csma.Install(NodeContainer(SeverNodes.Get(1),SwitchNodes.Get(1)));
NetDeviceContainer router_swi0_link=csma.Install(NodeContainer(SwitchNodes.Get(0),RouterNodes.Get(0)));
NetDeviceContainer router_swi1_link=csma.Install(NodeContainer(SwitchNodes.Get(1),RouterNodes.Get(0)));
   SeverDevices.Add(sev0_swi0_link.Get(0));
   SeverDevices.Add(sev1_swi1_link.Get(0));
   RouterDevices.Add(router_swi0_link.Get(1));
   RouterDevices.Add(router_swi1_link.Get(1));
   SwitchDevices_0.Add(sev0_swi0_link.Get(1));
   SwitchDevices_0.Add(router_swi0_link.Get(0));
   SwitchDevices_1.Add(sev1_swi1_link.Get(1));
   SwitchDevices_1.Add(router_swi1_link.Get(0));
                                          
                                          //install wifi devices on ApNodes
YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
phy.SetChannel (channel.Create ());

WifiHelper wifi;
wifi.SetRemoteStationManager ("ns3::AarfWifiManager");

WifiMacHelper mac;
Ssid ssid0 = Ssid ("ns-3-ssid0");
Ssid ssid1 = Ssid ("ns-3-ssid1");
mac.SetType ("ns3::StaWifiMac",
             "Ssid", SsidValue (ssid0),
             "ActiveProbing", BooleanValue (false));
Sta_ap0_devices=wifi.Install(phy,mac,sta_ap0_nodes);
mac.SetType ("ns3::StaWifiMac",
             "Ssid", SsidValue (ssid0));
ApDevices_wifi.Add(wifi.Install(phy,mac,ApNodes.Get(0)));
mac.SetType ("ns3::StaWifiMac",
             "Ssid", SsidValue (ssid1));
ApDevices_wifi.Add(wifi.Install(phy,mac,ApNodes.Get(1)));
                                          
MobilityHelper mobility;
                                          
mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                               "MinX", DoubleValue (0.0),
                               "MinY", DoubleValue (0.0),
                               "DeltaX", DoubleValue (5.0),
                               "DeltaY", DoubleValue (10.0),
                               "GridWidth", UintegerValue (3),
                               "LayoutType", StringValue ("RowFirst"));
                                          
mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                           "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
mobility.Install(sta_ap0_nodes);
                                          
 mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
mobility.Install(ApNodes);
                                          
//Install bridge
 BridgeHelper bridge;
 bridge.Install(SwitchNodes.Get(0),SwitchDevices_0);
 bridge.Install(SwitchNodes.Get(1),SwitchDevices_1);
 bridge.Install(ApNodes.Get(0),NetDeviceContainer(ApDevices_wifi.Get(0),ApDevices_csma.Get(0)));

//Add IP address
    NS_LOG_INFO ("Assign IP Addresses.");
    //不同的subnet下IP应该不一样
     Ipv4AddressHelper address;
    address.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer severinterfaces = address.Assign(SeverDevices.Get(0));
    address.Assign(RouterDevices.Get(0));
    address.Assign(Sta_ap0_devices);
                                          
    address.SetBase ("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer severinterfaces = address.Assign(SeverDevices.Get(1));
    address.Assign(RouterDevices.Get(1));
                                          
    NS_LOG_INFO ("Create Application.");
    UdpEchoServerHelper echoServer (9);
    ApplicationContainer serverApps0 = echoServer.Install (SeverNodes.Get (0));
    serverApps.Start (Seconds (1.0));
    serverApps.Stop (Seconds (10.0));
    ApplicationContainer serverApps0 = echoServer.Install (SeverNodes.Get (1));
    serverApps.Start (Seconds (1.0));
    serverApps.Stop (Seconds (10.0));
    
    UdpEchoClientHelper echoClient (severinterfaces.GetAddress(0), 9);
    echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
    echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
    echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
    
    ApplicationContainer clientApps = echoClient.Install (sta_ap0_nodes.Get (1));
    clientApps.Start (Seconds (2.0));
    clientApps.Stop (Seconds (10.0));
                                          
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    
    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();
                                          
    NS_LOG_INFO ("Run Simulation.");
    Simulator::Stop (Seconds(10.0));
    Simulator::Run ();
    monitor->CheckForLostPackets ();
Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
{
 Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
  //if ((t.sourceAddress=="10.1.1.0" && t.destinationAddress == "10.1.5.0"))
  {
std::cout << "Flow " << i->first  << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket
GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1024/1024  << " Mbps\n";
std::cout << "  Deley:      " << (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds()) << " s\n";
                                              }
                                          }

monitor->SerializeToXmlFile("NS3Project.flowmon", true, true);
                                          
  Simulator::Destroy ();
   NS_LOG_INFO ("Done.");
                                          
                                          }














