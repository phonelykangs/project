#include<boost/lexical_cast.hpp>
#include<yaml-cpp/yaml.h>
#include<string>
#include<vector>
#include<sstream>

namespace server{

template<typename Src,typename Target>
class LexicalCast{
public:
    Target operator()(const Src& s){
        return boost::lexical_cast<Target>(s);
    }
};

template<typename T>
class LexicalCast<std::string,std::vector<T>>{
public:
    std::vector<T> operator()(const std::string& s){
        YAML::Node node = YAML::Load(s);
        typename std::vector<T> vec;
        std::stringstream ss;
        for(YAML::const_iterator iter = node.begin();iter != node.end();++iter){
            ss.clear();
            ss.str("");
            ss << (*iter);
            vec.push_back(LexicalCast<std::string,T>()(ss.str()));
        }
        return vec;
    }
};

template<typename T>
class LexicalCast<std::vector<T>,std::string>{
    std::string operator()(const std::vector<T>& vec){
        YAML::Node node(YAML::NodeType::Sequence);
        for(auto& v:vec){
            node.push_back(YAML::Load(LexicalCast<T,std::string>()(v)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

}