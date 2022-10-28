/*
 * @lc app=leetcode.cn id=28 lang=c
 *
 * [28] 找出字符串中第一个匹配项的下标
 */

// @lc code=start
int strStr(char * haystack, char * needle){
    int m = strlen(haystack);
    int n = strlen(needle);
    if(n == 0){
        return 0;
    }
    int next[n];
    next[0] = 0;
    for(int i = 1,j = 0;i < n;++i){
        while(j > 0 && needle[i] != needle[j]){
            j = next[j-1];
        }
        if(needle[i] == needle[j]){
            ++j;
        }
        next[i] = j;
    }
    for(int i = 0,j = 0;i < m;++i){
        while(j > 0 && haystack[i] != needle[j]){
            j = next[j-1];
        }
        if(haystack[i] == needle[j]){
            ++j;
        }
        if(j == n){
            return i-n+1;
        }
    }
    return -1;
}
// @lc code=end

