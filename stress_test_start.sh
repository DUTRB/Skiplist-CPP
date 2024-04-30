###
 # @Author: rubo
 # @Date: 2024-04-30 15:25:11
 # @LastEditors: HUAWEI-Ubuntu ruluy0205@163.com
 # @LastEditTime: 2024-04-30 15:27:05
 # @FilePath: /Skiplist-CPP/stress_test_start.sh
 # @Description: 
### 
#!/bin/bash
g++ stress-test/stress_test.cpp -o ./bin/stress  --std=c++11 -pthread  
./bin/stress
