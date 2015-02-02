#include "NewColorHistogram.h"
#include "stdafx.h"
#include "atlimage.h"
#include <iostream>
using namespace std;
#include <fstream>

int NewColorHistogram( )
{
	char charArray[100];
	ifstream inFile("imagelist.txt", ios::in);
	ofstream outFile("NewColorHistogram.txt", ios::out);
	while (!inFile.eof( ))
	{
		inFile.getline(charArray, 100);

		CImage image;

		int iHeight, iWidth;
		BYTE byteR, byteG, byteB;
		int colorHistogram[288] = {0};

		image.Load(charArray);

		iHeight = image.GetHeight();
		iWidth = image.GetWidth();

		for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
		for ( int iRow=i*iWidth/3; iRow<(i+1)*iWidth/3; iRow++)
		for ( int iCol=j*iHeight/3; iCol<(j+1)*iHeight/3; iCol++ )
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

				if(V<=0.2) colorHistogram[(3*i+j)*32+0]++;//ºÚÉ«
				else if(S<=0.1&&V>0.2&&V<=0.5) colorHistogram[(3*i+j)*32+1]++;//Éî»Ò
				else if(S<=0.1&&V>0.5&&V<=0.8) colorHistogram[(3*i+j)*32+2]++;//Ç³»Ò
				else if(S<=0.1&&V>0.8) colorHistogram[(3*i+j)*32+3]++;//°×É«
				else if((H<=20.0||H>330.0)&&S<=0.45&&S>0.1&&V<=0.5&&V>0.2) colorHistogram[(3*i+j)*32+4]++;//µ­°µºì
				else if((H<=20.0||H>330.0)&&S<=0.45&&S>0.1&&V>0.5) colorHistogram[(3*i+j)*32+5]++;//µ­Ã÷ºì
				else if((H<=20.0||H>330.0)&&S>0.45&&V<=0.5&&V>0.2) colorHistogram[(3*i+j)*32+6]++;//Å¨°µºì
				else if((H<=20.0||H>330.0)&&S>0.45&&V>0.5) colorHistogram[(3*i+j)*32+7]++;//Å¨Ã÷ºì
				else if(H<=45.0&&H>20.0&&S<=0.45&&S>0.1&&V<=0.5&&V>0.2) colorHistogram[(3*i+j)*32+8]++;//µ­°µ³È
				else if(H<=45.0&&H>20.0&&S<=0.45&&S>0.1&&V>0.5) colorHistogram[(3*i+j)*32+9]++;//µ­Ã÷³È
				else if(H<=45.0&&H>20.0&&S>0.45&&V<=0.5&&V>0.2) colorHistogram[(3*i+j)*32+10]++;//Å¨°µ³È
				else if(H<=45.0&&H>20.0&&S>0.45&&V>0.5) colorHistogram[(3*i+j)*32+11]++;//Å¨Ã÷³È
				else if(H<=75.0&&H>45.0&&S<=0.45&&S>0.1&&V<=0.5&&V>0.2) colorHistogram[(3*i+j)*32+12]++;//µ­°µ»Æ
				else if(H<=75.0&&H>45.0&&S<=0.45&&S>0.1&&V>0.5) colorHistogram[(3*i+j)*32+13]++;//µ­Ã÷»Æ
				else if(H<=75.0&&H>45.0&&S>0.45&&V<=0.5&&V>0.2) colorHistogram[(3*i+j)*32+14]++;//Å¨°µ»Æ
				else if(H<=75.0&&H>45.0&&S>0.45&&V>0.5) colorHistogram[(3*i+j)*32+15]++;//Å¨Ã÷»Æ
				else if(H<=165.0&&H>75.0&&S<=0.45&&S>0.1&&V<=0.5&&V>0.2) colorHistogram[(3*i+j)*32+16]++;//µ­°µÂÌ
				else if(H<=165.0&&H>75.0&&S<=0.45&&S>0.1&&V>0.5) colorHistogram[(3*i+j)*32+17]++;//µ­Ã÷ÂÌ
				else if(H<=165.0&&H>75.0&&S>0.45&&V<=0.5&&V>0.2) colorHistogram[(3*i+j)*32+18]++;//Å¨°µÂÌ
				else if(H<=165.0&&H>75.0&&S>0.45&&V>0.5) colorHistogram[(3*i+j)*32+19]++;//Å¨Ã÷ÂÌ
				else if(H<=200.0&&H>165.0&&S<=0.45&&S>0.1&&V<=0.5&&V>0.2) colorHistogram[(3*i+j)*32+20]++;//µ­°µÇà
				else if(H<=200.0&&H>165.0&&S<=0.45&&S>0.1&&V>0.5) colorHistogram[(3*i+j)*32+21]++;//µ­Ã÷Çà
				else if(H<=200.0&&H>165.0&&S>0.45&&V<=0.5&&V>0.2) colorHistogram[(3*i+j)*32+22]++;//Å¨°µÇà
				else if(H<=200.0&&H>165.0&&S>0.45&&V>0.5) colorHistogram[(3*i+j)*32+23]++;//Å¨Ã÷Çà
				else if(H<=270.0&&H>200.0&&S<=0.45&&S>0.1&&V<=0.5&&V>0.2) colorHistogram[(3*i+j)*32+24]++;//µ­°µÀ¶
				else if(H<=270.0&&H>200.0&&S<=0.45&&S>0.1&&V>0.5) colorHistogram[(3*i+j)*32+25]++;//µ­Ã÷À¶
				else if(H<=270.0&&H>200.0&&S>0.45&&V<=0.5&&V>0.2) colorHistogram[(3*i+j)*32+26]++;//Å¨°µÀ¶
				else if(H<=270.0&&H>200.0&&S>0.45&&V>0.5) colorHistogram[(3*i+j)*32+27]++;//Å¨Ã÷À¶
				else if(H<=330.0&&H>270.0&&S<=0.45&&S>0.1&&V<=0.5&&V>0.2) colorHistogram[(3*i+j)*32+28]++;//µ­°µ×Ï
				else if(H<=330.0&&H>270.0&&S<=0.45&&S>0.1&&V>0.5) colorHistogram[(3*i+j)*32+29]++;//µ­Ã÷×Ï
				else if(H<=330.0&&H>270.0&&S>0.45&&V<=0.5&&V>0.2) colorHistogram[(3*i+j)*32+30]++;//Å¨°µ×Ï
				else  colorHistogram[(3*i+j)*32+31]++;//Å¨Ã÷×Ï

				/*printf("%Pixel at (%d,%d) is: R=0x%x,G=0x%x,B=0x%x\n",iRow, iCol, byteR, byteG, byteB);
				printf("%Pixel at (%d,%d) is: H=%lf,S=%lf,V=%lf\n",iRow, iCol, H, S, V);*/

			}
			int totalPixel = 0;
			for(int i = 0;i < 288;i++)
				totalPixel = totalPixel + colorHistogram[i];

			double percentageOfColor[288] = {0.0};
			
			//outFile<<charArray<<'(';
			for(int i = 0;i < 288;i++)
			{
				percentageOfColor[i] = 100 * double(colorHistogram[i]) / double(totalPixel);
				outFile<<percentageOfColor[i]<<' ';
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

int NewColorHistogram2()
{
	char charArray[100];
	ifstream inFile("imagelist.txt", ios::in);
	ofstream outFile("NewColorHistogram2.txt", ios::out);
	while (!inFile.eof( ))
	{
		inFile.getline(charArray, 100);

		CImage image;

		int iHeight, iWidth;
		BYTE byteR, byteG, byteB;
		int colorHistogram[45] = {0};

		image.Load(charArray);

		iHeight = image.GetHeight();
		iWidth = image.GetWidth();

		for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
		for ( int iRow=i*iWidth/3; iRow<(i+1)*iWidth/3; iRow++)
		for ( int iCol=j*iHeight/3; iCol<(j+1)*iHeight/3; iCol++ )
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

				if(V<=0.2) colorHistogram[(3*i+j)*5+0]++;//ºÚÉ«
				else if(S<=0.1) colorHistogram[(3*i+j)*5+1]++;//°×É«
				else if((H<=75.0||H>330.0)&&S>0.1&&V>0.2) colorHistogram[(3*i+j)*5+2]++;//ºì³È»Æ
				else if(H<=200.0&&H>75.0&&S>0.1&&V>0.2) colorHistogram[(3*i+j)*5+3]++;//ÂÌÇà
				else colorHistogram[(3*i+j)*5+4]++;//À¶×Ï

				/*printf("%Pixel at (%d,%d) is: R=0x%x,G=0x%x,B=0x%x\n",iRow, iCol, byteR, byteG, byteB);
				printf("%Pixel at (%d,%d) is: H=%lf,S=%lf,V=%lf\n",iRow, iCol, H, S, V);*/

			}
			int totalPixel = iWidth*iHeight;

			double percentageOfColor[45] = {0.0};
			
			//outFile<<charArray<<'(';
			for(int i = 0;i < 45;i++)
			{
				percentageOfColor[i] = 100 * double(colorHistogram[i]) / double(totalPixel);
				outFile<<percentageOfColor[i]<<' ';
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