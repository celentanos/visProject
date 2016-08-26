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
    unique_ptr< Primitive > texture;
    unique_ptr< Primitive > info;

public:
    struct Options : public VisAlgorithm::Options {
//    public:
        Options(Options::Control &control) : VisAlgorithm::Options(control) {
            add<Grid<2>>("Grid", "Texture coordinates");
            add < InputLoadPath >( "Input File", "The file to be read", "" );
        }
    };

    struct VisOutputs : public VisAlgorithm::VisOutputs {
        VisOutputs(VisOutputs::Control &control) : VisAlgorithm::VisOutputs( control ) {
            addGraphics( "Texture" );
        }
    };

    TextureAlgorithm(InitData &data) : VisAlgorithm(data) {}

    virtual void execute(const Algorithm::Options &options, const volatile bool &abortFlag) override {
        texture = getGraphics( "Texture" ).makePrimitive();

        string line = options.get<string>("Input File");

        /// Grid ###############################################################
        shared_ptr<const Grid<2> > grid = options.get<const Grid<2> >("Grid");
        if( !grid ) {
            throw std::logic_error( "Wrong type of grid!" );
        }

        const size_t nx = grid->structuringDimensionExtent(0);
        const size_t ny = grid->structuringDimensionExtent(1);
        debugLog() << "nx: " << nx << endl;
        debugLog() << "ny: " << ny << endl;

        double x1 = grid->points()[0][0];
        double x2 = grid->points()[nx - 1][0];
        double y1 = grid->points()[0][1];
        double y2 = grid->points()[nx * ny - 1][1];

        /// Info ###############################################################

        debugLog() << "path: " << line << endl;
        std::unique_ptr< Texture > texture2D = makeTexture(line);
        if(!texture2D)
            debugLog() << "texture2D = 0!" << endl;

        std::vector< Point3 > cube( 4 );
        cube[0] = Point3( x1, y1, 0);
        cube[1] = Point3( x2, y1, 0);
        cube[2] = Point3( x2, y2, 0);
        cube[3] = Point3( x1, y2, 0);

        std::vector< Point3 > texCoords( 4 );
        texCoords[0] = Point3( 0.0, 0.0, 1.0 );
        texCoords[1] = Point3( 1.0, 0.0, 1.0 );
        texCoords[2] = Point3( 1.0, 1.0, 1.0 );
        texCoords[3] = Point3( 0.0, 1.0, 1.0 );

        std::vector< unsigned int > sides(4);
        sides[0] = 0;
        sides[1] = 1;
        sides[2] = 2;
        sides[3] = 3;

        texture->setTexture( 0, *texture2D );
        texture->add( Primitive::QUADS ).setTexCoords( 0, texCoords ).setVertices( cube, sides );
    }
};

AlgorithmRegister<TextureAlgorithm> dummy("Vis Project/Texture 2D", "Interpolate in a data set.");

} // namespace
