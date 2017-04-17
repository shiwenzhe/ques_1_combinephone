# 问题1

手机号的属性标签合并，文件phone.txt有一亿条手机号标签记录，文件new.txt有100万条记录两个文件的手机号合并成一个phone_all.txt文件

例如:
 
phone.txt文件格式如下：  
>13800000000|卡商,诈骗  
>13800000001|诈骗  
>13800000002|卡商  
>13800000003|外卖  


new.txt文件格式：  
>13800000002|卡商,诈骗  
>13800000003|金融  
>13800000004|卡商  

合并之后phone_all.txt文件：  
>13800000000|卡商,诈骗  
>13800000001|诈骗  
>13800000002|卡商,诈骗  
>13800000003|外卖,金融  
>13800000004|卡商  
 
考虑程序性能，内存利用，算法复杂度。

## 字典树方法  
解析该题目的基本思想是**构造字典树的方法，分别逐行遍历phone.txt和new.txt两个文件，将两个文件根据电话号码的每一位数字作为一个节点标记，成功构建关于电话号码的字典树，其中，每一个节点代表了一个数字，由于电话号码的长度是固定的11位，所以字典树的深度最多是11层，由于要用最后一层的节点保存每个电话号码对应的属性标签，及斜杠后面的中文标签，这里采用了C++中的set的容器进行存储，其中，**字典树插入和查询的复杂度均为常量**。如此实现以下两个方面的功能：  
1. 重复的电话号码出现时，将会实时更新末尾节点中对应号码的属性，即更新set容器；  
2. 新的电话号码出现时，会在构件字典树的过程中扩充字典树，从而实现了新的电话号码的保存。  

如此，最后构建完成的字典树包含了两个文件中所有电话号码的信息，并且可以将重复的电话号码有效地合并，最后采用深度优先搜索的方法遍历改字典树，将遍历结果输出至文件phone_all.txt即可，**算法的时间复杂度是O(n)，其空间复杂度为 $O(10^{11})$ **。  

