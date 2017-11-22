#/bin/bash
echo 'Deleting plugins folder'
rm -rf plugins
echo 'Entering ns3 repo'
cd ns-3-allinone
echo 'Copying webots module'
cp -r ../webots ns-3-dev/src
echo 'Copying facilities module'
cp -r ../facilities ns-3-dev/src
cd ns-3-dev
echo 'Compiling sources'
./waf --build-profile=optimized --disable-examples --disable-tests --disable-python --enable-modules=webots configure
./waf
echo 'If the compilation fails, get back to ns3.25'
echo 'Copying libraries to ../../plugins/radio/omnet/dependencies'
mkdir -p ../../plugins/radio/omnet/dependencies
dependecies=(libns3-dev-bridge-optimized.so libns3-dev-core-optimized.so libns3-dev-energy-optimized.so libns3-dev-facilities-optimized.so libns3-dev-facilities-optimized.so libns3-dev-internet-optimized.so libns3-dev-mobility-optimized.so libns3-dev-mpi-optimized.so libns3-dev-network-optimized.so libns3-dev-propagation-optimized.so libns3-dev-stats-optimized.so libns3-dev-traffic-control-optimized.so libns3-dev-wave-optimized.so libns3-dev-wifi-optimized.so libns3-dev-spectrum-optimized.so libns3-dev-antenna-optimized.so)
for i in ${dependecies[@]}; do
	cp build/$i ../../plugins/radio/omnet/dependencies/$i
	echo "Copying $i to ../../plugins/radio/omnet/dependencies"
done
echo 'Creating libomnet.so for webots'
cp build/libns3-dev-webots-optimized.so ../../plugins/radio/omnet/libomnet.so
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../plugins/radio/omnet/dependencies #make sure dependencies are found by Webots/plugin

