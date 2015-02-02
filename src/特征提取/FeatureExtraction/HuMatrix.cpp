#include "HuMatrix.h"
#include "stdafx.h"
#include "atlimage.h"
#include <math.h>
#include <iostream>
using namespace std;
#include <fstream>

int HuMatrix()
{
	double M[7] = {0.0};
	char charArray[100];
	ifstream inFile("imagelist.txt", ios::in);
	ofstream outFile("HuMatrix.txt", ios::out);

	while (!inFile.eof( ))
	{
		inFile.getline(charArray, 100);

		CImage image;

		int iHeight, iWidth;
		BYTE byteR, byteG, byteB;
		int colorHistogram[12] = {0};

		image.Load(charArray);

		iHeight = image.GetHeight();
		iWidth = image.GetWidth();

		double m00=0,m11=0,m20=0,m02=0,m30=0,m03=0,m12=0,m21=0;//中心矩
		double x0=0,y0=0;
		double u20=0,u02=0,u11=0,u30=0,u03=0,u12=0,u21=0;//规范化后的中心矩
		double t1=0,t2=0,t3=0,t4=0,t5=0;//临时变量
		int center_x=0,center_y=0;//重心
		int i,j;

		double s10=0,s01=0,s00=0;//0阶矩和1阶矩

		for ( int iRow=0; iRow<iWidth; iRow++)
			for ( int iCol=0; iCol<iHeight; iCol++ )
			{
				COLORREF colorref = image.GetPixel( iRow, iCol );

				if ( colorref==CLR_INVALID )
				{
					printf("Out of range!");
					continue;
				}

				byteR =  GetRValue( colorref );
				byteG =  GetGValue( colorref );
				byteB =  GetBValue( colorref );

				double R = double(byteR)/0xff;
				double G = double(byteG)/0xff;
				double B = double(byteB)/0xff;
				double gray = R*0.299 + G*0.587 + B*0.114;

				s10=s10+iWidth*gray;
				s01=s01+iHeight*gray;
				s00=s00+gray;

				/*printf("%Pixel at (%d,%d) is: R=0x%x,G=0x%x,B=0x%x\n",iRow, iCol, byteR, byteG, byteB);
				printf("%Pixel at (%d,%d) is: H=%lf,S=%lf,V=%lf\n",iRow, iCol, H, S, V);*/

			}
			center_x = (int)(s10/s00+0.5);
			center_y = (int)(s01/s00+0.5);

			m00=s00;
			for(i=0;i<iHeight;i++)
			for(j=0;j<iWidth;j++)
			{
				COLORREF colorref = image.GetPixel( j, i );

				if ( colorref==CLR_INVALID )
				{
					printf("Out of range!");
					continue;
				}

				byteR =  GetRValue( colorref );
				byteG =  GetGValue( colorref );
				byteB =  GetBValue( colorref );

				double R = double(byteR)/0xff;
				double G = double(byteG)/0xff;
				double B = double(byteB)/0xff;
				double gray = R*0.299 + G*0.587 + B*0.114;
				
				x0=j-center_x;
				y0=i-center_y;
				m11+=x0*y0*gray;
				m20+=x0*x0*gray;
				m02+=y0*y0*gray;
				m03+=y0*y0*y0*gray;
				m30+=x0*x0*x0*gray;
				m12+=x0*y0*y0*gray;
				m21+=x0*x0*y0*gray;
			}

			//规范化后的中心矩
			u20=m20/pow(m00,2);
			u02=m02/pow(m00,2);
			u11=m11/pow(m00,2);
			u30=m30/pow(m00,2.5);
			u03=m03/pow(m00,2.5);
			u12=m12/pow(m00,2.5);
			u21=m21/pow(m00,2.5);

			//计算中间变量
			t1=u20-u02;
			t2=u30-3*u12;
			t3=3*u21-u03;
			t4=u30+u12;
			t5=u21+u03;

			//计算不变矩
			M[0]=u20+u02;
			M[1]=t1*t1+4*u11*u11;
			M[2]=t2*t2+t3*t3;
			M[3]=t4*t4+t5*t5;
			M[4]=t2*t4*(t4*t4-3*t5*t5)+t3*t5*(3*t4*t4-t5*t5);
			M[5]=t1*(t4*t4-t5*t5)+4*u11*t4*t5;
			M[6]=t3*t3*(t4*t4-3*t5*t5)-t2*t5*(3*t4*t4-t5*t5);
			
			//outFile<<charArray<<'(';
			for(int i = 0;i < 7;i++)
			{
				outFile<<M[i]<<' ';
			}
			outFile<<endl;

			cout<<charArray<<endl;

			image.GetBits();

			image.Destroy();

			//getchar();
	}
	inFile.close( ); 
	outFile.close();
	return 0;
}