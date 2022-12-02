#ifndef  __CONFIG_H__
#define  __CONFIG_H__

#include<boost/lexical_cast.hpp>
#include<yaml-cpp/yaml.h>
#include<string>
#include<vector>
#include<sstream>
#include<memory>
#include<algorithm>
#include<list>
#include<iostream>
#include<map>
#include"logger.h"

namespace server{

class ConfigVarBase{
public:
    typedef std::shared_ptr<ConfigVarBase> ptr;
    ConfigVarBase(const std::string& name,const std::string& description = "")
    :m_name(name)
    ,m_description(description){
        std::transform(m_name.begin(),m_name.end(),m_name.begin(),::tolower);
    };
    virtual ~ConfigVarBase(){};
    const std::string& getName() const { return m_name;}
    const std::string& getDescription() const { return m_description;}
    virtual std::string toString() = 0;
    virtual void fromString(const std::string& str) = 0;
    virtual std::string getTypeName() const = 0;
protected:
    std::string m_name;
    std::string m_description;
};

template<typename F,typename T>
class LexicalCast{
public:
    T operator()(const F& val){
        return boost::lexical_cast<T>(val);
    }
};

template<typename T>
class LexicalCast<std::string,std::vector<T> >{
public:
    std::vector<T> operator()(const std::string& str){
        YAML::Node node = YAML::Load(str);
        typename std::vector<T> vec;
        std::stringstream ss;
        for(auto iter = node.begin();iter != node.end();++iter){
            ss.clear();
            ss.str("");
            ss << *iter;
            vec.push_back(LexicalCast<std::string,T>()(ss.str()));
        }
        return vec;
    }
};

template<typename T>
class LexicalCast<std::vector<T>,std::string >{
public:
    std::string operator()(const std::vector<T>& vec){
        YAML::Node node(YAML::NodeType::Sequence);
        std::stringstream ss;
        for(auto iter = vec.begin();iter != vec.end();++iter){
            node.push_back(YAML::Load(LexicalCast<T,std::string>()(*iter)));
        }
        ss.clear();
        ss.str("");
        ss << node;
        return ss.str();
    }
};

template<typename T>
class LexicalCast<std::string,std::list<T> >{
public:
    std::list<T> operator()(const std::string& str){
        YAML::Node node = YAML::Load(str);
        typename std::list<T> vec;
        std::stringstream ss;
        for(auto iter = node.begin();iter != node.end();++iter){
            ss.clear();
            ss.str("");
            ss << *iter;
            vec.push_back(LexicalCast<std::string,T>()(ss.str()));
        }
        return vec;
    }
};

template<typename T>
class LexicalCast<std::list<T>,std::string >{
public:
    std::string operator()(const std::list<T>& vec){
        YAML::Node node(YAML::NodeType::Sequence);
        std::stringstream ss;
        for(auto iter = vec.begin();iter != vec.end();++iter){
            node.push_back(YAML::Load(LexicalCast<T,std::string>()(*iter)));
        }
        ss.clear();
        ss.str("");
        ss << node;
        return ss.str();
    }
};

template<typename T,class FromStr = LexicalCast<std::string,T> 
                   ,class ToStr = LexicalCast<T,std::string> >
class ConfigVar : public ConfigVarBase{
public:
    typedef std::shared_ptr<ConfigVar> ptr;

    ConfigVar(const std::string& name
    ,const T& default_value
    ,const std::string& description = "")
    :ConfigVarBase(name,description)
    ,m_val(default_value){}

    std::string toString() override {
        try{
            return ToStr()(m_val);
        }
        catch(const std::exception& e){
            std::cerr << e.what() << '\n';
        }
        return "";
    }

    void fromString(const std::string& str) override {
        try{
            setValue(FromStr()(str));
        }
        catch(const std::exception& e){
            std::cerr << e.what() << '\n';
        }
    }

    T getValue(){ return m_val; }

    void setValue(const T& val){
        if(val == m_val){
            return;
        }
        m_val = val;
    }

    std::string getTypeName() const override { return TypeToName<T>(); }
private:
    T m_val;
};

class Config{
public:
    typedef std::unordered_map<std::string,ConfigVarBase::ptr> ConfigVarMap;
    typedef RWLOCK RWMutexType;

    template<typename T>
    static typename ConfigVar<T>::ptr lookup(const std::string& name
    ,const T& default_value
    ,const std::string& description){
        auto iter = GetDatas().find(name);
        if(iter != GetDatas().end()){
            auto temp = std::dynamic_pointer_cast<ConfigVar<T> >(iter->second);
            if(temp){
                LOG_INFO(GET_LOG_ROOT()) << name << " exists! ";
                return temp;
            }
            else{
                LOG_ERROR(GET_LOG_ROOT()) << "Lookup name exists but Type not match";
                return nullptr;
            }
        }

        if(name.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._012345678") != std::string::npos){
            LOG_ERROR(GET_LOG_ROOT()) << "LookUp invalid name";
            throw std::invalid_argument(name);
        }
        
        typename ConfigVar<T>::ptr val = std::make_shared<ConfigVar<T> >(name,default_value,description);
        GetDatas()[name] = (ConfigVarBase::ptr)val;
        return val;
    }

    static void LoadFromYaml(const YAML::Node& node);

    static ConfigVarBase::ptr LookupBase(const std::string& name);
private:
    static ConfigVarMap& GetDatas(){
        static ConfigVarMap s_datas;
        return s_datas;
    }

    static RWMutexType& GetMutex(){
        static RWMutexType s_mutex;
        return s_mutex;
    }
};
    
}

#endif