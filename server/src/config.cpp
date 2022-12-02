#include"config.h"

static server::Logger::ptr g_logger = GET_LOG_NAME("system");

namespace server{

static void PrintYAML(const YAML::Node& node,int level){
    if(node.IsNull()){
        LOG_INFO(GET_LOG_ROOT()) << std::string(4*level, ' ') << "NULL" ;
    }
    else if(node.IsScalar()){
        LOG_INFO(GET_LOG_ROOT())  << std::string(4*level, ' ') << node.Scalar();
    }
    else if(node.IsMap()){
        for(auto iter = node.begin();iter != node.end();++iter){
            LOG_INFO(GET_LOG_ROOT())  << std::string(4*level, ' ') << iter->first.Scalar();
            PrintYAML(iter->second,level + 1);
        }
    }
    else if(node.IsSequence()){
        for(auto iter = node.begin();iter != node.end();++iter){
            PrintYAML(*iter,level+1);
        }
    }
}

ConfigVarBase::ptr Config::LookupBase(const std::string& name){
    RWLOCK::ReadLock lock(GetMutex());
    for(auto iter = GetDatas().begin();iter != GetDatas().end();++iter){
        if(iter->first == name){
            return iter->second;
        }
    }
    return nullptr;
}

static void ListAllMember(const std::string& prefix,
                          const YAML::Node& node,
                          std::list<std::pair<std::string,YAML::Node> >& output ){
    if(prefix.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._012345678")
                    != std::string::npos)
    {
        LOG_ERROR(g_logger) << "Config invalid" << "name: " << prefix << node;
        return;
    }
    output.push_back(std::make_pair(prefix,node));
    if(node.IsMap()){
        for(auto iter = node.begin();iter != node.end();++iter){
            ListAllMember(prefix.empty() ? iter->first.Scalar()
                                         : prefix + "." + iter->first.Scalar(), iter->second,output);
        }
    }
}

void Config::LoadFromYaml(const YAML::Node& node){
    std::list<std::pair<std::string,YAML::Node> > nodes;
    ListAllMember("",node,nodes);
    for(auto iter = nodes.begin();iter != nodes.end();++iter){
        std::string key = iter->first;
        if(key.empty()){
            continue;
        }

        std::transform(key.begin(),key.end(),key.begin(),::tolower);
        ConfigVarBase::ptr val = LookupBase(key);

        if(val){
            if(iter->second.IsScalar()){
                val->fromString(iter->second.Scalar());
            }
            else{
                std::stringstream ss;
                ss << iter->second;
                val->fromString(ss.str());
            }
        }
    }
}

}