

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "bathymetry_interp.c"

//Global Variables
//==================================================================
#define minlat -90  // minimum latitude
#define maxlat 90  // maximum latitude
#define minlon 0  // minimum longitude
#define maxlon 360 // maximum longitude

int nplat; //number of points in the y-direction
int nplon; //number of points in the x-direction
double dy; //step size in the y-direction
double dx; //step size in the x-direction
FILE* fout = NULL;

//=================================================================

// Methods and Functions
//=================================================================

int main(int argc, char* argv[]){
  //Gus Tropea 9/17/24 - Function main program
  //INPUTS: argc - expects 2 inputs
  //        argv - number of points of latitude and number of points of longitude i.e. "720 1440"
  //OUTPUTS: binary file of bathymetry terms

  //Check to see if there are enough arguements given         
  if(argc < 3){
    printf("%s \n","Too few arguments");
    exit(0);
  }
  else{
    //time the program
    clock_t tic = clock();
    
    //read in inputs
    nplat = atoi(argv[1]);
    nplon = atoi(argv[2]);
    
    //calculate step size
    dy = fabs(maxlat-minlat)/nplat;
    dx = fabs(maxlon-minlon)/nplon;

    printf("==============================================================\n");
    printf("Desired Grid Resolution: \n");
    printf("--------------------------------------------------------------\n");
    printf("Degrees of Latitude: %f,\tDegrees of Longitude: %f\n",dy,dx);
    printf("--------------------------------------------------------------\n");
    printf("\t Calculating bathymetry terms, and writing to a file...\n");

    //Open the file
    FILE *fout = fopen("data/demo_bathymetry.dat","wb");

    //Write the grid size to the file
    fwrite(&nplat,sizeof(int),1,fout);    
    fwrite(&nplon,sizeof(int),1,fout);

    //Write the bathymetry data to the file
    for(int i=0;i<nplat;i++){
      for(int j=0;j<nplon;j++){
        double y = minlat+(dy*i);
        double x = minlon+(dx*j);
        double b = bathymetry_interp(&y,&x);
        fwrite(&b,sizeof(double),1,fout);
      }
    }

    //Close the file
    fclose(fout);

    //Stop the clock
    clock_t toc = clock();

    printf("\t Elapsed Time: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
    printf("==============================================================\n");
  }
  return 0;
}
