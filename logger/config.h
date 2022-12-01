#ifndef  __CONFIG_H__
#define  __CONFIG_H__

#include<boost/lexical_cast.hpp>
#include<yaml-cpp/yaml.h>
#include<string>
#include<vector>
#include<sstream>
#include<memory>
#include<algorithm>

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
    const std::string& const getName() const { return m_name;}
    const std::string& const getDescription() const { return m_description;}
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
    std::vector<T> operator(const std::string& str){
        YAML::Node node = YAML::load(str);
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
    std::string operator()(const std::vector<T>& vec){
        YAML::Node node(YAML::NodeType::Sequence);
        std::stringstream ss;
        for(auto iter = vec.begin();iter != vec.end();++iter){
            node.push_back(YAML::load(LexicalCast<T,std::string>()(*iter)));
        }
        ss.clear();
        ss.str("");
        node >> ss;
        return ss.str();
    }
};

template<typename T,class FromStr = LexicalCast<std::string,T> 
                   ,class ToStr = LexicalCast<T,std::string> >
class ConfigVar : public ConfigVarBase{
public:
    typedef std::shared_ptr<ConfigVar> ptr;

    ConfigVar(const std::string& m_name
    ,const T& default_value
    ,const std::string& description = "")
    :ConfigVarBase(name,description,)
    ,m_val(,default_value){}

    std::string toString() override {
        try{
            return ToStr()(m_val);
        }
        catch(const std::exception& e){
            std::cerr << e.what() << '\n';
        }
        
    }

    void fromString(const std::string& str) override {
        try{
            setValue(FromStr(str));
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
private:
    T m_val;
};

class Config{
public:
    typedef std::unordered_map<std::string,ConfigVarBase::ptr> ConfigVarMap;

    template<typename T>
    static typename ConfigVar<T>::ptr lookup(const std::string& name
    ,const T& default_value
    ,const std::string& description){
        auto iter = GetDatas().find(name);
        if(iter != GetDatas().end()){
            auto tem = std::dynamic_pointer_cast<ConfigVar<T> >(iter->second);
        }
    }

    static void LoadFromYaml(const YAML::Node& node);
private:
    static ConfigVarMap& GetDatas(){
        static ConfigVarMap s_datas;
        return s_datas;
    }
};
    
}

#endif