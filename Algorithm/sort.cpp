#include<vector>
#include<iostream>

void insertSort(std::vector<int>& arr){
    int length = arr.size();
    for(int i = 1;i < length;++i){
        for(int j = i-1;j >= 0;--j){
            if(arr[j+1] < arr[j]){
                std::swap(arr[j],arr[j+1]);
            }
            else{
                break;
            }
        }
    }
}

int partition(std::vector<int>& nums,int flag,int start,int end){
    std::swap(nums[flag],nums[end]);
    int j = start-1;
    for(int i = start;i < end;++i){
        if(nums[i] < nums[end]){
            std::swap(nums[i],nums[++j]);
        }
    }
    std::swap(nums[end],nums[++j]);
    return j;
}

int randomPartition(std::vector<int>& nums,int start,int end){
    int flag = rand() % (end-start+1) + start;
    return partition(nums,flag,start,end);
}

void quickSort(std::vector<int>& nums,int start,int end){
    if(start > end){
        return;
    }
    srand((unsigned)time(NULL));
    int key = randomPartition(nums,start,end);
    quickSort(nums,start,key-1);
    quickSort(nums,key+1,end);
}

void printVec(std::vector<int>& vec){
    for(auto& i:vec){
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

int main(){
    std::vector<int> vec = {9,5,4,7,8,3,1,2,6,10};
    quickSort(vec,0,vec.size()-1);
    printVec(vec);
}