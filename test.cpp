#include <time.h>
#include <iostream>
#include <fstream>
#include <omp.h>
using namespace std;

double mapToReal(int x, int imageWidth, double minR, double maxR)
{
	double range = maxR - minR;
	return x * (range / imageWidth) + minR;
}

double mapToImaginary(int y, int imageHeight, double minI, double maxI)
{
	double range = maxI - minI;
	return y * (range / imageHeight) + minI;
}

int findMandelbrot(double cr, double ci, int max_iterations)
{
	int i = 0;
	double zr = 0.0, zi = 0.0;
	while (i < max_iterations && zr * zr + zi * zi < 4.0)
	{
		double temp = zr * zr - zi * zi + cr;
		zi = 2.0 * zr * zi + ci;
		zr = temp;
		i++;
	}
	return i;
}

int main()
{

	clock_t start, end;

	/* Recording the starting clock tick.*/
	start = clock();

	ifstream fin("input.txt");
	int imageWidth, imageheight, maxN;
	double minR, maxR, minI, maxI;
	if (!fin)
	{
		cout << "Cannot open file" << endl;
		cin.ignore();
		return 0;
	}
	fin >> imageWidth >> imageheight >> maxN;
	fin >> minR >> maxR >> minI >> maxI;
	fin.close();

	ofstream fout("output_image.ppm");
	fout << "P3" << endl;
	fout << imageWidth << " " << imageheight << endl;
	fout << "256" << endl;

	int threads = 8;
	int size = imageheight / threads;

	cout << size << endl;
#pragma omp parallel for num_threads(threads)
	for (int y = omp_get_thread_num() * size; y < (omp_get_thread_num() + 1) * size; y++)
	{
		for (int x = 0; x < imageWidth; x++)
		{
			double cr = mapToReal(x, imageWidth, minR, maxR);
			double ci = mapToImaginary(y, size, minI, maxI);

			int n = findMandelbrot(cr, ci, maxN);

			int r = (n * 3 % 256);
			int g = (n * n % 256);
			int b = (n % 256);

			fout << r << " " << g << " " << b << " ";
		}
	}

	fout << endl;
	fout.close();
	// Recording the end clock tick.
	end = clock();

	// Calculating total time taken by the program.
	double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
	cout << "Time taken by program is : " << fixed
		 << time_taken;
	cout << " sec " << endl;

	return 0;
}