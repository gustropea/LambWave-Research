
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


//Global Variables
//==================================================================
#define nlat 721  // number of rows in data file
#define nlon 1440  // number of columns in data file

static int dataread = 1; // set to 0 once data has been readin
static double bathymetry[nlat][nlon]; //bathymetry data array
static double nbrs[4][4]; //Keep track of the known points
                                //[upper left[lat, lon, I-idx, J-idx],
                                // upper right[lat, lon, I-idx, J-idx],
                                // lower left[lat, lon, I-idx, J-idx],
                                // lower right[lat, lon, I-idx, J-idx]]
const double eps = 1.0e-15; //epsilon value
const double dv = 0.25; //grid spacing in text file

//==================================================================


//Methods and Functions
//==================================================================

void readData(){
    //Gus Tropea 9/17/24 - Method to read in data file
    //INPUTS:   None
    //OUTPUTS:  None

    FILE *bfp;
    bfp = fopen("data/bathymetry_grid.dat", "r");
    //If the file could not be read print error and exit
    if (bfp == NULL){
        printf("readdata (bathymetry_interp) : File handle is NULL\n");
        exit(0);
    }
    //Read the data in to the bathymetry array
    for (int i = 0; i < nlat; i++){
        for (int j = 0; j < nlon; j++){
            fread(&bathymetry[i][j],sizeof(double),1,bfp);
        }
    }
    fclose(bfp);
}
    

//Find the four closest known grid points
void getNeighbors(double lat, double lon){
    //Gus Tropea 9/17/24 - Method to find the four closest know grid points and store the data.
    //Values are in ranges of lat=[-90,90] and lon=[0,360]
    //INPUTS:   lat - type double for a value of latitude
    //          lon - type double for a value of a longitude
    //OUTPUTS:  None

    double left;
    double right;
    double upper;
    double lower;
    int upperidx;
    int loweridx;
    int leftidx;
    int rightidx;

    //Find closest points of latitude

    //Check lower latitude boundary and wrap
    if(lat < -89.75){
        lower = -90;
        upper = lat+dv;
    }
    //Check upper latitude boundary and wrap
    else if( lat > 89.75){
        lower = lat-dv;
        upper = -90;
    }
    else{
        lower = floor(lat / dv) * dv;
        upper = ceil(lat / dv) * dv;
    }

    //Find closest points of longitude

    //Check left longitude boundary and wrap
    if(lon < 0.25){
        left = 0;
        right = lon+dv;
    }
    //check right longitude boundary and wrap
    else if( lon > 359.75){
        left = lon-dv;
        right = 0;
    }
    else{
        left = floor(lon / dv) * dv;
        right = ceil(lon / dv) * dv;
    }
    
    //Find the array index for the closest points of latitude and longitude
    upperidx = floor((upper + 90)/dv);
    loweridx = floor((lower + 90)/dv);
    leftidx = floor(left/dv);
    rightidx = floor(right/dv);

    //Set the neighbor array values
    nbrs[0][0] = upper;
    nbrs[0][1] = left;
    nbrs[0][2] = (double) upperidx;
    nbrs[0][3] = (double) leftidx;
    nbrs[1][0] = upper;
    nbrs[1][1] = right;
    nbrs[1][2] = (double) upperidx;
    nbrs[1][3] = (double) rightidx;
    nbrs[2][0] = lower;
    nbrs[2][1] = left;
    nbrs[2][2] = (double) loweridx;
    nbrs[2][3] = (double) leftidx;
    nbrs[3][0] = lower;
    nbrs[3][1] = right;
    nbrs[3][2] = (double) loweridx;
    nbrs[3][3] = (double) rightidx;
}


double bathymetry_interp(double *lat_ptr, double *lon_ptr){
    //Gus Tropea 9/17/24 - Function to calculate the scaling bathymetry term for any given latitude and longitude based on climate data
    //INPUTS:   lat_ptr - pointer to a type double value of a given latitude
    //          lon_ptr - Pointer to a type double value of a given longitude
    //OUTPUTS:  depth - type double value of a bathymetry term

    double lat = *lat_ptr; //dereference pointer
    double lon  = *lon_ptr; //dereference pointer
    double depth = 0;

    //Check to see if the file has already been read
    if(dataread == 1){
        readData();
        dataread = 0;
    }

    //Check to see if exact value is known
    double checklat = fmod(lat, dv);
    double checklon = fmod(lon, dv);
    //known latitude and longitude return the bathymetry term
    if((checklat < eps) && (checklon < eps)){
        double fixedlat = floor(lat / dv) * dv;
        double fixedlon = floor(lon / dv) * dv;
        int latidx = floor((fixedlat + 90)/dv);
        int lonidx = floor(fixedlon/dv);
        depth = bathymetry[latidx][lonidx];
        return depth;
    }

    //known latitude average over the line of longitude and return bathymetry term
    else if(checklat < eps){
        double fixedlat = floor(lat / dv) * dv;
        double lon1 = floor(lon / dv) * dv;
        double lon2 = ceil(lon / dv) * dv;
        int latidx = floor((fixedlat + 90)/dv);
        int lon1idx = floor(lon1/dv);
        int lon2idx = floor(lon2/dv);
        depth = (bathymetry[latidx][lon1idx] + bathymetry[latidx][lon2idx]) / 2;
        return depth;
    }

    //known longitude average over the line of latitude and return bathymetry term
    else if(checklon < eps){
        double fixedlon = floor(lon / dv) * dv;
        double lat1 = floor(lat / dv) * dv;
        double lat2 = ceil(lat / dv) * dv;
        int lonidx = floor(fixedlon/dv);
        int lat1idx = floor(lat1+90/dv);
        int lat2idx = floor(lat2+90/dv);
        depth = (bathymetry[lat1idx][lonidx] + bathymetry[lat2idx][lonidx]) / 2;
        return depth;
    }

    //No know values 
    else{
        //Find known data
        getNeighbors(lat,lon);
    
        //Get relative height at known points
        double f11 = bathymetry[(int) nbrs[2][2]][(int) nbrs[2][3]]; //lower left
        double f12 = bathymetry[(int) nbrs[0][2]][(int) nbrs[0][3]];  //upper left
        double f21 = bathymetry[(int) nbrs[3][2]][(int) nbrs[3][3]];  //lower right
        double f22 = bathymetry[(int) nbrs[1][2]][(int) nbrs[1][3]];  //upper right

        //90 degrees (same lat different lon):
        //Exact value at this latitude is known. Take the average value at the closest two longitudes.
        if(nbrs[0][2] == nbrs[3][2]){
            return (f11+f21)/2;
        }
  
        //90 degrees (same lon different lat): 
        //Exact value at this longitude is known. Take the average value at the closest two latitudes.
        else if (nbrs[0][3] == nbrs[2][3]){
            return (f11+f12)/2;
        }

        //No known points
        //Bilinear interpolate
        else{
            double x1 = nbrs[2][1]; //lower left longitude
            double x2 = nbrs[3][1]; //lower right longitude
            double y1 = nbrs[2][0]; //lower left latitude
            double y2 = nbrs[0][0]; //upper left latitude

            double u = f11*((x2-lon)/(x2-x1)) + f21*((lon-x1)/(x2-x1));
            double v = f12*((x2-lon)/(x2-x1)) + f22*((lon-x1)/(x2-x1));
            depth = u*((y2-lat)/(y2-y1)) + v*((lat-y1)/(y2-y1));

            return depth;
        }
    }
}
//==================================================================
