#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

#include <fantom/algorithm.hpp>
#include <fantom/fields.hpp>
#include <fantom/register.hpp>
#include <fantom/graphics.hpp>

#include <limits>
#include <string>
#include <iterator>
#include "lib/conrec_cxx.hpp"

using namespace std;
using namespace fantom;
namespace
{
class TextureAlgorithm : public VisAlgorithm
{
    unique_ptr< Primitive > isolines;
    unique_ptr< Primitive > info;

public:
    struct Options : public VisAlgorithm::Options {
//    public:
        Options(Options::Control &control) : VisAlgorithm::Options(control) {
            add<TensorFieldInterpolated<2, Scalar>>("Field", "interpolated TensorField");    /// interpolierte Werte an den Grid-Punkten
            add<string>("Levels", "Number os Isoline-levels", "0");
            add<double>("Width", "Isoline-Width", 2);
        }
    };

    struct VisOutputs : public VisAlgorithm::VisOutputs {
        VisOutputs(VisOutputs::Control &control) : VisAlgorithm::VisOutputs( control ) {
            addGraphics( "Isolines" );
            addGraphics( "Info" );
        }
    };

    TextureAlgorithm(InitData &data) : VisAlgorithm(data) {}

    virtual void execute(const Algorithm::Options &options, const volatile bool &abortFlag) override {
        isolines = getGraphics( "Isolines" ).makePrimitive();
        info = getGraphics("Info").makePrimitive();

        shared_ptr< const TensorFieldInterpolated<2, Scalar> > field = options.get< TensorFieldInterpolated<2, Scalar> >("Field");
        if( !field ) {
            debugLog() << "Input Field not set." << endl;
            return;
        }

        /// Levels #############################################################
        const string sLevels = options.get<string>("Levels");
        istringstream iss(sLevels);
        vector<string> vsLevels;
        copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(vsLevels));
        if(vsLevels.size() < 1) {
            debugLog() << "No isoline-levels present!" << endl;
            return;
        }
        vector<double> vdLevels;
        for (size_t i = 0; i < vsLevels.size(); ++i)
            vdLevels.push_back(stod(vsLevels[i]));
        for (size_t i = 1; i < vdLevels.size(); ++i)
            if(vdLevels[i] == 0.0) {
                debugLog() << "Wrong value in isoline-levels present!" << endl;
                return;
            }

        const double width = options.get<double>("Width");

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

        /// Datenstrukturen für conrec #########################################
        double **d = new double*[ny];
        for (size_t i = 0; i < ny; ++i)
            d[i] = new double[nx];

        for (size_t y = 0; y < ny; ++y)
            for (size_t x = 0; x < nx; ++x)
                d[y][x] = field->values()[y * nx + x][0];

        vector<double> x;
        for (size_t i = 0; i < nx; ++i)
            x.push_back(grid->points()[i][0]);

        vector<double> y;
        for (size_t i = 0; i < ny; ++i)
            y.push_back(grid->points()[i * nx][1]);

        debugLog() << "structs-init: fertig" << endl;

        /// Lines ##############################################################
        vector<vector<Vector3> *> v;
        for (size_t i = 0; i < vdLevels.size(); ++i)
            v.push_back(new vector<Vector3>);

        conrec(d, 0, static_cast<int>(ny), 0, static_cast<int>(nx), y, x, static_cast<int>(vdLevels.size()), vdLevels, v);
        debugLog() << "conrec: fertig" << endl;

        /// Isolines ###########################################################
        for (size_t i = 0; i < vdLevels.size(); ++i) {
            float r = i / (static_cast<float>(vdLevels.size()) - 1);
            float b = (static_cast<float>(vdLevels.size()) - 1 - i) / (static_cast<float>(vdLevels.size()) - 1);

            Color color(r, 0.25, b);

            isolines->add(Primitive::LINES)
            .setLineWidth(width)
            .setColor(color)
            .setVertices(*v[i]);
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
        info->addTextLabel(Vector3(grid->points()[0][0] - 1, grid->points()[nx * ny - 1][1], 0),
                           "Temp in oC\nmin: " + to_string(min) + "\nmax: " + to_string(max),
                           20,
                           Color(1, 0.5, 0.5),
                           Primitive::TopRight,
                           Primitive::Serif,
                           Primitive::Regular,
                           false);

        // Aufräumen -----------------------------------------------------------
        for (size_t i = 0; i < ny; ++i) {
            delete[] d[i];
            d[i] = 0;
        }
        delete[] d;
        d = 0;
        for (size_t i = 0; i < v.size(); ++i) {
            delete v[i];
            v[i] = 0;
        }
    }
};

AlgorithmRegister<TextureAlgorithm> dummy("Vis Project/Isolines", "Interpolate in a data set.");

} // namespace
