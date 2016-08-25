#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

#include <fantom/algorithm.hpp>
#include <fantom/fields.hpp>
#include <fantom/register.hpp>

#include "alglib/src/interpolation.h"

using namespace fantom;
namespace
{
/**
* Interpolates scattered data values on another set of points
*/
class InterpolateGeoDataAlgorithm : public DataAlgorithm
{
public:
    struct Options : public DataAlgorithm::Options {
    public:
        Options(Algorithm::Options::Control &control) : DataAlgorithm::Options(control) {
            add<DiscreteDomain<2>>("Points", "A point set or grid");
            add<TensorFieldDiscrete<Scalar>>("Field", "A scattered scalar field");
            add<Grid<2>>("Grid2D", "Grid where to interpolate");
        }
    };

    struct DataOutputs : public DataAlgorithm::DataOutputs {
    public:
        DataOutputs(Control &control) : DataAlgorithm::DataOutputs(control) {
//            add<TensorFieldBase>("Interpolated");
//            add<DiscreteDomain<2>>("Interpolated Domain");
            add<TensorFieldInterpolated<2, Scalar>>("interpolated Domain");     /// interpolierte Werte an den Grid-Punkten
            add<Grid<2>>("Grid 2D");
        }
    };

    InterpolateGeoDataAlgorithm(InitData &data) : DataAlgorithm(data) {}

    void execute(const Algorithm::Options &parameters, const volatile bool &abortFlag) {
        // nötige Strukturen ---------------------------------------------------
        auto points = parameters.get<DiscreteDomain<2>>("Points");              /// Werte-Koordinaten
        auto valueField = parameters.get<TensorFieldDiscrete<Scalar>>("Field"); /// Werte
        auto grid = parameters.get<Grid<2>>("Grid2D");                          /// generiertes Grid

        if (!grid || !points || !valueField) {
            debugLog() << "Field or domain not connected!";
            return;
        }

        alglib::idwinterpolant idw;
        alglib::real_2d_array pos;
        pos.setlength(points->numPoints(), 3);

        for (size_t i = 0; i < points->numPoints(); ++i) {
            pos(i, 0) = points->points()[i][0];
            pos(i, 1) = points->points()[i][1];
            pos(i, 2) = valueField->values()[i].operator[](0);
        }

//        alglib::idwbuildmodifiedshepard(pos, pos.rows(), 2, 1, 15, 25, idw);
        alglib::idwbuildmodifiedshepardr(pos, pos.rows(), 2, 4, idw);

        alglib::real_1d_array xy;
        xy.setlength(2);
        std::vector<Scalar> values;
        for (int i = 0; i < grid->numPoints(); ++i) {
            xy(0) = grid->points()[i].operator[](0);
            xy(1) = grid->points()[i].operator[](1);
            values.push_back(Scalar(alglib::idwcalc(idw, xy)));
        }

        auto tensorFieldInterpolated = DomainFactory::makeTensorField(*grid, values, Precision::FLOAT64, DomainType::points);

        // test ----------------------------------------------------------------

        std::shared_ptr< const Grid< 2 > > grid2 = std::dynamic_pointer_cast< const Grid< 2 > >( tensorFieldInterpolated->domain() );
        setResult("Grid 2D", grid2);

        const ValueArray< Point2 > &points2d = grid->points();
        Cell cell = grid->cell(0);
//        Point2 point2d;
        for (size_t i = 0; i < cell.numVertices(); ++i) {
            debugLog() << points2d[cell.index(i)] << std::endl;
        }
//        Grid<2> g = interpolatedValueDomain->domain();
        const std::shared_ptr<const TensorFieldInterpolated<2, Scalar>> bla = DomainFactory::makeTensorField(*grid, values, Precision::FLOAT64, DomainType::points);
        debugLog() << bla->makeDiscreteEvaluator()->value(0) << std::endl;

//        for (int i = 0; i < 100; ++i) {
//            debugLog() << i << "\t" << interpolatedValueDomain->values(i) << std::endl;
//        }
        // ---------------------------------------------------------------------

        // Strukturen nach Außen freigeben -------------------------------------
        setResult("interpolated Domain", tensorFieldInterpolated);
    }
};

AlgorithmRegister<InterpolateGeoDataAlgorithm> dummy("Vis Project/Interpolate Shepard alglib", "Interpolate in a data set.");

} // namespace
