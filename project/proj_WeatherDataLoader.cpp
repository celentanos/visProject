#include <fstream>
#include <sstream>
#include <vector>

#include <fantom/algorithm.hpp>
#include <fantom/fields.hpp>
#include <fantom/register.hpp>

#include <stdlib.h>
#include <iostream>
#include <sstream>

#define LINE_OFFSET     10
#define MISSING_VAL     -9999

using namespace std;
using namespace fantom;

namespace
{

class LoadGeoDataAlgorithm : public DataAlgorithm
{
public:

    struct Options : public DataAlgorithm::Options {
    public:
        Options( Algorithm::Options::Control &control ) : DataAlgorithm::Options( control ) {
            add < InputLoadPath >( "Input File", "The file to be read", "" );
        }
    };

    struct DataOutputs : public DataAlgorithm::DataOutputs {
    public:
        DataOutputs( Control &control ) : DataAlgorithm::DataOutputs( control ) {
            add< DomainBase >( "Points" );
            add< TensorFieldBase >( "Field" );
            // hier muessen weitere Ausgaben des Moduls ergaenzt werden, wenn welche vorhanden sind
        }
    };

    LoadGeoDataAlgorithm( InitData &data ) : DataAlgorithm( data ) {
    }

    void execute( const Algorithm::Options &parameters, const volatile bool &abortFlag ) {
        // Test, ob die Dati gesetzt ist, sonst nichts machen
        if( parameters.get<std::string>( "Input File" ) != "" ) {
            std::ifstream infile( parameters.get<std::string>( "Input File" ) );

            // Fehler werfen, wenn die Datei ungueltig ist
            if( !infile ) {
                throw "Input file cannot be opened";
            }

            std::string line;

            // skip first two lines
            for (int i = 0; i < LINE_OFFSET; ++i)
                getline( infile, line );

            // Speicher fuer die Daten
            std::vector< Point2 > points;
            std::vector< Scalar > values;

            // Schleife zum einlesen der Daten
            while( infile && !abortFlag ) {
                // Hier muessen die Daten geladen werden

                getline( infile, line );
                istringstream iss(line);
                int staid;
                int souid;
                string staname;
                double val;
                double valq;
                double x;
                double y;
                double high;
                iss >> staid;
                iss >> souid;
                iss >> staname;
                iss >> val;
                iss >> valq;
                iss >> x;
                iss >> y;
                iss >> high;
                if(val == MISSING_VAL || x == MISSING_VAL || y == MISSING_VAL)
                    continue;
                points.push_back(Point2(x, y));
                values.push_back(Scalar(val));
            }

            auto domain = DomainFactory::makeDomainArbitrary( points );
            auto field  = DomainFactory::makeTensorField( *domain, values );

            setResult( "Points", domain );
            setResult( "Field", field );
        }
    }

public:
    IMPLEMENT_CAN_HANDLE( ".dat" )

    static bool loaderSetOptions( Algorithm::Options &options, std::vector < std::string > &filenames ) {
        // find the name we will handle
        for( auto it = filenames.begin(); it != filenames.end(); ++it ) {
            if( it->substr( it->size() - 4, 4 ) == ".dat" ) {
                options.set < std::string >( "Input File", *it );
                it = filenames.erase( it );
                return true;
            }
        }
        return false;
    }
};

AlgorithmRegister<LoadGeoDataAlgorithm> dummy( "Vis Project/Load WeatherData", "Load a test WeatherData dataset." );
DataLoaderRegister<LoadGeoDataAlgorithm> dummy2( "Vis Project/Load WeatherData" );

} // namespace
