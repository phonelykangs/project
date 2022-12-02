#include"logger.h"
#include<memory.h>
#include<yaml-cpp/yaml.h>
#include<iostream>
#include"config.h"

server::ConfigVar<int>::ptr g_int_config = server::Config::lookup("system.port",(int)8080,"system.port");
server::ConfigVar<float>::ptr g_float_config = server::Config::lookup("system.value",(float)10.0,"system.value");
server::ConfigVar<std::vector<int> >::ptr g_int_vec_config = server::Config::lookup("system.int_vec",std::vector<int>{1,2},"int vector");
server::ConfigVar<std::list<int> >::ptr g_int_list_config = server::Config::lookup("system.int_list",std::list<int>{1,2},"int list");


void testYAML(){
    YAML::Node node = YAML::LoadFile("/home/phonelykangs/project/project/server/test.yml");
    server::Config::LoadFromYaml(node);

    LOG_INFO(GET_LOG_ROOT()) << g_int_config->getValue();
    LOG_INFO(GET_LOG_ROOT()) << g_float_config->getValue();
    for(auto& i:g_int_vec_config->getValue()){
        LOG_INFO(GET_LOG_ROOT()) << i;
    }
    
    std::cout << g_int_list_config->getValue().size() << std::endl;
    for(auto& i:g_int_list_config->getValue()){
        LOG_INFO(GET_LOG_ROOT()) << i;
    }    
}

int main(){

    testYAML();

    return 0;
}