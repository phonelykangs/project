#include"config.h"

namespace server{

    static Logger::ptr g_logger = GET_LOG_NAME("system");

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

}