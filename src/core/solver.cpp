#include "solver.hpp"

#include <string>
#include <fstream>

#include <Eigen/Sparse>
#include <Eigen/Dense>


Solver::Solver() : poissonRatio(0.3), youngModulus(2000.0) {};

Solver::Solver(double _poissonRatio, double _youngModulus) : poissonRatio(_poissonRatio), youngModulus(_youngModulus) {};

Solver::Solver(const std::string & filename) : poissonRatio(0.3), youngModulus(2000.0)
{
    loadGeometry(filename);
};

Solver::Solver(const std::string & filename, double _poissonRatio, double _youngModulus) : poissonRatio(_poissonRatio), youngModulus(_youngModulus)
{
    loadGeometry(filename);
};

void Solver::solve() 
{
    Eigen::SimplicialLDLT<Eigen::SparseMatrix<double> > solver(globalK);

	displacements = solver.solve(F);
};

void Solver::loadGeometry(const std::string & filename)
{
    geometry.loadFromFile(filename);

    // Prepare matrix and vector
    const int nodesCount = geometry.getNodes().size();
    globalK.resize(2 * nodesCount, 2 * nodesCount);
    this->F.resize(2 * nodesCount);
    F.setZero();
}

void Solver::calcuateStiffnessMatrix()
{
	Eigen::Matrix3d D;
	D << 1.0,        	poissonRatio,	0.0,
         poissonRatio,	1.0,         	0.0,
         0.0,        	0.0,        	(1.0 - poissonRatio) / 2.0;

    D *= youngModulus / (1.0f - pow(poissonRatio, 2.0f));

    auto elements = geometry.getElements();
    std::vector<Eigen::Triplet<double>> globalTriplets;
    for (auto element: elements)
    {
        auto localTriplets = element->calculateStiffnessMatrix(D);
        // This is bad solution, but unfortunately the sparse matrix must be set in single call
        globalTriplets.insert(globalTriplets.end(), localTriplets.begin(), localTriplets.end());
    }

    globalK.setFromTriplets(globalTriplets.begin(), globalTriplets.end());


    // Desirable, but unsupported solution
    // for (int i = 0; i < elements.size(); ++i)
    // {
    //     auto localMatrix = elements[i]->calculateStiffnessMatrix(D);
    //     globalK.setFromTriplets(localMatrix.begin(), localMatrix.end(), [] (const double &a, const double &b) { return a+b; });
    // }

};

void Solver::applyLoad()
{
    std::vector<int> indicesToConstraint;

    auto boundaries = geometry.getBoundaries();

    for (auto boundary : boundaries)
    {
        for (auto it = boundary.nodes.begin(); it!=boundary.nodes.end(); ++it)
        {
            if (it->type == BoundaryNode::UX)
            {
                indicesToConstraint.push_back(2 * it->node + 0);
            }
            if (it->type == BoundaryNode::UY)
            {
                indicesToConstraint.push_back(2 * it->node + 1);
            }
        }
    }

    std::vector<Node> & nodes = geometry.getNodes();
    const auto &f_boundary = boundaries[2].nodes;
    for (int i=0; i<f_boundary.size()-1; ++i)
    {
        double l = geometry.getNode(f_boundary[i+1].node + geometry.getShift()).y - geometry.getNode(f_boundary[i].node + geometry.getShift()).y;
        double f = 1000000.0 * l;
        F(2 * f_boundary[i].node + 0)   += 0.5 * f;
        F(2 * f_boundary[i+1].node + 0) += 0.5 * f;
    }

	for (int k = 0; k < globalK.outerSize(); ++k)
	{
		for (Eigen::SparseMatrix<double>::InnerIterator it(globalK, k); it; ++it)
		{
			for (std::vector<int>::iterator idit = indicesToConstraint.begin(); idit != indicesToConstraint.end(); ++idit)
			{
                if (it.row() == *idit || it.col() == *idit)
                {
                    it.valueRef() = it.row() == it.col() ? 1.0 : 0.0;
                }
			}
		}
	}
};

void Solver::save(const std::string & filename)
{
    std::ofstream output;
    output.open(filename);

    if (!output.is_open())
        throw "File not found";

    const int shift = geometry.getShift();

    for (int i = 0; i < geometry.getNodes().size(); ++i)
    {
        output << i + shift << " " << displacements[2*i+0] << " " << displacements[2*i+1] << std::endl;
    }

    output.close();
}

void Solver::saveSigma(const std::string & filename)
{
    std::ofstream output;
    output.open(filename);

    if (!output.is_open())
        throw "File not found";

    auto sigmas = calculateStress();

    output << "Sx\tSy\tSxy\tS" << std::endl;
    for (int i = 0; i < sigmas.size(); ++i)
    {
        output << sigmas[i][0] << " " << sigmas[i][1] << " " << sigmas[i][2] << " " << sigmas[i][3]<< std::endl;
    }
    output.close();
}

std::vector<std::vector<double>> Solver::calculateStress()
{
    Eigen::Matrix3d D;
	D << 1.0,        	poissonRatio,	0.0,
         poissonRatio,	1.0,         	0.0,
         0.0,        	0.0,        	(1.0 - poissonRatio) / 2.0;

    D *= youngModulus / (1.0f - pow(poissonRatio, 2.0f));

    auto elements = geometry.getElements();

    std::vector<std::vector<double>> result(elements.size());


    for (int i=0; i<elements.size(); ++i)
    {
        std::vector<double> sigma = elements[i]->calculateStress(displacements, D);
        double sigma_mises = sqrt(sigma[0] * sigma[0] - sigma[0] * sigma[1] + sigma[1] * sigma[1] + 3.0f * sigma[2] * sigma[2]);
        sigma.push_back(sigma_mises);
        result[i] = sigma;
    }

    return result;
}

