#include<iostream>
#include<fstream>

#define LENGTH 200

using namespace std;

int Partition(int *Array, int Head, int Tail)//单次分割
{
	int i = Head,j = Tail;
	int val = Array[Head];
	while(i<j)
	{
		for(;i<j;j--)//从尾到头
			if(Array[j]<val)
			{
				Array[i]=Array[j];//交换数据
				break;
			}
		for(;i<j;i++)//从头到尾
			if(val<Array[i])
			{
				Array[j]=Array[i];//交换数据
				break;
			}
	}
	Array[i] = val;
	return i;//返回主元
}

void QuickSort(int *a, int h, int t)//快速排序
{
	if(h<t)
	{
		int temp = Partition(a, h, t);
	    QuickSort(a, h, temp);//前一部分
		QuickSort(a, temp+1, t);//后一部分
	}
}

void ReadData(int *a, char *add)//从文件中读取数据
{
	ifstream fin(add);//打开文件用于读
    //ofstream foutC("D:\\C.txt");
	if(!fin)
		return;//打开文件失败则结束运行
    for(int i=0; i<200; i++) {
            fin>>a[i];
    }//文件中的数据写到数组中
}

void WriteData(int *a, char *add)//从文件中写入数据
{
	ofstream fout(add);//打开文件用于写，若文件不存在就创建它
    if(!fout)
		return;//打开文件失败则结束运行
    for(int i=0; i<200; i++) {
            fout<<a[i]<<endl;
    }//使用插入运算符写文件内容
}

void main()
{
	char address1[50] = "D:\\test.txt",address2[50] = "D:\\answer.txt";//设定文件路径
	//int Arrays[30]={2,4,23,5,12,3,66,90,1,35,51,28,45,31,7,9,22,26,13,79,34,55,100,29,33,44,6,8,19,99};
	int j, Arrays[LENGTH];

	ReadData(Arrays, address1);
	for(j=0;j<200;j++)
		cout<<Arrays[j]<<endl;
	QuickSort(Arrays, 0, LENGTH-1);
	
	WriteData(Arrays, address2);

}
