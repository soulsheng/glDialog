
#include "stdafx.h"
#include "read_xyzrgb.h"
#include <iostream>


void read_xyzrgb( const char*file_name, double* pBuffer, int size, float scale/* = 1*/, bool bTriple /*= false*/ )
{
	
	FILE *fp;
	
	if((fp=fopen(file_name, "rb")) == NULL) 
	{
		printf("请确认文件(%s)是否存在!\n", file_name);
		exit(1);
	}

#if 1

	double temp;
	while(1==fscanf(fp, "%lf", &temp)) 
	{
		*pBuffer++ = temp*scale;
	}

#else

	if( !bTriple )
	{
		//fread( pBuffer, sizeof(double), size, fp );
		for (int j=0;j<size/3;j++)
			fscanf( fp, "%lf%lf%lf", pBuffer+3*j, pBuffer+3*j+1, pBuffer+3*j+2);

	}
	else
	{
		double* pBufferTemp = new double[size];
		//fread( pBufferTemp, sizeof(double), size, fp );
		for (int j=0;j<size/3;j++)
			fscanf( fp, "%lf%lf%lf", pBufferTemp+3*j, pBufferTemp+3*j+1, pBufferTemp+3*j+2);

		for (int i=0;i<3;i++)
			for (int j=0;j<size/3;j++)
				pBuffer[j*3+i] = pBufferTemp[i*size/3+j];

		delete[] pBufferTemp;
	}
#endif

	fclose(fp);
}
