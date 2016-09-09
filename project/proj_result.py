### FAnToM Session
### API Version: 20140515
### Used core version:    GITDIR-NOTFOUND (GITDIR-NOTFOUND)
### Used toolbox version: GITDIR-NOTFOUND (GITDIR-NOTFOUND)

################################################################
###                  Reset GUI                               ###
################################################################
fantom.ui.setCamera( 0, fantom.ui.Camera( fantom.math.Vector3(47.7858, 1.17373, 15.5998), fantom.math.Vector3(47.9537, 1.67846, 14.753), fantom.math.Vector3(0.0484131, 0.85373, 0.51846), 0, 4.50128 ) )
fantom.ui.setCamera( 1, fantom.ui.Camera( fantom.math.Vector3(51.2, 33.4898, 0), fantom.math.Vector3(51.2, 32.4898, 0), fantom.math.Vector3(0, 0, 1), 0, 5.95021 ) )
fantom.ui.setCamera( 2, fantom.ui.Camera( fantom.math.Vector3(51.2, 10.5, 22.9898), fantom.math.Vector3(51.2, 10.5, 21.9898), fantom.math.Vector3(0, 1, 0), 0, 5.95021 ) )
fantom.ui.setCamera( 3, fantom.ui.Camera( fantom.math.Vector3(74.1898, 10.5, 0), fantom.math.Vector3(73.1898, 10.5, 0), fantom.math.Vector3(0, 0, 1), 0, 5.95021 ) )

fantom.ui.setClippingPlane( fantom.ui.ClippingPlane( 0, fantom.math.Matrix4( (1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) ), False ) )
fantom.ui.setClippingPlane( fantom.ui.ClippingPlane( 1, fantom.math.Matrix4( (1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) ), False ) )
fantom.ui.setClippingPlane( fantom.ui.ClippingPlane( 2, fantom.math.Matrix4( (1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) ), False ) )
fantom.ui.setClippingPlane( fantom.ui.ClippingPlane( 3, fantom.math.Matrix4( (1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) ), False ) )
fantom.ui.setClippingPlane( fantom.ui.ClippingPlane( 4, fantom.math.Matrix4( (1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) ), False ) )
fantom.ui.setClippingPlane( fantom.ui.ClippingPlane( 5, fantom.math.Matrix4( (1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) ), False ) )

fantom.ui.setBackgroundColor( fantom.math.Color(0, 0, 0, 1) )

fantom.ui.setRotationCenter( fantom.ui.RotationCenter( fantom.math.Vector3(58.5595, 14.0212, -3.3506), True, True, True ) )


################################################################
###                  Create algorithms                       ###
################################################################
VisProject_LoadWeatherData = fantom.makeAlgorithm("Vis Project/Load WeatherData")
VisProject_LoadWeatherData.setName( "Vis Project/Load WeatherData" )
VisProject_LoadWeatherData.setOption("Input File", "/home/celentano/workspace/cpp/fantom/praktikum/project/measure_tg_20160821_233059_DE_20160518.csv")
fantom.ui.setAlgorithmPosition(VisProject_LoadWeatherData, fantom.math.Vector2(0, 36))

VisProject_InterpolateShepardalglib = fantom.makeAlgorithm("Vis Project/Interpolate Shepard alglib")
VisProject_InterpolateShepardalglib.setName( "Vis Project/Interpolate Shepard alglib" )
VisProject_InterpolateShepardalglib.setOption("r", 6)
fantom.ui.setAlgorithmPosition(VisProject_InterpolateShepardalglib, fantom.math.Vector2(0, 249))

Grid_Generate = fantom.makeAlgorithm("Grid/Generate")
Grid_Generate.setName( "Grid/Generate" )
Grid_Generate.setOption("Type", "2D")
Grid_Generate.setOption("nx", 80)
Grid_Generate.setOption("ny", 90)
Grid_Generate.setOption("nz", 10)
Grid_Generate.setOption("Center", fantom.math.Vector3(51.2, 10.5, 0))
Grid_Generate.setOption("dx", 0.1)
Grid_Generate.setOption("dy", 0.1)
Grid_Generate.setOption("dz", 1)
fantom.ui.setAlgorithmPosition(Grid_Generate, fantom.math.Vector2(0, 118))

Grid_ShowOuterBounds = fantom.makeAlgorithm("Grid/Show Outer Bounds")
Grid_ShowOuterBounds.setName( "Grid/Show Outer Bounds" )
Grid_ShowOuterBounds.setOption("Line color", fantom.math.Color(0, 0.6667, 0, 1))
Grid_ShowOuterBounds.setOption("Line width", 1.5)
fantom.ui.setAlgorithmPosition(Grid_ShowOuterBounds, fantom.math.Vector2(162, 128.5))
Grid_ShowOuterBounds.setVisualOutputVisible('Boundary', True)

