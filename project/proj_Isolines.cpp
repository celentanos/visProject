#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

#include <fantom/algorithm.hpp>
#include <fantom/fields.hpp>
#include <fantom/register.hpp>
#include <fantom/graphics.hpp>

#include "lib/conrec_cxx.hpp"

using namespace std;
using namespace fantom;
namespace
{
class IsolinesAlgorithm : public VisAlgorithm
{
    unique_ptr< Primitive > isolines;
public:
    struct Options : public VisAlgorithm::Options {
//    public:
        Options(Options::Control &control) : VisAlgorithm::Options(control) {
            add<TensorFieldInterpolated<2, Scalar>>("Field", "interpolated TensorField");    /// interpolierte Werte an den Grid-Punkten
        }
    };

    struct VisOutputs : public VisAlgorithm::VisOutputs {
        VisOutputs(VisOutputs::Control &control) : VisAlgorithm::VisOutputs( control ) {
            addGraphics( "Isolines" );
        }
    };

    IsolinesAlgorithm(InitData &data) : VisAlgorithm(data) {}

    virtual void execute(const Algorithm::Options &options, const volatile bool &abortFlag) override {
        isolines = getGraphics( "Isolines" ).makePrimitive();

        shared_ptr< const TensorFieldInterpolated<2, Scalar> > field = options.get< TensorFieldInterpolated<2, Scalar> >("Field");
        if( !field ) {
            debugLog() << "Input Field not set." << endl;
            return;
        }

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

        double **d = new double*[ny];
        for (size_t i = 0; i < ny; ++i)
            d[i] = new double[nx];
        for (int y = ny - 1; y >= 0; --y)
            for (size_t x = 0; x < nx; ++x)
                d[y][x] = field->values()[y * nx + x][0];
        debugLog() << "value1: " << field->values()[7120][0] << endl;

        double *jub = new double[nx];         //x - jub
        for (size_t i = 0; i < nx; ++i)
            grid->points()[i][0];
        debugLog() << "x1: " << grid->points()[0][0] << endl;

        double *iub = new double[ny];         //y - iub
        for (int i = ny - 1; i >= 0; --i)
            grid->points()[i * nx][1];
        debugLog() << "y1: " << grid->points()[(ny - 1) * nx][1] << endl;

        double z[1] = {10};

        /// Lines ##############################################################
        vector<Vector3> v(2);
//        v[0] = Point3(45, 6, 0);
//        v[1] = Point3(55, 16, 0);

        conrec(d, 0, ny - 1, 0, nx - 1, jub, iub, 1, z, v);
        debugLog() << "conrec: fertig" << endl;

        isolines->add(Primitive::LINES).setColor(Color(1, 0, 0)).setVertices(v);
        // Aufräumen -----------------------------------------------------------
        for (size_t i = 0; i < ny; ++i)
            delete d[i];
        delete jub;
        delete iub;
    }
};

AlgorithmRegister<IsolinesAlgorithm> dummy("Vis Project/Isolines", "Interpolate in a data set.");

} // namespace
