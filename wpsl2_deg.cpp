#include<bits/stdc++.h>
#include<chrono>

using namespace std;
using namespace std::chrono;

std::random_device r;
std::seed_seq seed{r(), r(), r(), r(), r()};
std::mt19937 gen(seed);

void seed_generator() {
	auto now = system_clock::now();
	auto duration = now.time_since_epoch();
	auto seed_value = duration.count();
	gen.seed(seed_value);
}

// Defining a structure to represent a rectangle
struct Rectangle {
	double x; // x-coordinate of the rectangle's top-left corner
	double y; // y-coordinate of the rectangle's top-left corner
	double width; // Width of the rectangle
	double height; // Height of the rectangle
};

// Function to calculate the area of a rectangle
double calculateArea(const Rectangle& rectangle) {
	return rectangle.width * rectangle.height;
}

vector<Rectangle> lattice; //Vector to store rectangles
vector<double> intervals;
vector<double> neighbours;

string deg;
string deg_log;
string wpsl;
string sum_width_cube;

int iter=0;
int E=0;
double sum_width_power_cube=0.0;

void wpsl_build() {
	seed_generator();
	// Initializing the initial rectangle (unit square)
	Rectangle initialRectangle{0.0, 0.0, 1.0, 1.0};
	lattice.push_back(initialRectangle);

	for (int iteration = 0; iteration < iter; ++iteration) {

		vector<Rectangle> newRectangles; // Vector to store rectangles for the next iteration
		int selectedRectangleIndex;
		if (iteration == 0) {
			// Choose the initial square
			// No need to calculate area, as there is only one square
			selectedRectangleIndex = 0;
		} else {
			// Calculating the total area of all squares
			double totalArea = 0.0;
			for (int i = 0; i < lattice.size(); ++i) {
				totalArea += calculateArea(lattice[i]);
				//not needed
			}

			double interval_end=0.0;
			for (size_t i = 0; i < lattice.size(); ++i) {

				intervals.push_back(interval_end);
				interval_end += calculateArea(lattice[i]);
			}
			intervals.push_back(1.0);
			for(int i=0; i<intervals.size(); i++) {

				//cout<<"interval: "<<intervals[i]<<"  ";
			}
			//cout<<endl;
			
			uniform_real_distribution<double>dist(0.0,1.0);
			double areaThreshold = dist(gen); // Choosing a threshold from 0 to 1
            //cout<<"selected_area: "<<areaThreshold<<endl;
			
			// Choosing a rectangle based on its area
			for (int j = 0; j < intervals.size() - 1; ++j) {
				if (areaThreshold >= intervals[j] && areaThreshold < intervals[j + 1]) {
					selectedRectangleIndex = j;
					//cout<<"selected_index: "<<selectedRectangleIndex<<endl;
					break;
				}
			}

		}
		// Storing the selected rectangle in a variable
		Rectangle selectedRectangle = lattice[selectedRectangleIndex];
		// Removing the selected square from the lattice
		lattice.erase(lattice.begin() + selectedRectangleIndex);

		// Defining new coordinates using two random numbers
		uniform_real_distribution<double> distX(0.0, 1.0);
		uniform_real_distribution<double> distY(0.0, 1.0);
		double xCoord = distX(gen); // Random x-coordinate between 0 and 1
		double yCoord = distY(gen); // Random y-coordinate between 0 and 1
        
		// Printing the generated coordinates
        //cout << "xCoord: " << xCoord << ", yCoord: " << yCoord << endl;

		// Calculating new dimensions for the four smaller rectangles
		double newWidth1 = xCoord * selectedRectangle.width;
		double newHeight1 = yCoord * selectedRectangle.height;
		double newWidth2 = (1.0 - xCoord) * selectedRectangle.width;
		double newHeight2 = yCoord * selectedRectangle.height;
		double newWidth3 = xCoord * selectedRectangle.width;
		double newHeight3 = (1.0 - yCoord) * selectedRectangle.height;
		double newWidth4 = (1.0 - xCoord) * selectedRectangle.width;
		double newHeight4 = (1.0 - yCoord) * selectedRectangle.height;


		// Add the four new rectangles to the lattice for the next iteration
		newRectangles.push_back({selectedRectangle.x, selectedRectangle.y, newWidth1, newHeight1});
		newRectangles.push_back({selectedRectangle.x + newWidth1, selectedRectangle.y, newWidth2, newHeight2});
		newRectangles.push_back({selectedRectangle.x + newWidth1, selectedRectangle.y + newHeight1, newWidth4, newHeight4});
		newRectangles.push_back({selectedRectangle.x, selectedRectangle.y + newHeight1, newWidth3, newHeight3});

		// Erasing the selected rectangle if its width or height is less than or equal to zero
		//not needed
		if (lattice[selectedRectangleIndex].width <= 0 || lattice[selectedRectangleIndex].height <= 0) {
			lattice.erase(lattice.begin() + selectedRectangleIndex);
		}
		// Adding the new rectangles to the lattice
		lattice.insert(lattice.end(), newRectangles.begin(), newRectangles.end());

		vector<double>().swap(intervals);
		vector<Rectangle>().swap(newRectangles);
	}

}


