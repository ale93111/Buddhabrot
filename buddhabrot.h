#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h> 
#include <complex>
#include <random>
#include <algorithm> // std::max
#include <vector>

#define PI 3.14159265

struct Buddhabrot
{
	int width, height; // window size
	float minX, maxX, minY, maxY; // complex plane boundaries
	float radius;
	int Ndim;
	
	std::vector<int> hitcount;
		
	int NiterationsR; //= 800; //12700; // R
	int NiterationsG; //= 200; //3700;  // G
	int NiterationsB; //= 50; //670;   // B

	int Niterations; //= std::max( NiterationsR, std::max( NiterationsG, NiterationsB ));
	int Miterations; // = 10;
	
	int Npoints; // = 50000000;

	void allocator(int w, int h, int d);
	float saturate(float x);
	float smoothstep(float a, float b, float x);
	int incrementcount(float x, float y, int countr);
	float Finddublmax(int k,int w, int h);
	void Normalizehitcount();
	void fill();
	
	Buddhabrot(){}
	
	Buddhabrot(int Np, int w, int h, int NR=800, int NG=200, int NB=50) : Ndim(3), radius(4.0f), minX(-2.0f), maxX(2.0f), minY(-2.0f), maxY(2.0f)
	{
		Npoints 	 = Np;
		width   	 = w;
		height 		 = h;
		
		NiterationsR = NR;
		NiterationsG = NG;
		NiterationsB = NB;
		Niterations  = std::max( NiterationsR, std::max( NiterationsG, NiterationsB ));
		Miterations  = 0;
		
		allocator(width, height, Ndim);
		
	}	
	
	
};

void Buddhabrot::allocator(int w, int h, int d)
{
	hitcount.resize( w*h*d, 0);
}

float Buddhabrot::saturate(float x)
{
  return std::max(0.0f, std::min(1.0f, x));
}

float Buddhabrot::smoothstep(float a, float b, float x)
{
    float t = saturate((x - a)/(b - a));
    return t*t*(3.0 - (2.0*t));
}

int Buddhabrot::incrementcount(float x, float y, int countr)
{
	if(x || y)
	{
		int intu=0;
		int intv=0;

		intu=(int)(((x-minX)/(maxX-minX))*width );
		intv=(int)(((y-minY)/(maxY-minY))*height);
	
		
		if( countr < NiterationsB ) hitcount[2 + Ndim*(intv + height*intu)]++;
		else if( countr < NiterationsG ) hitcount[1 + Ndim*(intv + height*intu)]++;
		else if( countr < NiterationsR ) hitcount[0 + Ndim*(intv + height*intu)]++;
	}
		
	return countr;	
}

float Buddhabrot::Finddublmax(int k,int w, int h)
{
	float dmax=0.0;

	for(int i=0; i<w; i++)
	{
		for(int j=0; j<h; j++)
		{
			if(hitcount[k + Ndim*(j + height*i)]>dmax) dmax=hitcount[k + Ndim*(j + height*i)]+1;
		}
	} 
	//std::cout<<"dublmax="<<dmax<<std::endl;
		
	return dmax;
}


void Buddhabrot::Normalizehitcount() 
{
	float dublmax[Ndim];
	
	//width and height sono necessari o reshape non funziona
	for(int k=0; k<Ndim; k++) dublmax[k] = Finddublmax(k,width,height); 

	
	for(int i=0; i<width; i++)
	{
		for(int j=0; j<height; j++)
		{
			for(int k=0; k<Ndim; k++)
			{
				hitcount[k + Ndim*(j + height*i)] = 65535*smoothstep(0, sqrt(dublmax[k]), sqrt(hitcount[k + Ndim*(j + height*i)]));			
			}		
		}
	} 
}

					
void Buddhabrot::fill(/* float Zinitialx, float Zinitialy, float AngleRot*/)
{
	float x, y;
	int i;
	std::complex<float> z, c, oldz;

	
	//inizialize hitcount
	for(int i=0; i<width; i++)
	{
		for(int j=0; j<height; j++)
		{
			for(int k=0; k<Ndim; k++)
				hitcount[k + Ndim*(j + height*i)]=0;
		}
	}
	
	//random number generator: Mersenne Twister
	std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-2, 2);
	
	bool IsItPeriodic=false;
	
	float storePointsRE[Niterations];
	float storePointsIM[Niterations];
	
	float storePointsZR[Niterations];
	float storePointsZI[Niterations];
	float storePointsCR[Niterations];
	float storePointsCI[Niterations];
	
	for(int k=0; k<Npoints; k++)  //fill hitcount using N random points
	{
		IsItPeriodic=false;
		x=dis(gen); //random number between -2 and 2
		y=dis(gen);
		
		i = 0;
		c.real(x);
		c.imag(y);
	
		//are we inside the main cardiod?
		if( std::norm(c)*(8*std::norm(c)-3) <3/32.-std::real(c) ) continue;
		//if(x*x*x*x + 2*x*x*y*y - 0.375*x*x + 0.125*x + y*y*y*y - 0.125*y*y - 0.0117188 < 0.25*y*y) continue;
		//are we inside the 2nd order period bulb?
		if( x*x+2*x+y*y < -15/16. ) continue;
		//smaller bulb left of the period-2 bulb
		if((x+1.309)*(x+1.309) + y*y < 0.00345) continue;
		// smaller bulb top of the main cardioid
		if((x+0.125)*(x+0.125) + (y+0.744)*(y+0.744) < 0.0088) continue;
		// smaller bulb bottom of the main cardioid
		if((x+0.125)*(x+0.125) + (y-0.744)*(y-0.744) < 0.0088) continue;
		
		z = 0;
		//z.real(dis(gen));
		//z.imag(dis(gen));
		//z.real(Zinitialx);
		//z.imag(Zinitialy);
		oldz = 0;
		
		// iteration of f(z)=z*z+c, z and c complex numbers
		while ((std::real(z)*std::real(z)+std::imag(z)*std::imag(z)) < radius && i < Niterations)
		{
			storePointsZR[i] = std::real(z);
			storePointsZI[i] = std::imag(z);
			storePointsCR[i] = std::real(c);
			storePointsCI[i] = std::imag(c);
			
			//(i is a power of 2?)
			if( (i != 0) && ((i & (i - 1)) == 0))  oldz=z; //brent's method 

			z = z*z + c;
			i++;
			if( i != 0 && z == oldz) 
			{
				IsItPeriodic=true;
				break;
			}
		}
		
		if (IsItPeriodic) continue;
		
		if (i < Niterations && i > Miterations)
		{
			for(int j=0; j< i-1; j++)
			{
				storePointsRE[j] = storePointsZR[j];
				storePointsIM[j] = storePointsZI[j];
				//storePointsRE[j] = std::cos( AngleRot * PI / 180.0 )*storePointsZR[j]
				//				  -std::sin( AngleRot * PI / 180.0 )*storePointsZI[j];
				//storePointsIM[j] = std::sin( AngleRot * PI / 180.0 )*storePointsZI[j]
				//				  +std::cos( AngleRot * PI / 180.0 )*storePointsCI[j];
								  
				incrementcount(storePointsRE[j],  storePointsIM[j], j);
				incrementcount(storePointsRE[j], -storePointsIM[j], j); //y-axis symmetry
			}
		}
	}
	
	//Normalizehitcount();
}
