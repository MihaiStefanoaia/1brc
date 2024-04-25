#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <string.h>


struct result {
    int sum = 0;
    int entries = 0;
    int low = INT32_MAX;
    int high = INT32_MIN;
};

std::map<std::string,result> map;

int generate_value(char* temperature){
    int sign = 1;
    if(*temperature == '-'){
        sign = -1;
        temperature++;
    }
    if(temperature[1] == '.'){
        return sign * (10 * temperature[0] + temperature[2] - 11 * '0');
    }
    return sign * ((100 * temperature[0]) + 10 * temperature[1] + temperature[3] - 111 * '0');
}

int main(int argc, char const *argv[])
{
    char namebuf[1024];
    auto f = std::ifstream("measurements.txt");
    while(f.getline(namebuf,1023)){
        auto temperature = strtok(namebuf, ";");
        temperature = strtok(NULL, ";");

        auto temp = generate_value(temperature);
        auto& city = map[namebuf];
        city.entries += 1;
        city.sum     += temp;
        city.low      = (city.low  > temp) ? temp : city.low;
        city.high     = (city.high < temp) ? temp : city.high;
    }
    // output data
    bool flag = false;
    printf("{");
    for(auto& [city, result] : map){
        if (flag){
            printf(", ");
        }
        auto avg = (float)result.sum / (float)result.entries;
        printf("%s=%.1f/%.1f/%.1f", city.c_str(), ((float)result.low / 10.0f), (avg / 10.0f), ((float)result.high / 10.0f));
        // std::cout << city << '=' << (result.low / 10) << '.' << (result.low % 10) << '/' << (avg / 10) << '.' << (avg % 10) << '/' << (result.high / 10) << '.' << (result.high % 10); 
        flag = true;
    }
    printf("}\n");
    return 0;
}
