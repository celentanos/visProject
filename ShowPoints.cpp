#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/graphics.hpp>
#include <fantom/fields.hpp>

#include <fantom/inputs/Primitive.hpp>
#include "colormap.h"

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
            addGraphics( "labels" );
        }
    };

    struct Options : public DataAlgorithm::Options {
    public:
        Options( Algorithm::Options::Control &control ) : DataAlgorithm::Options( control ) {
            // Die Datenpunkte
            add< All_DiscreteDomain >( "Domain", "" );

            // Zusätzliche Optionen, wie Farbe und Goesse
            add< Color >( "Color", "", Color(1, 0, 0) );
            add< double >( "Size", "", 0.5 );

            // Intervall
            add< double >( TEMP_MIN, "", 0 );
            add< double >( TEMP_MAX, "", 20 );

            // Optional ein Feld mit Werten
            add< TensorFieldDiscrete< Scalar > >( "Field", "" );

            add<InputSlider>("slider", "", 10);
        }
    };

    ShowPointsAlgorithm( InitData &data ) : VisAlgorithm( data ) {}

    virtual void execute( const Algorithm::Options &options, const volatile bool & /*abortFlag*/ ) override {
        // loeschen der aktuellen Grafik und anlegen neuer Grafikobjekte
        mPoints.reset();
        mPoints = getGraphics( "points" ).makePrimitive();

        mLabels.reset();
        mLabels = getGraphics( "labels" ).makePrimitive();

        // Lesen der parameter
//        auto color = options.get< Color >( "Color" );

        auto domain = options.get< DiscreteDomain<2> >( "Domain" );

        // Variablen für die Intervalle
        auto tempMin = options.get< double >( TEMP_MIN );
        auto tempMax = options.get< double >( TEMP_MAX );

        auto field = options.get< const TensorFieldDiscrete< Scalar > >( "Field" );
        if( !field ) {
            debugLog() << "Field not connected." << std::endl;
            return;
        }

        double tempFieldMin = numeric_limits<double>::max();
        double tempFieldMax = numeric_limits<double>::min();
        for (size_t i = 0; i < domain->numPoints(); ++i) {
            if(tempFieldMin > field->values()[i][0])
                tempFieldMin = field->values()[i][0];
            if(tempFieldMax < field->values()[i][0])
                tempFieldMax = field->values()[i][0];
        }

        double tempFieldDiff = tempFieldMax - tempFieldMin;

        // Testen welche Eingaben gesetzt sind
        if( domain ) {
            // Nur Punkte? Zeige Punkte an.
            for( size_t i = 0; i < domain->numPoints(); ++i ) {

                if(field->values()[i][0] >= tempMin && field->values()[i][0] <= tempMax) {

                    double pointSize = options.get< double >( "Size" );
                    double pointSizeNew = pointSize / ((tempFieldDiff + 0.002) / (field->values()[i][0] + 0.001 - tempFieldMin));

//                    Color color(1.0 / ((tempFieldDiff + 0.002) / (field->values()[i][0] + 0.001 - tempFieldMin)), 0, 0);

//                    mPoints->addSphere( toPoint3( domain->points()[ i ] ), pointSizeNew, color );
                    ColorMap colormap(tempFieldMin, tempFieldMax);
                    mPoints->addSphere( toPoint3( domain->points()[ i ] ), 0.2, colormap.getColor(field->values()[i][0]) );

//                    mLabels->addTextLabel( toPoint3( domain->points()[ i ] ), to_string(i), 24 );

                    // Temperatur-Ausgabe
                    string sTemp = std::to_string(field->values()[i][0]);
                    for (int var = 0; var < 5; ++var)
                        sTemp.pop_back();
                    mLabels->addTextLabel( toPoint3( domain->points()[ i ] ), sTemp, 24 );
                } /*else
                    infoLog() << "i:" << i << " temp:" << to_string(field->values()[i][0]) << endl;*/
            }
        } else {
            // Punkte + Werte?
            // Zeige beides an
            auto field = options.get< const TensorFieldDiscrete< Scalar > >( "Field" );

            if( !field ) {
                debugLog() << "Field not connected." << std::endl;
                return;
            }

            auto domain = std::dynamic_pointer_cast< const DiscreteDomain<2> >( field->domain() );

            auto eval = field->makeDiscreteEvaluator();
            if( domain ) {
                for( size_t i = 0; i < domain->numPoints(); ++i ) {

                    double pointSize = options.get< double >( "Size" );
                    double pointSizeNew = pointSize / ((tempFieldDiff + 0.002) / (field->values()[i][0] + 0.001 - tempFieldMin));

//                    Color color(1.0 / ((tempFieldDiff + 0.002) / (field->values()[i][0] + 0.001 - tempFieldMin)), 0, 0);

//                    mPoints->addSphere( toPoint3( domain->points()[ i ] ), pointSizeNew, color );
                    ColorMap colormap(tempFieldMin, tempFieldMax);
                    mPoints->addSphere( toPoint3( domain->points()[ i ] ), 0.2, colormap.getColor(field->values()[i][0]) );

                    mLabels->addTextLabel( toPoint3( domain->points()[ i ]  ), std::to_string( i ), 24 );
                }
            }
        }
    }
};

AlgorithmRegister< ShowPointsAlgorithm > dummy( "Vis Praktikum/Show Points", "Show some example graphics." );
} // namespace
