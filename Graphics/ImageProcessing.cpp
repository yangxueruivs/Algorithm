#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define WIDTH(bits) (((bits)+31)/32*4)

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;

//位图文件头信息结构定义
typedef struct tagFILEHEAD {
DWORD bfSize; //文件大小
WORD bfReserved1; //保留字
WORD bfReserved2; //保留字
DWORD bfOffBits; //实际位图数据的偏移字节数，即前三个部分长度之和
} FILEHEAD;

//位图数据信息结构定义
typedef struct tagINFOHEAD{
DWORD biSize; //指定此结构体的长度
LONG biWidth; //位图宽
LONG biHeight; //位图高
WORD biPlanes; //平面数
WORD biBitCount; //采用颜色位数
DWORD biCompression; //压缩方式
DWORD biSizeImage; //实际位图数据占用的字节数
LONG biXPelsPerMeter; //X方向分辨率
LONG biYPelsPerMeter; //Y方向分辨率
DWORD biClrUsed; //使用的颜色数
DWORD biClrImportant; //重要颜色数
} INFOHEAD;

int QuickSort(int a[],int numsize);//中值排序
double Psnr(FILE *originalImage, FILE *processedImage, long sizeOfImage, long h, long w);//计算PSNR

void main()
{
	FILEHEAD bitHead;
	INFOHEAD bitInfoHead;

	int i, j, mode, Matrix[3][3]={-1,-1,-1,-1,8,-1,-1,-1,-1}, Scale=2, Mat[5][5]={1,2,3,2,1,2,5,6,5,2,3,6,8,6,3,2,5,6,5,2,1,2,3,2,1};
	char addr[50];
	FILE *fp, *fp1, *fp2, *fpw, *ffp;
	printf("Please input image's address:\n");
	scanf("%s",&addr);
	fp=fopen(addr,"rb+");//读取文件
	fp1=fp;

	if(fp1!=NULL)//若文件存在，打开正确
	{
		printf("file open successfully.\n");
		fseek(fp1,2,SEEK_CUR); 
		fread(&bitHead,1,sizeof(tagFILEHEAD),fp1);
		fread(&bitInfoHead,1,sizeof(INFOHEAD),fp1);
		printf("\n");
	}

	int width = bitInfoHead.biWidth;
	int height = bitInfoHead.biHeight;
	int l_width = WIDTH(width* bitInfoHead.biBitCount);//计算位图的实际宽度并确保它为32的倍数
	long nData = height*l_width;	
	unsigned char *sc=(unsigned char *)malloc(nData);
	memset(sc,0,nData);//初始化
	unsigned char *wc=(unsigned char *)malloc(nData);
	memset(wc,0,nData);//初始化

	fseek(fp1,1078,0);
	fread(sc, 1, nData, fp1);//数据读到数组中
	fseek(fp1,1078,0);
	fread(wc, 1, nData, fp1);

	/*
	菜单栏
	*/
	printf("Mode1:Laplacian边缘检测\n");
	printf("Mode2:Robert边缘检测\n");
	printf("Mode3:Sobel边缘检测\n");
	printf("Mode4:平均滤波\n");
	printf("Mode5:自适应平均滤波小矩阵\n");
	printf("Mode6:自适应平均滤波大矩阵\n");
	printf("Mode7:中值滤波\n");
	printf("Mode8:自适应中值滤波\n");
	printf("Mode9:高斯滤波大矩阵\n");
	printf("Mode10:高斯滤波小矩阵\n");
	printf("Mode0:不滤波\n");
	printf("Please choose the mode:1/2/3/4/5/6/7/8/9/10/11/0\n");
	scanf("%d",&mode);

	switch (mode)
	{
	case 1: 
			break;
	case 2:
		{
			goto Robert;
		}
			break;
	case 3:
		{
			goto Sobel;
		}
			break;
	case 4:
		{
			Scale=9;
			for(i=0;i<3;i++)
				for(j=0;j<3;j++)
					Matrix[i][j]=1;  
		}
			break;
	case 5:
		{
			Scale=9;
			goto avr2;
		}
			break;
	case 6:
		{
			Scale=25;
			goto avr1;
		}
			break;
	case 7:
		{
			goto midFilter;  
		}
			break;
	case 8:
		{
			goto midFilter1;  
		}
			break;
	case 9:
		{
			Scale=88;
			goto Gauss;
		}
			break;
	case 10:
		{
			Scale=16;
			Matrix[0][0]=Matrix[0][2]=Matrix[2][0]=Matrix[2][2]=1;  
			Matrix[0][1]=Matrix[1][0]=Matrix[1][2]=Matrix[2][1]=2;
			Matrix[1][1]=4;
		}
			break;
	case 0:
		{
			goto writeFile;
		}
		break;
    }
	/*
	处理均值滤波和高斯滤波3X3矩阵的部分
	*/
	for(i=1;i<height-1;i++)
		for(j=1;j<l_width-1;j++)
		{
			int k=(Matrix[0][0]*sc[i*l_width+j-l_width-1]+Matrix[0][1]*sc[i*l_width+j-l_width]+Matrix[0][2]*sc[i*l_width+j-l_width+1]
				  +Matrix[1][0]*sc[i*l_width+j-1]+Matrix[1][1]*sc[i*l_width+j]+Matrix[1][2]*sc[i*l_width+j+1]
				  +Matrix[2][0]*sc[i*l_width+j+l_width-1]+Matrix[2][1]*sc[i*l_width+j+l_width]+Matrix[2][2]*sc[i*l_width+j+l_width+1])/Scale;		
			if(k>255)
				k=255;
			else if(k<0)
				k=0;
			wc[i*l_width+j]=k;	
		}
	goto writeFile;

	/*
	Sobel边缘检测算子具体算法部分
	*/
Sobel:
	for(i=1;i<height-1;i++)
		for(j=1;j<l_width-1;j++)
		{
			int temp[9];
			temp[0]=sc[i*l_width+j-l_width-1];
			temp[1]=sc[i*l_width+j-l_width];
			temp[2]=sc[i*l_width+j-l_width+1];
			temp[3]=sc[i*l_width+j-1];
			temp[4]=sc[i*l_width+j];
			temp[5]=sc[i*l_width+j+1];
			temp[6]=sc[i*l_width+j+l_width-1];
			temp[7]=sc[i*l_width+j+l_width];
			temp[8]=sc[i*l_width+j+l_width+1];
			int a1=abs((temp[0]+2*temp[1]+temp[2])-(temp[6]+2*temp[7]+temp[8])), a2=abs((temp[0]+2*temp[3]+temp[6])-(temp[2]+2*temp[5]+temp[8]));
			if(a1>a2)
				wc[i*l_width+j]=a1;
			else 
				wc[i*l_width+j]=a2;
		}
	goto writeFile;

	/*
	Robet边缘检测算子具体算法部分
	*/
Robert:
	for(i=0;i<height-1;i++)
		for(j=0;j<l_width-1;j++)
		{
			int temp[4];
			temp[0]=sc[i*l_width+j];
			temp[1]=sc[i*l_width+j+1];
			temp[2]=sc[i*l_width+j+l_width];
			temp[3]=sc[i*l_width+j+l_width+1];
			wc[i*l_width+j]=abs(temp[0]-temp[3])+abs(temp[1]-temp[2]);
		}
	goto writeFile;

	/*
	自适应均值滤波算法5X5矩阵实现
	*/
avr1:
	for(i=2;i<height-2;i++)
		for(j=2;j<l_width-2;j++)
		{
			int temp2[8],center=sc[i*l_width+j], count, k1, sum1=0, c=0;
			float  temp3[8];
			temp2[0]=(sc[i*l_width+j-2*l_width]+sc[i*l_width+j-2*l_width+1]+sc[i*l_width+j-l_width])/3;
			temp2[1]=(sc[i*l_width+j-2*l_width+2]+sc[i*l_width+j-l_width+2]+sc[i*l_width+j-l_width+1])/3;
			temp2[2]=(sc[i*l_width+j+2]+sc[i*l_width+j+1]+sc[i*l_width+j+l_width+1+2])/3;
			temp2[3]=(sc[i*l_width+j+l_width+1]+sc[i*l_width+j+2*l_width+1]+sc[i*l_width+j+2*l_width+2])/3;
			temp2[4]=(sc[i*l_width+j+l_width]+sc[i*l_width+j+2*l_width]+sc[i*l_width+j+2*l_width-1])/3;
			temp2[5]=(sc[i*l_width+j+l_width-1]+sc[i*l_width+j+2*l_width-2]+sc[i*l_width+j+l_width-2])/3;
			temp2[6]=(sc[i*l_width+j-1]+sc[i*l_width+j-2]+sc[i*l_width+j-l_width-2])/3;
			temp2[7]=(sc[i*l_width+j-2*l_width-1]+sc[i*l_width+j-2*l_width-2]+sc[i*l_width+j-l_width-1])/3;
			sum1=(sc[i*l_width+j-2*l_width-2]+sc[i*l_width+j-2*l_width-1]+sc[i*l_width+j-2*l_width]+sc[i*l_width+j-2*l_width+1]+sc[i*l_width+j-2*l_width+2]
				  +sc[i*l_width+j-l_width-2]+sc[i*l_width+j-l_width-1]+sc[i*l_width+j-l_width]+sc[i*l_width+j-l_width+1]+sc[i*l_width+j-l_width+2]
				  +sc[i*l_width+j-2]+sc[i*l_width+j-1]+sc[i*l_width+j]+sc[i*l_width+j+1]+sc[i*l_width+j+2]
				  +sc[i*l_width+j+l_width-2]+sc[i*l_width+j+l_width-1]+sc[i*l_width+j+l_width]+sc[i*l_width+j+l_width+1]+sc[i*l_width+j+l_width+2]
				  +sc[i*l_width+j+2*l_width-2]+sc[i*l_width+j+2*l_width-1]+sc[i*l_width+j+2*l_width]+sc[i*l_width+j+2*l_width+1]+sc[i*l_width+j+2*l_width+2])/Scale;		
			for(count=0;count<8;count++)
			{
				if(abs(temp2[count]-sum1)<25)
				{
					temp3[count]=temp2[count];
					c++;
				}
				else
					temp3[count]=0;
			}
			for(count=0;count<8;count++)
				k1+=temp3[count];
			k1=((k1+center)/(c+1))-29;
			if(k1>255)
				k1=255;
			else if(k1<0)
				k1=0;
			wc[i*l_width+j]=k1;	
		}
	goto writeFile;

	/*
	自适应均值滤波算法3X3矩阵实现
	*/
avr2:
	for(i=1;i<height-1;i++)
		for(j=1;j<l_width-1;j++)
		{
			int temp2[4],center=sc[i*l_width+j], count, k1, sum1=0, c=0;
			float  temp3[4];
			temp2[0]=(sc[i*l_width+j-l_width]+sc[i*l_width+j-l_width+1])/2;
			temp2[1]=(sc[i*l_width+j+1]+sc[i*l_width+j+l_width+1])/2;
			temp2[2]=(sc[i*l_width+j+l_width]+sc[i*l_width+j+l_width-1])/2;
			temp2[3]=(sc[i*l_width+j-l_width-1]+sc[i*l_width+j-1])/2;
			sum1=(sc[i*l_width+j-l_width-1]+sc[i*l_width+j-l_width]+sc[i*l_width+j-l_width+1]
				  +sc[i*l_width+j-1]+sc[i*l_width+j]+sc[i*l_width+j+1]
				  +sc[i*l_width+j+l_width-1]+sc[i*l_width+j+l_width]+sc[i*l_width+j+l_width+1])/Scale;	
			for(count=0;count<4;count++)
			{
				if(abs(temp2[count]-sum1)<45)
				{
					temp3[count]=temp2[count];
					c++;
				}
				else
					temp3[count]=0;
			}
			for(count=0;count<4;count++)
				k1+=temp3[count];
			k1=((k1+center)/(c+1))-33;
			if(k1>255)
				k1=255;
			else if(k1<0)
				k1=0;
			wc[i*l_width+j]=k1;	
		}
	goto writeFile;

	/*
	中值滤波算法实现
	*/
midFilter:
	for(i=1;i<height-1;i++)
		for(j=1;j<l_width-1;j++)
		{
				int temp[9];
				temp[0]=sc[i*l_width+j-l_width-1];
				temp[1]=sc[i*l_width+j-l_width];
				temp[2]=sc[i*l_width+j-l_width+1];
				temp[3]=sc[i*l_width+j-1];
				temp[4]=sc[i*l_width+j];
				temp[5]=sc[i*l_width+j+1];
				temp[6]=sc[i*l_width+j+l_width-1];
				temp[7]=sc[i*l_width+j+l_width];
				temp[8]=sc[i*l_width+j+l_width+1];
				wc[i*l_width+j]=QuickSort(temp, 9);
		}
	goto writeFile;

	/*
	自适应型中值滤波算法实现
	*/
midFilter1:
	for(i=1;i<height-1;i++)
		for(j=1;j<l_width-1;j++)
		{
			if(sc[i*l_width+j]==0||sc[i*l_width+j]==255)
			{
				int temp[9];
				temp[0]=sc[i*l_width+j-l_width-1];
				temp[1]=sc[i*l_width+j-l_width];
				temp[2]=sc[i*l_width+j-l_width+1];
				temp[3]=sc[i*l_width+j-1];
				temp[4]=sc[i*l_width+j];
				temp[5]=sc[i*l_width+j+1];
				temp[6]=sc[i*l_width+j+l_width-1];
				temp[7]=sc[i*l_width+j+l_width];
				temp[8]=sc[i*l_width+j+l_width+1];
				wc[i*l_width+j]=QuickSort(temp, 9);
			}
			else
				wc[i*l_width+j]=sc[i*l_width+j];
		}
	goto writeFile;

	/*
	高斯滤波算法5X5矩阵实现
	*/
Gauss:
	for(i=2;i<height-2;i++)
		for(j=2;j<l_width-2;j++)
		{
			int k=(Mat[0][0]*sc[i*l_width+j-2*l_width-2]+Mat[0][1]*sc[i*l_width+j-2*l_width-1]+Mat[0][2]*sc[i*l_width+j-2*l_width]+Mat[0][3]*sc[i*l_width+j-2*l_width+1]+Mat[0][4]*sc[i*l_width+j-2*l_width+2]
				  +Mat[1][0]*sc[i*l_width+j-l_width-2]+Mat[1][1]*sc[i*l_width+j-l_width-1]+Mat[1][2]*sc[i*l_width+j-l_width]+Mat[1][3]*sc[i*l_width+j-l_width+1]+Mat[1][4]*sc[i*l_width+j-l_width+2]
				  +Mat[2][0]*sc[i*l_width+j-2]+Mat[2][1]*sc[i*l_width+j-1]+Mat[2][2]*sc[i*l_width+j]+Mat[2][3]*sc[i*l_width+j+1]+Mat[2][4]*sc[i*l_width+j+2]
				  +Mat[3][0]*sc[i*l_width+j+l_width-2]+Mat[3][1]*sc[i*l_width+j+l_width-1]+Mat[3][2]*sc[i*l_width+j+l_width]+Mat[3][3]*sc[i*l_width+j+l_width+1]+Mat[3][4]*sc[i*l_width+j+l_width+2]
				  +Mat[4][0]*sc[i*l_width+j+2*l_width-2]+Mat[4][1]*sc[i*l_width+j+2*l_width-1]+Mat[4][2]*sc[i*l_width+j+2*l_width]+Mat[4][3]*sc[i*l_width+j+2*l_width+1]+Mat[4][4]*sc[i*l_width+j+2*l_width+2])/Scale;		
			if(k>255)
				k=255;
			else if(k<0)
				k=0;
			wc[i*l_width+j]=k;	
		}
	goto writeFile;

	/*
	写文件
	*/
writeFile:
	fseek(fp1,0,0);
	fread(sc, 1, 1078, fp1);
	fp2=fopen("ans.bmp","wb");	
	fwrite(sc, 1078, 1,fp2);
	fseek(fp2,1078,0);
	fwrite(wc, 1 ,nData, fp2);
	char p;
	printf("是否计算Psnr？y/n:\n");//判断是否计算信噪比
	scanf("%s",&p);
	if(p=='y')
	{
		char addr1[50], addr11[50];
		printf("输入你要使用的图片:\n");//输入噪声图
		scanf("%s",&addr1);
		printf("输入你要对比的原图:\n");//输入用于对比的源图像
		scanf("%s",&addr11);
		ffp=fopen(addr11,"rb+");
		fpw=fopen(addr1,"rb+");
		long n=370*height;
		printf("PSNR为：%f\n",Psnr(ffp,fpw,nData,height,l_width));
	}
	free(wc);
	free(sc);
	fclose(fp);
	fclose(fp2);
}

