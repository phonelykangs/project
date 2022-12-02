#include"logger.h"
#include<memory.h>
#include<yaml-cpp/yaml.h>

void testYAML(){
    YAML::Node node = YAML::LoadFile("./test.yml");


    LOG_ERROR(GET_LOG_ROOT()) << node;
}

int main(){
    testYAML();
    return 0;
}