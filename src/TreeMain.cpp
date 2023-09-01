#include"B+tree.h"
#include <iostream>
#include <vector>  
#include <algorithm> 

int main()
{
    while(1) {
        int pro=0;
        std::cout<<"输入0退出，输入1开始"<<std::endl;
        std::cin>>pro;
        if (pro==0) {
            break;
        }
        std::cout<<"输入MAX_KEYNUM"<<std::endl;
        int sss;
        std::cin>>sss;
        MAX_DATANUM=MAX_POINTERNUM=MAX_KEYNUM=sss-1;
        ORDER=sss/2;
        
        //建树
        BPlusTree<int> x;
        int num=0;
        std::vector<int> temp;
        int iKEY,iValue,delKey;
        while(1) {
            int choice;
            std::cout<<"请选择操作：1插入，2范围查询，3删除，4打印，5~100为测试，-1退出"<<std::endl;
            std::cin>>choice;
            if(choice==-1) {
                break;
            }

            switch (choice) {
            case 1: 
                std::cout<<"输入键值对,（0,0）退出"<<std::endl;
                std::cin>>iKEY>>iValue;
                while((iValue!=0)&&(iKEY!=0)) {
                    x.Insert(iKEY, iValue);
                    std::cout<<"层序遍历"<<std::endl;
                    x.LevelTraversal(x.GetRoot());
                    std::cout<<"输入键值对,（0,0）退出"<<std::endl;
                    std::cin>>iKEY>>iValue;
                }
                break;

            case 2:
                int minKey,maxKey;
                std::cout<<"请输入查询范围"<<std::endl;
                std::cin>>minKey>>maxKey;
                x.RangeQuery(minKey,maxKey);
                break;
            case 3:
                std::cout<<"请输入要删除的key(-1退出)"<<std::endl;
                std::cin>>delKey;
                while (delKey!=-1) {
                    bool success=x.Delete(delKey);
                    if(success==true) {
                        std::cout<<"删除后的树："<<std::endl;
                        x.LevelTraversal(x.GetRoot());
                    }
                    else {
                        std::cout <<"未找到，删除失败！"<<std::endl;
                    }
                    std::cout<<"请输入要删除的key(-1退出)"<<std::endl;
                    std::cin>>delKey;
                    
                }
                break;
            case 4:
                std::cout<<"叶子节点遍历"<<std::endl;
                x.PrintTree();
                std::cout<<"层序遍历"<<std::endl;
                x.LevelTraversal(x.GetRoot());
                break;
            case 5:
                x.Insert(5,5);
                x.Insert(8,8);
                x.Insert(10,10);
                x.Insert(12,12);
                x.Insert(11,11);
                x.Insert(17,17);
                x.Insert(55,55);
                x.Insert(9,9);
                x.Insert(2,2);
                x.Insert(7,7);
                x.Insert(14,14);
                x.Insert(56,56);
                x.Insert(42,42);
                x.Insert(1,1);
                x.Insert(77,77);
                x.Delete(12);
                x.Delete(9);
                x.Delete(5);
                x.LevelTraversal(x.GetRoot());
                x.Delete(2);
                x.LevelTraversal(x.GetRoot());
                x.Delete(55);
                x.LevelTraversal(x.GetRoot());
                x.Delete(56);
                x.LevelTraversal(x.GetRoot());
                x.Delete(1);
                x.LevelTraversal(x.GetRoot());
                //x.Delete(11); 
                break;
            case 6:
                std::cin>>num;
                for(int i=1;i<num;i++) {
                    x.Insert(i, i);
                }
                break;
            case 7:
                int n;
                std::cout<<"请输入随机个数"<<std::endl;
                std::cin>>n;
                for (int i = 0; i < n; i++) {
                    temp.push_back(i+1);
                }
                srand(time(0));
                //std::random_shuffle(temp.begin(), temp.end());
                for (int i = 0; i < n; i++) {
                    x.Insert(temp[i], temp[i]);
                }
                x.LevelTraversal(x.GetRoot());
                break;
            default:
                break;  
            }    
        }
    }
}