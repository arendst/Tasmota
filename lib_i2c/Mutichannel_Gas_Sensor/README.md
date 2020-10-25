# Mutichannel_Gas_Sensor
This Arduino library is used for driving "Xadow - Mutichannel Gas Sensor" and "Grove - Mutichanel Gas Sensor"


## Usage:

mutichannelGasSensor.begin(0x04);

mutichannelGasSensor.powerOn();

then read the concentration of the specific gas you want to measure:

mutichannelGasSensor.measure_NH3();

mutichannelGasSensor.measure_CO();

mutichannelGasSensor.measure_NO2();

mutichannelGasSensor.measure_C3H8();

mutichannelGasSensor.measure_C4H10();

mutichannelGasSensor.measure_CH4();

mutichannelGasSensor.measure_H2();

mutichannelGasSensor.measure_C2H5OH();

For details please move to [wiki page](http://www.seeedstudio.com/wiki/Grove_-_Multichannel_Gas_Sensor).
 
 
 
----

This software is written by Jacky Zhang (![](http://www.seeedstudio.com/wiki/images/8/8f/Email_addr_of_jacky_zhang.png)) from [Seeed Technology Inc.](http://www.seeed.cc) and is licensed under [The MIT License](http://opensource.org/licenses/mit-license.php). Check License.txt/LICENSE for the details of MIT license.<br>

Contributing to this software is warmly welcomed. You can do this basically by<br>
[forking](https://help.github.com/articles/fork-a-repo), committing modifications and then [pulling requests](https://help.github.com/articles/using-pull-requests) (follow the links above<br>
for operating guide). Adding change log and your contact into file header is encouraged.<br>
Thanks for your contribution.

Seeed is a hardware innovation platform for makers to grow inspirations into differentiating products. By working closely with technology providers of all scale, Seeed provides accessible technologies with quality, speed and supply chain knowledge. When prototypes are ready to iterate, Seeed helps productize 1 to 1,000 pcs using in-house engineering, supply chain management and agile manufacture forces. Seeed also team up with incubators, Chinese tech ecosystem, investors and distribution channels to portal Maker startups beyond.


[![Analytics](https://ga-beacon.appspot.com/UA-46589105-3/Mutichannel_Gas_Sensor)](https://github.com/igrigorik/ga-beacon)
