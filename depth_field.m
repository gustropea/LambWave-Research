
%=========================================================================%
%This code computes the equivalent ocean depth to be used with shallow water models
%given global temperature data.
%
%Results can be ploted for the initail time step and the overall average
%depth field
%
%
%Latitude and longitude resoloution is 0.25 degrees
%Time step is one hour
%
%
%
%Written by Gus Tropea on Novenber 22, 2023
%           Computing Ph.d Student
%           Boise State University
%
%=========================================================================%

%=========================================================================%
%Show plots
%=========================================================================%

showplots = 1; % 0 for true 1 for false

%=========================================================================%
%Read in temperature data
%=========================================================================%

t2m = ncread("2m_temp.nc","t2m");
t100hpa = ncread("100hpa_temp.nc","t");

%=========================================================================%
%Calculate Depths
%=========================================================================%
[lon,lat,dt] = size(t2m);
depthfields = (t2m+t100hpa)./2;
avgdepthfield = zeros(lon,lat);

% Note: Here I am subsampling to only get the relavent data
for i=5:125 
    avgdepthfield = avgdepthfield+depthfields(:,:,i);
end
avgdepthfield = avgdepthfield./120;

%=========================================================================%
%Get the average global temperature over the time period
%=========================================================================%

T = sum(sum(avgdepthfield))/(lon*lat); %absolute temperature
disp(strcat("Global Average Temperature T = ",num2str(T)," k"));

%=========================================================================%
%Calculate the equivalent ocean depth
%=========================================================================%

lambda = 1.4; % ratio of specific heat of air corresponding to the range of atmospheric temperatures
R = 8314.36; % j kmol^(-1) k^(-1) universal gas constant
M = 28.966; % kg kmol^(−1) molecular mass for dry air
g = 9.81; % m s^(-2)
H = (lambda*R*T)/(M*g); % m equivalent ocean depth
H = H/1000; % km equivelent ocean depth

disp(strcat("Equivalent Ocean Depth: ",num2str(H)," km"));

%=========================================================================%
%Ploting
%=========================================================================%

if showplots == 0
    %Plot time zero
    figure;
    subplot(1,3,1);
    imagesc(t2m(:,:,5)');
    clim([150 340]);
    cb = colorbar;
    title(cb,"Kelvin")
    title("2m Temperature");
    xlabel("Degrees East");
    ylabel("Degrees North");
    subplot(1,3,2);
    imagesc(t100hpa(:,:,5)');
    clim([150 340]);
    cb = colorbar;
    title(cb,"Kelvin")
    title("100hpa Temperature");
    xlabel("Degrees East");
    ylabel("Degrees North");
    subplot(1,3,3);
    imagesc(depthfields(:,:,5)');
    clim([150 340]);
    cb = colorbar;
    title(cb,"Kelvin")
    title(["Depth Field", "(Average Temperature)"]);
    xlabel("Degrees East");
    ylabel("Degrees North");
    sgtitle("15 January 2022, 4:00 UTC");
    
    %Plot the average depth feild over 5 days
    figure;
    imagesc(avgdepthfield');
    clim([150 340]);
    cb = colorbar;
    title(cb,"Kelvin")
    xlabel("Degrees East");
    ylabel("Degrees North");
    title(["Average Depth Field","15 January 2022, 4:00 UTC - 20 January 2022, 4:00 UTC"]);
end