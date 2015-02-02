/*
*main_test.cpp
*Author: Qiao
*2014-5-19
*说明：本程序用来测试准确率以及组合特征，k-近邻搜索采取范围扩展的方式，搜索结果不含本身 ，即如果当前范围没有找到足够的结果，扩展搜索的范围，
*准确率的计算是将所有样例都进行测试取平均值，组合特征采取了每个特征独立查找再进行综合加权计算取数值较小的前十个图片的方式。
*/
#include <stdio.h>
#include "RTree.h"
#include<fstream>
#include<iostream>
using namespace std;

#define DIMENTION1 7
#define DIMENTION2 12
int fla[10];
//judge the correctess of search
int judge_similar(char *a,char *b)
{
	int i  = 0;
	while(1)
	{
		if(a[i] != b[i])	return 0;
		if(a[i] == '_')	return 1;
		i++;
	}
}

struct similarity
{
	int index;
	float resu;
};
struct Rect
{
  Rect()  {}

  Rect(int *a_max)
  {
	  for(int i = 0;i < DIMENTION1;i++)
	  {
		  min[i] = a_max[i];
		  max[i] = a_max[i];
	  }
  }


  double min[DIMENTION1];
  double max[DIMENTION1];
  int rect_id;
};

struct Rect2
{
  Rect2()  {}

  Rect2(int *a_max)
  {
	  for(int i = 0;i < DIMENTION2;i++)
	  {
		  min[i] = a_max[i];
		  max[i] = a_max[i];
	  }
  }


  double min[DIMENTION2];
  double max[DIMENTION2];
  int rect_id;
};

bool MySearchCallback(int id, void* arg) 
{
  //printf("Hit data rect %d\n", id);
  return true; // keep going
}

