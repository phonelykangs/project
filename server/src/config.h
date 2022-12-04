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
#include<functional>
#include<map>
#include<set>
#include<unordered_map>
#include<unordered_set>
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

template<typename T>
class LexicalCast<std::string,std::set<T> >{
public:
    std::set<T> operator()(const std::string& str){
        typename std::set<T> ret;
        YAML::Node node = YAML::Load(str);
        std::stringstream ss;
        for(auto iter = node.begin();iter != node.end();++iter){
            ss.clear();
            ss.str("");
            ss << *iter;
            ret.insert(LexicalCast<std::string,T>()(ss.str()));
        }
        return ret;
    }
};

template<typename T>
class LexicalCast<std::set<T>,std::string >{
public:
    std::string operator()(const std::set<T>& val){
        YAML::Node node(YAML::NodeType::Sequence);
        std::stringstream ss;
        for(auto iter = val.begin();iter != val.end();++iter){
            node.push_back(YAML::Load(LexicalCast<T,std::string>()(*iter)));
        }
        ss.clear();
        ss.str("");
        ss << node;
        return ss.str();
    }
};

template<typename T>
class LexicalCast<std::string,std::unordered_set<T> >{
public:
    std::unordered_set<T> operator()(const std::string& str){
        typename std::unordered_set<T> ret;
        YAML::Node node = YAML::Load(str);
        std::stringstream ss;
        for(auto iter = node.begin();iter != node.end();++iter){
            ss.clear();
            ss.str("");
            ss << *iter;
            ret.insert(LexicalCast<std::string,T>()(ss.str()));
        }
        return ret;
    }
};

template<typename T>
class LexicalCast<std::unordered_set<T>,std::string >{
public:
    std::string operator()(const std::unordered_set<T>& val){
        YAML::Node node(YAML::NodeType::Sequence);
        std::stringstream ss;
        for(auto iter = val.begin();iter != val.end();++iter){
            node.push_back(YAML::Load(LexicalCast<T,std::string>()(*iter)));
        }
        ss.clear();
        ss.str("");
        ss << node;
        return ss.str();
    }
};

template<typename T>
class LexicalCast<std::string,std::map<std::string,T> >{
public:
    std::map<std::string,T> operator()(const std::string& str){
        YAML::Node node = YAML::Load(str);
        typename std::map<std::string,T> ret;
        std::stringstream ss;
        for(auto iter = node.begin();iter != node.end();++iter){
            ss.clear();
            ss.str("");
            ss << iter->second;
            ret[iter->first.Scalar()] = LexicalCast<std::string,T>()(ss.str());
        }
        return ret;
    }
};

