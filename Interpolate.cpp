#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

#include <fantom/algorithm.hpp>
#include <fantom/fields.hpp>
#include <fantom/register.hpp>

#define NUM_P   "p"

using namespace fantom;
namespace
{
/**
* Interpolates scattered data values on another set of points
*/
class InterpolateGeoDataAlgorithm
    : public DataAlgorithm
{
public:
    struct Options : public DataAlgorithm::Options {
    public:
        Options(Algorithm::Options::Control &control) : DataAlgorithm::Options(control) {
            add<DiscreteDomain<2>>("Points", "A point set or grid");
            add<TensorFieldDiscrete<Scalar> >("Field", "A scattered scalar field");
            add<Grid<2>>("InterpolationPoints", "Grid where to interpolate");
            add<double>(NUM_P, "Exponent for interpolation", 2.0);                // für Shepard-Interpolation
        }
    };

    struct DataOutputs : public DataAlgorithm::DataOutputs {
    public:
        DataOutputs(Control &control) : DataAlgorithm::DataOutputs(control) {
            add<TensorFieldBase>("Interpolated");
            add<DiscreteDomain<2>>("InterpolatedDomain");
        }
    };

    InterpolateGeoDataAlgorithm(InitData &data) : DataAlgorithm(data) {}

    void execute(const Algorithm::Options &parameters, const volatile bool &abortFlag) {
        // get the point set
        auto domain = parameters.get<DiscreteDomain<2>>("Points");

        // get the tensor field
        auto field = parameters.get<TensorFieldDiscrete<Scalar> >("Field");
        auto gridDomain = parameters.get<Grid<2>>("InterpolationPoints");
        auto p = parameters.get<double>(NUM_P);

        if (!gridDomain || !domain || !field) {
            debugLog() << "Field or domain not connected!";
            return;
        }

        std::vector<Scalar> tempValues;
        std::vector<Point2> tempPoints;

        std::vector<Scalar> gridInterpolatedTemps;
        std::vector<Point2> gridPoints;

        // add temperatures from field to standard vector
        for (size_t i = 0; i < field->values().size(); ++i)
            tempValues.push_back(Scalar(field->values()[i]));

        // add positions from discrete domain to standard vector
        for (size_t i = 0; i < domain->numPoints(); ++i)
            tempPoints.push_back(domain->points()[i]);

        // interpolate grid values
        for (size_t i = 0; i < gridDomain->numPoints(); ++i) {
            gridPoints.push_back(gridDomain->points()[i]);

            double val = shepardInterp(gridDomain->points()[i], tempPoints, tempValues, p);
            gridInterpolatedTemps.push_back(Scalar(val));
        }

        // set results
        auto interPDomain = DomainFactory::makeDomainArbitrary(gridPoints);
        auto interpolatedField = DomainFactory::makeTensorField(*interPDomain, gridInterpolatedTemps);

        setResult("InterpolatedDomain", interPDomain);
        setResult("Interpolated", interpolatedField);
    }

private:
    // Gewichtberechnung -------------------------------------------------------
    double calcWeight(Point2 point1, Point2 point2, double p) {
        return (1.0 / ( pow(distanceBetween(point1, point2), p) ));
    }

    // Entfernung mit dem Satz des Pythagoras hergeleitet: https://de.serlo.org/mathe/deutschland/bayern/gymnasium/klasse-12/geraden-und-ebenen-im-raum/abstand-zweier-punkte-berechnen
    template<size_t N>
    double distanceBetween(Tensor<double, N> a, Tensor<double, N> b) {
        double quadraticSum = 0.0;

        for (ulong i = 0; i < N; ++i)
            quadraticSum += pow(a[i] - b[i], 2.0);

        return sqrt(quadraticSum);
    }

    double shepardInterp(Point2 gridPoint, std::vector<Point2> tempPoints, std::vector<Scalar> tempValues, double p) {
        double interpVal = 0;
        double totalWeights1 = 0;
        double totalWeights2 = 0;
        double localWeight = 0;

        // cache sum of weights
        for (size_t i = 0; i < tempPoints.size(); ++i) {
            Point2 tempPoint = tempPoints[i];

            if(distanceBetween(gridPoint, tempPoint) == 0)
                return tempValues[i][0];

            // calculate weight
            localWeight = calcWeight(gridPoint, tempPoint, p) * tempValues[i][0];
            totalWeights1 += localWeight;
            // calculate weight
            localWeight = calcWeight(gridPoint, tempPoint, p);
            totalWeights2 += localWeight;
        }
//        for (size_t i = 0; i < positions.size(); ++i) {
//            Point2 pointTemp = positions[i];
//            // calculate weight
//            localWeight = calcWeight(point, pointTemp, p);
//            totalWeights2 += localWeight;
//        }

        interpVal = totalWeights1 / totalWeights2;

        return interpVal;
    }
};

AlgorithmRegister<InterpolateGeoDataAlgorithm> dummy("Vis Praktikum/Interpolate/Shepard_1", "Interpolate in a data set.");

} // namespace
