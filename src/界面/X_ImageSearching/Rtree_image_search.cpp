/*
*Rtree_image_search.cpp
*Author: Qiao
*Time:2014-5-50
*以颜色矩作为特征检索
*/

#include "stdafx.h"

using namespace std;

#define DIMENTION 9
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

struct Rect
{
  Rect()  {}

  Rect(int *a_max)
  {
	  for(int i = 0;i < DIMENTION;i++)
	  {
		  min[i] = a_max[i];
		  max[i] = a_max[i];
	  }
  }


  double min[DIMENTION];
  double max[DIMENTION];
  int rect_id;
};

void calculate_similarity(int *similarimage,int nhits,int num,Rect *rects);

bool MySearchCallback(int id, void* arg) 
{
  printf("Hit data rect %d\n", id);
  return true; // keep going
}


void similar_image_search(char* image_name)
{
  RTree<int, double, DIMENTION, float> tree;
  int i, nhits, n;
  double temp;
  
  ifstream infile,infile_name;
  infile.open("ColorMoment_modified.txt");
  infile_name.open("imagelist.txt");
  Rect *rects;
  n = 0;
  rects = NULL;
  int number = 5614;
  rects = new Rect[number];
  while(!infile.bad())
  {	  
	  if(infile.eof()) break;
	  rects[n].rect_id = n;
	  for(int j = 0;j < DIMENTION;j++)
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
	  m++;
	  if(m >= number - 1)	break;	  
  }
  Rect search_rect;
  
  
  for(i=0; i < n; i++)
  { 
	  tree.Insert(rects[i].min, rects[i].max, i); // Note, all values including zero are fine in this version
  }
  //search similar image
  int num;
  double judge_rate = 0.0;
  int flag = 0;
  for(i = 0;i < 5614;i++)
  {
	  if(flag)	break;
	  for(int j = 0;;j++)
	  {
		  if(tree.b_n[i][j] == '\0')
		  {
			  num = i;
			  flag = 1;
			  break;
		  }
		  if(tree.b_n[i][j] != image_name[j])
		  {
			  break;
		  }
	  }
  }
  ofstream writefile1("result.txt");
  int num_pic = 0;//没搜索到10张的
  //for(num = 0;num < 5614;num++)
  {  
	  tree.foundCount = 0;
	  for(int l =0;l < DIMENTION;l++)
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
		  for(int l =0;l < DIMENTION;l++)
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
		  tem *= 10;
		  nhits = tree.Search(search_rect.min, search_rect.max, MySearchCallback, NULL);
	  }
	  i = 0;
	  calculate_similarity(tree.similar_image,nhits,num,rects);
	  int index_i = 0,k = 0;
	  while((k++) < 10 )
	  {
		  if(i >= nhits)	break;
		  if(fla[i] == number)
		  {
			  i++;
		  }
		  index_i += judge_similar(tree.b_n[fla[i]],tree.b_n[num]);
		  for(int j = 0;;j++)
		  {
			  //cout<<tree.b_n[tree.similar_image[i]][j];
			  writefile1<<tree.b_n[fla[i]][j];
			  if(tree.b_n[fla[i]][j] == '\0')	break;
		  }
		  //cout<<' '<<tree.similar_image[i]<<endl;
		  writefile1<<"\n";
		  i++;
	  }	  
	  if(k < 10)	num_pic++;
	  judge_rate += (double)index_i / (double)i;
  }
  cout<<"the accurate rate is "<<judge_rate/5614<<endl;
}

void calculate_similarity(int *similarimage,int nhits,int num,Rect *rects)
{
	double* similarity;
	similarity = new double[nhits];
	double weight;
	for(int i = 0;i < nhits;i++)
	{
		similarity[i] = 0;
		for(int j = 0;j < DIMENTION;j++)
		{
			similarity[i] +=  abs(rects[similarimage[i]].max[j] - rects[num].max[j]);
		}
	}
	double temp;
	int tm,j;
	for(int i = 0;i < 10 ;i++)
	{
		for(j = 0;j < nhits - i;j ++)
		{
			if(similarity[j] < similarity[j+1])
			{
				temp = similarity[j];
				similarity[j] = similarity[j + 1];
				similarity[j + 1] =temp;
				tm = j;
				fla[i] = similarimage[j];
			}
		}
		
	}
	
}
  
