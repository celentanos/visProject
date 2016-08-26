#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

#include <fantom/algorithm.hpp>
#include <fantom/fields.hpp>
#include <fantom/register.hpp>
#include <fantom/graphics.hpp>

#include <string>
#include <iterator>
#include "lib/conrec_cxx.hpp"

using namespace std;
using namespace fantom;
namespace
{
class RainfallAlgorithm : public VisAlgorithm
{
    unique_ptr< Primitive > rain;
    unique_ptr< Primitive > info;

public:
    struct Options : public VisAlgorithm::Options {
//    public:
        Options(Options::Control &control) : VisAlgorithm::Options(control) {
            add<TensorFieldInterpolated<2, Scalar>>("Field", "interpolated TensorField");    /// interpolierte Werte an den Grid-Punkten
            add<string>("Levels", "Number os Isoline-levels", "0");
            add<double>("Width", "Isoline-Width", 0.1);
            add<double>("Length", "Isoline-Lenth multiplicator", 0.2);
        }
    };

    struct VisOutputs : public VisAlgorithm::VisOutputs {
        VisOutputs(VisOutputs::Control &control) : VisAlgorithm::VisOutputs( control ) {
            addGraphics( "Rainfall" );
            addGraphics( "Info" );
        }
    };

    RainfallAlgorithm(InitData &data) : VisAlgorithm(data) {}

    virtual void execute(const Algorithm::Options &options, const volatile bool &abortFlag) override {
        rain = getGraphics( "Rainfall" ).makePrimitive();
        info = getGraphics("Info").makePrimitive();

        shared_ptr< const TensorFieldInterpolated<2, Scalar> > field = options.get< TensorFieldInterpolated<2, Scalar> >("Field");
        if( !field ) {
            debugLog() << "Input Field not set." << endl;
            return;
        }

        const double width = options.get<double>("Width");
        const double length = options.get<double>("Length");

        /// Grid ###############################################################
        shared_ptr<const Grid<2> > grid = dynamic_pointer_cast<const Grid<2> >(field->domain());
        if( !grid ) {
            throw std::logic_error( "Wrong type of grid!" );
        }

        debugLog() << "cells: " << grid->numCells() << endl;
        debugLog() << "values: " << field->values().size() << endl;
        const size_t nx = grid->structuringDimensionExtent(0);
        const size_t ny = grid->structuringDimensionExtent(1);
        debugLog() << "nx: " << nx << endl;
        debugLog() << "ny: " << ny << endl;

        /// Lines ##############################################################
        vector<Vector3> v;
        Color color(0.4, 0.4, 1);
        for (size_t i = 0; i < nx * ny; ++i) {
//            v.push_back(Vector3(grid->points()[i][0], grid->points()[i][1], 0));
//            v.push_back(Vector3(grid->points()[i][0], grid->points()[i][1], field->values()[i][0] / 50));
            rain->addCylinder(
                Vector3(grid->points()[i][0], grid->points()[i][1], field->values()[i][0]*length / 2),
                Vector3(0, 0, field->values()[i][0]*length),
                width, color);
        }

        /// Info ###############################################################
        double min = 1000.0;
        double max = -1000.0;
        for (size_t i = 0; i < nx * ny; ++i) {
            double val = field->values()[i][0];
            if(min > val)
                min = val;
            if(max < val)
                max = val;
        }
        info->addTextLabel(Vector3(grid->points()[0][0] - 1, grid->points()[nx * ny - 1][1] - 2, 0),
                           "Rain in mm\nmin: " + to_string(min) + "\nmax: " + to_string(max),
                           20,
                           Color(0.5, 0.5, 1),
                           Primitive::TopRight,
                           Primitive::Serif,
                           Primitive::Regular,
                           false);
    }
};

AlgorithmRegister<RainfallAlgorithm> dummy("Vis Project/Rainfall", "Interpolate in a data set.");

} // namespace
