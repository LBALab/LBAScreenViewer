/*
------------------------[ LBA Screen Viewer Source ]-------------------------

Copyright (C) 2007 - Spatarel Dan Constantin [MGLC]

-------------------------------[ GNU License ]-------------------------------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

-----------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef WITHOUT_BORLAND_COMPILER
#include <vcl.h>
#include "frmBMP2FLA.h"
#endif //WITHOUT_BORLAND_COMPILER

#define BMP_HEAD 54
#define HEIGTH 200
#define WIDTH  320
#define LINE_SIZE 3 * WIDTH
#define BMP_SIZE LINE_SIZE * HEIGTH
#define PALETTE 256

struct TFLAsample2
{
   short int sampleNum;
   short int freq;
   short int repeat;
   char dummy;
   unsigned char x;
   unsigned char y;
};

typedef struct Point2{
	unsigned char B;
	unsigned char G;
	unsigned char R;
}Point2;

typedef struct Picture{
	Point2 P[HEIGTH][WIDTH];
	unsigned char p[HEIGTH][WIDTH];
}Picture;

typedef struct TPalette{
	Point2 P[PALETTE];
}TPalette;

class BMPtoFLA{
private:
	int frames;					//total number of frames
	int is_sample;				//shows if there must be a sample
	TFLAsample2 FLAsample;		//sample
	char *keyframes;			//list of keyframes
	char *fadeouts;				//list of fadeouts
	char *file;					//the prefix of the BMP filenames
	TPalette *Palette;			//the palette

	Picture *Data;				//raw data

	int ***Cube;					//data cubed
	unsigned char ***CubePalette;	//palette cubed

	int prel;					//true if prel_arg allocated memory
	Point2 Ptmp[WIDTH];			//temporary BMP line

	void print_usage(char* program_name){
		printf("bmp2fla - Convert a set of BMP images to an FLA movie to be played");
		printf(" on the Little Big Planet 1 or 2 game engines.\n");
		printf("\n");
		printf("usage:\n");
		printf("\t%s -n number_of_frames -p filename_prefix\n", program_name);
		printf("\n");
		printf("Optional parameters:\n");
		printf("\t-s\tsample_number frequency repeat x y\n");
		printf("\t-k\tlist_of_additional_keyframes\n");
		printf("\t-f\tlist_of_frames_with_fadeouts\n");
		printf("\n");
	}

	int prel_arg(int argc, char** argv){
		int i, j;
		int val;
		#ifndef WITHOUT_BORLAND_COMPILER
		frm_BMP2FLA->pgr_fla->Progress = 0;
		frm_BMP2FLA->bt_playfla->Enabled = False;
		#endif //WITHOUT_BORLAND_COMPILER

		if (argc == 1){
			print_usage(argv[0]);
		    return 1;
		}

		for(i = 1; i < argc; ++i){
			if (strcmp(argv[i], "-h") == 0){
			    print_usage(argv[0]);
			    return 1;
			}else if (strcmp(argv[i], "-n") == 0){
				++i;
				if (i < argc){
					prel = 1;
					frames = atoi(argv[i]);
					#ifndef WITHOUT_BORLAND_COMPILER
					frm_BMP2FLA->pgr_fla->MaxValue = frames;
					#endif //WITHOUT_BORLAND_COMPILER
					keyframes = (char*)malloc(frames * sizeof(char));
					fadeouts = (char*)malloc(frames * sizeof(char));
					Palette = (TPalette*)malloc(frames * sizeof(TPalette));
					for(j = 0; j < frames; ++j){
						keyframes[j] = 0;
						fadeouts[j] = 0;
					}
					keyframes[0] = 1;
				}else{
					#ifndef WITHOUT_BORLAND_COMPILER
					ShowMessage("Error: Number of frames expected!\n");
					#else
					printf("Error: Number of frames expected!\n");
					#endif //WITHOUT_BORLAND_COMPILER
					return 1;
				}
			}else if (strcmp(argv[i], "-p") == 0){
				++i;
				if (i < argc){
					file = argv[i];
				}else{
					#ifndef WITHOUT_BORLAND_COMPILER
					ShowMessage("Error: BMP prefix expected!\n");
					#else
					printf("Error: BMP prefix expected!\n");
					#endif //WITHOUT_BORLAND_COMPILER
					return 1;
				}
			}else if (strcmp(argv[i], "-s") == 0){
				is_sample = 1;
				++i;
				if (i < argc){
					FLAsample.sampleNum = atoi(argv[i]);
				}
				++i;
				if (i < argc){
					FLAsample.freq = atoi(argv[i]);
				}
				++i;
				if (i < argc){
					FLAsample.repeat = atoi(argv[i]);
				}
				++i;
				if (i < argc){
					FLAsample.x = atoi(argv[i]);
				}
				++i;
				if (i < argc){
					FLAsample.y = atoi(argv[i]);
				}else{
					#ifndef WITHOUT_BORLAND_COMPILER
					ShowMessage("Error: Sample details expected!\n");
					#else
					printf("Error: Sample details expected!\n");
					#endif //WITHOUT_BORLAND_COMPILER
					return 1;
				}
			}else if (strcmp(argv[i], "-k") == 0){
				for(++i; i < argc && argv[i][0] != '-'; ++i){
					val = atoi(argv[i]);
					keyframes[val] = 1;
				}
				--i;
			}else if (strcmp(argv[i], "-f") == 0){
				for(++i; i < argc && argv[i][0] != '-'; ++i){
					val = atoi(argv[i]);
					fadeouts[val] = 1;
				}
				--i;
			}else{
				#ifndef WITHOUT_BORLAND_COMPILER
				AnsiString error = "Error: Unexpected parameter (";
				error = error + i;
				error = error + ")!\n";
				ShowMessage(error);
				#else
				printf("Error: Unexpected parameter (%d)!\n", i);
				#endif //WITHOUT_BORLAND_COMPILER
				return 1;
			}
		}
		if (!file){
			#ifndef WITHOUT_BORLAND_COMPILER
			ShowMessage("Error: BMP prefix not specified!\n");
			#else
			printf("Error: BMP prefix not specified!\n");
			#endif //WITHOUT_BORLAND_COMPILER
			return 1;
		}
		if (!(frames > 0)){
			#ifndef WITHOUT_BORLAND_COMPILER
			ShowMessage("Error: Number of frames not specified of specified incorecty!\n");
			#else
			printf("Error: Number of frames not specified of specified incorecty!\n");
			#endif //WITHOUT_BORLAND_COMPILER
			return 1;
		}
		return 0;
	}

	void free_prel(){
		if (prel){
			prel = 0;
			free(keyframes);
			free(fadeouts);
			free(Palette);
		}
	}

	int LoadBMPs(){
		int i, j, k;
		FILE *f;
		char filename[260];
		char stream[54];
		Data = (Picture*)malloc(frames  * sizeof(Picture));
		for(i = 0; i < frames; ++i){
			sprintf(filename, "%s%d.bmp", file, i);
			f = fopen(filename, "rb");
			if (!f){
				#ifndef WITHOUT_BORLAND_COMPILER
				AnsiString error = "Error: ";
				error = error + filename;
				error = error + " does not exist!\n";
				ShowMessage(error);
				#else
				printf("Error: %s does not exist!\n", filename);
				#endif //WITHOUT_BORLAND_COMPILER
				free(Data);
				return 1;
			}
			if (fread(stream, 1, BMP_HEAD, f) != BMP_HEAD){
				#ifndef WITHOUT_BORLAND_COMPILER
				AnsiString error = "Error: ";
				error = error + filename;
				error = error + " is not a valid BMP file!\n";
				ShowMessage(error);
				#else
				printf("Error: %s is not a valid BMP file!\n", filename);
				#endif //WITHOUT_BORLAND_COMPILER
				free(Data);
				return 1;
			}
			if (fread(Data+i, 1, BMP_SIZE, f) != BMP_SIZE){
				#ifndef WITHOUT_BORLAND_COMPILER
				AnsiString error = "Error: ";
				error = error + filename;
				error = error + " does not have the right size!\n";
				ShowMessage(error);
				#else
				printf("Error: %s does not have the right size!\n", filename);
				#endif //WITHOUT_BORLAND_COMPILER
				free(Data);
				return 1;
			}
			fclose(f);
			for(j = 0, k = HEIGTH - 1; j < k; ++j, --k){
				memcpy(Ptmp, Data[i].P[j], LINE_SIZE);
				memcpy(Data[i].P[j], Data[i].P[k], LINE_SIZE);
				memcpy(Data[i].P[k], Ptmp, LINE_SIZE);
			}
		}
		return 0;
	}

	void InitCube(){
		int i, j, k;
		Cube = (int***)malloc(256 * sizeof(int**));
		CubePalette = (unsigned char***)malloc(256 * sizeof(unsigned char**));
		for(i = 0; i < 256; ++i){
			Cube[i] = (int**)malloc(256 * sizeof(int*));
			CubePalette[i] = (unsigned char**)malloc(256 * sizeof(unsigned char*));
			for(j = 0; j < 256; ++j){
				Cube[i][j] = (int*)malloc(256 * sizeof(int));
				CubePalette[i][j] = (unsigned char*)malloc(256 * sizeof(unsigned char));
				for(k = 0; k < 256; ++k){
					Cube[i][j][k] = 0;
					CubePalette[i][j][k] = 0;
				}
			}
		}
	}

	void FreeCube(){
		int i, j;
		for(i = 0; i < 256; ++i){
			for(j = 0; j < 256; ++j){
				free(Cube[i][j]);
				free(CubePalette[i][j]);
			}
			free(Cube[i]);
			free(CubePalette[i]);
		}
		free(Cube);
		free(CubePalette);
	}

	/*
	void DataToxyz(int start, int stop){
		int i, j, k;
		x.clear();
		y.clear();
		z.clear();
		for(i = start; i < stop; ++i){
			for(j = 0; j < HEIGTH; ++j){
				for(k = 0; k < WIDTH; ++k){
					x.push_back((double)Data[i].P[j][k].R);
					y.push_back((double)Data[i].P[j][k].G);
					z.push_back((double)Data[i].P[j][k].B);
				}
			}
		}
	}//*/

	void DataToCube(int start, int stop){
		int i, j, k;
		for(i = 0; i < 256; ++i){
			for(j = 0; j < 256; ++j){
				for(k = 0; k < 256; ++k){
					Cube[i][j][k] = 0;
				}
			}
		}
		for(i = start; i <= stop; ++i){
			for(j = 0; j < HEIGTH; ++j){
				for(k = 0; k < WIDTH; ++k){
					Cube[Data[i].P[j][k].R][Data[i].P[j][k].G][Data[i].P[j][k].B]++;
				}
			}
		}
	}

	/*
	unsigned char fct(double arg){
		arg = ceil(arg+0.5);
		if (arg < 0) arg = 0;
		if (arg > 255) arg = 255;
		return (unsigned char)arg;
	}

	void SolToPal(int start, int stop){
		char buffer[260];
		double tmp1, tmp2, tmp3;
		int i, j;
		for(i = 0; i < PALETTE; ++i){
			for(j = 0; sol[i][j]; ++j){
				buffer[j] = sol[i][j];
			}
			sscanf(buffer, "%lf %lf %lf", &tmp1, &tmp2, &tmp3);
			for(j = start; j <= stop; ++j){
				Palette[j].P[i].R = fct(tmp1);
				Palette[j].P[i].G = fct(tmp2);
				Palette[j].P[i].B = fct(tmp3);
			}
		}
	}//*/

	/*
	void SolToPal2(int start, int stop){
		int i, j;
		for(i = 0; i < PALETTE; ++i){
			for(j = start; j <= stop; ++j){
				Palette[j].P[i].R = Palette[start].P[i].R;
				Palette[j].P[i].G = Palette[start].P[i].G;
				Palette[j].P[i].B = Palette[start].P[i].B;
			}
		}
	}//*/

	void CalcPalCul(int start, int stop){
		int i, j, k, l, a, b, c;
		int dif, min, minp;
		for (a = 0; a < 256; a++){
			for (b = 0; b < 256; b++){
				for (c = 0; c < 256; c++) if (Cube[a][b][c]){
					min = 1000;
					minp = -1;
					for(l = 0; l < PALETTE; ++l){
						dif =   abs(a - Palette[start].P[l].R) +
								abs(b - Palette[start].P[l].G) +
								abs(c - Palette[start].P[l].B);
						if (min > dif){
							min = dif;
							minp = l;
						}
					}
					CubePalette[a][b][c] = minp;
				}
			}
		}
		for(i = start; i <= stop; ++i){
			for(j = 0; j < HEIGTH; ++j){
				for(k = 0; k < WIDTH; ++k){
					Data[i].p[j][k] = CubePalette[Data[i].P[j][k].R][Data[i].P[j][k].G][Data[i].P[j][k].B];
				}
			}
		}
	}

	TPalette ReCalcPalette(int*** Cube){
		TPalette sol;
		int a, b, c, i = 0;
		for (a = 0; a < 256; a++){
			for (b = 0; b < 256; b++){
				for (c = 0; c < 256; c++) if (Cube[a][b][c]){
					if (i < 256){
						sol.P[i].R = a;
						sol.P[i].G = b;
						sol.P[i].B = c;
						i++;
					}else{
						#ifndef WITHOUT_BORLAND_COMPILER
						ShowMessage("Error: BMPs contain more than 256 diffrent colors!\n");
						#else
						printf("Error: BMPs contain more than 256 diffrent colors!\n");
						#endif //WITHOUT_BORLAND_COMPILER
						return sol;
					}
				}
			}
		}
		return sol;
	}

	void CalcPalette(){
		int i, i0;
		for(i = 0; i < frames; ){
			for(i0 = i++; i < frames && keyframes[i] == 0; ++i); --i;
			//printf("<%d %d>\n", i0, i);
			
			DataToCube(i0, i);
			
			Palette[i0] = ReCalcPalette(Cube);
			//SolToPal2(i0, i);
			CalcPalCul(i0, i);
			
			/*
			DataToxyz(i0, i);
			sol = solver.place(x, y, z, PALETTE);
			SolToPal(i0, i);//*/
			

			++i;
		}
	}

	void WriteHeader(FILE *f){
		unsigned char uc;
		unsigned short int usi;
		int i;
		fprintf(f, "V1.3%c%c", 0, 0);
		i = frames + 1;				fwrite(&i,   1, 4, f);
		uc = 12;					fwrite(&uc,  1, 1, f);
		uc = 217;					fwrite(&uc,  1, 1, f);
		usi = 320;					fwrite(&usi, 1, 2, f);
		usi = 200;					fwrite(&usi, 1, 2, f);
		i = is_sample;				fwrite(&i,   1, 4, f);
		if (is_sample){
			i = FLAsample.sampleNum;	fwrite(&i,   1, 4, f);
		}
	}

	int GetCompressedLine(char *&p, int frame, int line){
		p = (char*)malloc(1000 * sizeof(char));
		int p0 = 1;
		unsigned char val;
		int i0;
		int i;
		int p00 = -1;
		p[0] = 0;
		for(i = 0; i < WIDTH; ){
			for(i0 = i, val = Data[frame].p[line][i]; i - i0 < 127 && i < WIDTH && val == Data[frame].p[line][i]; ++i);
			//printf("%d %d %d %d\n", i, i0, p0, p00);
			//fflush(stdout);
			if (i - i0 >= 3){
				p[0]++;
				p[p0++] = i - i0;
				p[p0++] = val;
				p00 = -1;
			}else{
				if (p00 == -1 || p[p00] - (i - i0) <= -128){
					p[0]++;
					p00 = p0++;
					p[p00] = -(i - i0);
				}else{
					p[p00] -= i - i0;
				}
				for(; i0 < i; ++i0){
					p[p0++] = val;
				}
			}
		}
		//printf("->%d\n", p0);
		//fflush(stdout);
		//for(i = 0; i < p0; ++i){
		//	printf("%d ", p[i]);
		//}
		//printf("\n");
		return p0;
	}

	int GetCompressed2Line(char *&p, int frame, int line){
		p = (char*)malloc(1000 * sizeof(char));
		unsigned char skip;
		int p0 = 1;
		unsigned char val;
		int i0;
		int i;
		int p00 = -1;
		p[0] = 0;
		int Este[320];
		for(i = 0; i < WIDTH; ++i){
			if (Data[frame].p[line][i] == Data[frame - 1].p[line][i]){
				Este[i] = 1;
			}else{
				Este[i] = 0;
			}
		}
		for(i = 0; i < WIDTH; ){
			for(; i < WIDTH && Este[i] == 0; ++i);
			for(i0 = i; i < WIDTH && Este[i] == 1; ++i);
			if (i - i0 < 2){
				for(; i0 < i; ++i0){
					Este[i0] = 0;
				}
			}
		}
		//for(i = 0; i < WIDTH; ++i){
		//	printf("%d ", Data[frame].p[line][i]);
		//}
		//printf("\n");
		//for(i = 0; i < WIDTH; ++i){
		//	printf("%d ", Data[frame - 1].p[line][i]);
		//}
		//printf("\n");
		for(i = 0; i < WIDTH; ){
			for(skip = 0; i < WIDTH && Este[i] && skip < 255; ++i, ++skip);
			if (i == WIDTH)
				break;
			for(i0 = i, val = Data[frame].p[line][i]; i - i0 < 127 && i < WIDTH && val == Data[frame].p[line][i]; ++i);
			//printf("%d %d %d %d\n", i, i0, p0, p00);
			//fflush(stdout);
			if (i - i0 >= 5){
				p[0]++;
				p[p0++] = skip;
					p[p0++] = -(i - i0);
				p[p0++] = val;
				p00 = -1;
			}else{
				if (skip != 0 || p00 == -1 || p[p00] + (i - i0) >= 128){
					p[0]++;
					p[p0++] = skip;
					p00 = p0++;
					p[p00] = i - i0;
				}else{
					p[p00] += i - i0;
				}
				for(; i0 < i; ++i0){
					p[p0++] = val;
				}
			}
		}
		//if (frame == 20 && line == 2){
		//	printf("->%d\n", p0);
		//	fflush(stdout);
		//	for(i = 0; i < p0; ++i){
		//		printf("%u ", (unsigned char)p[i]);
		//	}
		//	printf("\n");
		//	fflush(stdout);
		//}
		return p0;
	}

	void GetKeyFrame(char **framestr, int *framelen, unsigned char *frameop, int frame){
		unsigned char uc;
		unsigned short int usi;
		int i;
		char *p, *p1, *p2;
		char *q;
		int qlen;
		p = *framestr = (char*)malloc(100000 * sizeof(char));
		p2 = p1 = (char*)malloc(100000 * sizeof(char));
		*frameop = 2;
		//write sample, if necessary
		if (frame == 0 && is_sample == 1){
			//printf("sample\n");
			(*frameop)++;
			uc = 3;						memcpy(p, &uc,  1); p++;
			uc = 0;						memcpy(p, &uc,  1); p++;
			usi = sizeof(TFLAsample2);	memcpy(p, &usi, 2); p+=2;
			memcpy(p, &FLAsample, sizeof(TFLAsample2)); p+=sizeof(TFLAsample2);
		}
		//write palette
		uc = 1;				memcpy(p, &uc,  1); p++;
		uc = 0;				memcpy(p, &uc,  1); p++;
		usi = 3*PALETTE+4;	memcpy(p, &usi, 2); p+=2;
		usi = PALETTE;		memcpy(p, &usi, 2); p+=2;
		usi = 0;			memcpy(p, &usi, 2); p+=2;
		int j;
		for(j = 0; j < PALETTE; ++j){
			uc = Palette[frame].P[j].R;	memcpy(p, &uc,  1); p++;
			uc = Palette[frame].P[j].G;	memcpy(p, &uc,  1); p++;
			uc = Palette[frame].P[j].B;	memcpy(p, &uc,  1); p++;
		}
		//write the keyframe itself
		uc = 8;				memcpy(p, &uc,  1); p++;
		uc = 0;				memcpy(p, &uc,  1); p++;
		//usi = HEIGTH*324;	memcpy(p, &usi, 2); p+=2;
		usi = 0;
		for(i = 0; i < HEIGTH; ++i){
			qlen = GetCompressedLine(q, frame, i); //HEIGTH - i - 1);
			usi += qlen;
			memcpy(p2, q, qlen); p2+=qlen;
			free(q);
		}
		memcpy(p, &usi, 2); p+=2;
		memcpy(p, p1, usi); p+=usi;
		free(p1);
		*framelen = p - *framestr;
	}

	void GetDeltaFrame(char **framestr, int *framelen, unsigned char *frameop, int frame){
		unsigned char uc;
		unsigned short int usi;
		int i, j;
		short int skip, height;
		char *p, *p1, *p2;
		char *q;
		int qlen;
		p = *framestr = (char*)malloc(100000 * sizeof(char));
		p2 = p1 = (char*)malloc(100000 * sizeof(char));
		*frameop = 1;
		//write the deltaframe itself
		uc = 6;				memcpy(p, &uc,  1); p++;
		uc = 0;				memcpy(p, &uc,  1); p++;
		//calculating skip and height
		for(i = 0; i < HEIGTH; ++i){
			for(j = 0; j < WIDTH && Data[frame].p[i][j] == Data[frame - 1].p[i][j]; ++j);
			if (j < WIDTH)
				break;
		}
		skip = i;
		for(i = HEIGTH - 1; i >= skip; --i){
			for(j = 0; j < WIDTH && Data[frame].p[i][j] == Data[frame - 1].p[i][j]; ++j);
			if (j < WIDTH)
				break;
		}
		height = i - skip + 1;
		if (height == 0){
			height = 1;
			skip--;
		}
		//usi = HEIGTH*324;	memcpy(p, &usi, 2); p+=2;
		usi = 0;
		usi+=2;
		memcpy(p2, &skip, 2); p2+=2;
		usi+=2;
		memcpy(p2, &height, 2); p2+=2;
		for(i = skip; i < skip + height; ++i){
			qlen = GetCompressed2Line(q, frame, i);
			usi += qlen;
			memcpy(p2, q, qlen); p2+=qlen;
			free(q);
		}
		memcpy(p, &usi, 2); p+=2;
		memcpy(p, p1, usi); p+=usi;
		free(p1);
		*framelen = p - *framestr;
	}

	void WriteFrame(FILE *f, int frame){
		char *framestr;
		int framelen;
		unsigned char frameop;
		unsigned char uc;
		int i;
		if (keyframes[frame])
			GetKeyFrame(&framestr, &framelen, &frameop, frame);
		else
			GetDeltaFrame(&framestr, &framelen, &frameop, frame);
		uc = frameop;		fwrite(&uc,  1, 1, f);
		uc = 0;				fwrite(&uc,  1, 1, f);
		i = framelen;		fwrite(&i,   1, 4, f);
		fwrite(framestr, framelen, 1, f);
		free(framestr);
	}

	void WriteFLA(){
		int i;
		FILE *f;
		char filename[260];
		sprintf(filename, "%s.fla", file);
		f = fopen(filename, "wb");
		WriteHeader(f);
		for(i = 0; i < frames; ++i){
			WriteFrame(f, i);
			#ifndef WITHOUT_BORLAND_COMPILER
			frm_BMP2FLA->pgr_fla->Progress++;
			#endif //WITHOUT_BORLAND_COMPILER
		}
		fclose(f);
	}

	void FreeMem(){
		free_prel();
		free(Data);
		FreeCube();
	}
public:
	BMPtoFLA(int argc, char** argv){
		if (prel_arg(argc, argv)){
			free_prel();
			return;
		}
		if (LoadBMPs())
			return;
		InitCube();
		CalcPalette();
		WriteFLA();
		FreeMem();
		#ifndef WITHOUT_BORLAND_COMPILER
		frm_BMP2FLA->bt_playfla->Enabled = True;
		#endif //WITHOUT_BORLAND_COMPILER
	}
};

#ifdef WITHOUT_BORLAND_COMPILER
int main(int argc, char** argv){
	BMPtoFLA* b2f = new BMPtoFLA(argc, argv);
	free(b2f);
	return 0;
}
#endif
