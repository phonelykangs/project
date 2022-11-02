#include<myString.h>
#include<iostream>

int main(){
    myString str("123456");
    myString str1("78919");
    myString str2 = str + str1;
    std::cout << str2 << std::endl;
    std::cout << str.getIndexOf('6',0) << std::endl;
    str2.replaceCh('9','0');
    std::cout << str2 << std::endl;
    std::cout << str2.sub_str(2,6) << std::endl;

    exit(0);
}