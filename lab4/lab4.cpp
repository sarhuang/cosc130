/*	Name: Sarah Huang
 *	Date: 2/25/2021
 *	Program: lab4.cpp (BMP Lab)
 *	Purpose: Read in a bitmap file and write a new one after inverting the colors and flipping the input.
 */

#include <cstdint>
#include <cstdio>
using namespace std;

#pragma pack(push, 2)

struct BitmapFileHeader{
	uint16_t type;			//The type of file --> bitmap in this case
	uint32_t size;			//Size of the entire file (including headers)
	uint16_t reserved1;		//Reserved for future expansion
	uint16_t reserved2;		//Also reserved for future expansion
	uint32_t offset;		//Tells you offset from top of file
};

struct BitmapInfoHeader{ 
	uint32_t size = 40;			//Size of info header
	uint32_t width;				//Width of image in pixels
	uint32_t height;			//Height of image in pixels
	uint16_t planes = 1;		//Number of layers
	uint16_t bitCount = 24;		//Number of bits per pixel	
	uint32_t compression = 0;	//Compression used (but there is none)
	uint32_t imageSize = 0;		//Size of image
	uint32_t xPixelsPerMeter;	//Number of pixels per meter horizontally
	uint32_t yPixelsPerMeter;	//Number of pixels per meter vertically	
	uint32_t colorUsed = 0;		//Color index used (but we're not using this)
	uint32_t colorImportant = 0;//Not using this for this lab

};

struct Pixel{	
	//Stores color channels
	uint8_t blue;
	uint8_t green;
	uint8_t red;
};

#pragma pack(pop)



int main(int argc, char **argv){
	FILE *fl;
	BitmapFileHeader fheader;
	BitmapInfoHeader iheader;
	int pix_pad = 0;


	//Error checking - making sure we can open the file
	//Thank you TA Gregory from 8 am for giving me wisdom
	if(argc != 3){
		printf("Usage: %s <inputfile> <outputfile>\n", argv[0]);
	}
	
	fl = fopen(argv[1], "rb");
	
	if(!fl){
		perror("Can't open file!\n");
		return 1;
	}

	
	//Read in both headers
	//Thanks to TA Gregory!
	fread(&fheader, 1, sizeof(fheader), fl);
	fread(&iheader, 1, sizeof(iheader), fl);
	

	//Reads in the max number of bytes in file
	fseek(fl, 0, SEEK_END);
	int maxBytes = ftell(fl);


	//Create pixel pointer array
	//p.s. I think the size should be (width * height) but I get a core dump without sizeof(Pixel) so...
	Pixel *pix;
	pix = new Pixel[iheader.width * iheader.height * sizeof(Pixel)];
	

	//Calculating padding
	int byte_width = iheader.width * sizeof(Pixel);	//width of a row in bytes
	int remainder = byte_width % 4;
	if(remainder != 0){
		pix_pad = 4 - remainder;	//pix_pad = 0, 1, 2, or 3 padded zeroes
	}
	

	//Reading the pixel data
	fseek(fl, fheader.offset, SEEK_SET);	//Read in the first row of pixels and add padding
	fread(pix, 1, byte_width, fl);
	fseek(fl, pix_pad, SEEK_CUR);
	
	//Calculate the current row of pixels using the file position (+1 because we already read in a row)
	unsigned int curRow = ftell(fl)/(sizeof(Pixel) * iheader.width) + 1; 
	
	//Continue to read in pixel rows until we have read all the remaining, complete rows
	for(unsigned int i = curRow; i < iheader.height; ++i){
		fread(pix + (ftell(fl)/sizeof(Pixel)), 1, byte_width, fl);
		//printf("Row number%lu\n", ftell(fl)/sizeof(Pixel));
	}
	
	//Read in the remaining bytes of data that are not a full row
	fread(pix + (ftell(fl)/sizeof(Pixel)), 1, maxBytes - ftell(fl), fl);
	//printf("final row%lu\n", ftell(fl)/sizeof(Pixel));
	
	fclose(fl);
	

/*
	//Inverting colors
	for(unsigned int px = 0; px < (iheader.width * iheader.height * sizeof(Pixel)); ++px){	
		pix[px].blue = 255 - pix[px].blue;		//255 is the max value of a pixel
		pix[px].green = 255 - pix[px].green;
		pix[px].red = 255 - pix[px].red;
	}
*/


	//Flipping the image
	
	Pixel temp;
/*	for(unsigned int row = 0; row < iheader.height; ++row){
		for(unsigned int i = 0; i <= ((iheader.width - 1) / 2); ++i){
			temp = pix[i + (row * (iheader.width))];
			pix[i + (row * (iheader.width))] = pix[((iheader.width - 1) - i) + (row * (iheader.width))];
			pix[((iheader.width - 1) - i) + (row * (iheader.width))] = temp;
		}
	}*/

	
	for(unsigned int i = 0; i < iheader.width/2; i++){
		temp = pix[i];
		pix[i] = pix[iheader.width - 1 - i];
		pix[iheader.width - 1 - i] = temp;
	}

	for(unsigned int i = 0; i < iheader.width/2; i++){
		temp = pix[i + (iheader.width + 18)];
		pix[i + (iheader.width + 18)] = pix[(iheader.width - 1 - i) + (iheader.width + 18)];
		pix[(iheader.width - 1 - i) + (iheader.width + 18)] = temp; 
	}

	for(unsigned int i = 0; i < iheader.width/2; i++){
		temp = pix[i + (iheader.width + 301)];
		pix[i + (iheader.width + 301)] = pix[iheader.width - 1 - i + (iheader.width + 301)];
		pix[(iheader.width - 1 - i) + (iheader.width + 301)] = temp;
	}





	//Open new file and write the headers
	fl = fopen(argv[2], "wb");
	fwrite(&fheader, 1, sizeof(fheader), fl);
	fwrite(&iheader, 1, sizeof(iheader), fl);


	//Writing the file - exact same process as reading
	fseek(fl, fheader.offset, SEEK_SET);
	fwrite(pix, 1, byte_width, fl);
	fseek(fl, pix_pad, SEEK_CUR);

	curRow = ftell(fl) / (sizeof(Pixel) * iheader.width) + 1;
	
	for(unsigned int i = curRow; i < iheader.height; ++i){
		fwrite(pix + (ftell(fl)/sizeof(Pixel)), 1, byte_width, fl);
	}
	
	fwrite(pix + (ftell(fl)/sizeof(Pixel)), 1, maxBytes - ftell(fl), fl);
	

	fclose(fl);
	delete[] pix;
	return 0;
}	
