#include "ColorHistogram.h"
#include "stdafx.h"
#include "atlimage.h"
#include <iostream>
using namespace std;
#include <fstream>

int ColorHistogram( )
{
	char charArray[100];
	ifstream inFile("imagelist.txt", ios::in);
	ofstream outFile("ColorHistogram.txt", ios::out);
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
				double tmp, minByte, maxByte, delta;
				double V, S, H;

				tmp = R>G?G:R;  
				minByte = tmp>B?B:tmp;  
				tmp = R>G?R:G;
				maxByte = tmp>B?tmp:B;  

				V = maxByte;//V
				delta = maxByte - minByte;
				if(maxByte != 0)
					S = delta / maxByte;//S
				else
				{
					S = 0;
					H = 0;
				}
				if (delta == 0)
				{  
					H = 0; 
				}  
				else if(R == maxByte)
				{  
					if (G >= B)  
						H = (G - B) / delta;  
					else  
						H = (G - B) / delta + 6.0;  
				}  
				else if( G == maxByte )  
					H = 2.0 + ( B - R ) / delta; 
				else if (B == maxByte)  
					H = 4.0 + ( R - G ) / delta;  
				H *= 60.0;  

				if(H<=60.0&&S<=0.5) colorHistogram[0]++;
				else if(H<=60.0&&S>0.5) colorHistogram[1]++;
				else if(H<=120.0&&S<=0.5) colorHistogram[2]++;
				else if(H<=120.0&&S>0.5) colorHistogram[3]++;
				else if(H<=180.0&&S<=0.5) colorHistogram[4]++;
				else if(H<=180.0&&S>0.5) colorHistogram[5]++;
				else if(H<=240.0&&S<=0.5) colorHistogram[6]++;
				else if(H<=240.0&&S>0.5) colorHistogram[7]++;
				else if(H<=300.0&&S<=0.5) colorHistogram[8]++;
				else if(H<=300.0&&S>0.5) colorHistogram[9]++;
				else if(H<=360.0&&S<=0.5) colorHistogram[10]++;
				else colorHistogram[11]++;


				/*printf("%Pixel at (%d,%d) is: R=0x%x,G=0x%x,B=0x%x\n",iRow, iCol, byteR, byteG, byteB);
				printf("%Pixel at (%d,%d) is: H=%lf,S=%lf,V=%lf\n",iRow, iCol, H, S, V);*/

			}
			int totalPixel = 0;
			for(int i = 0;i < 12;i++)
				totalPixel = totalPixel + colorHistogram[i];

			double percentageOfColor[12] = {0.0};
			
			outFile<<charArray<<'(';
			for(int i = 0;i < 12;i++)
			{
				percentageOfColor[i] = 100 * double(colorHistogram[i]) / double(totalPixel);
				outFile<<percentageOfColor[i]<<' ';
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
