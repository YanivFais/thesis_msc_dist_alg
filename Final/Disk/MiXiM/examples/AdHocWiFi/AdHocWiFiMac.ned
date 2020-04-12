
package org.mixim.examples.AdHocWiFi;

import org.mixim.base.modules.BaseMacLayer;
import org.mixim.modules.mac.Mac80211;


simple AdHocWiFiMac extends Mac80211
{
    parameters:
        @class(AdHocWiFiMac);   

    // name of file name used for per to snr communication data
    string per2snr_filename;

    // adjust MCS according to SNR
    bool adjustMCS;
}
