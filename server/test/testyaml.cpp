#include"logger.h"
#include<memory.h>
#include<yaml-cpp/yaml.h>
#include<iostream>
#include"config.h"

void testYAML(){
    server::ConfigVar<int>::ptr g_int_config = server::Config::lookup("system.port",(int)8080,"system.port");
    server::ConfigVar<float>::ptr g_float_config = server::Config::lookup("system.value",(float)10.0,"system.value");
    server::ConfigVar<std::vector<int> >::ptr g_int_vec_config = server::Config::lookup("system.int_vec",std::vector<int>{1,2},"int vector");
    server::ConfigVar<std::list<int> >::ptr g_int_list_config = server::Config::lookup("system.int_list",std::list<int>{1,2},"int list");
    server::ConfigVar<std::set<int> >::ptr g_int_set_config = server::Config::lookup("system.int_set",std::set<int>{1,2,2,2},"int set");
    server::ConfigVar<std::unordered_set<int> >::ptr g_int_uset = server::Config::lookup("system.int_unorder_set",std::unordered_set<int>{1},"system.unordered_set");
    server::ConfigVar<std::map<std::string,int> >::ptr g_int_map_config = server::Config::lookup("system.int_map",std::map<std::string, int>{ {"type",1} },"int map");
    server::ConfigVar<std::unordered_map<std::string,int> >::ptr g_int_umap_config = server::Config::lookup("system.int_umap",std::unordered_map<std::string, int>{ {"type",1} },"int umap");
    server::ConfigVar<std::vector<std::vector<int> > >::ptr g_int_vec_vec_config = server::Config::lookup("system.int_vec_vec",std::vector<std::vector<int> >{{1},{2}},"int vector vec");

    YAML::Node node = YAML::LoadFile("/home/phonelykangs/project/project/server/test/testyaml.cpp");
    server::Config::LoadFromYaml(node);

    LOG_INFO(GET_LOG_ROOT()) << g_int_config->getValue();
    LOG_INFO(GET_LOG_ROOT()) << g_float_config->getValue();

#define YY(value)  { \
    for(auto& v:value->getValue()){ \
        LOG_INFO(GET_LOG_ROOT()) << v; \
    } \
} 

#define XX(value)  { \
    for(auto& v:value->getValue()){ \
        LOG_INFO(GET_LOG_ROOT()) << v.first << ":" << v.second; \
    } \
} 

    YY(g_int_vec_config)
    YY(g_int_list_config)
    YY(g_int_set_config)
    YY(g_int_uset)

    XX(g_int_map_config)
    XX(g_int_umap_config)

#undef YY
#undef XX

    for(auto& v: g_int_vec_vec_config->getValue()){
        for(auto& i :v){
            LOG_INFO(GET_LOG_ROOT()) << i;
        }
    }
}

void testLogger(){
    static server::Logger::ptr s_logger = GET_LOG_ROOT();
    LOG_INFO(s_logger) << server::LogMgr::GetInstance()->toYAMLString();
    YAML::Node node = YAML::LoadFile("/home/phonelykangs/project/project/server/test/test.yml");
    server::Config::LoadFromYaml(node);
    std::cout << server::LogMgr::GetInstance()->toYAMLString();
    server::Logger::ptr g_logger = GET_LOG_ROOT();
    LOG_FATAL(g_logger) << 123;
}

int main(){
    testLogger();
    return 0;
}