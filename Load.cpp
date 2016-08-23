#include <fstream>
#include <sstream>
#include <vector>

#include <fantom/algorithm.hpp>
#include <fantom/fields.hpp>
#include <fantom/register.hpp>

#include <stdlib.h>
#include <iostream>
#include <sstream>

using namespace std;
using namespace fantom;
namespace
{

/**
* Loader for a simple tabular format with Geo/Meteo information
* Each entry is a line
* Each line contains (at least) the following data
* latitude longitude Temperature
* The first two lines are comment lines and are ignored
*/
class LoadGeoDataAlgorithm
    : public DataAlgorithm
{
public:

    struct Options
            : public DataAlgorithm::Options {
    public:
        Options( Algorithm::Options::Control &control )
            : DataAlgorithm::Options( control ) {
            add < InputLoadPath >( "Input File", "The file to be read", "" );
        }
    };

    struct DataOutputs
            : public DataAlgorithm::DataOutputs {
    public:
        DataOutputs( Control &control )
            : DataAlgorithm::DataOutputs( control ) {
            add< DomainBase >( "Points" );
            add< TensorFieldBase >( "Temperature" );
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
            getline( infile, line );
            getline( infile, line );

            // Speicher fuer die Daten
            std::vector< Point2 > positions;
            std::vector< Scalar > temperatures;

            // Schleife zum einlesen der Daten
            while( infile && !abortFlag ) {
                // Hier muessen die Daten geladen werden

                getline( infile, line );
                istringstream iss(line);
                double x;
                double y;
                double tem;
                iss >> x;
                iss >> y;
                iss >> tem;
                positions.push_back(Point2(x, y));
                temperatures.push_back(Scalar(tem));
            }

            auto domain = DomainFactory::makeDomainArbitrary( positions );
            auto field  = DomainFactory::makeTensorField( *domain, temperatures );

            setResult( "Points", domain );
            setResult( "Temperature", field );
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

AlgorithmRegister<LoadGeoDataAlgorithm> dummy( "Load/GeoData", "Load a test GeoData dataset." );
DataLoaderRegister<LoadGeoDataAlgorithm> dummy2( "Load/GeoData" );

} // namespace
