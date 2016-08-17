#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Words 
	{
		char name[20]; 
		int freq; 
		struct Words *pNext;
		struct Words *pBack;
		struct Words *pAlph;
		struct Words *pBeta;
	}Words;//定义链表的结构

void main()
{
	int i,k;
	char word[20];
	struct Words *p[26], *pHead[26];

	FILE *fp1, *fp2;
	Words *tail=NULL,*head=NULL, *p1, *p2, *p3, *p4, *p5, *p6; 
	fp1=fopen("哈利波特全集_英文版_.txt","r+");

	for(int j=0;j<26;j++) 
	{
		p[j]=NULL;
		pHead[j]=NULL;
	}

	while((word[0]=fgetc(fp1))!=EOF)
	{
		p1=(Words *)malloc(sizeof(Words));
		if(65>word[0] || word[0]>122 || (90<word[0] && word[0]<97))
			continue;
		for(i=0;;)
		{
			if(65>word[i] || word[i]>122 || (90<word[i] && word[i]<97))
			{
				word[i]='\0';
				p1->name[i]=word[i];
				break;
			}
			p1->name[i]=word[i];
			p1->freq=1;
			i++;
			word[i] = fgetc(fp1);
		}//从文件中读入数据，并存储到链表中

		if(word[0]<91)
			k=word[0]-65;
		else
			k=word[0]-97;
	
		p4=p3=NULL;
		if(p[k]==NULL)
		{
			p[k]=p1;
			p1->pAlph=NULL;
			p1->pBeta=NULL;
			pHead[k]=p1;
		}
		else
			p3=p[k];

		while(p3!=NULL)
		{
			if(!stricmp(word, p3->name))
				break;
			p4=p3;
			p3=p3->pAlph;
		}//实现字符串搜索
	
		p6=p5=p2=p3;		
		if(p2!=0)
		{	
			p2->freq++;
			if(p2->pNext!=NULL)
			{
				p3=p2->pNext;
				while(p3->freq<p2->freq)
				{
					if(p3->pNext==NULL)
					{
						p3=p3->pNext;
						break;
					}
					p3=p3->pNext;
				}
				if(p2->pNext!=p3)
				{
					if(p2->pBack==NULL && p3==NULL)
					{
						head=p2->pNext;
						p2->pNext->pBack=NULL;
						tail->pNext=p2;
						p2->pBack=tail;
						p2->pNext=NULL;
						tail=p2;
					}
					else if(p2->pBack==NULL)
					{
						head=p2->pNext;
						p2->pNext->pBack=NULL;
						p3->pBack->pNext=p2;
						p2->pNext=p3;
						p2->pBack=p3->pBack;
						p3->pBack=p2;
					}
					else if(p3==NULL)
					{
						p2->pBack->pNext=p2->pNext;
						p2->pNext->pBack=p2->pBack;
						tail->pNext=p2;
						p2->pBack=tail;
						p2->pNext=NULL;
						tail=p2;
					}
					else
					{
						p2->pBack->pNext=p2->pNext;
						p2->pNext->pBack=p2->pBack;
						p3->pBack->pNext=p2;
						p2->pNext=p3;
						p2->pBack=p3->pBack;
						p3->pBack=p2;
					}
				}
			}//若链表中已有该单词，调整词频并排序

			if(p5->pBeta!=NULL)
			{		
				p6=p5->pBeta;
				while(p6->freq<p5->freq)
				{
					if(p6->pBeta==NULL)
					{
						p6=p6->pBeta;
						break;
					}
					p6=p6->pBeta;
				}
				if(p5->pBeta!=p6)
				{
					if(p5->pAlph==NULL && p6==NULL)
					{
						pHead[k]=p5->pBeta;
						p5->pBeta->pAlph=NULL;
						p[k]->pBeta=p5;
						p5->pAlph=p[k];
						p5->pBeta=NULL;
						p[k]=p5;
						
					}
					else if(p5->pAlph==NULL)
					{
						pHead[k]=p5->pBeta;
						p5->pBeta->pAlph=NULL;
						p6->pAlph->pBeta=p5;
						p5->pBeta=p6;
						p5->pAlph=p6->pAlph;
						p6->pAlph=p5;
					}
					else if(p6==NULL)
					{
						p5->pAlph->pBeta=p5->pBeta;
						p5->pBeta->pAlph=p5->pAlph;
						pHead[k]->pBeta=p5;
						p5->pAlph=p[k];
						p5->pBeta=NULL;
						p[k]=p5;
					}
					else
					{
						p5->pAlph->pBeta=p5->pBeta;
						p5->pBeta->pAlph=p5->pAlph;
						p6->pAlph->pBeta=p5;
						p5->pBeta=p6;
						p5->pAlph=p6->pAlph;
						p6->pAlph=p5;
					}
				}	
			}
		}//调整首字母的链表顺序

		else
		{		
			p1->pNext=head;	
			if(head!=NULL)
			{	
				p1->pBack=head->pBack;
				p1->pNext->pBack=p1;
				head->pBack=p1;
			}
			else	
			{
				p1->pBack=NULL;
				tail=p1;
			}
			if(p4!=NULL)
			{
				p4->pAlph=p1;
				p1->pBeta=p4;
				p1->pAlph=NULL;
				pHead[k]=p1;
			}	
			head=p1;
		}//若没有，直接插入到表头	
	}
	
	fp2=fopen("answer","w+");
	p3=head;
	while(p3!=NULL)
	{
		fprintf(fp2,"%s%d\n",p3->name,p3->freq);
		p3=p3->pNext;
	}//写入到目标文件中
	fclose(fp1);
	
	return;
}