#include <iostream>


#include "solver.hpp"



int main(int argc, char * argv[])
{
    if (argc == 1)
    {
        std::cout << "Error: Mesh file not set";
        return 1;
    }
    double poissonRatio = 0.3;
    double youngModulus = 2.e11;
    if (argc > 2)
    {
        poissonRatio = std::stod(argv[2]);
    }
    if (argc > 3)
    {   

        youngModulus = std::stod(argv[3]);
    }

    Solver solver(poissonRatio, youngModulus);
    std::cout << "Loading mesh from " << argv[1] << " ..." << std::endl;
    try
    {
        solver.loadGeometry(argv[1]);
    }
    catch (...)
    {
        std::cout << "Error while loading mesh" << std::endl;
        return 1;
    }
    

    std::cout << "Stiffness matrix calculation ..." << std::endl;
    solver.calcuateStiffnessMatrix();


    std::cout << "Applying loads ..." << std::endl;;
    solver.applyLoad();

  
    std::cout << "Solving ..." << std::endl;
    solver.solve();

    auto stress = solver.calculateStress();
    auto max_stress = std::max_element(stress.begin(), stress.end(), 
                                       [](const std::vector<double> & first, const std::vector<double> & second) { return (first[3] < second[3]); });


    std::cout << "Max stresses: " << std::endl;
    std::cout << "Sx\tSy\tSxz\tS\t" <<std::endl;
    std::cout << (*max_stress)[0] << "\t" << (*max_stress)[1] << "\t" << (*max_stress)[2] << "\t" << (*max_stress)[3]<< std::endl;

    solver.save("result.txt");
    solver.saveSigma("stress.txt");

    return 0;
}