int QuickSort(int a[],int num)
{
	int i=0, j=num-1;
	int val=a[0];
	if(num>1)
	{
		while(i<j)
		{
			for(;j>i;j--)//从后向前搜索比val小的元素，找到后填到a[i]中并跳出循环
				if(a[j]<val)
				{
					a[i]=a[j];
					break;
				}
			for(;i<j;i++)//从前往后搜索比val大的元素，找到后填到a[j]中并跳出循环
				if(a[i]>val)
				{ 
					a[j]=a[i];
					break;
				}
		}
		a[i]=val;
		QuickSort(a,i);//递归，对前i个数排序
		QuickSort(a+i+1,num-1-i);//对i+1到numsize这numsize-1-i个数排序
	}
	return a[4];
}

double Psnr(FILE *originalImage, FILE *processedImage, long sizeOfImage, long h, long w)
{
	FILE *p1, *p2;
	int m,n;
	p1=originalImage;
	p2=processedImage;
	fseek(p1,1078,0);
	fseek(p2,1078,0);
	unsigned char *oc=(unsigned char *)malloc(sizeOfImage);
	memset(oc,0,sizeOfImage);
	unsigned char *pc=(unsigned char *)malloc(sizeOfImage);
	memset(pc,0,sizeOfImage);
	fread(oc, 1, sizeOfImage, p1);
	fread(pc, 1, sizeOfImage, p2);
	unsigned long sum=0;
	for(m=0;m<h;m++)
		for(n=0;n<w;n++)
		{	sum+=(abs(oc[m*w+n]-pc[m*w+n])*abs(oc[m*w+n]-pc[m*w+n]));//平方差累加
		//printf("MSE为：%d\n",abs(oc[m*w+n]-pc[m*w+n]));
		}
	double temp=sum;
	
	temp=sum/sizeOfImage;//计算MSE
	printf("MSE为：%f\n",temp);
	double psnr=10*(log10(255*255/temp));//计算PSNR
	free(oc);
	free(pc);
	return psnr;
}