ColorMapping_ShowColorMap = fantom.makeAlgorithm("Color Mapping/Show Color Map")
ColorMapping_ShowColorMap.setName( "Color Mapping/Show Color Map" )
ColorMapping_ShowColorMap.setOption("Time is", "physical time")
ColorMapping_ShowColorMap.setOption("Time", 0)
ColorMapping_ShowColorMap.setOption("Time step", 0)
ColorMapping_ShowColorMap.setOption("Offset", fantom.math.Vector3(0, 0, 0))
ColorMapping_ShowColorMap.setOption("Enable surface normals", True)
ColorMapping_ShowColorMap.setOption("Enable Alpha Blending", True)
ColorMapping_ShowColorMap.setOption("Colormap using shader", True)
ColorMapping_ShowColorMap.setOption("Shading", "Blinn-Phong")
fantom.ui.setAlgorithmPosition(ColorMapping_ShowColorMap, fantom.math.Vector2(391.312, 352))
ColorMapping_ShowColorMap.setVisualOutputVisible('color map', True)

VisProject_Isolines = fantom.makeAlgorithm("Vis Project/Isolines")
VisProject_Isolines.setName( "Vis Project/Isolines" )
VisProject_Isolines.setOption("Levels", "6 8 10 12 13 14 15")
VisProject_Isolines.setOption("Width", 4)
fantom.ui.setAlgorithmPosition(VisProject_Isolines, fantom.math.Vector2(0, 352))
VisProject_Isolines.setVisualOutputVisible('Info', True)
VisProject_Isolines.setVisualOutputVisible('Isolines', True)

VisProject_Rainfall = fantom.makeAlgorithm("Vis Project/Rainfall")
VisProject_Rainfall.setName( "Vis Project/Rainfall" )
VisProject_Rainfall.setOption("wigth", 0.05)
VisProject_Rainfall.setOption("high", 0.2)
VisProject_Rainfall.setOption("threshold", 0.1)
fantom.ui.setAlgorithmPosition(VisProject_Rainfall, fantom.math.Vector2(187, 352))
VisProject_Rainfall.setVisualOutputVisible('Info', True)
VisProject_Rainfall.setVisualOutputVisible('Rainfall', True)

VisProject_InterpolateShepardalglib_2 = fantom.makeAlgorithm("Vis Project/Interpolate Shepard alglib")
VisProject_InterpolateShepardalglib_2.setName( "Vis Project/Interpolate Shepard alglib::2" )
VisProject_InterpolateShepardalglib_2.setOption("r", 8)
fantom.ui.setAlgorithmPosition(VisProject_InterpolateShepardalglib_2, fantom.math.Vector2(349, 249))

VisProject_LoadWeatherData_2 = fantom.makeAlgorithm("Vis Project/Load WeatherData")
VisProject_LoadWeatherData_2.setName( "Vis Project/Load WeatherData::2" )
VisProject_LoadWeatherData_2.setOption("Input File", "/home/celentano/workspace/cpp/fantom/praktikum/project/measure_rr_20160821_233709_DE_20160518.csv")
fantom.ui.setAlgorithmPosition(VisProject_LoadWeatherData_2, fantom.math.Vector2(287, 36))

Grid_Generate_2 = fantom.makeAlgorithm("Grid/Generate")
Grid_Generate_2.setName( "Grid/Generate::2" )
Grid_Generate_2.setOption("Type", "2D")
Grid_Generate_2.setOption("nx", 20)
Grid_Generate_2.setOption("ny", 22)
Grid_Generate_2.setOption("nz", 10)
Grid_Generate_2.setOption("Center", fantom.math.Vector3(51.2, 10.5, 0))
Grid_Generate_2.setOption("dx", 0.4)
Grid_Generate_2.setOption("dy", 0.4)
Grid_Generate_2.setOption("dz", 1)
fantom.ui.setAlgorithmPosition(Grid_Generate_2, fantom.math.Vector2(473, 118))

VisProject_Texture2D = fantom.makeAlgorithm("Vis Project/Texture 2D")
VisProject_Texture2D.setName( "Vis Project/Texture 2D" )
VisProject_Texture2D.setOption("Input File", "/home/celentano/workspace/cpp/fantom/praktikum/project/Germany_location_map.jpg")
fantom.ui.setAlgorithmPosition(VisProject_Texture2D, fantom.math.Vector2(391.312, 411))
VisProject_Texture2D.setVisualOutputVisible('Texture', False)



################################################################
###                     Make Connections                     ###
################################################################
VisProject_InterpolateShepardalglib.connect("interpolated TensorField", ColorMapping_ShowColorMap, "Field")
VisProject_InterpolateShepardalglib_2.connect("interpolated TensorField", VisProject_Rainfall, "Field")
VisProject_LoadWeatherData.connect("Field", VisProject_InterpolateShepardalglib, "Field")
VisProject_LoadWeatherData_2.connect("Points", VisProject_InterpolateShepardalglib_2, "Points")
Grid_Generate.connect("grid", VisProject_InterpolateShepardalglib, "Grid2D")
VisProject_LoadWeatherData.connect("Points", VisProject_InterpolateShepardalglib, "Points")
Grid_Generate.connect("grid", VisProject_Texture2D, "Grid")
VisProject_InterpolateShepardalglib.connect("interpolated TensorField", VisProject_Isolines, "Field")
Grid_Generate.connect("grid", Grid_ShowOuterBounds, "Grid")
VisProject_LoadWeatherData_2.connect("Field", VisProject_InterpolateShepardalglib_2, "Field")
Grid_Generate_2.connect("grid", VisProject_InterpolateShepardalglib_2, "Grid2D")


################################################################
###                      Run algorithms                      ###
################################################################
fantom.scheduleAllNecessary()