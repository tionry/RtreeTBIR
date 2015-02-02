#include "ColorMoment.h"
#include "stdafx.h"
#include "atlimage.h"
#include <math.h>
#include <iostream>
using namespace std;
#include <fstream>

int ColorMoment()
{
	char charArray[100];
	ifstream inFile("imagelist.txt", ios::in);
	ofstream outFile("ColorMoment.txt", ios::out);
	while (!inFile.eof( ))
	{
		inFile.getline(charArray, 100);

		CImage image;

		int iHeight, iWidth;
		BYTE byteR, byteG, byteB;
		double colorMoment[9] = {0.0};

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

				colorMoment[0] = colorMoment[0] + H;
				colorMoment[1] = colorMoment[1] + S;
				colorMoment[2] = colorMoment[2] + V;
			}
			colorMoment[0] = colorMoment[0]/double(iWidth*iHeight);
			colorMoment[1] = colorMoment[1]/double(iWidth*iHeight);
			colorMoment[2] = colorMoment[2]/double(iWidth*iHeight);

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

					colorMoment[3] = colorMoment[3] + (H-colorMoment[0])*(H-colorMoment[0]);
					colorMoment[4] = colorMoment[4] + (S-colorMoment[1])*(S-colorMoment[1]);
					colorMoment[5] = colorMoment[5] + (V-colorMoment[2])*(V-colorMoment[2]);
					colorMoment[6] = colorMoment[6] + (H-colorMoment[0])*(H-colorMoment[0])*(H-colorMoment[0]);
					colorMoment[7] = colorMoment[7] + (S-colorMoment[1])*(S-colorMoment[1])*(S-colorMoment[1]);
					colorMoment[8] = colorMoment[8] + (V-colorMoment[2])*(V-colorMoment[2])*(V-colorMoment[2]);
				}

				colorMoment[3] = pow(colorMoment[3]/(double)(iWidth*iHeight),1.0/2.0);
				colorMoment[4] = pow(colorMoment[4]/(double)(iWidth*iHeight),1.0/2.0);
				colorMoment[5] = pow(colorMoment[5]/(double)(iWidth*iHeight),1.0/2.0);
				if(colorMoment[6]<0.0) colorMoment[6] = (-1)*pow((-1)*colorMoment[6]/(double)(iWidth*iHeight),1.0/3.0);
				else colorMoment[6] = pow(colorMoment[6]/(double)(iWidth*iHeight),1.0/3.0);
				if(colorMoment[7]<0.0) colorMoment[7] = (-1)*pow((-1)*colorMoment[7]/(double)(iWidth*iHeight),1.0/3.0);
				else colorMoment[7] = pow(colorMoment[7]/(double)(iWidth*iHeight),1.0/3.0);
				if(colorMoment[8]<0.0) colorMoment[8] = (-1)*pow((-1)*colorMoment[8]/(double)(iWidth*iHeight),1.0/3.0);
				else colorMoment[8] = pow(colorMoment[8]/(double)(iWidth*iHeight),1.0/3.0);

			outFile<<charArray<<'(';
			for(int i = 0;i < 9;i++)
			{
				outFile<<colorMoment[i]<<' ';
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