template<typename T>
class LexicalCast<std::map<std::string,T>,std::string>{
public:
    std::string operator()(const std::map<std::string,T>& val){
        YAML::Node node(YAML::NodeType::Map);
        for(auto iter = val.begin();iter != val.end();++iter){
            node[iter->first] = YAML::Load(LexicalCast<T,std::string>()(iter->second));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template<typename T>
class LexicalCast<std::string,std::unordered_map<std::string,T> >{
public:
    std::unordered_map<std::string,T> operator()(const std::string& str){
        YAML::Node node = YAML::Load(str);
        typename std::unordered_map<std::string,T> ret;
        std::stringstream ss;
        for(auto iter = node.begin();iter != node.end();++iter){
            ss.clear();
            ss.str("");
            ss << iter->second;
            ret[iter->first.Scalar()] = LexicalCast<std::string,T>()(ss.str());
        }
        return ret;
    }
};

template<typename T>
class LexicalCast<std::unordered_map<std::string,T>,std::string>{
public:
    std::string operator()(const std::unordered_map<std::string,T>& val){
        YAML::Node node(YAML::NodeType::Map);
        for(auto iter = val.begin();iter != val.end();++iter){
            node[iter->first] = YAML::Load(LexicalCast<T,std::string>()(iter->second));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template<typename T,class FromStr = LexicalCast<std::string,T> 
                   ,class ToStr = LexicalCast<T,std::string> >
class ConfigVar : public ConfigVarBase{
public:
    typedef RWLOCK RWMutexType;
    typedef std::shared_ptr<ConfigVar> ptr;
    typedef std::function<void (const T& oldvalue,const T& newvalue)> on_change_cb;

    ConfigVar(const std::string& name
             ,const T& default_value
             ,const std::string& description = "")
    :ConfigVarBase(name,description)
    ,m_val(default_value){}

    std::string toString() override {
        try{
            //RWMutexType::ReadLock rlock(m_rwmutex);
            return ToStr()(m_val);
        }
        catch(const std::exception& e){
            LOG_ERROR(GET_LOG_ROOT()) << "ConfigVar::toString exception "
            << e.what() << " convert: " << TypeToName<T>() << " to string"
            << " name:" << m_name;
        }
        return "";
    }

    void fromString(const std::string& str) override {
        try{
            setValue(FromStr()(str));
        }
        catch(const std::exception& e){
            LOG_ERROR(GET_LOG_ROOT()) << "ConfigVar::fromString exception "
            << e.what() << " convert string to " << TypeToName<T>()
            << " name:" << m_name
            << " val " << str;
        }
    }

    T getValue(){
        //RWMutexType::ReadLock rlock(m_rwmutex);
         return m_val; 
    }

    void setValue(const T& val){
        {
            //RWMutexType::ReadLock rlock(m_rwmutex);
            if(val == m_val){
                return;
            }
            for(auto iter = m_cbs.begin();iter != m_cbs.end();++iter){
                iter->second(m_val,val);
            }
        }
        //RWMutexType::WriteLock wlock(m_rwmutex);
        m_val = val;
    }

    uint64_t addCallBackFunc(on_change_cb cb){
        static uint64_t s_func_id = 0;
        //RWMutexType::WriteLock wlock(m_rwmutex);
        ++s_func_id;
        m_cbs[s_func_id] = cb;
        return s_func_id;
    }

    void delCallBackFunc(uint64_t key){
        //RWMutexType::WriteLock wlock(m_rwmutex);
        m_cbs.erase(key);
    }

    void delCallBackFunc(on_change_cb cb){
        //RWMutexType::WriteLock wlock(m_rwmutex);
        for(auto iter = m_cbs.begin();iter != m_cbs.end();++iter){
            if(iter->second == cb){
                m_cbs.erase(iter);
            }
        }
    }

    on_change_cb getCallBackFunc(uint64_t key){
        //RWMutexType::ReadLock rlock(m_rwmutex);
        auto iter = m_cbs.find(key);
        return iter == m_cbs.end() ? nullptr : m_cbs[key];
    }

    void clearCallBacks(){
        //RWMutexType::WriteLock wlock(m_rwmutex);
        m_cbs.clear();
    }

    std::string getTypeName() const override { return TypeToName<T>(); }
private:
    //RWMutexType m_rwmutex;
    T m_val;
    std::map<uint64_t,on_change_cb> m_cbs;
};

class Config{
public:
    typedef std::unordered_map<std::string,ConfigVarBase::ptr> ConfigVarMap;
    typedef RWLOCK RWMutexType;

    template<typename T>
    static typename ConfigVar<T>::ptr lookup(const std::string& name
                                            ,const T& default_value
                                            ,const std::string& description){
        RWMutexType::WriteLock wlock(GetMutex());
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

    template<typename T>
    static typename ConfigVar<T>::ptr lookup(const std::string& name){
        RWMutexType::ReadLock rlock(GetMutex());
        auto iter = GetDatas().find(name);
        if(iter == GetDatas().end()){
            return nullptr;
        }
        return std::dynamic_pointer_cast<ConfigVar<T> >(iter->second);
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