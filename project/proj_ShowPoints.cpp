#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/graphics.hpp>
#include <fantom/fields.hpp>

#include <fantom/inputs/Primitive.hpp>

#define TEMP_MIN    "Temp min"
#define TEMP_MAX    "Temp max"

using namespace fantom;
using namespace std;

namespace
{

class ShowPointsAlgorithm : public VisAlgorithm
{
    // Zeiger auf interne Grafikobjekte
    std::unique_ptr< Primitive > mPoints;
    std::unique_ptr< Primitive > mLabels;

public:
    // Ausgaben der Visualisierung bekanntmachen
    struct VisOutputs : public VisAlgorithm::VisOutputs {
        VisOutputs( fantom::VisOutputs::Control &control ) : VisAlgorithm::VisOutputs( control ) {
            // Zwei benannte Grafikobjekte
            addGraphics( "points" );
            addGraphics( "field" );
        }
    };

    struct Options : public DataAlgorithm::Options {
    public:
        Options( Algorithm::Options::Control &control ) : DataAlgorithm::Options( control ) {
            // Die Datenpunkte
            add< All_DiscreteDomain >( "Domain", "" );

            // Zusätzliche Optionen, wie Farbe und Goesse
            add< double >( "Size", "", 0.05 );
            add< Color >( "Color", "", Color(1, 0, 0) );

            // Optional ein Feld mit Werten
            add< TensorFieldDiscrete< Scalar > >( "Field", "" );

            add<InputSlider>("slider", "", 5);
        }
    };

    ShowPointsAlgorithm( InitData &data ) : VisAlgorithm( data ) {}

    virtual void execute( const Algorithm::Options &options, const volatile bool & /*abortFlag*/ ) override {
        // loeschen der aktuellen Grafik und anlegen neuer Grafikobjekte
        mPoints.reset();
        mPoints = getGraphics( "points" ).makePrimitive();

        mLabels.reset();
        mLabels = getGraphics( "field" ).makePrimitive();

        // Lesen der parameter -------------------------------------------------
        auto slider = options.get< size_t >( "slider" );
        auto size = options.get< double >( "Size" ) * slider;
        auto color = options.get< Color >( "Color" );
        auto domain = options.get< DiscreteDomain<2> >( "Domain" );
        auto field = options.get< const TensorFieldDiscrete< Scalar > >( "Field" );
        if( !field ) {
            debugLog() << "Field not connected." << std::endl;
            return;
        }

        // Testen welche Eingaben gesetzt sind
        if( domain ) {
            // Nur Punkte? Zeige Punkte an.
            for( size_t i = 0; i < domain->numPoints(); ++i ) {
                // points zeichnen
                mPoints->addSphere( toPoint3( domain->points()[ i ] ), size, color);

                // Temperatur-Ausgabe
                string sTemp = std::to_string(field->values()[i][0]);
                for (int var = 0; var < 5; ++var)
                    sTemp.pop_back();
                mLabels->addTextLabel( toPoint3( domain->points()[ i ] ), sTemp, 24 );
            }
        }
    }
};

AlgorithmRegister< ShowPointsAlgorithm > dummy( "Vis Project/Show Points", "Show points(x,y)." );
} // namespace