void calculate_similarity(int *similarimage,int *similarimage2,int nhits,int nhits2,int num,Rect *rects,Rect2 *rects2);
void main()
{
  RTree<int, double, DIMENTION1, float> tree;
  RTree<int, double, DIMENTION2, float> tree2;
  int i, nhits, nhits2, n;
  double temp;
  spilts_time = 0;
  
  ifstream infile,infile2,infile_name;
  //此处是读取的特征文件
  infile.open("HuMatrix.txt");
  infile2.open("ColorHistogram_modified.txt");
  infile_name.open("imagelist.txt");
  Rect *rects;
  Rect2 *rects2;
  n = 0;
  rects = NULL;
  rects2 = NULL;
  int number = 5614;
  int hits_num = 0;
  rects = new Rect[number];
  rects2 = new Rect2[number];
  while(!infile.bad())
  {	  
	  if(infile.eof()) break;
	  rects[n].rect_id = n;
	  for(int j = 0;j < DIMENTION1;j++)
	  {
		  infile>>temp;
		  rects[n].max[j] = temp;
		  rects[n].min[j] = temp;
	  }
	  n++;
	  if(n >= number) break;
  }
  int m = 0;
  while(!infile_name.bad())
  {
	  if(infile_name.eof())	break;
	  infile_name.getline(tree.b_n[m],30);
	  for(int l = 0;;l++)
	  {
		  tree2.b_n[m][l] = tree.b_n[m][l];
		  if(tree.b_n[m][l] == '\0')	break;
	  }
	  m++;
	  if(m >= number - 1)	break;	  
  }
  n = 0;
  while(!infile2.bad())
  {	  
	  if(infile2.eof()) break;
	  rects2[n].rect_id = n;
	  for(int j = 0;j < DIMENTION2;j++)
	  {
		  infile2>>temp;
		  rects2[n].max[j] = temp;
		  rects2[n].min[j] = temp;
	  }
	  n++;
	  if(n >= number) break;
  }
  m = 0;
  Rect search_rect;
  Rect2 search_rect2;
  
  for(i=0; i < n; i++)
  { 
	  tree.Insert(rects[i].min, rects[i].max, i); // Note, all values including zero are fine in this version
	  tree2.Insert(rects2[i].min, rects2[i].max, i);
  }


  //search similar image
  int num;
  double judge_rate = 0.0;
  ofstream writefile1("result.txt");
  int num_pic = 0;//没搜索到10张的
  //num = 10;
  for(num = 0;num < number;num++)
  {  
	  tree.foundCount = 0;
	  for(int l =0;l < DIMENTION1;l++)
	  {
		  if(rects[num].min[l] >= 0)
		  {
			  search_rect.max[l] = rects[num].max[l] * 1.3;
			  search_rect.min[l] = rects[num].min[l] * 0.7;
		  }
		  else
		  {
			  search_rect.max[l] = rects[num].max[l] * 0.7;
			  search_rect.min[l] = rects[num].min[l] * 1.3;
		  }
	  }
	  nhits = tree.Search(search_rect.min, search_rect.max, MySearchCallback, NULL);
	  int tem = 1;
	  while(nhits < 10)
	  {
		  tree.foundCount = 0;
		  for(int l =0;l < DIMENTION1;l++)
		  {
			  if(rects[num].min[l] >= 0)
			  {
				  search_rect.max[l] += tem * 0.1;
				  search_rect.min[l] -= tem * 0.1;
			  }
			  else
			  {
				  search_rect.max[l] += tem * 0.1;
				  search_rect.min[l] -= tem * 0.1;
			  }
		  }
		  tem *= 5;
		  nhits = tree.Search(search_rect.min, search_rect.max, MySearchCallback, NULL);
	  }
	  
	  cout<<nhits<<endl;
	  hits_num += nhits;
	  nhits2  = 0;
	  //
	  tree2.foundCount = 0;
	  for(int l = 0;l < DIMENTION2;l++)
	  {
		  if(rects2[num].min[l] >= 0)
		  {
			  search_rect2.max[l] = rects2[num].max[l] * 1.3;
			  search_rect2.min[l] = rects2[num].min[l] * 0.7;
		  }
		  else
		  {
			  search_rect2.max[l] = rects2[num].max[l] * 0.7;
			  search_rect2.min[l] = rects2[num].min[l] * 1.3;
		  }
	  }
	  nhits2 = tree2.Search(search_rect2.min, search_rect2.max, MySearchCallback, NULL);
	  tem = 1;
	  while(nhits2 < 10)
	  {
		  tree2.foundCount = 0;
		  for(int l =0;l < DIMENTION2;l++)
		  {
			  if(rects2[num].min[l] >= 0)
			  {
				  search_rect2.max[l] += tem * 0.1;
				  search_rect2.min[l] -= tem * 0.1;
			  }
			  else
			  {
				  search_rect2.max[l] += tem * 0.1;
				  search_rect2.min[l] -= tem * 0.1;
			  }
		  }
		  tem *= 5;
		  nhits2 = tree2.Search(search_rect2.min, search_rect2.max, MySearchCallback, NULL);
	  }
	  
	  //
	  i = 0;
	  calculate_similarity(tree.similar_image,tree2.similar_image,nhits,nhits2,num,rects,rects2);
	  int index_i = 0,k = 0;
	  while((k++) < 10 )
	  {
		  if(i >= nhits)	break;
		  index_i += judge_similar(tree.b_n[fla[k]],tree.b_n[num]);		 
		  i++;
	  }	  
	  judge_rate += (double)index_i / (double)i;
  }
  cout<<hits_num / number;
  cout<<"the accurate rate is "<<judge_rate/number<<endl;
  cout<<spilts_time<<endl;
}
void calculate_similarity(int *similarimage,int *similarimage2,int nhits,int nhits2,int num,Rect *rects,Rect2 *rects2)
{
	similarity *sim;
	if(nhits>4000)	nhits = 4000;
	if(nhits2> 4000) nhits2 = 4000;
	sim = new similarity[nhits + nhits2];
	for(int i = 0;i < nhits;i++)
	{
		sim[i].resu = 0;
		sim[i].index = similarimage[i];
		for(int j = 0;j < DIMENTION1;j++)
		{
			sim[i].resu +=  1 * abs(rects[similarimage[i]].max[j] - rects[num].max[j]);
		}
		for(int j = 0;j < DIMENTION2;j++)
		{
			sim[i].resu +=  1 * abs(rects2[similarimage2[i]].max[j] - rects2[num].max[j]);
		}
	}
	for(int i = nhits;i < nhits + nhits2;i++)
	{
		sim[i].resu = 0;
		sim[i].index = similarimage2[i - nhits];
		for(int j = 0;j < DIMENTION2;j++)
		{
			sim[i].resu +=  1 * abs(rects2[similarimage2[i - nhits]].max[j] - rects2[num].max[j]);
		}
		for(int j = 0;j < DIMENTION1;j++)
		{
			sim[i].resu +=  1 * abs(rects[similarimage2[i - nhits]].max[j] - rects[num].max[j]);
		}
	}
	double temp;
	int tm,j;
	for(int i = 0;i < 10 ;i++)
	{
		for(j = 0;j < nhits - i;j ++)
		{
			if(sim[j].resu < sim[j+1].resu)
			{
				temp = sim[j].resu;
				sim[j].resu = sim[j + 1].resu;
				sim[j + 1].resu = temp;
				tm = j;
				fla[i] = sim[j].index;
			}
		}
		i++;
		i--;
	}
	delete []sim;
}