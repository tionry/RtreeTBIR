#include "InformationEntropy.h"
#include "stdafx.h"
#include "atlimage.h"
#include <math.h>
#include <iostream>
using namespace std;
#include <fstream>

int informationEntropy()
{
	char charArray[100];
	ifstream inFile("imagelist.txt", ios::in);
	ofstream outFile("InformationEntropy.txt", ios::out);
	while (!inFile.eof( ))
	{
		inFile.getline(charArray, 100);

		CImage image;

		int iHeight, iWidth;
		BYTE byteR, byteG, byteB;
		double s[16] = {0.0};
		double totalGary = 0.0;
		int k = 0;

		image.Load(charArray);

		iHeight = image.GetHeight();
		iWidth = image.GetWidth();
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
			{
				for ( int iRow=i*iWidth/4; iRow<(i+1)*iWidth/4; iRow++)
					for ( int iCol=j*iHeight/4; iCol<(j+1)*iHeight/4; iCol++ )
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

						totalGary = totalGary + gray;
					}
					s[k] = totalGary / double(iWidth*iHeight/16);
					s[k] = -s[k]*log(s[k])/log(2.0)-(1.0-s[k])*log(1.0-s[k])/log(2.0);
					k++;
					totalGary = 0;
			}
			
		outFile<<charArray<<'(';
		for(int i = 0;i < 16;i++)
		{
			outFile<<s[i]<<' ';
		}
		outFile<<')'<<endl;
		cout<<charArray<<endl;

		image.GetBits();

		image.Destroy();

		//getchar();
	}
	inFile.close( ); 
	outFile.close();
	return 0;
}