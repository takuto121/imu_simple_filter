#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <assert.h>

#include "udp.h"
#include "imu_simple_filter.h"

UDPManager rudp("0.0.0.0",4001);
UDPManager sudp("127.0.0.1",4002);
IMUFilterSimple ifs;

using namespace std;

vector<string> split(string str, string separator) {
    if (separator == "") return {str};
    vector<string> result;
    string tstr = str + separator;
    long l = tstr.length(), sl = separator.length();
    string::size_type pos = 0, prev = 0;
    
    for (;pos < l && (pos = tstr.find(separator, pos)) != string::npos; prev = (pos += sl)) {
        result.emplace_back(tstr, prev, pos - prev);
    }
    return result;
}

int main() {
    float wx, wy, wz;
    double dt, pre_ts=0;

    rudp.udp_bind();
    while (1){
        std::string rdata=rudp.udp_recv();

        vector<string> arr1 = split(rdata.c_str(), "\t");
        std::string sensorType = arr1[1];

        if (sensorType=="GYRO") {
            vector<string> arr2 = split(arr1[2].c_str(), ",");
            double ts = std::stod(arr2[0].c_str());
            float wx = std::stof(arr2[1].c_str());
            float wy = std::stof(arr2[2].c_str());
            float wz = std::stof(arr2[3].c_str());

            if (pre_ts==0) {pre_ts = ts;}
            else {
                dt = ts - pre_ts;
                ifs.update(wx, wy, wz, dt);
                pre_ts = ts;
            };

            printf("R: %f, %f, %f, %f, %f, %f, %f, %f, %f\n", ifs.r11, ifs.r12, ifs.r13, ifs.r21, ifs.r22, ifs.r23, ifs.r31, ifs.r32, ifs.r33);

            std::string r11_s = to_string(ifs.r11);
            std::string r12_s = to_string(ifs.r12);
            std::string r13_s = to_string(ifs.r13);
            std::string r21_s = to_string(ifs.r21);
            std::string r22_s = to_string(ifs.r22);
            std::string r23_s = to_string(ifs.r23);
            std::string r31_s = to_string(ifs.r31);
            std::string r32_s = to_string(ifs.r32);
            std::string r33_s = to_string(ifs.r33);

            std::string sdata = r11_s+','+r12_s+','+r13_s+','+r21_s+','+r22_s+','+r23_s+','+r31_s+','+r32_s+','+r33_s;

            sudp.udp_send(sdata);
        }
    }
    return 0;
};