bool approximately_equal(double a, double b, double epsilon = 0.00000001) {
	// Comparing the absolute difference to a fraction of the larger number
	double abs_diff = abs(a - b);
	//double maxVal = max(abs(a),abs(b));
	return abs_diff <= epsilon;
}

void first_neighbours() {

	ofstream file1(deg);
	ofstream file2(deg_log);
	ofstream file3(sum_width_cube);
	//ofstream file(wpsl);
    neighbours.resize(3*iter +1); // Initializing neighbours vector size, it is required 
    
	for(int en=0; en<E; en++) {
		wpsl_build();

		for(int i=0; i<lattice.size(); i++) {
			int count=0;
			const Rectangle& lattice_element_i = lattice[i];
			for(int j=0; j<lattice.size(); j++) {
				const Rectangle& lattice_element_j = lattice[j];
				if(j != i) {
					//if((lattice_element_i.x == lattice_element_j.x+lattice_element_j.width || lattice_element_j.x == lattice_element_i.x+lattice_element_i.width) && (
					if((approximately_equal(lattice_element_i.x, lattice_element_j.x+lattice_element_j.width) || approximately_equal(lattice_element_j.x,lattice_element_i.x+lattice_element_i.width)) && (
					            (lattice_element_j.y > lattice_element_i.y && lattice_element_j.y < lattice_element_i.y+lattice_element_i.height) ||
					            (lattice_element_j.y+lattice_element_j.height > lattice_element_i.y && lattice_element_j.y+lattice_element_j.height < lattice_element_i.y+lattice_element_i.height) ||
					            (lattice_element_i.y > lattice_element_j.y && lattice_element_i.y < lattice_element_j.y+lattice_element_j.height) ||
					            (lattice_element_i.y+lattice_element_i.height > lattice_element_j.y && lattice_element_i.y+lattice_element_i.height < lattice_element_j.y+lattice_element_j.height) ||
					            (lattice_element_i.y == lattice_element_j.y && lattice_element_i.y+lattice_element_i.height == lattice_element_j.y+lattice_element_j.height))) 
								{
						count++;

					}

					//else if((lattice_element_i.y == lattice_element_j.y+lattice_element_j.height || lattice_element_j.y == lattice_element_i.y+lattice_element_i.height) && (
					else if((approximately_equal(lattice_element_i.y,lattice_element_j.y+lattice_element_j.height) || approximately_equal(lattice_element_j.y,lattice_element_i.y+lattice_element_i.height)) && (
					            (lattice_element_j.x > lattice_element_i.x && lattice_element_j.x < lattice_element_i.x+lattice_element_i.width) ||
					            (lattice_element_j.x+lattice_element_j.width > lattice_element_i.x && lattice_element_j.x+lattice_element_j.width < lattice_element_i.x+lattice_element_i.width) ||
					            (lattice_element_i.x > lattice_element_j.x && lattice_element_i.x < lattice_element_j.x+lattice_element_j.width) ||
					            (lattice_element_i.x+lattice_element_i.width > lattice_element_j.x && lattice_element_i.x+lattice_element_i.width < lattice_element_j.x+lattice_element_j.width) ||
					            (lattice_element_i.x == lattice_element_j.x && lattice_element_i.x+lattice_element_i.width == lattice_element_j.x+lattice_element_j.width))) 
								{
						count++;

					}

					//else if((lattice_element_i.x == (lattice_element_j.x+lattice_element_j.width - 1) || lattice_element_j.x == (lattice_element_i.x+lattice_element_i.width - 1)) && (
					else if((approximately_equal(lattice_element_i.x,lattice_element_j.x+lattice_element_j.width - 1) || approximately_equal(lattice_element_j.x,lattice_element_i.x+lattice_element_i.width - 1)) && (
					            (lattice_element_j.y > lattice_element_i.y && lattice_element_j.y < lattice_element_i.y+lattice_element_i.height) ||
					            (lattice_element_j.y+lattice_element_j.height > lattice_element_i.y && lattice_element_j.y+lattice_element_j.height < lattice_element_i.y+lattice_element_i.height) ||
					            (lattice_element_i.y > lattice_element_j.y && lattice_element_i.y < lattice_element_j.y+lattice_element_j.height) ||
					            (lattice_element_i.y+lattice_element_i.height > lattice_element_j.y && lattice_element_i.y+lattice_element_i.height < lattice_element_j.y+lattice_element_j.height) ||
					            (lattice_element_i.y == lattice_element_j.y && lattice_element_i.y+lattice_element_i.height == lattice_element_j.y+lattice_element_j.height)
					            )) 
								{
						count++;

					}

					//else if((lattice_element_i.y == (lattice_element_j.y+lattice_element_j.height - 1)|| lattice_element_j.y == (lattice_element_i.y+lattice_element_i.height - 1)) && (
					else if((approximately_equal(lattice_element_i.y,lattice_element_j.y+lattice_element_j.height - 1) || approximately_equal(lattice_element_j.y,lattice_element_i.y+lattice_element_i.height - 1)) && (
					            (lattice_element_j.x > lattice_element_i.x && lattice_element_j.x < lattice_element_i.x+lattice_element_i.width) ||
					            (lattice_element_j.x+lattice_element_j.width > lattice_element_i.x && lattice_element_j.x+lattice_element_j.width < lattice_element_i.x+lattice_element_i.width) ||
					            (lattice_element_i.x > lattice_element_j.x && lattice_element_i.x < lattice_element_j.x+lattice_element_j.width) ||
					            (lattice_element_i.x+lattice_element_i.width > lattice_element_j.x && lattice_element_i.x+lattice_element_i.width < lattice_element_j.x+lattice_element_j.width) ||
					            (lattice_element_i.x == lattice_element_j.x && lattice_element_i.x+lattice_element_i.width == lattice_element_j.x+lattice_element_j.width)
					            )) 
								{
						count++;

					}

				}

			}
			neighbours[count]++;
		}
		
		
	for(int i=0; i<lattice.size(); i++) {
		const Rectangle& lattice_element_i = lattice[i];
		sum_width_power_cube += pow(lattice_element_i.width,3) /** pow(lattice_element_i.height,1)*/;
	}
		
		vector<Rectangle>().swap(lattice);
		cout<<"ensem "<<en<<" ok"<<endl;
	}
	
	//printing values
	for(int i=0; i<neighbours.size(); i++) {
		if(neighbours[i] > 0.0) {
			cout<<fixed<< i <<"  "<<(double)neighbours[i]/E<<endl;
			
			file1<<fixed<< i <<"  "<<(double)neighbours[i]/E<<endl;
			file2<<fixed<<setprecision(10)<<(double)log(i)<<"  "<<setprecision(10)<<(double)log((double)(neighbours[i]/(E*(3*(double)iter+1))))<<endl;
		}
	}
	
    cout<<"sum of width power cube: "<<sum_width_power_cube/E<<endl;
    file3<<"sum of width power cube: "<<sum_width_power_cube/E<<endl;
	/*for (const Rectangle& lattice_element : lattice) {
	    //cout << "x: " << lattice_element.x << ", y: " << lattice_element.y
	      //   << ", width: " << lattice_element.width << ", height: " << lattice_element.height << endl;

	    file << "set object rect from " << lattice_element.x <<"," << lattice_element.y << " to " << lattice_element.x + lattice_element.width  <<"," << lattice_element.y + lattice_element.height << endl;

	}
	    file << "set xr [0:1]" << endl;  //x range scaling
	    file << "set yr [0:1]" << endl;
	cout<<"done"<<endl;*/
	vector<double>().swap(neighbours);
}


int main(int argc, char* argv[]) {
	iter=atoi(argv[1]);
	E = atoi(argv[2]);

	deg="deg_wpsl2_iter"+to_string(iter)+"_en"+to_string(E)+".dat";
	deg_log="deg_log_wpsl_iter"+to_string(iter)+"_en"+to_string(E)+".dat";
	sum_width_cube="sum_width_cube_iter"+to_string(iter)+"_en"+to_string(E)+".dat";
	//wpsl="wpsl_iter"+to_string(iter)+".txt";

	first_neighbours();

	return 0;
}
